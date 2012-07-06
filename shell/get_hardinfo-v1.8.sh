#!/bin/bash

# sh get_hardinfo.sh [savefilename]
# ver 1.5
# modify 11/11/09 15:41:23


#update 
if [ "$1" == "update" ];then
	date="modify `date '+%y/%m/%d %H:%M:%S'`"
	sed -i "s@\(# \)modify.*@\1$date@" $0
	exit 0
fi
date=`date '+%y/%m/%d %H:%M:%S'`
#output to a file
if [ "$1" != "" ];then
	sh $0 | tee $1
	exit 0
fi

audio_code_file=".tmp"
hdmi_code_file="$audio_code_file"
rm -rf "$audio_code_file"
touch "$audio_code_file"

do_card(){
result=`head -n1 "$1/codec#0" | grep -i HDMI `

if [ "$result" == "" ];then
	echo "Audio Codec:" > .card$2
	audio_code_file=".card$2"

head -n6 "$1/codec#0" >> .card$2
audio_codec_name=`head -n1 "$1/codec#0"`
audio_codec_vid=`cat .card$2 |grep Vendor |awk '{print $3}'`
audio_codec_sid=`cat .card$2 |grep Subsystem |awk '{print $3}'`
audio_codec_revid=`cat .card$2 |grep Revision |awk '{print $3}'`
else
	echo "HDMI Codec:" > .card$2
	hdmi_code_file=".card$2"
head -n6 "$1/codec#0" >> .card$2
hdmi_codec_name=`head -n1 "$1/codec#0"`
hdmi_codec_vid=`cat .card$2 |grep Vendor |awk '{print $3}'`
hdmi_codec_sid=`cat .card$2 |grep Subsystem |awk '{print $3}'`
hdmi_codec_revid=`cat .card$2 |grep Revision |awk '{print $3}'`
fi

head -n6 "$1/codec#0" >> .card$2
echo  >> .card$2
}

#cpu
cpu=`cat /proc/cpuinfo | grep "model name" | cut -d":" -f 2- | head -n 1`

#chipset
chipset=" `dmesg | awk -F': ' '/Chipset/{print $2}'`"

#Memory
mem_type=`dmidecode -t 17 | grep "Type: DDR" | cut -d":" -f2 | head -n1`
mem_all=`dmidecode -t 17 | grep "Size:"  | grep -i 'MB'| cut -d":" -f2 | cut -d" " -f2`
for x in $mem_all
do
	tmp=`echo "$x 1024" | awk '{print $1 / $2}'`
	mem="$mem + $tmp"  
done
mem=`echo "$mem" | cut -d"+" -f2-`

#HDD
hdd=`hdparm -i /dev/sda | grep Model= | cut -d"," -f 1 | cut -d"=" -f 2 `
hdd_size=`fdisk -l /dev/sda| grep "Disk /dev" | head -n1 | cut -d":" -f2 | cut -d"," -f1`
sdb=`fdisk -l |grep sdb`
if [ $? -ne 0 ];then
hdd2=`hdparm -i /dev/sdb | grep Model= | cut -d"," -f 1 | cut -d"=" -f 2 `
hdd2_size=`fdisk -l /dev/sdb| grep "Disk /dev" | head -n1 | cut -d":" -f2 | cut -d"," -f1`
else
hdd2=""
hdd2_size=""
fi
#VGA
vga_2=" No"
vga_1=`lspci -nn | grep VGA | cut -d":" -f 3- | head -n1`
vga_count=`lspci -nn | grep VGA | wc -l`
[ "$vga_count" == "2" ] && vga_2=`lspci -nn | grep VGA | cut -d":" -f3- | tail -n1`

vga1_desc=`lspci -nn | grep VGA | cut -d":" -f 3- | sed -n '1p'`
vga2_desc=`lspci -nn | grep VGA | cut -d":" -f 3- | sed -n '2p'`
vga1_vendor=`lspci -nn | grep VGA | cut -d":" -f 3- | sed -n '1p' |awk '{printf $1}'`
vga2_vendor=`lspci -nn | grep VGA | cut -d":" -f 3- | sed -n '2p' |awk '{printf $1}'`
if [ "$vga2_vendor" != "" ];then
vga_vendor="${vga1_vendor}+${vga2_vendor}"
else
vga_vendor="${vga1_vendor}"
fi

vga1_id=`lspci -nn | grep VGA | cut -d":" -f 3- | sed -n '1p'|awk '{printf $4}'`
vga2_id=`lspci -nn | grep VGA | cut -d":" -f 3- | sed -n '2p'|awk '{printf $4}'`
#3D controller
controller=`lspci -nn | grep "3D controller" | cut -d":" -f 3- | head -n1`
[ "$controller" == "" ] && controller=" No"

