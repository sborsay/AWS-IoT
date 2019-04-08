#include <WioLTEforArduino.h>
#include <WioLTEClient.h>
#include <PubSubClient.h>    // https://github.com/SeeedJP/pubsubclient
#include <stdio.h>

#define APN               "soracom.io"
#define USERNAME          "sora"
#define PASSWORD          "sora"

#define MQTT_SERVER_HOST  "beam.soracom.io"
#define MQTT_SERVER_PORT  (1883)

#define ID                "WioLTE"
#define OUT_TOPIC         "outTopic"
#define IN_TOPIC          "inTopic"

#define INTERVAL          (60000)

WioLTE Wio;
WioLTEClient WioClient(&Wio);
PubSubClient MqttClient;

void callback(char* topic, byte* payload, unsigned int length) {
  SerialUSB.print("Subscribe:");
  for (int i = 0; i < length; i++) SerialUSB.print((char)payload[i]);
  SerialUSB.println("");
}

void setup() {
  delay(200);

  SerialUSB.println("");
  SerialUSB.println("--- START ---------------------------------------------------");
  
  SerialUSB.println("### I/O Initialize.");
  Wio.Init();
  
  SerialUSB.println("### Power supply ON.");
  Wio.PowerSupplyLTE(true);
  delay(500);

  SerialUSB.println("### Turn on or reset.");
  if (!Wio.TurnOnOrReset()) {
    SerialUSB.println("### ERROR! ###");
    return;
  }

  SerialUSB.println("### Connecting to \""APN"\".");
  if (!Wio.Activate(APN, USERNAME, PASSWORD)) {
    SerialUSB.println("### ERROR! ###");
    return;
  }

  SerialUSB.println("### Connecting to MQTT server \""MQTT_SERVER_HOST"\"");
  MqttClient.setServer(MQTT_SERVER_HOST, MQTT_SERVER_PORT);
  MqttClient.setCallback(callback);
  MqttClient.setClient(WioClient);
  if (!MqttClient.connect(ID)) {
    SerialUSB.println("### ERROR! ###");
    return;
  }
  MqttClient.subscribe(IN_TOPIC);

  SerialUSB.println("### Setup completed.");
}

void loop() {
  char fakeData[100];
//choose your own number of variables and data types, but avoid data buffer size overload
  float var1 =  random(55,77); //fake number range, adjust as you like
  float var2 =  random(77,99);
  
  
  sprintf(fakeData,  "{\"uptime\":%lu,\"variable1\":%f,\"variable2\":%f}", millis() / 1000, var1, var2);
  SerialUSB.print("Publish:");
  SerialUSB.print(fakeData);
  SerialUSB.println("");
  MqttClient.publish(OUT_TOPIC, fakeData);
  
err:
  unsigned long next = millis();
  while (millis() < next + INTERVAL)
  {
    MqttClient.loop();
  }
}

