/*

This sketch is based on the work of:
github.com/256dpi/arduino-mqtt/blob/master/examples/ESP32DevelopmentBoardSecure

modfied for AWS and provided on AWS-Samples found Here:
github.com/aws-samples/lets-build-on-aws-iot

Message buffer and adjustable delay to publish added by Stephen Borsay:
github.com/sborsay/AWS-IoT/tree/master/alternateESP32toAWS
*/

#include <WiFiClientSecure.h>
#include <MQTTClient.h>

#include "secrets.h"

const char* ssid = "Your_WiFi-Network";
const char* password = "Your-Password";


#define AWS_THING_NAME "esp32"
#define AWS_IOT_PUBLISH_TOPIC   "outTopic"  //subscribe to this topic name on AWS
#define AWS_IOT_SUBSCRIBE_TOPIC "inTopic"   //publish to this topic name on AWS

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(512);

unsigned long lastMillis = 0;

void connectAWS() {
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  Serial.println("Connecting to the AWS IoT MQTT Broker");

  while (!client.connect(AWS_THING_NAME)) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("AWS IoT Connected!");
}

void connectWiFi() {
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// the setup function runs once when you power the board or press reset
void setup() {
  Serial.begin(115200);
  connectWiFi();
  connectAWS();

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  // Create a message handler
  client.onMessage(messageHandler);
}

void messageHandler(String &topic, String &payload) {
  Serial.println("Receiving MQTT message:");
  Serial.println(topic + " " + payload);
}

// the loop function runs over and over again forever
void loop() {


    // Sends and receives packets
  client.loop();
    delay(10);  // <- fixes some issues with WiFi stability
  
     //If you need to increase buffer size, you need to change MQTT PACKET SIZE in MQTTClient library
  char fakeData[512];

  float var1 =  random(55,77); //fake number range, adjust as you like
  float var2 =  random(77,99);
  sprintf(fakeData,  "{\"uptime\":%lu,\"temperature\":%f,\"humidity\":%f}", millis() / 1000, var1, var2);
  
//delay publish as you like to adjust frequency (currently set to 1000 milliseconds)
if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    
  
  // Publish to a topic
  client.publish(AWS_IOT_PUBLISH_TOPIC, fakeData);

   }//end millis() delay
 
}
