#!/bin/bash
echo "install vim"
sudo apt-get install vim

echo "update & upgrade apt-get"
sudo apt-get update
sudo apt-get upgrade

echo "install dnsmasq & hostapd"
sudo apt-get install dnsmasq hostapd

echo "update /etc/dhcpcd.conf"
echo "copy of /etc/dhcpcd.conf is /etc/dhcpcd.bak"
cd /etc
#make backup file
sudo cp dhcpcd.conf dhcpcd.bak

#add "denyinterfaces wlan0" to last line of /etc/dhcpcd.conf
fileName="dhcpcd.conf"
sudo sed -i '$ a\denyinterfaces wlan0' $fileName

#confirm by user
sudo vim /etc/dhcpcd.conf

#======================================
cd
cd /etc/network
fileName="interfaces"
#find line to change
line=$(perl -lne 'm|iface wlan0 inet manual| && print $.' $fileName)
echo $line

#change to "iface wlan0 inet static"
sudo sed -i "$line s/.*/iface wlan0 inet static/g" $fileName

#make line to point next
line=`expr $line + 1`
echo $line

#add ip data
sudo sed -i "$line s/.*/\taddress 172.24.1.1\n\tnetmask 255.255.255.0\
    \n\tnetwork 172.24.1.0\n\tbroadcast 172.24.1.255\n/g" $fileName

#add "#    wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf"
sudo sed -i '20i\#    wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf' $fileName

#confirm by user (don'n make as a comment/ need to change addr)
echo "need to change addr"
sudo vim /etc/network/interfaces

sudo service dhcpcd restart
sudo ifdown wlan0; sudo ifup wlan0
#======================================
echo "CONFIGURE HOSTAPD"
cd
sudo cp hostapd.conf /etc/hostapd/

#confirm by user (change SSID if you need)
echo "change SSID if you need"
sudo vim /etc/hostapd/hostapd.conf

#check
# sudo /usr/sbin/hostapd /etc/hostapd/hostapd.conf
# echo "ctrl+c to stop"

#update hostapd
cd /etc/default
fileName="hostapd"

#change to "DAEMON_CONF="/etc/hostapd/hostapd.conf""
sudo sed -i '10i\DAEMON_CONF="/etc/hostapd/hostapd.conf"' $fileName
#confirm by user (remove after confirm)
# sudo vim /etc/default/hostapd

#======================================
echo "CONFIGURE DNSMASQ"
sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
cd
sudo cp dnsmasq.conf /etc

#confirm by user (remove after confirm)
# sudo vim /etc/dnsmasq.conf
#======================================
#enable ip4
cd /etc
fileName="sysctl.conf"
line=28
sudo sed -i "$line s/.*/net.ipv4.ip_forward=1/g" $fileName

#confirm by user (remove after confirm)
#sudo vim /etc/sysctl.conf

#update config right now
sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"

#set NAT
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo iptables -A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT
sudo iptables -A FORWARD -i wlan0 -o eth0 -j ACCEPT

#save rules
sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"

#let pi to know to run this after every reboot
cd /etc
fileName="rc.local"
sudo sed -i '19i\iptables-restore < /etc/iptables.ipv4.nat' $fileName

#confirm by user (remove after confirm)
#sudo vim /etc/rc.local

#start service
sudo service hostapd start
sudo service dnsmasq start