#Audio
audio_2="              "
audio_1=`lspci -nn | grep Audio | cut -d":" -f 3- | head -n1`
audio_count=`lspci | grep Audio | wc -l`
if [ "$audio_count" == "2" ];then
	 audio_2="$audio_2`lspci -nn | grep VGA | cut -d':' -f3- | tail -n1`"
fi
audio_id=`lspci -nn |grep Audio | cut -d":" -f 3- |awk '{printf $4}'`
#Wlan
wlan=`lspci -nn | grep Network | cut -d":" -f3- `
wlan=`echo "$wlan" | sed "s@\(.*\)\(\[.*\]\)@ \2\1@"`
[ "$wlan" == "" ] && wlan=" No"

wlan_id=`lspci -nn | grep Network | cut -d":" -f3-  | sed "s@\(.*\)\(\[.*\]\)@ \2\1@" |awk '{print $1}'`
#lan
lan=`lspci -nn | grep Ethernet | cut -d":" -f3- `
lan=`echo "$lan" | sed "s@\(.*\)\(\[.*\]\)@ \2\1@"`
[ "$lan" == "" ] && lan=" No"

lan_id=`lspci -nn | grep Ethernet | cut -d":" -f3- | sed "s@\(.*\)\(\[.*\]\)@ \2\1@" |awk '{printf $1}'`
#bluetooth
bluetooth=`lsusb -vvv | grep -i "Bluetooth Module" -B13 | head -n1 | cut -d":" -f2- | sed -s "s@ID @[@" | sed -s "s@\(\w\) @\1] @"`
[ "$bluetooth" == "" ] && bluetooth=`lsusb -vvv | grep -i "BCM" -B13 | head -n1 | cut -d":" -f2- | sed -s "s@ID @[@" | sed -s "s@\(\w\) @\1] @"`
bluetooth_id=`lsusb -vvv | grep -i "Bluetooth" -B13 |grep "ID" |awk '{print $6}'`
bluetooth_desc=`lsusb -vvv | grep -i "Bluetooth" -B13 |grep "ID"`

#Touchpad
touchpad_name=`cat /proc/bus/input/devices | grep Pad | cut -d"=" -f2`
touchpad_id=`cat /proc/bus/input/devices | grep Pad -B1 | head -n1 | cut -d"=" -f3,4 | sed -s "s@\(.*\) Product=\(.*\) Version@[\1:\2]@"`
touchpad=" $touchpad_id $touchpad_name"

#Cardreader
cardreader=`lsusb -vvv | grep "USB2.0" -B13 | head -n1 | cut -d":" -f2- | sed -s "s@ID @[@" | sed -s "s@\(\w\) @\1] @" `
cardreader_id=`lsusb -vvv | grep "USB2.0" -B13 | head -n1 | cut -d":" -f2- | sed -s "s@ID @[@" | sed -s "s@\(\w\) @\1] @"|awk '{printf $1}' `

#Carmera:
camera=`lsusb -vv | egrep  -i 'iProduct.*Camera' -B13 | head -n1 | cut -d":" -f2- | sed -s "s@ID @[@" | sed -s "s@\(\w\) @\1] @" `
camera_id=`lsusb -vv | egrep  -i 'iProduct.*Camera' -B13 | head -n1 | cut -d":" -f2- | sed -s "s@ID @[@" | sed -s "s@\(\w\) @\1] @"|awk '{printf $1}' `

#VGA output
vga_out=" No"
[ "`xrandr | grep 'CRT\|VGA'`" != "" ] && vga_out=" Yes"

#HDMI output
hdmi_out=" No"
[ "`xrandr | grep 'DFP\|HDMI'`" != "" ] && hdmi_out=" Yes"

#cdrom
cdrom=" No"
[ "`echo /dev/sr*`" != "/dev/sr*" ] && cdrom=" Yes"


#codec
card_file="/proc/asound/card"
cards_count=`cat ${card_file}s | grep 'irq' | wc -l`
for x in $(seq $cards_count)
do
	y=`echo $x 1 | awk '{print $1 - $2 }'`
	do_card "${card_file}$y" $y
done


#Module
module=`dmidecode -t 1 | awk -F": " '/Version/{print $2}'`
module_tmp=`echo $module | awk '{print $2}'`
[ "$module_tmp" != "" ] && module="$module_tmp"
module=" $module"

#LCD EDID
EDID=`/usr/bin/edid-decode /sys/class/drm/card0-LVDS-1/edid | grep Manufacturer | awk '{printf $2}'`

#lan-mac
lan_mac=`ifconfig |grep HWaddr |grep -v wlan |awk '{print $5}'`

