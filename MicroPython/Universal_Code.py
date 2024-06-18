# https://gist.github.com/StanS-AWS/a243ffac4fd19a3a8ab243633aa322db

# Rename "main.py"

#AWS MQTT client cert example for esp8266 or esp32 running MicroPython 1.9 
from umqtt.robust import MQTTClient
import time

#Certs for ESP32
#CERT_FILE = "/flash/8266-01.cert.pem"
#KEY_FILE = "/flash/8266-01.private.key"

#Certs for ESP8266
#CERT_FILE = "/flash/8266-01.cert.der"
#KEY_FILE = "/flash/8266-01.private.key.der"

#This works for either ESP8266 ESP32 if you rename certs before moving into /flash 
CERT_FILE = "/flash/cert"
KEY_FILE = "/flash/key"

#if you change the ClientId make sure update AWS policy
MQTT_CLIENT_ID = "basicPubSub"
MQTT_PORT = 8883

#if you change the topic make sure update AWS policy
MQTT_TOPIC = "sdk/test/Python"

#Change the following three settings to match your environment
MQTT_HOST = "CHANGE_ME_xxxxxxxxxxxx.iot.xx-xxxx-x.amazonaws.com"
WIFI_SSID = "CHANGE_ME_WIFI_SSID"
WIFI_PW = "CHANGE_ME_WIFI_SSID"

mqtt_client = None

def pub_msg(msg):
    global mqtt_client
    try:    
        mqtt_client.publish(MQTT_TOPIC, msg)
        print("Sent: " + msg)
    except Exception as e:
        print("Exception publish: " + str(e))
        raise

def connect_mqtt():    
    global mqtt_client

    try:
        with open(KEY_FILE, "r") as f: 
            key = f.read()

        print("Got Key")
            
        with open(CERT_FILE, "r") as f: 
            cert = f.read()

        print("Got Cert")	

        mqtt_client = MQTTClient(client_id=MQTT_CLIENT_ID, server=MQTT_HOST, port=MQTT_PORT, keepalive=5000, ssl=True, ssl_params={"cert":cert, "key":key, "server_side":False})
        mqtt_client.connect()
        print('MQTT Connected')

        
    except Exception as e:
        print('Cannot connect MQTT: ' + str(e))
        raise


def connect_wifi(ssid, pw):
    import network
    wlan = network.WLAN(network.STA_IF)
    
    if(wlan.isconnected()):
        wlan.disconnect()  
    nets = wlan.scan()	
    
    if not wlan.isconnected():

        wlan.active(True)
        wlan.connect(WIFI_SSID, WIFI_PW)
        while not wlan.isconnected():
            pass
    print("connected:", wlan.ifconfig())

        
def connect_wifi_sdk(ssid, pw):
    from network import WLAN
    from network import STA_IF
    import machine

    wlan = WLAN(STA_IF)
    nets = wlan.scan()
    if(wlan.isconnected()):
        wlan.disconnect()            
    wlan.connect(ssid, pw)         
    while not wlan.isconnected():             
        machine.idle() # save power while waiting
        print('WLAN connection succeeded!')         
        break 
    print("connected:", wlan.ifconfig())

#start execution
try:
    print("Connecting WIFI")
    connect_wifi(WIFI_SSID, WIFI_PW)
    print("Connecting MQTT")
    connect_mqtt()
    print("Publishing")
    pub_msg("{\"AWS-MQTT-8266-01\":" + str(time.time()) + "}")
    print("OK")
except Exception as e:
    print(str(e))
