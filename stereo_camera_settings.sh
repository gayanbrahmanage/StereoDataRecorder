v4l2-ctl -d /dev/video0 --set-ctrl=brightness=0
v4l2-ctl -d /dev/video2 --set-ctrl=brightness=0

v4l2-ctl -d /dev/video0 --set-ctrl=contrast=43
v4l2-ctl -d /dev/video2 --set-ctrl=contrast=43

v4l2-ctl -d /dev/video0 --set-ctrl=saturation=64
v4l2-ctl -d /dev/video2 --set-ctrl=saturation=64

v4l2-ctl -d /dev/video0 --set-ctrl=hue=0
v4l2-ctl -d /dev/video2 --set-ctrl=hue=0

v4l2-ctl -d /dev/video0 --set-ctrl=white_balance_temperature_auto=1
v4l2-ctl -d /dev/video2 --set-ctrl=white_balance_temperature_auto=1

v4l2-ctl -d /dev/video0 --set-ctrl=gamma=90
v4l2-ctl -d /dev/video2 --set-ctrl=gamma=90

v4l2-ctl -d /dev/video0 --set-ctrl=gain=0
v4l2-ctl -d /dev/video2 --set-ctrl=gain=0

v4l2-ctl -d /dev/video0 --set-ctrl=sharpness=6
v4l2-ctl -d /dev/video2 --set-ctrl=sharpness=6

v4l2-ctl -d /dev/video0 --set-ctrl=backlight_compensation=1
v4l2-ctl -d /dev/video2 --set-ctrl=backlight_compensation=1

v4l2-ctl -d /dev/video0 --set-ctrl=exposure_absolute=400
v4l2-ctl -d /dev/video2 --set-ctrl=exposure_absolute=400

v4l2-ctl -d /dev/video0 --set-ctrl=exposure_auto_priority=0
v4l2-ctl -d /dev/video2 --set-ctrl=exposure_auto_priority=0
