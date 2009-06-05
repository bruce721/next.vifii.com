/*
 * This file is part of maemo-af-desktop
 *
 * Copyright (C) 2007 Nokia Corporation.
 *
 * Author:  Moises Martinez <moises.martinez@nokia.com>
 *          Lucas Rocha <lucas.rocha@nokia.com>
 * Contact: Karoliina Salminen <karoliina.t.salminen@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
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

#include "hildon-desktop-popup-menu.h"

#include <gtk/gtkseparatormenuitem.h>
#include <gtk/gtkhbox.h>
#include <gtk/gtkscrolledwindow.h>
#include <gtk/gtkimage.h>
#include <gtk/gtkbutton.h>
#include <gtk/gtkwindow.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#define HILDON_DESKTOP_POPUP_MENU_GET_PRIVATE(object) \
	        (G_TYPE_INSTANCE_GET_PRIVATE ((object), HILDON_DESKTOP_TYPE_POPUP_MENU, HildonDesktopPopupMenuPrivate))

G_DEFINE_TYPE (HildonDesktopPopupMenu, hildon_desktop_popup_menu, GTK_TYPE_VBOX);

enum
{
  PROP_POPUP_ITEM_HEIGHT=1,
  PROP_POPUP_RESIZE_PARENT,
  PROP_POPUP_PARENT,
};

enum
{
  SIGNAL_POPUP_RESIZE,
  SIGNAL_POPUP_SHOW_CONTROLS,
  N_SIGNALS
};

static gint signals[N_SIGNALS];

#define MENU_SCROLL_TIMEOUT          50
#define MENU_SCROLL_INITIAL_TIMEOUT  200

struct _HildonDesktopPopupMenuPrivate
{
  GtkWidget    *parent;

  GtkWidget    *viewport;
  GtkWidget    *box_items;

  GtkWidget    *box_buttons;
  GtkWidget    *scroll_down;
  GtkWidget    *scroll_up;
  gboolean      controls_on;

  guint         n_items;

  guint         item_height;

  GtkMenuItem  *selected_item;

  gboolean      resize_parent;

  guint         toggle_size;

  gboolean      rtl;

  gint          scroll_id;
  gint          scroll_dir;
};

static GObject *hildon_desktop_popup_menu_constructor (GType gtype,
                                                       guint n_params,
                                                       GObjectConstructParam *params);

static void hildon_desktop_popup_menu_finalize (GObject *object);

static void hildon_desktop_popup_menu_get_property (GObject *object,
                                                    guint prop_id,
                                                    GValue *value,
                                                    GParamSpec *pspec);

static void hildon_desktop_popup_menu_set_property (GObject *object,
                                                    guint prop_id,
                                                    const GValue *value,
                                                    GParamSpec *pspec);

static void hildon_desktop_popup_menu_size_request (GtkWidget *widget, GtkRequisition *req);
static void hildon_desktop_popup_menu_size_allocate (GtkWidget *widget, GtkAllocation *allocation);
static gboolean hildon_desktop_popup_menu_motion_notify (GtkWidget *widget, GdkEventMotion *event);
static gboolean hildon_desktop_popup_menu_press_event (GtkWidget *widget, GdkEventButton *event);
static gboolean hildon_desktop_popup_menu_release_event (GtkWidget *widget, GdkEventButton *event);
static gboolean hildon_desktop_popup_menu_key_press_event (GtkWidget *widget, GdkEventKey *event);
static void hildon_desktop_popup_menu_select_next_prev_item (HildonDesktopPopupMenu *menu, gboolean next);
static void hildon_desktop_popup_menu_scroll_start (GtkWidget *widget, HildonDesktopPopupMenu *menu);
static void hildon_desktop_popup_menu_scroll_stop (GtkWidget *widget, HildonDesktopPopupMenu *menu);
static void hildon_desktop_popup_menu_adjustment_changed (GtkAdjustment *adj, HildonDesktopPopupMenu *menu);
static void hildon_desktop_popup_menu_box_remove (HildonDesktopPopupMenu *menu, GtkWidget *widget);

static void 
hildon_desktop_popup_menu_init (HildonDesktopPopupMenu *menu)
{
  menu->priv = HILDON_DESKTOP_POPUP_MENU_GET_PRIVATE (menu);

  menu->priv->parent = NULL;
  
  menu->priv->item_height  =
  menu->priv->n_items = 0;

  menu->priv->resize_parent = TRUE;

  menu->priv->controls_on = FALSE;

  menu->priv->selected_item = NULL;

  menu->priv->toggle_size = 0;
}

static void 
hildon_desktop_popup_menu_class_init (HildonDesktopPopupMenuClass *menu_class)
{
  GObjectClass *object_class         = G_OBJECT_CLASS (menu_class);
  GtkWidgetClass *widget_class       = GTK_WIDGET_CLASS (menu_class);

  object_class->constructor = hildon_desktop_popup_menu_constructor;
  object_class->finalize    = hildon_desktop_popup_menu_finalize;

  object_class->get_property = hildon_desktop_popup_menu_get_property;
  object_class->set_property = hildon_desktop_popup_menu_set_property;

  widget_class->motion_notify_event  = hildon_desktop_popup_menu_motion_notify;
  widget_class->button_press_event = hildon_desktop_popup_menu_press_event;
  widget_class->button_release_event = hildon_desktop_popup_menu_release_event;
  widget_class->key_press_event      = hildon_desktop_popup_menu_key_press_event;
  
  widget_class->size_request = hildon_desktop_popup_menu_size_request;
  widget_class->size_allocate = hildon_desktop_popup_menu_size_allocate;

  g_type_class_add_private (object_class, sizeof (HildonDesktopPopupMenuPrivate));

  signals[SIGNAL_POPUP_RESIZE] =
        g_signal_new ("popup-menu-resize",
                      G_OBJECT_CLASS_TYPE (object_class),
                      G_SIGNAL_RUN_FIRST,
                      0,
                      NULL, NULL,
                      g_cclosure_marshal_VOID__VOID,
                      G_TYPE_NONE, 0);
 
  signals[SIGNAL_POPUP_SHOW_CONTROLS] =
        g_signal_new ("show-controls",
                      G_OBJECT_CLASS_TYPE (object_class),
                      G_SIGNAL_RUN_LAST,
                      0,
                      NULL, NULL,
                      g_cclosure_marshal_VOID__BOOLEAN,
                      G_TYPE_NONE, 1, G_TYPE_BOOLEAN); 
  
  g_object_class_install_property (object_class,
                                   PROP_POPUP_ITEM_HEIGHT,
                                   g_param_spec_uint(
                                           "item-height",
                                           "item-height",
                                           "Height of the menu items",
                                            1,
                                            G_MAXINT,
                                            40,
                                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class,
                                   PROP_POPUP_RESIZE_PARENT,
                                   g_param_spec_boolean(
                                           "resize-parent",
                                           "resize-parent",
                                           "Whether resize or not parent window of menu",
                                            TRUE,
                                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

  g_object_class_install_property (object_class,
                                   PROP_POPUP_PARENT,
                                   g_param_spec_object(
                                           "parent",
                                           "parent",
                                           "The menu parent window that should be resized",
                                            GTK_TYPE_WIDGET,
                                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

}

static gboolean
hildon_desktop_popup_menu_paint_viewport (GtkWidget      *widget,
                                          GdkEventExpose *event,
                                          gpointer        user_data)
{
  static GtkWidgetClass *parent_class = NULL;

  if (!parent_class)
    parent_class =
        GTK_WIDGET_CLASS (g_type_class_peek_parent (GTK_VIEWPORT_GET_CLASS (widget)));

  parent_class->expose_event (widget, event);

  return TRUE;
}

static gboolean
hildon_desktop_popup_menu_paint_pane (GtkWidget      *widget,
                                      GdkEventExpose *event,
                                      gpointer        user_data)
{
  static GtkWidgetClass *parent_class = NULL;

  if (!parent_class)
    parent_class =
        GTK_WIDGET_CLASS (g_type_class_peek_parent (GTK_VBOX_GET_CLASS (widget)));

  gtk_paint_box (widget->style,
                 widget->window,
                 GTK_STATE_NORMAL,
                 GTK_SHADOW_NONE,
                 NULL,
                 widget,
                 NULL,
                 widget->allocation.x,
                 widget->allocation.y,
                 widget->allocation.width,
                 widget->allocation.height);

  parent_class->expose_event (widget, event);

  return TRUE;
}

static gboolean
hildon_desktop_popup_menu_item_is_selectable (GtkWidget *menu_item)
{
  if ((!GTK_BIN (menu_item)->child && GTK_IS_MENU_ITEM (menu_item)) ||
      GTK_IS_SEPARATOR_MENU_ITEM (menu_item) ||
      !GTK_WIDGET_IS_SENSITIVE (menu_item) ||
      !GTK_WIDGET_VISIBLE (menu_item))
    return FALSE;

  return TRUE;
}
	
static gboolean
hildon_desktop_popup_menu_item_is_valid (GtkWidget *menu_item)
{
  if ((!GTK_BIN (menu_item)->child && GTK_IS_MENU_ITEM (menu_item) 
			           && !GTK_IS_SEPARATOR_MENU_ITEM (menu_item)) ||
      !GTK_WIDGET_VISIBLE (menu_item))
    return FALSE;

  return TRUE;
}

static GObject *
hildon_desktop_popup_menu_constructor (GType gtype,
                                       guint n_params,
                                       GObjectConstructParam *params)
{
  HildonDesktopPopupMenu *menu;
  GtkAdjustment *adj;
  GObject *object;

  object = G_OBJECT_CLASS (hildon_desktop_popup_menu_parent_class)->constructor (gtype,
                                                                                 n_params,
     										 params);
  menu = HILDON_DESKTOP_POPUP_MENU (object);

  menu->priv->rtl = (gtk_widget_get_direction (GTK_WIDGET (menu)) == GTK_TEXT_DIR_RTL);
  menu->priv->viewport = gtk_viewport_new (NULL, NULL);

  gtk_box_pack_start (GTK_BOX (menu),
                      menu->priv->viewport,
                      FALSE, FALSE, 0);

  gtk_widget_show (menu->priv->viewport);

  adj = gtk_viewport_get_vadjustment (GTK_VIEWPORT (menu->priv->viewport));
  
  g_signal_connect (adj,
                    "value-changed",
                    G_CALLBACK (hildon_desktop_popup_menu_adjustment_changed), 
                    menu);

  g_signal_connect (adj,
                    "changed",
                    G_CALLBACK (hildon_desktop_popup_menu_adjustment_changed), 
                    menu);

  gtk_widget_push_composite_child ();

  menu->priv->box_items = gtk_vbox_new (FALSE, 0); /* FIXME: add spacing decoration */

  g_signal_connect_swapped (menu->priv->box_items, "remove",
                            G_CALLBACK (hildon_desktop_popup_menu_box_remove),
                            menu);
  
  gtk_widget_set_size_request (menu->priv->box_items, 1, -1);

  gtk_container_add (GTK_CONTAINER (menu->priv->viewport), menu->priv->box_items);

  gtk_widget_show (menu->priv->box_items);
  
  menu->priv->box_buttons = gtk_hbox_new (TRUE,0);

  menu->priv->scroll_down = gtk_button_new ();
  menu->priv->scroll_up   = gtk_button_new ();

  g_object_set (G_OBJECT (menu->priv->scroll_up),
		"can-focus", FALSE,
		NULL);
  
  g_object_set (G_OBJECT (menu->priv->scroll_down),
		"can-focus", FALSE,
		NULL);
  
  g_signal_connect (menu->priv->scroll_down,
		    "pressed",
		    G_CALLBACK (hildon_desktop_popup_menu_scroll_start),
		    (gpointer)menu);

  g_signal_connect (menu->priv->scroll_down,
		    "released",
		    G_CALLBACK (hildon_desktop_popup_menu_scroll_stop),
		    (gpointer)menu);

  g_signal_connect (menu->priv->scroll_up,
		    "pressed",
		    G_CALLBACK (hildon_desktop_popup_menu_scroll_start),
		    (gpointer)menu);

  g_signal_connect (menu->priv->scroll_up,
		    "released",
		    G_CALLBACK (hildon_desktop_popup_menu_scroll_stop),
		    (gpointer)menu);

  gtk_box_pack_start (GTK_BOX (menu->priv->box_buttons),
		      menu->priv->scroll_up,
		      TRUE, TRUE, 0);
  
  gtk_box_pack_start (GTK_BOX (menu->priv->box_buttons),
		      menu->priv->scroll_down,
		      TRUE, TRUE, 0);
  
  gtk_widget_show (menu->priv->scroll_down);
  gtk_widget_show (menu->priv->scroll_up);
  
  g_object_ref (G_OBJECT (menu->priv->box_buttons));
  g_object_ref_sink (GTK_OBJECT (menu->priv->box_buttons));
		      
  gtk_widget_pop_composite_child ();

  g_signal_connect (menu,
                    "expose_event",
                    G_CALLBACK (hildon_desktop_popup_menu_paint_pane), 
                    NULL);
  
  g_signal_connect (menu->priv->viewport,
                    "expose_event",
                    G_CALLBACK (hildon_desktop_popup_menu_paint_viewport), 
                    NULL);
  
  return object;
}

