/*
 * This file is part of hildon-desktop
 *
 * Copyright (C) 2007 Nokia Corporation.
 *
 * Author:  Johan Bilien <johan.bilien@nokia.com>
 * Contact: Karoliina Salminen <karoliina.t.salminen@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include "hbm-background.h"
#include "background-manager.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkx.h>

#include <libgnomevfs/gnome-vfs.h>

#include <X11/extensions/Xrender.h>

#include <libhildondesktop/hildon-desktop-picture.h>

#ifdef HAVE_LIBOSSO
#include <osso-mem.h>
#endif

#define MAXIMUM_WIDTH                   1600
#define MAXIMUM_HEIGHT                  960
#define BUFFER_SIZE                     8192
#define HBM_CACHE_PERMISSION (GNOME_VFS_PERM_USER_READ | \
                              GNOME_VFS_PERM_USER_WRITE | \
                              GNOME_VFS_PERM_GROUP_READ | \
                              GNOME_VFS_PERM_GROUP_WRITE | \
                              GNOME_VFS_PERM_OTHER_READ)


static void hbm_background_set_property (GObject            *object,
                                         guint               property_id,
                                         const GValue       *value,
                                         GParamSpec         *pspec);

static void hbm_background_get_property (GObject            *object,
                                         guint               property_id,
                                         GValue             *value,
                                         GParamSpec         *pspec);

static void hbm_background_finalize (GObject *object);

static void hbm_background_apply (HildonDesktopBackground *background,
                                  GdkWindow               *window,
                                  GError                 **error);


enum
{
  PROP_WIDTH = 1,
  PROP_HEIGHT,
  PROP_DISPLAY,
  PROP_PICTURE
};

struct _HBMBackgroundPrivate
{
  GdkDisplay                   *display;

  GdkPixbufLoader              *loader;
  gint                          width, height;
  GnomeVFSHandle               *handle;
  GnomeVFSHandle               *cache_handle;

  GnomeVFSFileSize              bytes_read;

  GdkPixbuf                    *pixbuf;
  gint                          pixbuf_width, pixbuf_height;

  gboolean                      caching;

  gboolean                      oom;

  guchar                        buffer[BUFFER_SIZE];

  Picture                       picture;
};

G_DEFINE_TYPE (HBMBackground, hbm_background, HILDON_DESKTOP_TYPE_BACKGROUND);

static void
hbm_background_init (HBMBackground *background)
{
  background->priv = G_TYPE_INSTANCE_GET_PRIVATE (background,
                                                  HBM_TYPE_BACKGROUND,
                                                  HBMBackgroundPrivate);

}

static void
hbm_background_class_init (HBMBackgroundClass *klass)
{
  GObjectClass                 *object_class;
  HildonDesktopBackgroundClass *background_class;
  GParamSpec                   *pspec;

  object_class = G_OBJECT_CLASS (klass);
  background_class = HILDON_DESKTOP_BACKGROUND_CLASS (klass);

  object_class->set_property = hbm_background_set_property;
  object_class->get_property = hbm_background_get_property;
  object_class->finalize = hbm_background_finalize;

  background_class->apply = hbm_background_apply;

  pspec = g_param_spec_int ("width",
                            "width",
                            "Width of the background to render",
                            0,
                            G_MAXINT,
                            0,
                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

  g_object_class_install_property (object_class,
                                   PROP_WIDTH,
                                   pspec);

  pspec = g_param_spec_int ("height",
                            "height",
                            "Height of the background to render",
                            0,
                            G_MAXINT,
                            0,
                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

  g_object_class_install_property (object_class,
                                   PROP_HEIGHT,
                                   pspec);

  pspec = g_param_spec_object ("display",
                               "display",
                               "Display to which the background shall be "
                               "rendered",
                               GDK_TYPE_DISPLAY,
                               G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

  g_object_class_install_property (object_class,
                                   PROP_DISPLAY,
                                   pspec);

  pspec = g_param_spec_int ("picture",
                            "picture",
                            "The Picture XID on which the image shall be "
                            "rendered",
                            0,
                            G_MAXINT,
                            None,
                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

  g_object_class_install_property (object_class,
                                   PROP_PICTURE,
                                   pspec);

  g_type_class_add_private (klass, sizeof (HBMBackgroundPrivate));

}

static void
hbm_background_finalize (GObject *object)
{

}

static void
hbm_background_set_property (GObject            *object,
                             guint               property_id,
                             const GValue       *value,
                             GParamSpec         *pspec)
{
  HBMBackgroundPrivate *priv = HBM_BACKGROUND (object)->priv;

  switch (property_id)
  {
      case PROP_WIDTH:
          priv->width = g_value_get_int (value);
          break;
      case PROP_HEIGHT:
          priv->height = g_value_get_int (value);
          break;
      case PROP_DISPLAY:
          priv->display = g_value_get_object (value);
          break;
      case PROP_PICTURE:
          priv->picture = (Picture)g_value_get_int (value);
          break;
      default:
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
          break;
  }

}

static void
hbm_background_get_property (GObject            *object,
                             guint               property_id,
                             GValue             *value,
                             GParamSpec         *pspec)
{
  HBMBackgroundPrivate *priv = HBM_BACKGROUND (object)->priv;

  switch (property_id)
  {
      case PROP_WIDTH:
          g_value_set_int (value, priv->width);
          break;
      case PROP_HEIGHT:
          g_value_set_int (value, priv->height);
          break;
      case PROP_DISPLAY:
          g_value_set_object (value, priv->display);
          break;
      case PROP_PICTURE:
          g_value_set_int (value, (gint)priv->picture);
          break;
      default:
          G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
          break;
  }
}

static void
hbm_background_size_prepared (HBMBackground    *background,
                              gint              width,
                              gint              height)
{
  HBMBackgroundPrivate         *priv = background->priv;
  gdouble                       x_ratio = 0;
  gdouble                       y_ratio = 0;
  gdouble                       ratio;
  HildonDesktopBackgroundMode   mode;

  g_object_get (background,
                "mode",   &mode,
                NULL);

  switch (mode)
  {
    case HILDON_DESKTOP_BACKGROUND_STRETCHED:
        priv->pixbuf_width  = priv->width;
        priv->pixbuf_height = priv->height;
        break;
    case HILDON_DESKTOP_BACKGROUND_SCALED:
        x_ratio = (gdouble)width  / priv->width;
        y_ratio = (gdouble)height / priv->height;
        ratio = MAX (x_ratio, y_ratio);
        priv->pixbuf_width  = (gint) (width  / ratio);
        priv->pixbuf_height = (gint) (height / ratio);
        break;
    case HILDON_DESKTOP_BACKGROUND_CROPPED:
        x_ratio = (gdouble)width  / priv->width;
        y_ratio = (gdouble)height / priv->height;
        ratio = MIN (x_ratio, y_ratio);
        priv->pixbuf_width  = (gint) (width  / ratio);
        priv->pixbuf_height = (gint) (height / ratio);
        break;
    case HILDON_DESKTOP_BACKGROUND_TILED:
    case HILDON_DESKTOP_BACKGROUND_CENTERED:
        /* For these, no scaling, but we still limit the
         * size to avoid loading a huge pixmap to X */

        if (width > MAXIMUM_WIDTH)
          x_ratio = (gdouble) width / MAXIMUM_WIDTH;

        if (height > MAXIMUM_HEIGHT)
          y_ratio = (gdouble) height / MAXIMUM_HEIGHT;

        ratio = MAX (x_ratio, y_ratio);

        if (ratio)
        {
          priv->pixbuf_width  = (gint) ((gdouble)width  / ratio);
          priv->pixbuf_height = (gint) ((gdouble)height / ratio);
        }
        else
        {
          priv->pixbuf_width  = width;
          priv->pixbuf_height = height;
        }
  }


  gdk_pixbuf_loader_set_size (priv->loader,
                              priv->pixbuf_width, priv->pixbuf_height);
}

