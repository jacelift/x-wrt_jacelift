#!/bin/sh

led_ctrl() {
	local oldvalue=
	while :; do
		mobile_network=0
		mobile_4g=0
		mobile_3g=0
		rssi1=0
		rssi2=0
		rssi3=0
		rssi4=0
		rssi5=0

		newvalue="$mobile_network$mobile_4g$mobile_3g$rssi1$rssi2$rssi3$rssi4$rssi5"
		if [ "x$oldvalue" != "x$newvalue" ]; then
		for num in $mobile_network $mobile_4g $mobile_3g $rssi1 $rssi2 $rssi3 $rssi4 $rssi5; do
			echo "$num" >/sys/class/gpio/led_b/value
			echo "0" >/sys/class/gpio/led_clk/value
			echo "1" >/sys/class/gpio/led_clk/value
		done
		fi
		oldvalue=$newvalue
		sleep 5
		[ -d /tmp/led-ctrl.lck ] || break
	done
}

case $1 in
	start)
		mkdir /tmp/led-ctrl.lck 2>/dev/null && {
			led_ctrl &
		}
    ;;
	stop)
		rm -rf /tmp/led-ctrl.lck
    ;;
esac
