/*
 * This file is part of maemo-af-desktop
 *
 * Copyright (C) 2006 Nokia Corporation.
 *
 * Author:  Moises Martinez <moises.martinez@nokia.com>
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

#include "tasknavigator-item.h"

/**
 * SECTION:tasknavigator-item
 * @short_description: base class from which task navigator items are derived.
 * @see_also: #HildonDesktopPanelItem
 */

G_DEFINE_TYPE (TaskNavigatorItem, tasknavigator_item, HILDON_DESKTOP_TYPE_PANEL_ITEM)

/* FIXME: change tasknavigator to task_navigator or TaskNavigator to Tasknavigator */

static void
tasknavigator_item_class_init (TaskNavigatorItemClass *klass)
{
}

static void
tasknavigator_item_init (TaskNavigatorItem *self)
{
  self->menu   = NULL;
}
