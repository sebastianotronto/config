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
xsetroot -cursor_name left_ptr &
setxkbmap -option compose:ralt
setxkbmap -option caps:swapescape
xbacklight -set 30
xset s off -dpms
xrdb -merge .Xresources

# Background and time
sleep 0.05
xwallpaper --zoom ~/box/pictures/wallpapers/marmolada.jpg
sleep 0.01
event list past | sed -E 's/....-..-.. .{1,2}:..[[:space:]]*//g' | \
while read text; do
	notify push "$text"
done &

xbanish &
telegram-desktop &
exec dwm
