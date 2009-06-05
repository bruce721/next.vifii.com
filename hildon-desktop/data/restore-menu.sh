#!/bin/sh

OSSO_CONF_DIR="${HOME}/.osso"
MENU_CONF_DIR="${OSSO_CONF_DIR}/menus"
DESKTOP_CONF_DIR="${OSSO_CONF_DIR}/hildon-desktop"
OSSO_SOFTWARE_VERSION_FILE="/etc/osso_software_version"

if [ ! $2 ]
then
  exit 0
fi

if [ ! -e $OSSO_SOFTWARE_VERSION_FILE ]
then
  exit 0
fi

OSSO_VERSION=`cat ${OSSO_SOFTWARE_VERSION_FILE}`

if [ $2 != $OSSO_VERSION ]
then
  # NB#82941 - This might not be the right place to put this fix, however this file was intended to fix this problem.
  # Removing the files does not work, this leads me to believe the files were already open when the removal 
  # was supposed to take place.
  # So instead before removing the file statusbar.conf we must ensure that the update notifier is added to the configured
  # statusbar applets if the file fails to be removed later the update notifier will still be added. The extra echo is 
  # is intended to avoid some weird xkbmap bug with \n
  echo >> ${DESKTOP_CONF_DIR}/statusbar.conf
  echo "[/usr/share/applications/hildon-status-bar/hildon-update-notifier.desktop]" >> ${DESKTOP_CONF_DIR}/statusbar.conf
  echo "X-Load=true" >> ${DESKTOP_CONF_DIR}/statusbar.conf

  rm -f "${MENU_CONF_DIR}/applications.menu"
  rm -f "${DESKTOP_CONF_DIR}/*.conf"
fi

