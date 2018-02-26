import RPi.GPIO as GPIO
from time import sleep

GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)
GPIO.setup(18, GPIO.IN)

GPIO.output(11, GPIO.HIGH)

sleep(10)

GPIO.output(11, GPIO.LOW)
GPIO.cleanup()

