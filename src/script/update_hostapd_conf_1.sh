#!/bin/bash
argc=$#
SSID=$1
passphrase=$2



function write_conf()
{
    #change /etc/hostapd/hostapd.conf
    (
        fileName="hostapd_1.conf"
        cd
        cd /etc/hostapd/hostapd_1.conf

        #find line number of ssid conf
        ssid_line=$(perl -lne 'm|This is the name of the network| && print $.' $fileName)
        ssid_line=`expr $ssid_line + 1`

        #change to "ssid=<ssid>"
        sudo sed -i "$ssid_line s/.*/ssid=$SSID/g" $fileName


        #find line number of passphrase conf
        passphrase_line=$(perl -lne 'm|The network passphrase| && print $.' $fileName)
        passphrase_line=`expr $passphrase_line + 1`

        #change to "wpa_passphrase=<passphrase>"
        sudo sed -i "$passphrase_line s/.*/wpa_passphrase=$passphrase/g" $fileName


    )
}


if [ 2 -eq $argc ]; then
    write_conf
elif [ 2 -ne $argc ]; then
    echo "Error<lack of parameters> Run in form of: ./change_conf SSID passphrase"
fi
