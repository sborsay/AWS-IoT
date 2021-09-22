/* ESP32 HTTP(s) Certificate Secured to AWS IoT
 *  
 * Author: Anthony Elder and Earle F. Philhower, III
 * License: Apache License v2
 * Sketch Modified by Stephen Borsay for www.udemy.com and my AWS IoT Live Workshops
 * myClient://github.com/sborsay
 * Add in Char buffer utilizing sprintf to dispatch JSON data to AWS IoT Core
 * Use and replace your own SID, PW, AWS Account Endpoint, Client cert, private cert, x.509 CA root Cert
 * 
 * also
 *  github.com/espressif/arduino-esp32/blob/master/libraries/WiFiClientSecure/examples/WiFiClientSecure/WiFiClientSecure.ino
 * 
 * references:
 *docs.aws.amazon.com/iot/latest/developerguide/http.html#httpurl
 *github.com/espressif/arduino-esp32/blob/master/libraries/WiFiClientSecure/examples/WiFiClientSecure/WiFiClientSecure.ino
 *github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/BearSSL_ServerClientCert/BearSSL_ServerClientCert.ino
 */
#include <WiFiClientSecure.h>


const char* ssid = "yyyyyyyyy"; //your network
const char* password = "xxxxxxxxxxxxxx";  //your wifi password

const char* host = "aiknowkungfues-ats.iot.us-east-1.amazonaws.com"; //AWS IoT Core--> Settings (Device data endpoint)
const int Port = 8443;  //typical values are HTTPS= 443, MQTT=8883, and HTTP = 80 but AWS IoT Core uses 84443 for HTTP(s) Certificate secured

//AWS IoT Topic Subscribe to "myTopic" in MQTT test client
const char *uri = "/topics/myTopic?qos=1";  //see https://docs.aws.amazon.com/iot/latest/developerguide/http.html


// xxxxxxxxxx-certificate.pem.crt
static const char certificate_pem_crt[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMTA3MjQxNjU4
MDBaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDGqz8yFYKa9AdlAnbY
Az7oGlWE4uHy5oowwl4LoclGiZVVikzFa/3AT7YhkoDKtmpFsaoxA/jy7fcxwVqq
yhrRQqibSTy0aIH82nEn9trDAawNXFldniAzHS9BDa6EsBewfMYZUQplACBYu5QR
i70heg+/IEppUfV9bM7JYUdQn5mCGWGDhf+Yf8Er+sFNufWjTpi6djAz4X1qDOnE
EES8wt9r/GTvrLpmVndvhswnlMKaMfupcHyrftJELW3pYu4LH8ugsBVKgv8kA8GF
jZyZWZYmuf3JEPrWPprOU0Kw8vwIOQ/VC1ZKA/VxSHeyo3Q5rJYnSxwruo1SdZF4
XwUrAgMBAAGjYDBeMB8GA1UdIwQYMBaAFBnqGRCT6Kgp0DTTqr/ZZwttLH22MB0G
A1UdDgQWBBTeZ+ZusU9cMJDfjiANujMjioeSRzAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAljcZHiWWxoagJtJNPN3wgUtM
yQqJFGl8aU6LQTVW7WQSr5f28l7+6Nsx/4b/BvmiR8E9hwkRky4bJiIFEiVjkWGG
Z1fq2T9q7OlvMF/OIOUCTENdIH+Ljp9YR5cbXHDaPYhBudS7CW+cprFaTGmFdk/f
PMQLpk5iSC2n8qFhEGjlp3TR/mpxs8a4ZJVOqWFt+5rXqVb9JTW/diO7UgD6OmSX
viaGV1lKh7Ju+x1Pj3TG79m4HIGmnjUmWFhZf+RrJVM6t92A20ydrQt1MN0PrY/4
vjFh1BaO3FxCExxqWPzxfpqk3ciJzn0bjmacz2F1ZMCK9B+VgI5VlKjwJnfllw==
-----END CERTIFICATE-----
)KEY";

