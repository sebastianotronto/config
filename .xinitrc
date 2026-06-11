# Bring up ethernet and keep checking if available
doas ip link set eth1 up
while true; do
	if [ "$(cat /sys/class/net/eth1/carrier)" = 1 ]; then
		if ! doas ip -4 addr show eth1 | grep inet; then
			doas udhcpc -i eth1 -n && iwctl station wlan0 disconnect
		fi
	fi
	sleep 3
done &

# Check every minute
while true; do
	battery-checknow &
	sleep 0.01
	event list now | sed -E 's/....-..-.. .{1,2}:..[[:space:]]*//g' | \
	while read text; do
		notify push "$text"
	done &
	sleep 0.01 &

	xroot-update

	sleep $((60 - $(date +%-S)))
done &

# Extra settings
xkfix # From scripts
brightnessctl set 30%
xset s off -dpms
xrdb -merge ~/.Xresources

# Background and time
sleep 0.05
xwallpaper --zoom ~/box/pictures/wallpapers/marmolada.jpg
sleep 0.01
event list past | sed -E 's/....-..-.. .{1,2}:..[[:space:]]*//g' | \
while read text; do
	notify push "$text"
done &

xbanish &
Telegram &
#exec dwm
exec dbus-launch --exit-with-session dwm
