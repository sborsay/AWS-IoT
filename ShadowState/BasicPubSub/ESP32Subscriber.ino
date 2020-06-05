/* ESP32 AWS IoT
 *  
 * Simplest possible example (that I could come up with) of using an ESP32 with AWS IoT.
 *  
 * Author: Anthony Elder 
 * License: Apache License v2
 * Sketch Modified by Stephen Borsay for www.udemy.com
 * https://github.com/sborsay
 * Add in Char buffer utilizing sprintf to dispatch JSON data to AWS IoT Core
 * Use and replace your own SID, PW, AWS Account Endpoint, Client cert, private cert, x.509 CA root Cert
 */
#include <WiFiClientSecure.h>
#include <PubSubClient.h> // install with Library Manager, I used v2.6.0

const char* ssid = "<YOUR-WIFI-NETWORK>";
const char* password = "<YOUR-WIFI-PASSWORD>";

const char* awsEndpoint = "<YOUR-AWS-ID>-ats.iot.<YOUR-AWS-REGION>.amazonaws.com";

// Update the two certificate strings below. Paste in the text of your AWS 
// device certificate and private key. Add a quote character at the start
// of each line and a backslash, n, quote, space, backslash at the end 
// of each line:

// xxxxxxxxxx-certificate.pem.crt
const char* certificate_pem_crt = \

"-----BEGIN CERTIFICATE-----\n" \
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXKoZIhvcNAQEL\n" \
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n" \
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMDUxMjAwMDU0\n" \
"MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n" \
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOlFTaW4OuK9C+/cBKns\n" \
"jQNsyseHbP4C1+5Xji9a+Fvc3k+oWwDKXPswKz+hcuS6F2Cd0aAA8UghruwLTmSs\n" \
"aGzJdPyRbeq+n6ICRQ4iGOkdq2EH0ot11lcHaib8lxg1jamNhAJhswJCyFlQjrln\n" \
"HH2T6/GAyuE3NIui37N6ATusc0XeT+kgUGowQuWl9kuRJ2p3I7zLTHXtKS6fKCZ1\n" \
"hmqIcCqhU3viNhletB1kKipUk85T1/yjDzm500HNOf/G4z8npV98VwjndBBbgt7B\n" \
"/z4P74brP97bHcDSCiPbkwAu/UBth9MJPVTBNe0ug5QtppbXVyfwSEDMiInOtnwb\n" \
"YwcCAwEAAaNgMF4wHwYDVR0jBBgwFoAUaSs3dQYUconS0a4gHuUEnRYHFUkwHQYD\n" \
"VR0OBBYEFOvuPMsmqyF6Pf0bKL0zRkWahBSqMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n" \
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAW2zFLlkSk4oHFy9mGytPT6jnQ\n" \
"F8kAI7//aR4LyD5/Q9qjInQENQq5Gg1NAboXwabDqGUKYhNbX8EcLR338z77Sj7d\n" \
"mHMvPF13AgH7a7cenYN7nC9UJ1OfuDdo5fO30f2LZckUHyjo+iHO9nHTj1ujadiD\n" \
"J+uyGPujWoYTouYyQCsccMeeeyy04bLvvdqJgaOUzUwXIXO3ZAHQ4DQo55SuY5pC\n" \
"zUAh6VBM1BIsinbq9wLQhIR1qCNJEaEROdHhUQHmnIqK+d9smWR0gY8RLdJZPiNm\n" \
"rMo47WyiRtUQQfjhJHdMw6wdTwm9/KCanIESbft0PpPqv0BYlebgapqlX7jC\n" \ 
"-----END CERTIFICATE-----\n";

// xxxxxxxxxx-private.pem.key
const char* private_pem_key = \

