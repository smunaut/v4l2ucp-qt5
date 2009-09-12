#!/bin/sh
cat <<EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=v4l2ucp
GenericName=V4L2 control panel
Comment=A universal control panel for V4L2 devices
Exec=$1/bin/v4l2ucp
Categiroies=Application;Video;
Icon=$1/share/icons/v4l2ucp.png
EOF