static void 
hildon_desktop_popup_menu_finalize (GObject *object)
{
  g_object_unref (G_OBJECT (HILDON_DESKTOP_POPUP_MENU (object)->priv->box_buttons));

  G_OBJECT_CLASS (hildon_desktop_popup_menu_parent_class)->finalize (object);	
}	

static void
hildon_desktop_popup_menu_size_allocate (GtkWidget *widget, GtkAllocation *allocation)
{
  HildonDesktopPopupMenu *menu;
  HildonDesktopPopupMenuPrivate *priv;
  GtkRequisition child_requisition;
  GtkAllocation child_allocation;
  GList *children, *iter;
  gint height = 0;
  
  menu = HILDON_DESKTOP_POPUP_MENU (widget);
  priv = menu->priv;

  iter = children = gtk_container_get_children (GTK_CONTAINER (priv->box_items));
  
  while (iter)
  {
    GtkWidget *child;

    child = iter->data;
    iter  = iter->next;

    if (GTK_WIDGET_VISIBLE (child))
    {
      gtk_widget_size_request (GTK_WIDGET (child), &child_requisition);

      child_allocation.width = allocation->width;
      child_allocation.height = child_requisition.height;

      if (priv->rtl)
        child_allocation.x = child_allocation.width;
      else
        child_allocation.x = 0;

      child_allocation.y = height;

      gtk_menu_item_toggle_size_allocate (GTK_MENU_ITEM (child),
                                          priv->toggle_size);

      gtk_widget_size_allocate (child, &child_allocation);

      height += child_requisition.height;
    }
  }

  g_list_free (children);

  GTK_WIDGET_CLASS (hildon_desktop_popup_menu_parent_class)->size_allocate (widget, allocation);
}

