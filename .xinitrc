# Check every minute
while true; do
	battery-checknow &
	sleep 0.01
	event list now | sed -E 's/....-..-.. .{1,2}:..[[:space:]]*//g' | \
	while read text; do
		notify push "$text"
	done &
	sleep 0.01 &

	if [ "$(cat /sys/class/net/eth0/carrier)" = 1 ]; then
		iwctl station wlan0 disconnect
	fi

	xroot-update

	sleep $((60 - $(date +%-S)))
done &

# Extra settings
setxkbmap -option compose:ralt
setxkbmap -option caps:swapescape
brightnessctl set 30%
xset s off -dpms

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