#print
#echo "Model:	$module"
#echo "ODM:	"
#echo "Sample Stage:	"
#echo "CPU:	$cpu"
#echo "Chipset:	$chipset"
#echo "Memory:	$mem GB $mem_type"	
#echo "HDD:	$hdd ($hdd_size)"
#echo "VGA1:	$vga_1"
#echo "VGA2:	$vga_2"
#echo "3D controller:	$controller"
#echo -e "Audio:	$audio_1 \n $audio_2"
#cat $audio_code_file  
#echo "Wlan:	$wlan"
#echo "lan:	$lan"
#echo "Bluetooth:	$bluetooth"
#echo "3G:	"
#echo "Touchpad:	$touchpad"
#echo "Cardreader:	$cardreader"
#echo "Camera:	$camera"
#echo "CD-ROM:	$cdrom"
#echo "VGA output:	$vga_out"
#echo "HDMI output:	$hdmi_out"
#cat $hdmi_code_file 
#echo "LCD EDID:	$EDID"
echo  "[hardware_info]"  >>/tmp/autotest.ini
echo  "id="  >>/tmp/autotest.ini
echo  "hw_id="  >>/tmp/autotest.ini
echo  "zichan_id="  >>/tmp/autotest.ini
echo  "model="  >>/tmp/autotest.ini
echo  "sku="  >>/tmp/autotest.ini
echo  "vga_vendor=$vga_vendor"  >>/tmp/autotest.ini
echo  "odm="  >>/tmp/autotest.ini
echo  "sample_stage="  >>/tmp/autotest.ini
echo  "manufacture=Lenovo"  >>/tmp/autotest.ini
echo  "cpu_info=$cpu"  >>/tmp/autotest.ini
echo  "chipset=$chopset"  >>/tmp/autotest.ini
echo  "memory=$mem GB $mem_type"  >>/tmp/autotest.ini
echo  "hdd1=$hdd ($hdd_size)"  >>/tmp/autotest.ini
echo  "hdd2=$hdd2 ($hdd2_size)"  >>/tmp/autotest.ini
echo  "vga1_id=$vga1_id"  >>/tmp/autotest.ini
echo  "vga1_desc=$vga1_desc"  >>/tmp/autotest.ini
echo  "vga2_id=$vga2_id"  >>/tmp/autotest.ini
echo  "vga2_desc=$vga2_desc"  >>/tmp/autotest.ini
echo  "audio_id=$audio_id"  >>/tmp/autotest.ini
echo  "audio_desc=$audio_1"  >>/tmp/autotest.ini
echo  "audio_codec_name=$audio_codec_name"  >>/tmp/autotest.ini
echo  "audio_codec_vid=$audio_codec_vid"  >>/tmp/autotest.ini
echo  "audio_codec_sid=$audio_codec_sid"  >>/tmp/autotest.ini
echo  "audio_codec_revid=$audio_codec_revid"  >>/tmp/autotest.ini
echo  "wlan_id=$wlan_id"  >>/tmp/autotest.ini
echo  "wlan_desc=$wlan"  >>/tmp/autotest.ini
echo  "lan_id=$lan_id"  >>/tmp/autotest.ini
echo  "lan_desc=$lan"  >>/tmp/autotest.ini
echo  "bluetooth_id=$bluetooth_id"  >>/tmp/autotest.ini
echo  "bluetooth_desc=$bluetooth_desc"  >>/tmp/autotest.ini
echo  "3G_id="  >>/tmp/autotest.ini
echo  "3G_desc="  >>/tmp/autotest.ini
echo  "touchpad=$touchpad"  >>/tmp/autotest.ini
echo  "cardreader_id=$cardreader_id"  >>/tmp/autotest.ini
echo  "cardreader_desc=$cardreader"  >>/tmp/autotest.ini
echo  "camera_id=$camera_id"  >>/tmp/autotest.ini
echo  "camera_desc=$camera"  >>/tmp/autotest.ini
echo  "vga_output=$vga_out"  >>/tmp/autotest.ini
echo  "hdmi_output=$hdmi_out"  >>/tmp/autotest.ini
echo  "hdmi_codec_name=$hdmi_codec_name"  >>/tmp/autotest.ini
echo  "hdmi_codec_vid=$hdmi_codec_vid"  >>/tmp/autotest.ini
echo  "hdmi_codec_sid=$hdmi_codec_sid"  >>/tmp/autotest.ini
echo  "hdmi_codec_revid=$hdmi_codec_revid"  >>/tmp/autotest.ini
echo  "update=$date"  >>/tmp/autotest.ini
echo  "LCD=$EDID"  >>/tmp/autotest.ini
echo  "keeper="  >>/tmp/autotest.ini
echo  "status="  >>/tmp/autotest.ini
echo  "come_from="  >>/tmp/autotest.ini
echo  "lan_mac=$lan_mac"  >>/tmp/autotest.ini