static void
hbm_background_open_file (HBMBackground *background, GError **error)
{
  HBMBackgroundPrivate *priv = background->priv;
  gchar                *filename = NULL;
  GnomeVFSResult        result;

  g_object_get (background,
                "filename", &filename,
                NULL);

  result = gnome_vfs_open (&priv->handle, filename, GNOME_VFS_OPEN_READ);

  if (result != GNOME_VFS_OK)
  {
    g_set_error (error,
                 BACKGROUND_MANAGER_ERROR,
                 BACKGROUND_MANAGER_ERROR_UNREADABLE,
                 "Unable to open `%s': %s",
                 filename,
                 gnome_vfs_result_to_string (result));
  }
}

static gboolean
hbm_background_open_cache_file (HBMBackground *background, GError **error)
{
  HBMBackgroundPrivate *priv = background->priv;
  gchar                *cache = NULL;
  GnomeVFSResult        result;

  g_object_get (background,
                "cache", &cache,
                NULL);

  if (!cache || !*cache)
    /* No caching */
    return FALSE;

  result = gnome_vfs_create (&priv->cache_handle,
                             cache,
                             GNOME_VFS_OPEN_WRITE,
                             FALSE,
                             HBM_CACHE_PERMISSION);

  if (result != GNOME_VFS_OK)
  {
    g_set_error (error,
                 BACKGROUND_MANAGER_ERROR,
                 BACKGROUND_MANAGER_ERROR_FLASH_FULL,
                 "Unable to open `%s': %s",
                 cache,
                 gnome_vfs_result_to_string (result));

    return FALSE;
  }

  return TRUE;
}