static void
hildon_desktop_popup_menu_size_request (GtkWidget *widget, GtkRequisition *req)
{
  HildonDesktopPopupMenu *menu;
  HildonDesktopPopupMenuPrivate *priv;
  GtkRequisition child_requisition;
  guint max_toggle_size;
  GList *children, *iter;
  
  g_return_if_fail (HILDON_DESKTOP_IS_POPUP_MENU (widget));
  g_return_if_fail (req != NULL);

  menu = HILDON_DESKTOP_POPUP_MENU (widget);
  priv = menu->priv;
  
  max_toggle_size = 0;

  iter = children = gtk_container_get_children (GTK_CONTAINER (priv->box_items));

  while (iter)
  {
    GtkMenuItem *child;
    gint toggle_size;

    child = (GtkMenuItem *) iter->data;
    iter  = iter->next;

    if (GTK_WIDGET_VISIBLE (child))
    {
      gtk_widget_size_request (GTK_WIDGET (child), &child_requisition);

      gtk_menu_item_toggle_size_request (child, &toggle_size);

      max_toggle_size = MAX (max_toggle_size, toggle_size);
    }
  }

  priv->toggle_size = max_toggle_size;

  g_list_free (children);

  GTK_WIDGET_CLASS (hildon_desktop_popup_menu_parent_class)->size_request (widget, req);
}

