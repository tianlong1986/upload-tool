#!/bin/sh
write_linpus_ap2_conf()
{
echo "
ESSID="linpus-ap2"
MODE=Managed
BSSID=F0:7D:68:7C:B5:FA
KEY_MGMT=WPA-PSK
TYPE=Wireless
BOOTPROTO=dhcp
DEFROUTE=yes
PEERDNS=yes
PEERROUTES=yes
IPV4_FAILURE_FATAL=yes
NAME="Wireless connection linpus-ap2"
UUID=6de6816f-9b40-423c-943d-da40a8077acf
ONBOOT=yes
" >/etc/sysconfig/network-scripts/ifcfg-wireless-linpus-ap2

 echo "WPA_PSK=\"warrencoles\"" > /etc/sysconfig/network-scripts/keys-wireless-linpus-ap2
}

write_linpus_armtp_conf()
{
 echo "
ESSID="Linpus-ArmTP"
MODE=Managed
BSSID=00:26:5A:B0:C6:CA
SECURITYMODE=open
DEFAULTKEY=1
TYPE=Wireless
BOOTPROTO=dhcp
DEFROUTE=yes
PEERDNS=yes
PEERROUTES=yes
IPV4_FAILURE_FATAL=yes
NAME=Wireless-Linpus-ArmTP
UUID=9678925d-1ae5-4a37-9b68-28c8f9884698
ONBOOT=yes
" >/etc/sysconfig/network-scripts/ifcfg-wireless-linpus-armtp

echo "KEY1=s:98765" > /etc/sysconfig/network-scripts/keys-wireless-linpus-armtp
}

ping_test()
{
ping www.baidu.com -c 5
if [ $? = 0 ]; then
	echo "connection ping Baidu successful"|tee -a wifi_result.log
	return 0
else
	echo "connection ping Baidu failed"|tee  -a wifi_result.log
	return 1
fi
}

ftp_download_big_buck_test()
{
echo "now download big-buck-bunny.ogv..."
#wget ftp://olpc:olpc@192.168.1.200/tmp/LinuxPkg_5.16.tar.bz2 --delete-after -o wget.log
wget ftp://olpc:olpc@192.168.1.200/QA/test_samples/Video/big-buck-bunny.ogv -o wget.log

if [ $? = 0 ]; then
	download_speed=`cat wget.log|grep saved | sed 's/.*(\(.*\)).*/\1/'`
	echo "FTP download speed:$download_speed"|tee -a wifi_result.log
	return 0
else
	echo "FTP download file test failed"|tee  -a wifi_result.log
	return 1
fi
}
ftp_download_ogg_test()
{
#wget ftp://olpc:olpc@192.168.1.200/tmp/LinuxPkg_5.16.tar.bz2 --delete-after -o wget.log
echo "now download Sample.ogg..."
wget ftp://olpc:olpc@192.168.1.200/QA/test_samples/Media_Type\(Sample\)/Sample.ogg -o wget.log
if [ $? = 0 ]; then
	download_speed=`cat wget.log|grep saved | sed 's/.*(\(.*\)).*/\1/'`
	echo "FTP download speed:$download_speed"|tee -a wifi_result.log
	return 0
else
	echo "FTP download file test failed"|tee -a wifi_result.log
	return 1
fi
}
#clear old wifi config files
clear_all()
{
	sudo rm -rf /etc/sysconfig/network-scripts/*Linpus-ArmTP
	sudo rm -rf /etc/sysconfig/network-scripts/*linpus-ap2
	cat /tmp/autotest.ini|grep network_info
	if [ $? = 0 ];then
		sed -i -e '/network_info/,/ping=/d' /tmp/autotest.ini	
	fi
}

#begin to test
clear_all
WIRED_DEV=`cat /proc/net/dev |grep :|awk '{print $1}'|grep -v [wlan,wwan,lo]|sed 's/:$//'`
sudo ifconfig $WIRED_DEV down
sleep 1
#test Linpus-ArmTp
echo "Begin to test Linpus-ArmTP"
write_linpus_armtp_conf
sleep 1
echo "Connect Linpus-ArmTP ..."
nmcli con up uuid 9678925d-1ae5-4a37-9b68-28c8f9884698 --timeout 35
if [ $? = 0 ];then
	ARMTP_CONN=1
else
	ARMTP_CONN=3
	FAIL_REASON="Linpus-ArmTP connected fail,"
fi
echo "Linpus-ArmTP Connected."|tee -a wifi_result.log
ping_test
if [ $? = 0 ];then
	ARMTP_PING=1
else
	ARMTP_PING=3
	FAIL_REASON=$FAIL_REASON"ping baidu.com fail,"
fi
ftp_download_big_buck_test
if [ $? = 0 ];then
	ARMTP_FTP=1
else
	ARMTP_FTP=3
	FAIL_REASON=$FAIL_REASON"download big buck from ftp200 fail,"
fi
echo "Linpus_ArmTP connection test finish"
echo "Disconnect Linpus_ArmTP"
nmcli con down uuid 9678925d-1ae5-4a37-9b68-28c8f9884698
sleep 1
FAIL_REASON=$FAIL_REASON". "
#test linpus-ap2
echo "Begin to test linpus-ap2"
write_linpus_ap2_conf
sleep 3
echo "Connect linpus-ap2..."
nmcli con up uuid 6de6816f-9b40-423c-943d-da40a8077acf --timeout 35
if [ $? = 0 ];then
	AP2_CONN=1
else
	AP2_CONN=3
	FAIL_REASON="linpus-ap2 connected fail,"
fi
echo "linpus-ap2 Connected."|tee -a wifi_result.log

ping_test
if [ $? = 0 ];then
	AP2_PING=1
else
	AP2_PING=3
	FAIL_REASON=$FAIL_REASON"ping baidu.com fail,"
fi
ftp_download_ogg_test
if [ $? = 0 ];then
	AP2_FTP=1
else
	AP2_FTP=3
	FAIL_REASON=$FAIL_REASON"download a ogg file from ftp200 fail,"
fi
echo "linpus-ap2 connection test finish"
#处理结果
if [ $ARMTP_PING = 1 ] && [ $AP2_PING = 1 ];then
	PING="P"
else
	PING="F"
fi
if [ $ARMTP_FTP = 1 ] && [ $AP2_FTP = 1 ];then
	FTP_DOWNLOAD="P"
else
	FTP_DOWNLOAD="F"
fi
if [ $ARMTP_CONN = 1 ] && [ $AP2_CONN = 1 ];then
	CONNECTED="P"
else
	CONNECTED="F"
fi
if [ $CONNECTED = "P" ] && [ $PING = "P" ] && [ $FTP_DOWNLOAD = "P" ];then
	RESULT="P"
else
	RESULT="F"
fi
ifconfig $WIRED_DEV up
LAN_MAC=`ifconfig |grep $WIRED_DEV|awk '{print $5}'`
DATE=`date +%Y-%m-%d`
OS_VER=`cat /etc/linpus-subversion`
echo "
[network_info]
result=$RESULT
connected=$CONNECTED
ftp_download=$FTP_DOWNLOAD
fail_reason=$FAIL_REASON
comment=
date=$DATE
lan_mac=$LAN_MAC
os_ver=$OS_VER
ping=$PING" >> /tmp/autotest.ini