static void
hbm_background_is_oom (HBMBackground *background, GError **error)
{
  if (background->priv->oom)
  {
    g_set_error (error,
                 BACKGROUND_MANAGER_ERROR,
                 BACKGROUND_MANAGER_ERROR_MEMORY,
                 "Unable to load background: Out of memory");

  }
}


#ifdef HAVE_LIBOSSO
static void
hbm_background_memory_cap_cb (size_t    current_size,
                              size_t    max_size,
                              void     *context)
{
  HBMBackground *background = context;

  background->priv->oom = TRUE;
}
#endif

static void
hbm_background_enable_memory_cap (HBMBackground *background)
{
  background->priv->oom = FALSE;

#ifdef HAVE_LIBOSSO
  if (osso_mem_saw_enable (osso_mem_get_lowmem_limit() / 3,
                           0,
                           hbm_background_memory_cap_cb,
                           background) < 0)
  {

    g_warning ("osso_mem_saw_enable failed");
  }
#endif
}

static void
hbm_background_disable_memory_cap (HBMBackground *background)
{
#ifdef HAVE_LIBOSSO
  osso_mem_saw_disable ();
#endif
}

static gboolean
hbm_background_read (HBMBackground *background, GError **error)
{
  HBMBackgroundPrivate *priv = background->priv;
  GnomeVFSResult        result;

  hbm_background_is_oom (background, error);
  if (*error) return FALSE;

  result = gnome_vfs_read (priv->handle,
                           priv->buffer,
                           BUFFER_SIZE,
                           &priv->bytes_read);

  hbm_background_is_oom (background, error);
  if (*error) return FALSE;

  if (result == GNOME_VFS_OK || result == GNOME_VFS_ERROR_EOF)
  {
    GError     *local_error = NULL;
    gdk_pixbuf_loader_write (priv->loader,
                             priv->buffer,
                             priv->bytes_read,
                             &local_error);

    if (local_error)
    {
      if ((local_error->domain == GDK_PIXBUF_ERROR) &&
          ((local_error->code == GDK_PIXBUF_ERROR_CORRUPT_IMAGE) ||
          ((local_error->code == GDK_PIXBUF_ERROR_UNKNOWN_TYPE))))
      {
        g_set_error (error, BACKGROUND_MANAGER_ERROR,
                     BACKGROUND_MANAGER_ERROR_CORRUPT,
                     "Unable to load file: %s",
                     local_error->message);

        g_error_free (local_error);
      }
      else
        g_propagate_error (error, local_error);

      return FALSE;
    }
  }
  else /* GNOME-VFS error */
  {
    g_set_error (error,
                 BACKGROUND_MANAGER_ERROR,
                 BACKGROUND_MANAGER_ERROR_IO,
                 "Unable to load: read failed");
    return FALSE;
  }

  if (result == GNOME_VFS_ERROR_EOF || priv->bytes_read == 0)
    return TRUE;

  return FALSE;

}

