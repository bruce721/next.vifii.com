# Copyright (c) 2007 Nokia Corporation
#
# Contact: xan.lopez@nokia.com
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# 

#!/bin/sh -e
prefix=/opt/hildon
plankton=${prefix}/share/themes/plankton

unset LANG LC_CTYPE LC_NUMERIC LC_TIME LC_COLLATE LC_MONETARY LC_MESSAGES LC_PAPER LC_NAME LC_ADDRESS LC_TELEPHONE LC_MEASUREMENT LC_IDENTIFICATION LC_ALL
LANG=en_GB.utf8
export LANG

exec matchbox-window-manager \
        -theme ${plankton}/matchbox/theme.xml \
        -use_titlebar yes \
        -use_desktop_mode plain \
        -use_lowlight no \
        -use_cursor yes \
        -use_super_modal yes &
matchboxpid=$!

exec ${prefix}/lib/sapwood/sapwood-server &
sapwoodpid=$!

GTK2_RC_FILES=$HOME/.gtkrc-2.0
GTK2_RC_FILES="$GTK2_RC_FILES:${plankton}/gtk-2.0/gtkrc"
GTK2_RC_FILES="$GTK2_RC_FILES:${plankton}/gtk-2.0/gtkrc.maemo_af_desktop"
export GTK2_RC_FILES

dbus-launch hildon-desktop

kill $sapwoodpid $matchboxpid