// xxxxxxxxxx-private.pem.key
static const char private_pem_key[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
xWv9wE+2IZKAyrZqRbGqMQP48u33McFaqsoa0UKom0k8tGiB/NpxJ/bawwGsDVxZ
XZ4gMx0vQQ2uhLAXsHzGGVEKZQAgWLuUEYu9IXoPvyBKaVH1fWzOyWFHUJ+Zghlh
g4X/mH/BK/rBTbn1o06YunYwM+F9agzpxBBEvMLfa/xk76y6ZlZ3b4bMJ5TCmjH7
qXB8q37SRC1t6WLuCx/LoLAVSoL/JAPBhY2cmVmWJrn9yRD61j6azlNCsPL8CDkP
1QtWSgP1cUh3sqN0OayWJ0scK7qNUnWReF8FKwIDAQABAoIBAHj/lXbAiRIs7FAF
5zH7EBY2670Kngjdhm7aAoNdHwRaCVzW5ZkcA6lcIxZhbxWHckROR9SgkcALVRG1
rK32d8zZp20kdtd+CyVgOajYfTZlz0833qnZA819yej6Fz9zLIpuBA0pQYZwREMm
w7DIBgHcoLiMRyXMKBMi8PLPz6p25zZYggPoMJ+DfaF1lS2ZVo3F2t5WV1E8Q541
L9ZgOAzks3vhETd1ZbFegrAllSuM5AvoKELdg11QKX89pcis8ir34Z7nzyE8riAt
+fKke6raGMfKmbHo2s37gSlJI9SAAwmQdXkY0vbCXenRPT0mUi/VNFhfJ1Z+6UBd
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
-----END RSA PRIVATE KEY-----
)KEY";

/* root CA can be downloaded in:
  myClient://www.symantec.com/content/en/us/enterprise/verisign/roots/VeriSign-Class%203-Public-Primary-Certification-Authority-G5.pem
  fungible between regions, I dont care if you use mine below or paste in your own.
*/
static const char rootCA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
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
-----END CERTIFICATE-----
)EOF";

WiFiClientSecure myClient;

void setup() {
  Serial.begin(115200); delay(50); Serial.println();
  Serial.println("ESP32 AWS IoT Example");
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  myClient.setCACert(rootCA);
  myClient.setCertificate(certificate_pem_crt);
  myClient.setPrivateKey(private_pem_key);
}


void loop() {
  
  Serial.println(host);
  myClient.setTimeout(15000); // 15 Seconds
  delay(1000);
  
  Serial.print("HTTPS Connecting to AWS IoT Core\n");
  int r=0; //retry counter
  while((!myClient.connect(host, Port)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }
  if(r==30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected");
  }
  
  
  char fakeData[128]; 
  float temperature = random(25,77);  //fake number range, adjust as you like  
  float humidity =    random(77,99);  //You can use decimal instead of floats to save mem space
  
  sprintf(fakeData,  "{\"uptime\":%lu,\"temp\":%f,\"humid\":%f}", millis() / 1000, temperature, humidity); // JSON Object with escapes
                                                                                                           //Use %d instead of %f to save memory space
  char postStr[40];
  sprintf(postStr, "POST %s HTTP/1.1", uri);  // put together the string for HTTP POST
  
  Serial.println("request is sending");
  
  myClient.println(postStr);
  myClient.print("Host: ");
  myClient.println(host);
  myClient.println("Content-Type: application/json");
  myClient.print("Content-Length: "); 
  myClient.println(strlen(fakeData));
  myClient.println();    // extra `\r\n` to separate the http header and http body
  myClient.println(fakeData);          

  Serial.println(fakeData);
  Serial.println("request sent");
  Serial.print("Heap space remaining: ");
  Serial.println(ESP.getFreeHeap());  //Libraby macro to observe remaining heap size in bytes.
  delay(10000);  // myClient.stop must be placed after some delay, not before
  myClient.stop();  //prevents heap colliding with stack, remember, no automatic garbage collection in C/C++
                    //needed for ESP32 in this sketch but not ESP8266 consistently

}