static void
hbm_background_write_to_cache (HBMBackground *background, GError **error)
{
  HBMBackgroundPrivate *priv = background->priv;
  GnomeVFSResult        result;
  GnomeVFSFileSize      bytes_written;
  GnomeVFSFileSize      to_write = priv->bytes_read;

  hbm_background_is_oom (background, error);
  if (*error) return;

  result = GNOME_VFS_OK;

  while (result == GNOME_VFS_OK && to_write > 0)
  {
    result = gnome_vfs_write (priv->cache_handle,
                              priv->buffer,
                              to_write,
                              &bytes_written);

    to_write -= bytes_written;
  }

  hbm_background_is_oom (background, error);
  if (*error) return;

  if (result == GNOME_VFS_ERROR_NO_SPACE)
  {
    g_set_error (error,
                 BACKGROUND_MANAGER_ERROR,
                 BACKGROUND_MANAGER_ERROR_FLASH_FULL,
                 "Unable to write to cache: device is full");
  }

  if (result != GNOME_VFS_OK)
  {
    g_set_error (error,
                 BACKGROUND_MANAGER_ERROR,
                 BACKGROUND_MANAGER_ERROR_IO,
                 "Unable to write to cache: write failed");
  }

  return;
}

static void
hbm_background_load_pixbuf (HBMBackground *background, GError **error)
{
  HBMBackgroundPrivate *priv = background->priv;
  gchar                *filename = NULL;
  GError               *local_error = NULL;
  gboolean              do_cache;

  g_object_get (background,
                "filename", &filename,
                NULL);

  if (!filename || !*filename)
    return;

  if (priv->loader)
    g_object_unref (priv->loader);

  hbm_background_enable_memory_cap (background);

  if (local_error) goto error;

  priv->loader = gdk_pixbuf_loader_new ();
  g_signal_connect_swapped (priv->loader, "size-prepared",
                            G_CALLBACK (hbm_background_size_prepared),
                            background);

  hbm_background_open_file (background, &local_error);
  if (local_error) goto error;

  do_cache = hbm_background_open_cache_file (background, &local_error);
  if (local_error) goto error;

  while (TRUE)
  {
    gboolean eof;

    eof = hbm_background_read (background, &local_error);
    if (local_error)
      goto error;

    if (do_cache)
    {
      hbm_background_write_to_cache (background, &local_error);
      if (local_error)
        goto error;
    }

    if (eof)
      break;
  }

  gdk_pixbuf_loader_close (priv->loader, &local_error);
  if (local_error) goto error;

  gnome_vfs_close (priv->handle);
  priv->handle = NULL;

  hbm_background_is_oom (background, &local_error);
  if (local_error) goto error;

  priv->pixbuf = gdk_pixbuf_loader_get_pixbuf (priv->loader);

  hbm_background_is_oom (background, &local_error);
  if (local_error) goto error;

  if (priv->pixbuf)
    g_object_ref (priv->pixbuf);

  g_object_unref (priv->loader);
  priv->loader = NULL;

  hbm_background_disable_memory_cap (background);
  return;

error:
  if (priv->pixbuf)
  {
    g_object_unref (priv->pixbuf);
    priv->pixbuf = NULL;
  }
  if (priv->handle)
  {
    gnome_vfs_close (priv->handle);
    priv->handle = NULL;
  }
  if (priv->loader)
  {
    g_object_unref (priv->loader);
    priv->loader = NULL;
  }
  hbm_background_disable_memory_cap (background);
  g_propagate_error (error, local_error);
  return;

}

static void
hbm_background_crop_pixbuf (HBMBackground *background)
{
  HBMBackgroundPrivate         *priv = background->priv;
  HildonDesktopBackgroundMode   mode;
  GdkPixbuf                    *cropped;

  g_object_get (background,
                "mode", &mode,
                NULL);

  if (priv->pixbuf_width > priv->width ||
      priv->pixbuf_height > priv->height)
  {
    switch (mode)
    {
        case HILDON_DESKTOP_BACKGROUND_CENTERED:
        case HILDON_DESKTOP_BACKGROUND_CROPPED:
        case HILDON_DESKTOP_BACKGROUND_SCALED:
            cropped = gdk_pixbuf_new (GDK_COLORSPACE_RGB,
                                      gdk_pixbuf_get_has_alpha (priv->pixbuf),
                                      gdk_pixbuf_get_bits_per_sample (priv->pixbuf),
                                      MIN (priv->pixbuf_width,  priv->width),
                                      MIN (priv->pixbuf_height, priv->height));

            gdk_pixbuf_copy_area (priv->pixbuf,
                                  MAX ((priv->pixbuf_width -
                                        priv->width) / 2, 0),
                                  MAX ((priv->pixbuf_height -
                                        priv->height) / 2,
                                       0),
                                  MIN (priv->pixbuf_width,  priv->width),
                                  MIN (priv->pixbuf_height, priv->height),
                                  cropped,
                                  0, 0);

            gdk_pixbuf_unref (priv->pixbuf);
            priv->pixbuf = cropped;
            break;

        case HILDON_DESKTOP_BACKGROUND_TILED:
            cropped = gdk_pixbuf_new (GDK_COLORSPACE_RGB,
                                      gdk_pixbuf_get_has_alpha (priv->pixbuf),
                                      gdk_pixbuf_get_bits_per_sample (priv->pixbuf),
                                      MIN (priv->pixbuf_width,  priv->width),
                                      MIN (priv->pixbuf_height, priv->height));

            gdk_pixbuf_copy_area (priv->pixbuf,
                                  0, 0,
                                  MIN (priv->pixbuf_width, priv->width),
                                  MIN (priv->pixbuf_height, priv->height),
                                  cropped,
                                  0, 0);
            gdk_pixbuf_unref (priv->pixbuf);
            priv->pixbuf = cropped;
            break;
        default:
            break;
    }
  }
}

