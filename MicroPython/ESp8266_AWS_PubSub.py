#openssl x509 -in 5d93250f80-certificate.pem.crt -out  ClientCert.cert.der -outform DER
#openssl rsa -in 5d93250f80-private.pem.key -out  privateKey.key.der -outform DER


#AWS MQTT client cert example for esp8266 or esp32 running MicroPython 
from umqtt.robust import MQTTClient
import time
import random

#This works for either ESP8266 ESP32 if you rename certs before moving into /flash 
CERT_FILE = "/cert33.cert.der"  # certs at same directory level as this program
KEY_FILE = "/private33.key.der"

#if you change the ClientId make sure update AWS policy
MQTT_CLIENT_ID = "baskj478888" #Be Unique
MQTT_PORT = 8883   #MQTT secured port, don't 1883 me

#if you change the topic make sure update AWS policy
MQTT_TOPIC = "outTopic" #maybe outTopic or iot/something

#Change the following three settings to match your environment
MQTT_HOST = "iknowkungfuyes-ats.iot.us-west-2.amazonaws.com"
WIFI_SSID = "ssid"
WIFI_PW = "pw"" #ex: password123

mqtt_client = None

def do_connect():
    import network
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print('connecting to network...')
        sta_if.active(True)
        sta_if.connect(WIFI_SSID , WIFI_PW)
        while not sta_if.isconnected():
            pass #I do nothing and like it.
    print('network config:', sta_if.ifconfig())

def pub_msg(msg):
    global mqtt_client
    try:    
        mqtt_client.publish(MQTT_TOPIC, msg)
        print("Sent: " + msg)
    except Exception as e:
        print("Exception publish: " + str(e))
        raise #throw error stop flow

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
        raise #throw error stop flow

#start execution
try:
    print("Connecting WIFI")
    do_connect()
    #connect_wifi(WIFI_SSID, WIFI_PW)
    print("Connecting MQTT")
    connect_mqtt()
    while True: #loop forever
        #rando1 = random.randint(0, 100)
        tt = time.time()
        tt = (tt % 3333) / 10
        hh = tt / 1.5
        #rando2 = random.randint(0, 100)
        deviceTime = time.time()
        print("Publishing")
        pub_msg("{\n  \"temperature\": %d,\n  \"humidity\": %d,\n  \"timestamps\": %d\n}"%(tt,hh,deviceTime))
        print("OK")
        time.sleep(5)  # 5 second delay between publishing
except Exception as e:
    print(str(e))
