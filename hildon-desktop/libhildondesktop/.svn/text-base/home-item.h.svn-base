/*
 * This file is part of hildon-desktop
 *
 * Copyright (C) 2007 Nokia Corporation.
 *
 * Contact: Karoliina Salminen <karoliina.t.salminen@nokia.com>
 * Author:  Johan Bilien <johan.bilien@nokia.com>
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


#ifndef __HOME_ITEM_H__
#define __HOME_ITEM_H__

#include <glib.h>
#include <glib-object.h>
#include <libhildondesktop/hildon-desktop-home-item.h>

G_BEGIN_DECLS

#define HOME_TYPE_ITEM                  (home_item_get_type ())
#define HOME_ITEM(obj)                  (GTK_CHECK_CAST (obj, HOME_TYPE_ITEM, HomeItem))
#define HOME_ITEM_CLASS(klass) \
                                        (GTK_CHECK_CLASS_CAST ((klass), HOME_TYPE_ITEM, HomeItemClass))
#define HOME_IS_ITEM(obj) \
                                        (GTK_CHECK_TYPE (obj, HOME_TYPE_ITEM))
#define HOME_IS_ITEM_CLASS(klass) \
                                        (GTK_CHECK_CLASS_TYPE ((klass), HOME_TYPE_ITEM))



typedef struct _HomeItem HomeItem;
typedef struct _HomeItemClass HomeItemClass;

struct _HomeItem {
  HildonDesktopHomeItem             parent;
};

struct _HomeItemClass {
  HildonDesktopHomeItemClass        parent_class;

};

GType       home_item_get_type (void);

void        home_item_set_resize_type           (HomeItem *item,
                                                 HildonDesktopHomeItemResizeType resize_type);

G_END_DECLS
#endif /* __HOME_ITEM_H__ */