static void
hbm_background_composite_pixbuf (HBMBackground *background)
{
  HBMBackgroundPrivate         *priv = background->priv;
  Picture                       image_picture = None, image_mask = None;
  Display                      *xdisplay = GDK_DISPLAY_XDISPLAY (priv->display);
  HildonDesktopBackgroundMode   mode;
  gint                          off_x, off_y;

  hildon_desktop_picture_and_mask_from_pixbuf (priv->display,
                                               priv->pixbuf,
                                               &image_picture,
                                               &image_mask);

  if (image_picture == None)
  {
    g_warning ("Failed to upload picture to X");
    return;
  }

  g_object_unref (priv->pixbuf);
  priv->pixbuf = NULL;

  if (priv->pixbuf_width  == priv->width &&
      priv->pixbuf_height == priv->height)
  {
    gdk_error_trap_push ();
    XRenderComposite (xdisplay,
                      PictOpOver,
                      image_picture,
                      image_mask,
                      priv->picture,
                      0, 0,
                      0, 0,
                      0, 0,
                      priv->width, priv->height);
    gdk_flush ();
    if (gdk_error_trap_pop ())
      g_warning ("Error when compositing image on background");

    if (image_picture != None)
      XRenderFreePicture (xdisplay, image_picture);

    if (image_mask != None)
      XRenderFreePicture (xdisplay, image_mask);
    return;
  }

  g_object_get (background,
                "mode", &mode,
                NULL);

  switch (mode)
  {
      case HILDON_DESKTOP_BACKGROUND_CENTERED:
      case HILDON_DESKTOP_BACKGROUND_SCALED:
      case HILDON_DESKTOP_BACKGROUND_STRETCHED:
      case HILDON_DESKTOP_BACKGROUND_CROPPED:
          off_x = (priv->width  - priv->pixbuf_width)  / 2;
          off_y = (priv->height - priv->pixbuf_height) / 2;

          XRenderComposite (xdisplay,
                            PictOpOver,
                            image_picture,
                            image_mask,
                            priv->picture,
                            0, 0,
                            0, 0,
                            MAX (off_x, 0), MAX (off_y, 0),
                            priv->pixbuf_width,
                            priv->pixbuf_height);

          break;
      case HILDON_DESKTOP_BACKGROUND_TILED:
          XRenderComposite (xdisplay,
                            PictOpOver,
                            image_picture,
                            image_mask,
                            priv->picture,
                            0, 0,
                            0, 0,
                            0, 0,
                            priv->width, priv->height);
          break;

  }

  if (image_picture != None)
    XRenderFreePicture (xdisplay, image_picture);

  if (image_mask != None)
    XRenderFreePicture (xdisplay, image_mask);

}

static void
hbm_background_apply (HildonDesktopBackground *background,
                      GdkWindow               *window,
                      GError                 **error)
{
  HBMBackgroundPrivate *priv = HBM_BACKGROUND (background)->priv;
  GError               *local_error = NULL;

  hbm_background_load_pixbuf (HBM_BACKGROUND (background), &local_error);
  if (local_error) goto error;

  if (priv->pixbuf)
  {
    hbm_background_crop_pixbuf (HBM_BACKGROUND (background));
    hbm_background_composite_pixbuf (HBM_BACKGROUND (background));
  }

  return;

error:
  g_debug ("Got error %i %i %s",
           local_error->domain,
           local_error->code,
           local_error->message);
  g_propagate_error (error, local_error);
  return;
}
