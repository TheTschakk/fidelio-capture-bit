#!/usr/bin/python

import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)

import datetime
import time
import subprocess
import ephem

observer = ephem.Observer()

observer.lat = '49.869802'
observer.lon = '8.778283'
dev = "/dev/video0"

current = ephem.now()
sunset = observer.next_setting(ephem.Sun())
sunrise = observer.next_rising(ephem.Sun())

print("current time: " + str(current))
print("next sunset: " + str(sunset))
print("next sunrise: " + str(sunrise))

# checks if night already has begun when routine is called

if (sunrise-sunset) > 0:
    night = int((sunrise-sunset) * 24 * 3600)
    wait = int((sunset-current) * 24 * 3600)
    print("night begins in " + time.strftime('%H:%M:%S', time.gmtime(wait)))
    time.sleep(wait)

else:
    print("The Sun has already set! Immediately starting observation!")
    night = int((sunrise-current) * 24 * 3600)

# starting observation

GPIO.output(11, GPIO.HIGH)
#subprocess.call(["./fidelio", dev, night])
GPIO.output(11, GPIO.LOW)
GPIO.cleanup()

