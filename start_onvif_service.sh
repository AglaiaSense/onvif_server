#!/bin/sh
cp /home/root/onvif/onvif.service /etc/systemd/system/
sync
systemctl daemon-reload
sudo ln -s /lib/systemd/systemd /lib/systemd/systemd-sysv-install
systemctl enable onvif.service
systemctl start onvif.service
