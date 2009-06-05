/* 
 * This file is part of libhildonwm
 *
 * Copyright (C) 2005, 2006, 2007 Nokia Corporation.
 *
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gdk/gdkx.h>

#include "hd-wm-util.h"


gulong 
hd_wm_util_getenv_long (gchar *env_str, gulong val_default)
{
    gchar *val_str;
    gulong val;

    if ((val_str = getenv(env_str)) != NULL)
      val = strtoul(val_str, NULL, 10);
    else
      val = val_default;

    return val;
}

void*
hd_wm_util_get_win_prop_data_and_validate (Window     xwin, 
					   Atom       prop, 
					   Atom       type, 
					   int        expected_format,
					   int        expected_n_items,
					   int       *n_items_ret)
{
  Atom           type_ret;
  int            format_ret;
  unsigned long  items_ret;
  unsigned long  after_ret;
  unsigned char *prop_data;
  int            status;

  prop_data = NULL;
  
  gdk_error_trap_push();

  status = XGetWindowProperty (GDK_DISPLAY(), 
			       xwin, 
			       prop, 
			       0, G_MAXLONG, 
			       False,
			       type, 
			       &type_ret, 
			       &format_ret, 
			       &items_ret,
			       &after_ret, 
			       &prop_data);


  if (gdk_error_trap_pop() || status != Success || prop_data == NULL)
    goto fail;

  if (expected_format && format_ret != expected_format)
    goto fail;

  if (expected_n_items && items_ret != expected_n_items)
    goto fail;

  if (n_items_ret)
    *n_items_ret = items_ret;
  
  return prop_data;

 fail:

  if (prop_data)
    XFree(prop_data);

  return NULL;
}

gboolean
hd_wm_util_send_x_message (Window        xwin_src, 
			   Window        xwin_dest, 
			   Atom          delivery_atom,
			   long          mask,
			   unsigned long data0,
			   unsigned long data1,
			   unsigned long data2,
			   unsigned long data3,
			   unsigned long data4)
{
  XEvent ev;

  memset(&ev, 0, sizeof(ev));
  ev.xclient.type = ClientMessage;

  ev.xclient.window       = xwin_src;
  ev.xclient.message_type = delivery_atom;
  ev.xclient.format       = 32;
  ev.xclient.data.l[0]    = data0;
  ev.xclient.data.l[1]    = data1;
  ev.xclient.data.l[2]    = data2;
  ev.xclient.data.l[3]    = data3;
  ev.xclient.data.l[4]    = data4;

  gdk_error_trap_push();
    
  XSendEvent(GDK_DISPLAY(), xwin_dest, mask, False, &ev);
  XSync(GDK_DISPLAY(), FALSE);
  
  if (gdk_error_trap_pop())
    {
      return FALSE;
    }

  return TRUE;
}

void
hd_wm_util_broadcast_message (Atom info, Atom begin_info, const gchar *message)
{
  Window xwindow;
	
  {
    XSetWindowAttributes attrs;

    attrs.override_redirect = True;
    attrs.event_mask = PropertyChangeMask | StructureNotifyMask;

    xwindow =
      XCreateWindow (GDK_DISPLAY(),
                     GDK_ROOT_WINDOW(),
                     -100, -100, 1, 1,
                     0,
                     CopyFromParent,
                     CopyFromParent,
                     CopyFromParent,
                     CWOverrideRedirect | CWEventMask,
                     &attrs);
  }

  {
    XEvent xevent;
    const char *src;
    const char *src_end;
    char *dest;
    char *dest_end;

    xevent.xclient.type = ClientMessage;
    xevent.xclient.message_type = begin_info;
    xevent.xclient.display = GDK_DISPLAY ();
    xevent.xclient.window = xwindow;
    xevent.xclient.format = 8;

    src = message;
    src_end = message + strlen (message) + 1; /* +1 to include nul byte */

    while (src != src_end)
    {
      dest = &xevent.xclient.data.b[0];
      dest_end = dest + 20;

      while (dest != dest_end &&
             src != src_end)
      {
         *dest = *src;
         ++dest;
         ++src;
      }

      gdk_error_trap_push ();

      XSendEvent (GDK_DISPLAY (),
                  GDK_ROOT_WINDOW (),
                  False,
                  PropertyChangeMask,
                  &xevent);
      XSync (GDK_DISPLAY (), FALSE);

      if (gdk_error_trap_pop ())
        g_warning ("Failed to broadcast startup");	      
      
      xevent.xclient.message_type = info;
    }
  }

  XDestroyWindow (GDK_DISPLAY (), xwindow);
  XFlush (GDK_DISPLAY ());
}


/* Function to retrieve the size of VmData for a process
 * Returns -1 on failure
 */
gint hd_wm_get_vmdata_for_pid(gint pid)
{
  char buf[2 * 1024]; /* path to /proc/<pid>/status file and contents of status file, 1K is enough */
  int  fd;
  int  vmdata = -1;

  snprintf(buf, sizeof(buf), "/proc/%i/status", pid);
  fd = open(buf, O_RDONLY);
  if (fd < 0)
  {
    g_print("No process data available for %s\n", buf);
  }
  else
  {
    if (read(fd, buf, sizeof(buf)) > 0)
    {
      /* Process and convert VmData string to number */
      static const char szVmData[] = "VmData:";
      const char* ptr = strstr(buf, szVmData);

      if (ptr && (vmdata = atoi(ptr + sizeof(szVmData))) <= 0)
        vmdata = -1;
    }

    close(fd);
  }

  return (gint)vmdata;
} /* hd_wm_get_vmdata_for_pid */
