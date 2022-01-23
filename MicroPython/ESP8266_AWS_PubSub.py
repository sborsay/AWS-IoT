#openssl x509 -in 5d93250f80-certificate.pem.crt -out  ClientCert.cert.der -outform DER
#openssl rsa -in 5d93250f80-private.pem.key -out  privateKey.key.der -outform DER


#AWS MQTT client cert example for esp8266 or esp32 running MicroPython 1.9 
from umqtt.robust import MQTTClient
import time
import random
import machine
pin = machine.Pin(2)

#This works for either ESP8266 ESP32 if you rename certs before moving into /flash 
CERT_FILE = "/myCert.cert.der"
KEY_FILE = "/myKey.key.der"

#if you change the ClientId make sure update AWS policy
MQTT_CLIENT_ID = "bas638"
MQTT_PORT = 8883

#if you change the topic make sure update AWS policy
PUB_TOPIC = "outTopic" #coming out of device
SUB_TOPIC = "inTopic"  #coming into device

#Change the following three settings to match your environment
MQTT_HOST = "IKnowKungFuees-ats.iot.us-west-2.amazonaws.com"
WIFI_SSID = "iPhone"
WIFI_PW = "burger888"

MQTT_CLIENT = None

print("h3fhf")
#do_connect()

def do_connect():
    print("h3fhf")
    import network
    print("hfhf")
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print('connecting to network...')
        sta_if.active(True)
        sta_if.connect(WIFI_SSID , WIFI_PW)
        while not sta_if.isconnected():
            pass
    print('network config:', sta_if.ifconfig())
    

def pub_msg(msg):  #publish is synchronous so we poll and publish
    global MQTT_CLIENT
    try:    
        MQTT_CLIENT.publish(PUB_TOPIC, msg)
        print("Sent: " + msg)
    except Exception as e:
        print("Exception publish: " + str(e))
        raise

def sub_cb(topic, msg):
    print('Device received a Message: ')
    print((topic, msg))  #print incoing message asychronously
    pin.value(0) #blink if incoming message by toggle off
     #if topic == "inTopic" and msg == b'received':
        #print('ESP received hello message')
    

#def sub_msg():    #Subscribe incoming message is asynchronously delivered 
 #   global MQTT_CLIENT
 #   try:
 #   MQTT_CLIENT.set_callback(sub_cb)
 #       MQTT_CLIENT.subscribe(SUB_TOPIC)
 #       print('Subscribed to %s topic' % (SUB_TOPIC))
        #print("Received: " + msg)
 #   except Exception as e:
 #       print("Exception subscribe: " + str(e))
 #       raise

    

def connect_mqtt():    
    global MQTT_CLIENT

    try:  #all this below runs once ,eqivealent to Arduino's "setup" function)
        with open(KEY_FILE, "r") as f: 
            key = f.read()

        print("Got Key")
            
        with open(CERT_FILE, "r") as f: 
            cert = f.read()

        print("Got Cert")

        MQTT_CLIENT = MQTTClient(client_id=MQTT_CLIENT_ID, server=MQTT_HOST, port=MQTT_PORT, keepalive=5000, ssl=True, ssl_params={"cert":cert, "key":key, "server_side":False})
        MQTT_CLIENT.connect()
        print('MQTT Connected')
        #sub_msg() #set up subscription and wait for incoming messages, subsciption is set up below
        MQTT_CLIENT.set_callback(sub_cb)
        MQTT_CLIENT.subscribe(SUB_TOPIC)
        print('Subscribed to %s as the incoming topic' % (SUB_TOPIC))
        return MQTT_CLIENT
    except Exception as e:
        print('Cannot connect MQTT: ' + str(e))
        raise


#start execution
try:
    print("Connecting WIFI")
    do_connect()
    #connect_wifi(WIFI_SSID, WIFI_PW)
    print("Connecting MQTT")
    connect_mqtt()
    print("cc")
    while True: #loop forever
            pin.value(1)
            #new_message =  MQTT_CLIENT.check_msg()
            new_message = MQTT_CLIENT.check_msg()  # check for new subsciption payload incoming
            if new_message != 'None':  #check if we have a message and continue to publish, if so then get the message
        #if new_message != 'None':     #without using this check we can easily miss incoming messages
                temp = (time.time())%99#random.randint(0, 100)
                humid = (time.time())%98#random.randint(0, 100)
                deviceTime = time.time()
                #temp= random.seed(int(time.time()))
                #temp = int( time.time() * 1000.0 )
                #x= int(random.seed(10))
                #y=urandom.seed(22)
                #temp = int(random.getrandbits(x))
                #{\n  \"temperature\": 55,\n  \"humidity\": 66,\n  \"timestamps\": 12345\n}
                pub_msg("{\n  \"temperature\": %d,\n  \"humidity\": %d,\n  \"timestamps\": %d\n}"%(temp,humid,deviceTime))
                print("OK")
                time.sleep(5)  # 5 second delay between publishing
            
except Exception as e:
    print(str(e))
