# AWS IoT Controlled publish period 
# Created at 2017-10-03 08:49:48.182639

import streams
import json
import timers
from wireless import wifi

# choose a wifi chip supporting secure sockets and client certificates
from espressif.esp32net import esp32wifi as wifi_driver

# import aws iot module
from aws.iot import iot

# import helpers functions to easily load keys, certificates and thing configuration
import helpers


buttonPin=BTN0
pinMode(buttonPin,INPUT_PULLUP)

# THING KEY AND CERTIFICATE FILE MUST BE PLACED INSIDE PROJECT FOLDER 
new_resource('private.pem.key')
new_resource('certificate.pem.crt')
# SET THING CONFIGURATION INSIDE THE FOLLOWING JSON FILE
new_resource('thing.conf.json')

# define a callback for shadow updates
def shadow_callback(requested):
    global publish_period
    print('requested publish period:', requested['publish_period'])
    publish_period = requested['publish_period']
    return {'publish_period': publish_period}

streams.serial()
wifi_driver.auto_init()

print('connecting to wifi...')
# place here your wifi configuration
wifi.link("JoeyCat",wifi.WIFI_WPA2,"joeylovesfood")

pkey, clicert = helpers.load_key_cert('private.pem.key', 'certificate.pem.crt')
thing_conf = helpers.load_thing_conf()
publish_period = 1000 #decreased from 5 secs to one sec

# create aws iot thing instance, connect to mqtt broker, set shadow update callback and start mqtt reception loop
thing = iot.Thing(thing_conf['endpoint'], thing_conf['mqttid'], clicert, pkey, thingname=thing_conf['thingname'])
print('connecting to mqtt broker...')
thing.mqtt.connect()
thing.on_shadow_request(shadow_callback)
thing.mqtt.loop()
thing.update_shadow({'publish_period': publish_period})


#while True:
def pressed():
    print('publish random sample...')
    thing.mqtt.publish("dev/sample", json.dumps({ 'Device ID' : thing_conf['thingname'], 'Time': timers.now(), 'Humidity': random(0,50), 'Temperature': random(0,100)  }))
    #sleep(publish_period)
    
#onPinFall(buttonPin,pressed)  #better not to use this or it now resets program

while True:
    onPinFall(buttonPin,pressed)
    sleep(publish_period)


