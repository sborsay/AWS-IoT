/* ESP8266 HTTPS to AWS IoT
 *  Tested with ESP8266 Board Manager 2.74, version 3+ may not work, may need BEARSSL cert adjustment
 * Authors: Anthony Elder and Earle F. Philhower, III, Rui Santos and other sketches I pilfered for inspiration.
 * Sketch Modified by Stephen Borsay for www.udemy.com and my AWS IoT Live Workshops
 * License: Apache License v2
 * github.com/sborsay
 * Add in Char buffer utilizing sprintf to dispatch JSON data to AWS IoT Core
 * Use and replace your own SID, PW, AWS Account Endpoint, Client cert, private cert, x.509 CA root Cert
 
  ----------------------------
 
 ### Compile this only if using ESP8266 version 3+, not backwards comparable
 
 ## Tools--> Board--> Board Manager-->ESP8266 version 3+ by ESP8266 Community
 
 ### Older ESP8266 sketch is not forward compatible
 
 --------------------------------
 * 
 * also
 * github.com/espressif/arduino-esp32/blob/master/libraries/WiFiClientSecure/examples/WiFiClientSecure/WiFiClientSecure.ino
 * 
 * references
 *docs.aws.amazon.com/iot/latest/developerguide/http.html#httpurl
 *github.com/espressif/arduino-esp32/blob/master/libraries/WiFiClientSecure/examples/WiFiClientSecure/WiFiClientSecure.ino
 *github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/BearSSL_ServerClientCert/BearSSL_ServerClientCert.ino
 */
#include <ESP8266WiFi.h>
extern "C" {
#include "libb64/cdecode.h"
}

#include <WiFiClientSecure.h> 

const char* ssid = "NETWORK";  //your WiFi network
const char* password = "myPassword";  //Your WiFi password


const char *host = "a3iknowkungfus-ats.iot.us-east-1.amazonaws.com"; //AWS IoT Core--> Settings (Device data endpoint)
const int httpsPort = 8443; //typical values are HTTPS= 443, MQTT=8883, and HTTP = 80 but AWS IoT Core uses 84443 for HTTP(s) Certificate secured
//AWS IoT Topic Subscribe to "myTopic" in MQTT test client
const char *uri = "/topics/outTopic?qos=1";  //see https://docs.aws.amazon.com/iot/latest/developerguide/http.html


// For the two certificate strings below paste in the text of your AWS 
// device certificate and private key, comment out the BEGIN and END 
// lines, add a quote character at the start of each line and a quote 
// and backslash at the end of each line:

// xxxxxxxxxx-certificate.pem.crt
const String certificatePemCrt = \
//-----BEGIN CERTIFICATE-----
R"EOF(
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMTA3MjQxNjU4
MDBaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwdonttrytcopymycertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxa9AdlAnbY
Az7oGlWE4uHy5oowwl4LoclGiZVVikzFa/3AT7YhkoDKtmpFsaoxA/jy7fcxwVqq
yhrRQqibSTy0aIH82nEn9trDAawNXFldniAzHS9BDa6EsBewfMYZUQplACBYu5QR
i70heg+/IEppUfVgetyourowncertsjTpi6djAz4X1qDOnExxxxxxxxxxxxxxxxx
EES8wt9r/GTvrLpmVndvhswnlMKaMfupcHyrftJELW3pYu4LH8ugsBVKgv8kA8GF
jZyZWZYmuf3JEPrWPprOU0Kw8vwIOQ/VC1ZKA/VxSHeyo3Q5rJYnSxwruo1SdZF4
XwUrAgMBAAGjYlikenowgetyourownawsiotcertificatesgr/ZZwttLH22MB0G
A1UdDgQWBBTeZ+ZusU9cMJDfjiANujMjioeSRzAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAljcZHiWWxoagJtJNPN3wgUtM
yQqJFGl8aU6LQTVW7WQSr5f28l7+6Nsx/4b/BvmiR8E9hwkRky4bJiIFEiVjkWGG
Z1fq2T9q7OlvMF/OIOUCTENdIH+Ljp9YR5cbXHDaPYhBudS7CW+cprFaTGmFdk/f
PMQLpk5iSC2n8qFhEGjlp3TR/mpxs8a4ZJVOqWFt+5rXqVb9JTW/diO7UgD6OmSX
viaGV1lKh7Ju+x1Pj3TG79m4HIGmnjUmWFhZf+RrJVM6t92A20ydrQt1MN0PrY/4
vjFh1BaO3FxCExxqWPzxfpqk3ciJzn0bjmacz2F1ZMCK9B+VgI5VlKjwJnfllw==
)EOF"; 
//-----END CERTIFICATE-----

// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
R"EOF(
xWv9wE+2IZKAyrZqRbGqMQP48u33McFaqsoa0UKom0k8tGiB/NpxJ/bawwGsDVxZ
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
XZ4gMx0vQQ2uhLAXsHzGGVEKZQAgWLuUEYu9IXoPvyBKaVH1fWzOyWFHUJ+Zghlh
g4X/mH/BK/rBTbn1o06YunYwM+F9agzpxBBEvMLfa/xk76y6ZlZ3b4bMJ5TCmjH7
qXSdGUwdonttrytcopymycertxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxa9vvvY
Az7oGlWE4uHy5oowwl4LoclGiZVVikzFa/3AT7YhkoDKtmpFsaoxA/jy7fcxwVqq
yhrRQqibSTy0aIH82nEn9trDAawNXFldniAzHS9BDa6EsBewfMYZUQplACBYu5QR
i70heg+/IEppUfVgetyourowncertsjTpi6djAz4X1qDOnExxxxxxxxxxxxxxxxx
EES8wt9r/GTvrLpmVndvhswnlMKaMfupcHyrftJELW3pYu4LH8ugsBVKgv8kA8GF
jZyZWZYmuf3JEPrWPprOU0Kw8vwIOQ/VC1ZKA/VxSHeyo3Q5rJYnSxwruo1SdZF4
GjYlikenowgetyourownawsiotcertificatesgr/ZZdXkY0vbCXenRPT0mUi/Vh
8oG8DHkCgYEA5pCww2lS2R0qYnH7L2H0pXbI8KdgJ7GF3iIzRE+oSlLKz2vSVKN8
ZUDcEbm6ewjzmNVNsK5dX+eazxaIqnOKwkTsoW3CEN3cdd9uI1HimLgZCX1rVOnZ
vWGgEViXdeaZ3QndU16eDTAEi79CNLckl2EAxsLK2/jDX/HXUIQrwj0CgYEA3JXJ
DPPtqWLGY9uXbyoTatj00D3r3afza8AjmFxjAuRco3wXYVFPMarufXL99NH8fUnn
GDXYHKafNU38koWzXTryY8txbqwbsrNq7ziixYsuyTt9nFfE7OUeZ5CxzomIc0CM
26EYSQvno8AZ9SGbsJW+DkZG2cTuhT4KgPbGY4cCgYBWLd7kK4n7/RrKkTACFesL
rcqNqQIX1BLlZIMlFWEh5rBHt1V2rWThs8yOE43QTxg+F/xh/cqI/liu7w1GKMnE
s15/ODdBdqvgoxLXwNOhoa46DdcAdigc0VCzKxnxx6/bHsTLVKJe64PHeEWWzp2u
t2NC56xLbEAarJp+TejNQQKBgARqezvnJjrd31rgaoevL/RTo5MHqlElq5ncntnN
/61vJOug8FbiadN0uJGKW8sq6QMZF6dEbK8mJsnKZeQJCS40n+WfjQXKLEPHk7x2
RcvunWEp77nRh/+2FoW8P9MCrdig0jETaIkN0/VTQzHFGnjOhp+CiKiTaLMCKvOp
/Q7BAoGAb4U+1aIoZG8pkuH7iorVjj3mzn3QGL5SUGQ/mPUM+n4g/eNc8pfPg7zO
1OwucCY8x2wArFE4LG6G+pOVAmGlbqPMl7lUhOahw5BVETrNYw2L2EWf32/GwhRL
WvMotZjt5nJJXZiMo0y4h3unOvqTNQD/Yp2OdGPwhEp0J7XSxOM=
)EOF";
//-----END RSA PRIVATE KEY-----

// This is the AWS IoT CA Certificate from: 
// https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication
// This one in here is the 'RSA 2048 bit key: Amazon Root CA 1' which is valid 
// until January 16, 2038 so unless it gets revoked you can leave this as is:
const String caPemCrt = \
//-----BEGIN CERTIFICATE-----
R"EOF(
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
)EOF";
//-----END CERTIFICATE-----

WiFiClientSecure wiFiClient;

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("ESP8266 AWS IoT Example");

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  // get current time, otherwise certificates are flagged as expired
  setCurrentTime();

  uint8_t binaryCert[certificatePemCrt.length() * 3 / 4];
  int len = b64decode(certificatePemCrt, binaryCert);
  wiFiClient.setCertificate(binaryCert, len);
  
  uint8_t binaryPrivate[privatePemKey.length() * 3 / 4];
  len = b64decode(privatePemKey, binaryPrivate);
  wiFiClient.setPrivateKey(binaryPrivate, len);

  uint8_t binaryCA[caPemCrt.length() * 3 / 4];
  len = b64decode(caPemCrt, binaryCA);
  wiFiClient.setCACert(binaryCA, len);
}


void loop() {

  Serial.println(host);

  wiFiClient.setTimeout(15000); // 15 Seconds
  delay(1000);
  
  Serial.print("HTTPS Connecting\n");
  int r=0; //retry counter
  while((!wiFiClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }
  if(r==30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to web");
  }
  

  char fakeData[128]; 
  float temperature =  random(25,97); //fake number range, adjust as you like  
  float humidity =  random(77,99);  
  sprintf(fakeData,  "{\"uptime\":%lu,\"temp\":%f,\"humid\":%f}", millis() / 1000, temperature, humidity); // JSON Object with escapes
                                                                                                           //Use %d instead of %f to save memory space

  char postStr[64];
  sprintf(postStr, "POST %s HTTP/1.1", uri);  // put together the string for HTTP POST
  
  wiFiClient.println(postStr);
  wiFiClient.print("Host: ");
  wiFiClient.println(host);
  wiFiClient.println("Content-Type: application/json");
  wiFiClient.print("Content-Length: "); 
  wiFiClient.println(strlen(fakeData));
  wiFiClient.println();    // extra `\r\n` to separate the http header and http body
  wiFiClient.println(fakeData);

  Serial.println(fakeData);
  Serial.println("request sent");
  Serial.print("Heap space remaining: ");
  Serial.println(ESP.getFreeHeap());  //Libraby macro to observe remaining heap size in bytes.
  delay(10000);  // myClient.stop must be placed after some delay
  wiFiClient.stop();  //prevents heap colliding with stack, remember, no automatic garbage collection in C/C++
                      //This is a problem with the ESP32 but not usually the ESP8266 for some reason
                      //Compiler optimization may be better at freeing out of scope object memory in the ESP8266 than the ESP32

}

int b64decode(String b64Text, uint8_t* output) {
  base64_decodestate s;
  base64_init_decodestate(&s);
  int cnt = base64_decode_block(b64Text.c_str(), b64Text.length(), (char*)output, &s);
  return cnt;
}

void setCurrentTime() {
  configTime(3 * 3600, 0,"pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}
