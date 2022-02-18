#https://gpiocc.github.io/learn/micropython/esp/2020/04/04/martin-ku-getting-started-with-micropython-for-esp32.html
from machine import Pin
from time import sleep

def flash(led, num_of_times):
    for i in range(num_of_times):
        led.on()
        print('ON')
        sleep(1)
        led.off()
        print('OFF')
        sleep(1)

led = Pin(2, Pin.OUT)
flash(led, 10)
