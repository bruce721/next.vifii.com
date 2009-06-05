/*
 * This file is part of maemo-af-desktop
 *
 * Copyright (C) 2006 Nokia Corporation.
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
/* Only if we have esd, also removed from configure.ac for fremantle
#define HILDON_HAS_ESD 
*/

#ifdef HILDON_HAS_ESD 
gint
hn_as_sound_init(void);

gint
hn_as_sound_register_sample (gint esd_socket, const gchar *sample);

void
hn_as_sound_deregister_sample (gint esd_socket, gint sample_id);

gint
hn_as_sound_play_sample (gint esd_socket, gint sample_id);

void
hn_as_sound_deinit (gint socket);
#endif
