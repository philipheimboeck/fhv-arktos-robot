#!/bin/bash
#/etc/init.d/bluetooth restart

###
# FIXME
# Change to correct bluetooth mac addr
###
#bluetoothAddr="00:0D:1A:10:00:86"
bluetoothAddr="7C:7A:91:C8:16:9D"

sdpResult=-1
grepRegExComPort="(COM[0-9]+)"
grepRegExRfcommChannel="[0-9]+"
rfcommPort="rfcomm0"
gameProcess="arctos-robot"
gamePath="/root/arctos-robot"
gamePid=$(pidof $gameProcess)
maxCounter=10

###
# Check if connection is already established
###
rfcommResult=$(rfcomm show $rfcommPort)
echo $rfcommResult
if [[ "$rfcommResult" == *"$bluetoothAddr channel"*"connected"* ]]
then
	echo "$rfcommPort already connected"

	if [[ -z $gamePid ]]
	then
		echo "Starting game"
		$gamePath/$gameProcess "/dev/$rfcommPort" &
	else
		echo "Game already started"
	fi
	exit 1
else
	if [[ ! -z $gamePid ]]
        then
                kill $gamePid
        fi
fi

###
# Connect bluetooth
###
echo "Try to connect to $bluetoothAddr"
con=$(hcitool con | grep -o $bluetoothAddr)
while [ "$con" != "$bluetoothAddr" ]
do
bluetoothctl << EOF
	power on
	connect $bluetoothAddr
EOF
	sleep 2
	con=$(hcitool con | grep -o $bluetoothAddr)
	echo $con
	sleep 2
done
echo "Connected to $bluetoothAddr"

###
# Find connection
###
echo "Find RFCOMM connection"
counter=0
while [[ $counter -lt $maxCounter && $sdpResult -ne 0 ]]
do
	sdpResult=$(sdptool browse $bluetoothAddr | grep -oE $grepRegExComPort -m 1)
	sleep 2
done
echo $sdpResult
if [[ -z $sdpResult || $sdpResult -ne 0 ]]
then
	exit -1
fi
echo "Connection found"

###
# Get COM port and RFCOMM channel
###
echo "Get COM port and RFCOMM channel"
comPort=$(sdptool browse $bluetoothAddr | grep -oE $grepRegExComPort -m 1)
while [ "$comPort" == "" ]
do
	sleep 2
	comPort=$(sdptool browse $bluetoothAddr | grep -oE $grepRegExComPort -m 1)
done
echo "COM port found ($comPort)"
rfcommChannel=$(sdptool browse $bluetoothAddr | grep -A 7 -E $grepRegExComPort | grep "Channel" | grep -oE $grepRegExRfcommChannel )
while [ "$rfcommChannel" == "" ]
do
        sleep 2
	rfcommChannel=$(sdptool browse $bluetoothAddr | grep -A 7 -E $grepRegExComPort | grep "Channel" | grep -oE $grepRegExRfcommChannel )
done
echo "RFCOMM channel found ($rfcommChannel)"

###
# Establish RFCOMM
###
echo "Establish RFCOMM"
counter=0
rfcommResult=$(rfcomm show $rfcommPort)
while [[ $counter -lt $maxCounter && "$rfcommResult" != *"$bluetoothAddr channel $rfcommChannel connected"* ]]
do
	rfcomm connect $rfcommPort $bluetoothAddr $rfcommChannel &
	sleep 2
	rfcommResult=$(rfcomm show $rfcommPort)
	((counter++))
done

if [[ "$rfcommResult" == *"$bluetoothAddr channel $rfcommChannel connected"* ]]
then
	echo "$rfcommPort connected"

        if [[ -z $gamePid ]]
        then
                echo "Starting game"
                $gamePath/$gameProcess "/dev/$rfcommPort" &
        fi

	exit 1
else
	exit -1
fi
