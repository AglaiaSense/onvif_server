#! /bin/sh
cd /dbg/hhbin
#rm dana_debug.bin
#tftp -g 10.30.31.204 -l dana_debug.bin
#chmod 777 dana_debug.bin              
#./dana_debug.bin                 
rm onvif.bin                     
tftp -g 10.30.31.204 -l onvif.bin
chmod 777 onvif.bin
./onvif.bin