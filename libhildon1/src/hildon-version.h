/*
 * This file is a part of hildon
 *
 * Copyright (C) 2005, 2006, 2007 Nokia Corporation, all rights reserved.
 *
 * Contact: Michael Dominic Kostrzewa <michael.kostrzewa@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; version 2.1 of
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

#ifndef                                         __HILDON_VERSION_H__
#define                                         __HILDON_VERSION_H__

#define HILDON_MAJOR_VERSION				    (2)
#define HILDON_MINOR_VERSION				    (0)
#define HILDON_MICRO_VERSION				    (7)

#define	HILDON_CHECK_VERSION(major,minor,micro)	\
    (HILDON_MAJOR_VERSION > (major) || \
     (HILDON_MAJOR_VERSION == (major) && HILDON_MINOR_VERSION > (minor)) || \
     (HILDON_MAJOR_VERSION == (major) && HILDON_MINOR_VERSION == (minor) && \
      HILDON_MICRO_VERSION >= (micro)))

#endif                                          /* __HILDON_VERSION_H__ */

