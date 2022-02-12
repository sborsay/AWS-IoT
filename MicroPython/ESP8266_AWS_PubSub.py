#For Windows install 'Git for Windows' then openSSL can be found in-->  C:\Program Files\Git\usr\bin>
#openssl x509 -in 5d93250f80-certificate.pem.crt -out  ClientCert.cert.der -outform DER
#openssl rsa -in 5d93250f80-private.pem.key -out  privateKey.key.der -outform DER

#AWS MQTT client cert example for esp8266, this sketch is a combination of various sources:
#https://awsiot.wordpress.com/2019/01/10/connect-8266-to-aws-mqtt-using-miropython/
#https://randomnerdtutorials.com/micropython-mqtt-esp32-esp8266/
#https://forum.micropython.org/viewtopic.php?t=5166
#original code added by Stephen Borsay for Udemy Course


#AWS MQTT Connect Pub/Sub
from umqtt.robust import MQTTClient
import time
import random
import machine
pin = machine.Pin(2)

#Convert your AWS Certs with OpenSSL:  directions in the header of this code
#Open.SSL can be found with default install  in--> C:\Program Files\Git\usr\bin>
CERT_FILE = "/<Your-Client-Cert>.cert.der"
KEY_FILE = "/<Your-Private-Key>.key.der"

#ClientId should be unique per device
MQTT_CLIENT_ID = "HeavyPetter33"
MQTT_PORT = 8883 #MQTT Secured

#Pub and Sub topics 
PUB_TOPIC = "outTopic" #coming out of device
SUB_TOPIC = "inTopic"  #coming into device

#Change the following three settings to match your environment
MQTT_HOST = "IKnowKungFuees-ats.iot.us-east-1.amazonaws.com"  #Your AWS IoT Endpoint IoT Core-->Settings
WIFI_SSID = "<Your-WiFi-Network>"
WIFI_PW = "<Your-WiFi-Password>"

MQTT_CLIENT = None  #empty obecjt

print("Starting program...")

def do_connect():
    print("In connect method")
    import network
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print('connecting to network...')
        sta_if.active(True)
        sta_if.connect(WIFI_SSID , WIFI_PW)
        while not sta_if.isconnected():
            pass  # I do nothing and like it
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
    print("Entering Loop")
    while True: #loop forever
            pin.value(1)
            new_message = MQTT_CLIENT.check_msg()  # check for new subsciption payload incoming
            if new_message != 'None':  #check if we have a message and continue to publish, if so then get the message
                temp = (time.time())%99   #random.randint(0, 100)  because randomint doesnt work
                humid = (time.time())%98  #random.randint(0, 100)
                deviceTime = time.time()
                pub_msg("{\n  \"temperature\": %d,\n  \"humidity\": %d,\n  \"timestamps\": %d\n}"%(temp,humid,deviceTime))
                print("OK Published payload")
                time.sleep(5)  # 5 second delay between publishing
            
except Exception as e:
    print(str(e))
