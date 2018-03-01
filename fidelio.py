import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)

import datetime
import subprocess
import ephem

observer = ephem.Observer()
observer.lat, observer.lon = '49.869802', '8.778283'

print(observer.next_setting(ephem.Sun()))
print(observer.next_rising(ephem.Sun()))

sunset = observer.next_setting(ephem.Sun())
sunrise = observer.next_rising(ephem.Sun())
night = int((sunset - sunrise) * 24 * 3600)

print(night)


GPIO.output(11, GPIO.HIGH)
subprocess.call(["./fidelio", "/dev/video0", night])
GPIO.output(11, GPIO.LOW)
GPIO.cleanup()
