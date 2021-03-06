#!/usr/bin/python3

import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)

import datetime
import time
import subprocess
import ephem
import os

observer = ephem.Observer()

observer.lat = os.environ["CAMLAT"] # '49.869802'
observer.lon = os.environ["CAMLON"] # '8.778283'
dev = os.environ["METDEV"] # "/dev/video0"
exe = os.environ["METHOME"] + "/bash/run.sh" # "/home/pi/fidelio-capture-bit/run.sh"

print("device: " + dev)

current = ephem.now()
sunset = observer.next_setting(ephem.Sun())
sunrise = observer.next_rising(ephem.Sun())

print("current time: " + str(current))
print("next sunset: " + str(sunset))
print("next sunrise: " + str(sunrise))

#night = 1000

# checks if night already has begun when routine is called and if necessary shortens "night" parameter

if (sunrise-sunset) > 0:
    night = int((sunrise-sunset) * 24 * 3600)
    wait = int((sunset-current) * 24 * 3600)
    print("night begins in " + time.strftime('%H:%M:%S', time.gmtime(wait)))
    time.sleep(wait)

else:
    print("The Sun has already set! Immediately starting observation!")
    night = int((sunrise-current) * 24 * 3600)
"""
"""

# starting observation

print("scheduled night length: " + str(night) + " sec")

GPIO.output(11, GPIO.HIGH)
subprocess.call([exe, str(night)])
GPIO.output(11, GPIO.LOW)
GPIO.cleanup()
