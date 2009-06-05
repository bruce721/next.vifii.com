/*
 * This file is part of hildon-desktop
 *
 * Copyright (C) 2006, 2007 Nokia Corporation.
 *
 * Author:  Lucas Rocha <lucas.rocha@nokia.com>
 *          Johan Bilien <johan.bilien@nokia.com>
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <locale.h>

#include <glib/gi18n.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <libgnomevfs/gnome-vfs.h>

#include <libhildonwm/hd-wm-memory.h>

/* getpriority, setpriority */
#include <sys/time.h>
#include <sys/resource.h>

/* dlsym */
#define __USE_GNU
#include <dlfcn.h>

/* open */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* signal */
#include <signal.h>

/* exit */
#include <stdlib.h>

#include <errno.h>

#include "hd-desktop.h"

#define OSSO_USER_DIR        ".osso"
#define HILDON_DESKTOP_GTKRC "current-gtk-theme.maemo_af_desktop"

/*
 * hildon-desktop is typically running with higher priority and
 * runs protected against out-of-memory killing from the kernel.
 * Spawned children however should not be given these priviledges.
 */
pid_t
fork (void)
{
  static pid_t (*fork_real) () = NULL;
  pid_t         pid;

  if (fork_real == NULL)
    fork_real = dlsym (RTLD_NEXT, "fork");

  pid = fork_real ();

  if (pid == 0)
  {
    pid_t       child_pid;
    int         priority;
    int         fd;

    errno = 0;

    child_pid = getpid ();

    /* If the child process inherited desktop's high priority,
     * give child default priority */
    priority = getpriority (PRIO_PROCESS, child_pid);

    if (!errno && priority < 0)
    {
      setpriority (PRIO_PROCESS, child_pid, 0);
    }

    /* Unprotect from OOM */
    fd = g_open ("/proc/self/oom_adj", O_WRONLY, 0);

    if (fd >= 0)
    {
      write (fd, "0", sizeof (char));
      close (fd);
    }

  }

  return pid;
}

/* signal handler, hildon-desktop sends SIGTERM to all tracked applications
 * when it receives SIGTEM itselgf */
static void
signal_handler (int signal)
{
  if (signal == SIGTERM)
  {
    hd_wm_memory_kill_all_watched (FALSE);

#ifdef HAVE_SAFE_MODE
    /* 
     * Clean up stamp file created by hd_desktop_init 
     * On next startup the stamp file is created again and hildon-desktop remains
     * in normal operation mode without entering into safe mode where some plugins
     * are disabled.
     */
    if (g_file_test (HILDON_DESKTOP_STAMP_FILE, G_FILE_TEST_EXISTS)) 
    {
      g_unlink (HILDON_DESKTOP_STAMP_FILE);
    }
#endif

    exit (0);
  }
}

int main (int argc, char **argv)
{
  GObject *desktop;
  gchar *gtkrc = NULL;

  g_thread_init (NULL);
  setlocale (LC_ALL, "");

  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);

  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

  textdomain (GETTEXT_PACKAGE);

  /* Read the maemo-af-desktop gtkrc file */
  gtkrc = g_build_filename (g_get_home_dir (),
                            OSSO_USER_DIR,
                            HILDON_DESKTOP_GTKRC,
                            NULL);

  if (gtkrc && g_file_test ((gtkrc), G_FILE_TEST_EXISTS))
  {
    gtk_rc_add_default_file (gtkrc);
  }

  g_free (gtkrc);

  /* If gtk_init was called already (maemo-launcher is used),
   * re-parse the gtkrc to include the maemo-af-desktop specific one */
  if (gdk_screen_get_default ())
  {
    gtk_rc_reparse_all_for_settings (
            gtk_settings_get_for_screen (gdk_screen_get_default ()),
                                         TRUE);
  }

  gtk_init (&argc, &argv);

  gnome_vfs_init ();

  signal (SIGTERM, signal_handler);

  desktop = hd_desktop_new ();

  hd_desktop_run (HD_DESKTOP (desktop));

  gtk_main ();

  return 0;
}
