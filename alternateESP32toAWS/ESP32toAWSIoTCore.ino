/*





*/

#include <WiFiClientSecure.h>
#include <MQTTClient.h>  //download this library from 

#include "secrets.h"

const char* ssid = "YOUR-WiFi-NETWORK";
const char* password = "YOUR-PASWORD";

#define AWS_THING_NAME "esp32"
#define AWS_IOT_PUBLISH_TOPIC   "foodstuff/publish"
#define AWS_IOT_SUBSCRIBE_TOPIC "foodstuff/subscribe"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(512);

void connectAWS() {
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  Serial.println("Connecting to AWS IOT");

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

  // Publish to a topic
  client.publish(AWS_IOT_PUBLISH_TOPIC, "{\"message\": \"Hello from ESP32!\"}");
}