static void 
hildon_desktop_popup_menu_get_property (GObject *object,
                                        guint prop_id,
                                        GValue *value,
                                        GParamSpec *pspec)
{
  HildonDesktopPopupMenu *menu = HILDON_DESKTOP_POPUP_MENU (object);

  switch (prop_id)
  {
    case PROP_POPUP_ITEM_HEIGHT:
      g_value_set_uint (value, menu->priv->item_height);
      break;

    case PROP_POPUP_RESIZE_PARENT:
      g_value_set_boolean (value, menu->priv->resize_parent);
      break;

    case PROP_POPUP_PARENT:
      g_value_set_object (value, menu->priv->parent);
      break;

   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void 
hildon_desktop_popup_menu_set_property (GObject *object,
                                        guint prop_id,
                                        const GValue *value,
                                        GParamSpec *pspec)
{
  HildonDesktopPopupMenu *menu = HILDON_DESKTOP_POPUP_MENU (object);

  switch (prop_id)
  {
    case PROP_POPUP_ITEM_HEIGHT:
      menu->priv->item_height = g_value_get_uint (value);
      break;

   case PROP_POPUP_RESIZE_PARENT:
      menu->priv->resize_parent = g_value_get_boolean (value);
      break;

   case PROP_POPUP_PARENT:
      menu->priv->parent = g_value_get_object (value);
      break;

   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static gboolean 
hildon_desktop_popup_menu_motion_notify (GtkWidget      *widget,
					 GdkEventMotion *event)
{
  HildonDesktopPopupMenu *menu = HILDON_DESKTOP_POPUP_MENU (widget);
  GList *menu_items = NULL, *l;
  gint w,h,x,y;

  gtk_widget_get_pointer (GTK_WIDGET (widget), &x, &y);

  w = widget->allocation.width;
  h = widget->allocation.height;

  if (!((x >= 0) && (x <= w) && (y >= 0) && (y <= h)))
    return TRUE;    	 

  menu_items = 
    gtk_container_get_children (GTK_CONTAINER (menu->priv->box_items));

  for (l = menu_items; l != NULL; l = g_list_next (l))
  {
    if (!hildon_desktop_popup_menu_item_is_selectable (GTK_WIDGET (l->data)))
      continue;

    gtk_widget_get_pointer (GTK_WIDGET (l->data), &x, &y);

    w = GTK_WIDGET (l->data)->allocation.width;
    h = GTK_WIDGET (l->data)->allocation.height;

    if ((x >= 0) && (x <= w) && (y >= 0) && (y <= h))
    {
      if (menu->priv->selected_item != GTK_MENU_ITEM (l->data))
      {
        if (GTK_IS_ITEM (menu->priv->selected_item))
          gtk_item_deselect (GTK_ITEM (menu->priv->selected_item));
        gtk_item_select (GTK_ITEM (l->data));
        menu->priv->selected_item = GTK_MENU_ITEM (l->data);
      }

      break;
    }
  }

  g_list_free (menu_items);

  return FALSE;
}

static void 
hildon_desktop_popup_menu_update_scroll_buttons (HildonDesktopPopupMenu *menu)
{
  GtkAdjustment *adj;

  adj = gtk_viewport_get_vadjustment (GTK_VIEWPORT (menu->priv->viewport));

  gtk_widget_set_sensitive (menu->priv->scroll_up, 
		            (adj->value > 0));

  gtk_widget_set_sensitive (menu->priv->scroll_down, 
		            (adj->value < adj->upper - adj->page_size));
}

static void 
hildon_desktop_popup_menu_adjustment_changed (GtkAdjustment *adj, 
					      HildonDesktopPopupMenu *menu)
{
  hildon_desktop_popup_menu_update_scroll_buttons (menu);
}

static void
hildon_desktop_popup_menu_remove_scroll_timeout (HildonDesktopPopupMenu *menu)
{
  if (menu->priv->scroll_id > 0)
  {
    g_source_remove (menu->priv->scroll_id);
    menu->priv->scroll_id = 0;
  }
}
	
static gboolean 
hildon_desktop_popup_menu_scroll_step (gpointer data)
{
  HildonDesktopPopupMenu *menu = HILDON_DESKTOP_POPUP_MENU (data);
  GtkAdjustment *adj;
  gint delta = menu->priv->item_height + 1;
 
  adj = gtk_viewport_get_vadjustment (GTK_VIEWPORT (menu->priv->viewport));

  delta *= menu->priv->scroll_dir;
  
  if ((gint) (adj->value + (gdouble) delta) >= 0 &&
      (gint) (adj->value + (gdouble) delta) <= adj->upper - adj->page_size)
  {
    adj->value += (gdouble) delta; 
    gtk_adjustment_value_changed (adj);
  }
  else
  {
    if (delta > 0)
      adj->value = adj->upper - adj->page_size;
    else
      adj->value = 0;

    gtk_adjustment_value_changed (adj);
    return FALSE;
  }

  return TRUE;
}

static gboolean
hildon_desktop_popup_menu_scroll_initial_step (gpointer data)
{
  HildonDesktopPopupMenu *menu = HILDON_DESKTOP_POPUP_MENU (data);

  hildon_desktop_popup_menu_remove_scroll_timeout (menu);
  
  menu->priv->scroll_id = g_timeout_add (MENU_SCROLL_TIMEOUT, 
					 hildon_desktop_popup_menu_scroll_step, 
					 menu);

  return FALSE;
}

static void 
hildon_desktop_popup_menu_scroll_start (GtkWidget *widget, HildonDesktopPopupMenu *menu)
{
  if (widget == menu->priv->scroll_up)
    menu->priv->scroll_dir = -1;
  else
    menu->priv->scroll_dir = 1;

  hildon_desktop_popup_menu_scroll_step (menu);

  menu->priv->scroll_id = g_timeout_add (MENU_SCROLL_INITIAL_TIMEOUT, 
					 hildon_desktop_popup_menu_scroll_initial_step, 
					 menu);
}

static void 
hildon_desktop_popup_menu_scroll_stop (GtkWidget *widget, HildonDesktopPopupMenu *menu)
{
  hildon_desktop_popup_menu_remove_scroll_timeout (menu);
}

static gboolean 
hildon_desktop_popup_menu_release_event (GtkWidget      *widget,
                                         GdkEventButton *event)
{
  HildonDesktopPopupMenu *menu = HILDON_DESKTOP_POPUP_MENU (widget);
  gint w,h,x,y;

  if (!menu->priv->selected_item)
    return TRUE;
  
  gtk_widget_get_pointer (GTK_WIDGET (menu->priv->selected_item), &x, &y);

  w = GTK_WIDGET (menu->priv->selected_item)->allocation.width;
  h = GTK_WIDGET (menu->priv->selected_item)->allocation.height;

  if ((x >= 0) && (x <= w) && (y >= 0) && (y <= h))
  {
    gtk_menu_item_activate (menu->priv->selected_item);
  }

  return TRUE;
} 

static gboolean 
hildon_desktop_popup_menu_press_event (GtkWidget      *widget,
                                       GdkEventButton *event)
{
  HildonDesktopPopupMenu *menu = HILDON_DESKTOP_POPUP_MENU (widget);
  GList *menu_items = NULL, *l;
  gint w,h,x,y;

  menu_items =
    gtk_container_get_children (GTK_CONTAINER (menu->priv->box_items));

  for (l = menu_items; l != NULL; l = g_list_next (l))
  {
    if (!hildon_desktop_popup_menu_item_is_selectable (GTK_WIDGET (l->data)))
      continue;

    gtk_widget_get_pointer (GTK_WIDGET (l->data), &x, &y);

    w = GTK_WIDGET (l->data)->allocation.width;
    h = GTK_WIDGET (l->data)->allocation.height;

    if ((x >= 0) && (x <= w) && (y >= 0) && (y <= h))
    {
      if (GTK_IS_MENU_ITEM (l->data))
      {
        if (menu->priv->selected_item != GTK_MENU_ITEM (l->data))
        {
          if (GTK_IS_ITEM (menu->priv->selected_item))
            gtk_item_deselect (GTK_ITEM (menu->priv->selected_item));
          gtk_item_select (GTK_ITEM (l->data));
          menu->priv->selected_item = GTK_MENU_ITEM (l->data);
        }

        break;
      }
    }
  }

  g_list_free (menu_items);

  return TRUE;
}

static void 
hildon_desktop_menu_check_scroll_item (HildonDesktopPopupMenu *menu)
{
  GtkWidget *view = menu->priv->box_items;
  GtkWidget *menu_item = GTK_WIDGET (menu->priv->selected_item);
  GtkWidget *viewport = menu->priv->viewport;
  GtkAdjustment *adj;
  GtkRequisition req;
  gint visible_y;

  if (menu_item == NULL) 
    return;
  
  gtk_widget_size_request (menu->priv->parent, &req);

  adj = gtk_viewport_get_vadjustment (GTK_VIEWPORT (menu->priv->viewport));
  
  g_return_if_fail ((adj->upper - adj->lower) && view->allocation.height);

  visible_y = view->allocation.y +
     (gint)(view->allocation.height * adj->value / (adj->upper - adj->lower));

  if (menu_item->allocation.y < visible_y)
  {
    adj->value = menu_item->allocation.y * (adj->upper - adj->lower)
                                        / view->allocation.height;
    gtk_adjustment_value_changed (adj);
  }
  else if (menu_item->allocation.y + menu_item->allocation.height > 
           visible_y + viewport->allocation.height)
  {
    adj->value = (menu_item->allocation.y + menu_item->allocation.height
           - viewport->allocation.height) * (adj->upper - adj->lower)
           / view->allocation.height;
    gtk_adjustment_value_changed (adj);
  }
}

static gboolean 
hildon_desktop_popup_menu_key_press_event (GtkWidget   *widget,
					   GdkEventKey *event)
{
  HildonDesktopPopupMenu *menu = HILDON_DESKTOP_POPUP_MENU (widget);

  if (event->keyval == GDK_Up ||
      event->keyval == GDK_KP_Up)
  {
    hildon_desktop_popup_menu_select_next_prev_item (menu, FALSE);
    return TRUE;
  }
  else
  if (event->keyval == GDK_Down ||
      event->keyval == GDK_KP_Down)
  {
    hildon_desktop_popup_menu_select_next_prev_item (menu, TRUE);
    return TRUE;
  }
  else
  if (event->keyval == GDK_Return   ||
      event->keyval == GDK_KP_Enter ||
      event->keyval == GDK_ISO_Enter)
  {	  
    if (menu->priv->selected_item)
      gtk_menu_item_activate (menu->priv->selected_item);
  }

  return FALSE;	
}

static void 
hildon_desktop_popup_menu_show_controls (HildonDesktopPopupMenu *menu)
{	
  if (!menu->priv->controls_on)	
  {	 	  
    gtk_box_pack_start (GTK_BOX (menu),
		        menu->priv->box_buttons,
		        FALSE, FALSE, 0);

    gtk_widget_show (menu->priv->box_buttons);

    menu->priv->controls_on = TRUE;

    hildon_desktop_popup_menu_update_scroll_buttons (menu);

    g_signal_emit_by_name (menu, "show-controls", TRUE);
  }
}

static void 
hildon_desktop_popup_menu_hide_controls (HildonDesktopPopupMenu *menu)
{
  if (menu->priv->controls_on)
  {
    gtk_container_remove (GTK_CONTAINER (menu),
                          menu->priv->box_buttons);	    
    menu->priv->controls_on = FALSE;

    g_signal_emit_by_name (menu, "show-controls", FALSE);
  }
}

void 
hildon_desktop_popup_menu_parent_size (HildonDesktopPopupMenu *menu)
{
  GtkRequisition req, parent_req;
  GList *children = NULL, *l;
  gint d_height = 0;
  gint max_height;
  gboolean show_scroll_controls = FALSE;

  if (menu->priv->parent)
    gtk_widget_size_request (menu->priv->parent, &parent_req);

  if (!menu->priv->resize_parent && menu->priv->parent)
    max_height = parent_req.height; 
  else
    max_height = gdk_screen_height ();

  children = gtk_container_get_children (GTK_CONTAINER (menu->priv->box_items));
  
  for (l = children; l != NULL; l = g_list_next (l))
  {
    if (hildon_desktop_popup_menu_item_is_valid (GTK_WIDGET (l->data)))
    {
      gtk_widget_size_request (GTK_WIDGET (l->data), &req);

      d_height += req.height;

      if (d_height > max_height)
      {	  
        d_height -= menu->priv->item_height;	  
        show_scroll_controls = TRUE;
        break;
      }
    }
  }

  if (menu->priv->resize_parent && GTK_IS_WINDOW (menu->priv->parent))
  {
    gint border_width;

    g_object_get (G_OBJECT (menu->priv->parent),
		  "border-width", &border_width,
		  NULL); 
    
    gtk_widget_set_size_request (menu->priv->parent,
		    		 parent_req.width,
				 show_scroll_controls ? 
				 max_height : 
				 d_height + (2 * border_width));

    if (GTK_WIDGET_REALIZED (menu->priv->parent))
      gdk_window_resize (menu->priv->parent->window,
		         parent_req.width,
			 show_scroll_controls ? 
			 max_height : 
	                 d_height + (2 * border_width));

    if (GTK_WIDGET_MAPPED (menu->priv->parent))
    {
      g_signal_emit_by_name (menu, "popup-menu-resize");

      gtk_widget_queue_resize (GTK_WIDGET (menu)); 
      gtk_widget_queue_resize (menu->priv->parent);
    }
  }

  if (show_scroll_controls)
  {
    hildon_desktop_popup_menu_show_controls (menu);	  
    gtk_widget_set_size_request 
       (menu->priv->viewport, -1, max_height - menu->priv->item_height - 9);
  }
  else
  {
    hildon_desktop_popup_menu_hide_controls (menu);
    gtk_widget_set_size_request
      (menu->priv->viewport, -1, d_height);
  }

  gtk_widget_queue_resize (menu->priv->viewport);
 
  g_list_free (children);
}	

static void 
hildon_desktop_popup_menu_select_next_prev_item (HildonDesktopPopupMenu *menu, gboolean next)
{
  GList *children, *item, *l;
  GtkMenuItem *previous_selected_item;
  
  if (!menu->priv->selected_item)
    return;	  

  previous_selected_item = menu->priv->selected_item;

  children = gtk_container_get_children (GTK_CONTAINER (menu->priv->box_items));
  
  if (children)
  {
    l = item = g_list_find (children, menu->priv->selected_item);

    while (l)
    {
      if (l != item)
      {
        if (l->data && hildon_desktop_popup_menu_item_is_selectable (GTK_WIDGET (l->data)))
        {
          hildon_desktop_popup_menu_select_item (menu, GTK_MENU_ITEM (l->data));
	  break;
        }		
      }	

      if (next)
        l = l->next;
      else
	l = l->prev;      
    }	    

    if (previous_selected_item == menu->priv->selected_item)
    {
      if (next) 
        hildon_desktop_popup_menu_select_first_item (menu); 
      else
        hildon_desktop_popup_menu_select_last_item (menu); 
    }
      
    g_list_free (children);
  }	  
}

static void 
hildon_desktop_popup_menu_real_add_item (HildonDesktopPopupMenu *menu, 
				         GtkMenuItem *item, 
					 gint item_width)
{
  gtk_box_pack_end (GTK_BOX (menu->priv->box_items),
		    GTK_WIDGET (item),
		    FALSE, FALSE, 0);

  if (GTK_IS_SEPARATOR_MENU_ITEM (item))
  {
    GtkRequisition req_sep;

    gtk_widget_size_request (GTK_WIDGET (item), &req_sep);

    gtk_widget_set_size_request (GTK_WIDGET (item), item_width, req_sep.height);
  }
  else
  {
    gtk_widget_set_size_request (GTK_WIDGET (item), item_width, menu->priv->item_height);
  }
  
  menu->priv->n_items++;
}

static void
hildon_desktop_popup_menu_real_remove_item (HildonDesktopPopupMenu *menu, GtkMenuItem *item)
{
  gtk_container_remove (GTK_CONTAINER (menu->priv->box_items), GTK_WIDGET (item));

}

static void
hildon_desktop_popup_menu_box_remove (HildonDesktopPopupMenu *menu,
                                      GtkWidget *widget)
{
  if (GTK_IS_MENU_ITEM (widget))
  {
    GtkMenuItem *item = (GtkMenuItem *)widget;

    if (menu->priv->selected_item == item)
      hildon_desktop_popup_menu_deselect_item (menu, item);

    menu->priv->n_items--;
  }

}

void
hildon_desktop_popup_menu_add_item (HildonDesktopPopupMenu *menu, GtkMenuItem *item)
{
  GtkRequisition req;
  gint item_width = -1;
  
  g_return_if_fail (HILDON_DESKTOP_IS_POPUP_MENU (menu));
  g_return_if_fail (GTK_IS_MENU_ITEM (item));

  if (GTK_IS_WINDOW (menu->priv->parent))
  {
      gtk_widget_size_request (menu->priv->parent, &req);
      item_width = req.width;
  }

  hildon_desktop_popup_menu_real_add_item (menu, item, item_width);
  
  gtk_widget_set_size_request (menu->priv->box_buttons, 
		  	       item_width, 
			       menu->priv->item_height + 2);

  hildon_desktop_popup_menu_parent_size (menu);
}

void
hildon_desktop_popup_menu_replace_items (HildonDesktopPopupMenu *menu, GList *items)
{
  GtkRequisition req;
  GList *children, *l;
  gint item_width = -1;

  g_assert (HILDON_DESKTOP_IS_POPUP_MENU (menu));

  if (GTK_IS_WINDOW (menu->priv->parent))
  {
      gtk_widget_size_request (menu->priv->parent, &req);
      item_width = req.width;
  }

  children = hildon_desktop_popup_menu_get_children (menu);
 
  for (l = children; l != NULL; l = l->next)
  {
    GtkWidget *item = GTK_WIDGET (l->data);
    
    hildon_desktop_popup_menu_real_remove_item (menu, GTK_MENU_ITEM (item));
  }
  g_list_free (children);
 
  for (l = items; l != NULL; l = l->next)
  {
    GtkWidget *item = GTK_WIDGET (l->data);

    if (GTK_IS_MENU_ITEM (item))
      hildon_desktop_popup_menu_real_add_item (menu, 
		                               GTK_MENU_ITEM (item), 
				   	       item_width);
  }
  
  gtk_widget_set_size_request (menu->priv->box_buttons, 
		  	       item_width, 
			       menu->priv->item_height + 2);

  hildon_desktop_popup_menu_parent_size (menu);
}

void 
hildon_desktop_popup_menu_remove_item (HildonDesktopPopupMenu *menu, GtkMenuItem *item)
{
  g_assert (HILDON_DESKTOP_IS_POPUP_MENU (menu));
  g_return_if_fail (GTK_IS_MENU_ITEM (item));

  hildon_desktop_popup_menu_real_remove_item (menu, item);
  
  hildon_desktop_popup_menu_parent_size (menu);

}

GList *
hildon_desktop_popup_menu_get_children (HildonDesktopPopupMenu *menu)
{
  GList *list = NULL, *children, *l;
	
  g_return_val_if_fail (HILDON_DESKTOP_IS_POPUP_MENU (menu), NULL);

  children = gtk_container_get_children (GTK_CONTAINER (menu->priv->box_items));

  for (l = children; l != NULL; l = l->next)
  {
    if (hildon_desktop_popup_menu_item_is_valid (GTK_WIDGET (l->data)))
    {
      list = g_list_append (list, l->data);
    }
  }

  g_list_free (children);
  
  return list;
}

void 
hildon_desktop_popup_menu_select_item (HildonDesktopPopupMenu *menu, GtkMenuItem *item)
{
  g_return_if_fail (HILDON_DESKTOP_IS_POPUP_MENU (menu));
  g_return_if_fail (GTK_IS_MENU_ITEM (item));

  if (hildon_desktop_popup_menu_item_is_selectable (GTK_WIDGET (item)))
  {
    if (GTK_IS_ITEM (menu->priv->selected_item))
      gtk_item_deselect (GTK_ITEM (menu->priv->selected_item));	    

    gtk_item_select (GTK_ITEM (item));
    menu->priv->selected_item = item;
    hildon_desktop_menu_check_scroll_item (menu);
  }
} 

void 
hildon_desktop_popup_menu_deselect_item (HildonDesktopPopupMenu *menu, GtkMenuItem *item)
{
  g_return_if_fail (HILDON_DESKTOP_IS_POPUP_MENU (menu));

  if (menu->priv->selected_item == item)
  {
    gtk_item_deselect (GTK_ITEM (item));
    menu->priv->selected_item = NULL;
  }
} 

void
hildon_desktop_popup_menu_select_first_item (HildonDesktopPopupMenu *menu)
{
  GList *children = NULL, *l;

  g_return_if_fail (HILDON_DESKTOP_IS_POPUP_MENU (menu));

  children = gtk_container_get_children (GTK_CONTAINER (menu->priv->box_items));

  for (l = children; l != NULL; l = g_list_next (l))
  {	  
    if (hildon_desktop_popup_menu_item_is_selectable (GTK_WIDGET (l->data)))
    {
      if (GTK_IS_ITEM (menu->priv->selected_item))
        gtk_item_deselect (GTK_ITEM (menu->priv->selected_item));

      gtk_item_select (GTK_ITEM (l->data));
      menu->priv->selected_item = GTK_MENU_ITEM (l->data);
      hildon_desktop_menu_check_scroll_item (menu);

      break;
    }	    
  }

  if (children)
    g_list_free (children);
}

void
hildon_desktop_popup_menu_select_last_item (HildonDesktopPopupMenu *menu)
{
  GList *children = NULL, *l;

  g_return_if_fail (HILDON_DESKTOP_IS_POPUP_MENU (menu));

  children = gtk_container_get_children (GTK_CONTAINER (menu->priv->box_items));

  for (l = g_list_last (children); l != NULL; l = g_list_previous (l))
  {
    if (hildon_desktop_popup_menu_item_is_selectable (GTK_WIDGET (l->data)))
    {
      if (GTK_IS_ITEM (menu->priv->selected_item))
        gtk_item_deselect (GTK_ITEM (menu->priv->selected_item));

      gtk_item_select (GTK_ITEM (l->data));
      menu->priv->selected_item = GTK_MENU_ITEM (l->data);
      hildon_desktop_menu_check_scroll_item (menu);

      break;
    }
  }

  if (children)
    g_list_free (children);
}

void
hildon_desktop_popup_menu_activate_item (HildonDesktopPopupMenu *menu, GtkMenuItem *item)
{	
  GList *children = NULL, *l;

  g_return_if_fail (HILDON_DESKTOP_IS_POPUP_MENU (menu));

  children = gtk_container_get_children (GTK_CONTAINER (menu->priv->box_items));

  for (l = children; l != NULL; l = g_list_next (l))
  {
    if (l->data == item)
    {
      gtk_menu_item_activate (GTK_MENU_ITEM (item));      
      break;
    }
  }

  if (children)
    g_list_free (children);				  
}	

GtkMenuItem *
hildon_desktop_popup_menu_get_selected_item (HildonDesktopPopupMenu *menu)
{
  return menu->priv->selected_item;
}

void 
hildon_desktop_popup_menu_select_next_item (HildonDesktopPopupMenu *menu)
{
  hildon_desktop_popup_menu_select_next_prev_item (menu, TRUE);
}	

void 
hildon_desktop_popup_menu_select_prev_item (HildonDesktopPopupMenu *menu)
{
  hildon_desktop_popup_menu_select_next_prev_item (menu, FALSE); 
}	

void   
hildon_desktop_popup_menu_scroll_to_selected (HildonDesktopPopupMenu *menu)
{
  g_return_if_fail (HILDON_DESKTOP_IS_POPUP_MENU (menu));

  if (!menu->priv->controls_on)
    return;	  
 
  if (menu->priv->selected_item)
    hildon_desktop_menu_check_scroll_item (menu);
}

const GtkWidget *
hildon_desktop_popup_menu_get_scroll_button_up (HildonDesktopPopupMenu *menu)
{
  return menu->priv->scroll_up;
}	

const GtkWidget *
hildon_desktop_popup_menu_get_scroll_button_down (HildonDesktopPopupMenu *menu)
{
  return menu->priv->scroll_down;
}	

GtkAdjustment *
hildon_desktop_popup_menu_get_adjustment (HildonDesktopPopupMenu *menu)
{
  g_return_val_if_fail (HILDON_DESKTOP_IS_POPUP_MENU (menu), NULL);

  return gtk_viewport_get_vadjustment (GTK_VIEWPORT (menu->priv->viewport));
}
