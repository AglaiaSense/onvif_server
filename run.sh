#! /bin/sh
#make clean
rm bin/onvif.bin
make -j4
cd bin
pkill -9 "onvif.bin"
pkill -9 -f "gst-variable-rtsp-server -p 8554"
pkill -9 -f "gst-variable-rtsp-server -p 8555"
./onvif.bin &
