/*
 * This file is part of python-hildondesktop
 *
 * Copyright (C) 2007 Nokia Corporation.
 *
 * Author:  Lucas Rocha <lucas.rocha@nokia.com>
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

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <libhildondesktop/hildon-desktop-item.h>
#include <libhildondesktop/statusbar-item.h>
#include <libhildondesktop/hildon-desktop-toggle-button.h>

#define HD_SB_BATTERY_PLUGIN   "hildon-status-bar-battery.desktop"
#define HD_SB_INTERNET_PLUGIN  "hildon-status-bar-internet.desktop"
#define HD_SB_SOUND_PLUGIN     "sound.desktop"
#define HD_SB_DISPLAY_PLUGIN   "hildon-status-bar-display.desktop"
#define HD_SB_PRESENCE_PLUGIN  "presence.desktop"
#define HD_SB_CONTACT_PLUGIN   "contact.desktop"
#define HD_SB_BLUETOOTH_PLUGIN "hildon-status-bar-bluetooth.desktop"
#define HD_SB_LOCATION_PLUGIN  "location.desktop"
#define HD_SB_ALARM_PLUGIN     "hildon-status-bar-alarm.desktop"
#define HD_SB_USB_PLUGIN       "hildon-status-bar-usb.desktop"

G_MODULE_EXPORT GList             *hd_ui_policy_module_filter_plugin_list (GList *plugin_list);
G_MODULE_EXPORT gchar             *hd_ui_policy_module_get_default_item   (gint position);
G_MODULE_EXPORT HildonDesktopItem *hd_ui_policy_module_get_failure_item   (gint position);

GList *
hd_ui_policy_module_filter_plugin_list (GList *plugin_list)
{
  GList *f_plugin_list = NULL;

#ifdef HAVE_SAFE_MODE
  if (plugin_list == NULL)
  {
    g_debug ("NAO OK!");
    
    f_plugin_list = g_list_append (f_plugin_list, g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_BATTERY_PLUGIN));
    f_plugin_list = g_list_append (f_plugin_list, g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_INTERNET_PLUGIN));
    f_plugin_list = g_list_append (f_plugin_list, g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_SOUND_PLUGIN));
    f_plugin_list = g_list_append (f_plugin_list, g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_DISPLAY_PLUGIN));
    f_plugin_list = g_list_append (f_plugin_list, g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_PRESENCE_PLUGIN));
    f_plugin_list = g_list_append (f_plugin_list, g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_CONTACT_PLUGIN));
    f_plugin_list = g_list_append (f_plugin_list, g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_BLUETOOTH_PLUGIN));
    f_plugin_list = g_list_append (f_plugin_list, g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_LOCATION_PLUGIN));
    f_plugin_list = g_list_append (f_plugin_list, g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_ALARM_PLUGIN));
    f_plugin_list = g_list_append (f_plugin_list, g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_USB_PLUGIN));
  }
#endif
  
  return f_plugin_list;
}

gchar *
hd_ui_policy_module_get_default_item (gint position)
{
  gchar *plugin_id = NULL;

  switch (position)
  {
    case 0:
      plugin_id = g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_BATTERY_PLUGIN);
      break;

    case 1:
      plugin_id = g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_INTERNET_PLUGIN);
      break;

    case 2:
      plugin_id = g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_SOUND_PLUGIN);
      break;

    case 3:
      plugin_id = g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_DISPLAY_PLUGIN);
      break;

    case 4:
      plugin_id = g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_PRESENCE_PLUGIN);
      break;

    case 5:
      plugin_id = g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_CONTACT_PLUGIN);
      break;

    case 6:
      plugin_id = g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_BLUETOOTH_PLUGIN);
      break;

    case 7:
      plugin_id = g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_LOCATION_PLUGIN);
      break;

    case 8:
      plugin_id = g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_ALARM_PLUGIN);
      break;

    case 9:
      plugin_id = g_strdup (HD_SB_ENTRY_PATH "/" HD_SB_USB_PLUGIN);
      break;
  }

  return plugin_id;
}

HildonDesktopItem *
hd_ui_policy_module_get_failure_item (gint position)
{
  return NULL;
}
