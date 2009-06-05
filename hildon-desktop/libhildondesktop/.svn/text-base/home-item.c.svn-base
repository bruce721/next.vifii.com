/*
 * This file is part of hildon-desktop
 *
 * Copyright (C) 2007 Nokia Corporation.
 *
 * Contact: Karoliina Salminen <karoliina.t.salminen@nokia.com>
 * Author: Johan Bilien <johan.bilien@nokia.com>
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

#include "home-item.h"


G_DEFINE_TYPE (HomeItem, home_item, HILDON_DESKTOP_TYPE_HOME_ITEM)

static void
home_item_init (HomeItem *item)
{
}

static void
home_item_class_init (HomeItemClass *klass)
{
}

void
home_item_set_resize_type (HomeItem *item,
                           HildonDesktopHomeItemResizeType resize_type)
{
  g_return_if_fail (HOME_IS_ITEM (item));

  hildon_desktop_home_item_set_resize_type (HILDON_DESKTOP_HOME_ITEM (item),
                                            resize_type);

}