"-----BEGIN RSA PRIVATE KEY-----\n" \
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXOL1r4W9zeT6hb\n" \
"AMpc+zArP6Fy5LoXYJ3RoADxSCGu7AtOZKxobMl0/JFt6r6fogJFDiIY6R2rYQfS\n" \
"i3XWVwdqJvyXGDWNqY2EAmGzAkLIWVCOuWccfZPr8YDK4Tc0i6Lfs3oBO6xzRd5P\n" \
"6SBQajBC5aX2S5EnancjvMtMde0pLp8oJnWGaohwKqFTe+I2GV60HWQqKlSTzlPX\n" \
"/KMPObnTQc05/8bjPyelX3xXCOd0EFuC3sH/Pg/vhus/3tsdwNIKI9uTAC79QG2H\n" \
"0wk9VME17S6DlC2mltdXJ/BIQMyIic62fBtjBwIDAQABAoIBAEatse4hWO40W3aM\n" \
"knkt7Df6caJtHRAFqolTzAf7Jz8mN9Go8BHYPr35BVGdSGOXItgqWUaCopi5Y/ev\n" \
"IUyNlIJJtAGGyugSY+nBiEzU9flD/RZEOX358TyjSX2QkPNL3qVd9H6IPaXUuEGf\n" \
"j+8LF+3QsO+Wl2IPyQL6EY+eytraPE0Ja3U+rKiYYdFo13xoefBLAXqc60+yfrxW\n" \
"QYmSOySVtExBqtu9CmPXlLHVAg71dAUoWOiuF8a4jmv/O/cLQGtyhZEzpwCleAgY\n" \
"1J6lQP3uYtMsIHAjtT98kcQvz62lceaplJnEolhFK3R9Vqb1ekHYyx6HH3K7G5ro\n" \
"lu3HE0ECgYEA/fO0KOkUdTtfO79mcLjgj1Mq489lwCAHmwJxtweBwYX+qqTHFssH\n" \
"2CwSBFamFbka3B1kkXBo/4Nq+YH/8NNvebzrDZCe9wh2xEJ8Z8gkT7LZLUb17Ecn\n" \
"NGBvZ/q528b376ZGLPLG7efKuPO/bm3k9byJI37yxJxzN4tpR9YtWecCgYEA6ybm\n" \
"8GSrBjaniRryMiGwu0A+fyfOVjgXrLDGD6U24Ggx8EyzsB9Gm6Sw8TLmGT0VOFWW\n" \
"vn8Jv52M/TIR2NB0wvv9vlIQzJ1OXzU3jMCnDNRIKpeJFrzyR2wlMu4alox/N2xK\n" \
"QC9I+VvSj6wilB7rvEVjI548LvSEp9KFiSzvyeECgYAYZocj+yk9DmRbIEveftUM\n" \
"0Gfpla7oI+u3KCf29yCvkIlkqgp7FSz+x1U/13eD1NJQ3f4gv1VHX8AykxwdbqNi\n" \
"ZhWLqx6rC8QlnWHIswFSKxQU9qGAf2Gz9aQesIeVJ2wBOc+6V+VnTIM8/CajnKmI\n" \
"NwJSR8tZutKTPrDx9xFKdwKBgBct0n3HroQCRylgmpcOcxlCD+cAyXn+NXc0RZGt\n" \
"O2eLXcDODrhBnPgil75uwfei6cCPzzgf1ZopsYgTs2anxZqitDRaaoWAzATefgdF\n" \
"xjGhiyuHnF+9aDRaGKCYj85fMvsHlZoWDx9lSbAXy6DGG36HaGpOYqhOOfVA7yDC\n" \
"aTCBAoGBAMJjaEDxxn3PdhLeepS6AgCcQyV3BeXrdRdiJSy7V3y2/Aywm3yYuzqR\n" \
"XcGj5pTJUO3cCz/Lh3PyoQK7c4eZ2YLdSn35J27OjmQa2EQVlz9VQC7FrAQgZ/hy\n" \
"dsslVtrgGnge/WemraqW9hgjZFPBVHBSmE3zUYO6uYQJBwtGvtx/\n" \
"-----END RSA PRIVATE KEY-----\n";

/* root CA can be downloaded in:
  https://www.symantec.com/content/en/us/enterprise/verisign/roots/VeriSign-Class%203-Public-Primary-Certification-Authority-G5.pem
*/
const char* rootCA = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
"rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
"-----END CERTIFICATE-----\n";

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

void setup() {
  Serial.begin(115200); delay(50); Serial.println();
  Serial.println("ESP32 AWS IoT Example");
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  wiFiClient.setCACert(rootCA);
  wiFiClient.setCertificate(certificate_pem_crt);
  wiFiClient.setPrivateKey(private_pem_key);
}

unsigned long lastPublish;
int msgCount;

void loop() {

  pubSubCheckConnect();
  
  if (millis() - lastPublish > 10000) {
   Serial.print("I'm the Subscriber ");
    lastPublish = millis();
  }// end if
  
} //end loop

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
     //If using Shadow service then topic publisher and subscriber must have a
     //Different ClientID than Subscriber. Reccomened it is a registered 'Thing'
      pubSubClient.connect("<YOUR-UNIQUE-CLIENTID>");
      delay(1000);
    }
    Serial.println(" connected");
    pubSubClient.subscribe("$aws/things/<YOUR-THING>/shadow/update/accepted"); //Here name in the topic name MUST be shared between Publisher & Subsciber
                                                                        
  }
  pubSubClient.loop();
}
