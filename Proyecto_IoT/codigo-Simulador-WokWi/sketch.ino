#include "DHTesp.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>  

DHTesp dht;
WiFiClientSecure espClient;
PubSubClient client(espClient);

static const char root_CA[] PROGMEM = R"EOF(-----BEGIN CERTIFICATE-----
MIIEVzCCAj+gAwIBAgIRAIOPbGPOsTmMYgZigxXJ/d4wDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjQwMzEzMDAwMDAw
WhcNMjcwMzEyMjM1OTU5WjAyMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg
RW5jcnlwdDELMAkGA1UEAxMCRTUwdjAQBgcqhkjOPQIBBgUrgQQAIgNiAAQNCzqK
a2GOtu/cX1jnxkJFVKtj9mZhSAouWXW0gQI3ULc/FnncmOyhKJdyIBwsz9V8UiBO
VHhbhBRrwJCuhezAUUE8Wod/Bk3U/mDR+mwt4X2VEIiiCFQPmRpM5uoKrNijgfgw
gfUwDgYDVR0PAQH/BAQDAgGGMB0GA1UdJQQWMBQGCCsGAQUFBwMCBggrBgEFBQcD
ATASBgNVHRMBAf8ECDAGAQH/AgEAMB0GA1UdDgQWBBSfK1/PPCFPnQS37SssxMZw
i9LXDTAfBgNVHSMEGDAWgBR5tFnme7bl5AFzgAiIyBpY9umbbjAyBggrBgEFBQcB
AQQmMCQwIgYIKwYBBQUHMAKGFmh0dHA6Ly94MS5pLmxlbmNyLm9yZy8wEwYDVR0g
BAwwCjAIBgZngQwBAgEwJwYDVR0fBCAwHjAcoBqgGIYWaHR0cDovL3gxLmMubGVu
Y3Iub3JnLzANBgkqhkiG9w0BAQsFAAOCAgEAH3KdNEVCQdqk0LKyuNImTKdRJY1C
2uw2SJajuhqkyGPY8C+zzsufZ+mgnhnq1A2KVQOSykOEnUbx1cy637rBAihx97r+
bcwbZM6sTDIaEriR/PLk6LKs9Be0uoVxgOKDcpG9svD33J+G9Lcfv1K9luDmSTgG
6XNFIN5vfI5gs/lMPyojEMdIzK9blcl2/1vKxO8WGCcjvsQ1nJ/Pwt8LQZBfOFyV
XP8ubAp/au3dc4EKWG9MO5zcx1qT9+NXRGdVWxGvmBFRAajciMfXME1ZuGmk3/GO
koAM7ZkjZmleyokP1LGzmfJcUd9s7eeu1/9/eg5XlXd/55GtYjAM+C4DG5i7eaNq
cm2F+yxYIPt6cbbtYVNJCGfHWqHEQ4FYStUyFnv8sjyqU8ypgZaNJ9aVcWSICLOI
E1/Qv/7oKsnZCWJ926wU6RqG1OYPGOi1zuABhLw61cuPVDT28nQS/e6z95cJXq0e
K1BcaJ6fJZsmbjRgD5p3mvEf5vdQM7MCEvU0tHbsx2I5mHHJoABHb8KVBgWp/lcX
GWiWaeOyB7RP+OfDtvi2OsapxXiV7vNVs7fMlrRjY1joKaqmmycnBvAq14AEbtyL
sVfOS66B8apkeFX2NY4XPEYV4ZSCe8VHPrdrERk2wILG3T/EGmSIkCYVUMSnjmJd
VQD9F6Na/+zmXCc=
-----END CERTIFICATE-----)EOF";

const int dht_pin = 23;
const int red_pin = 25;
const int blue_pin = 12;

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_user = "javierrdgzrz";
const char* mqtt_pass = "11ag08dsl04.";
const char* mqtt_server = "mqtt.practicas-iot.tokioschool.com";
const int mqtt_port = 8883;

void setup() {
  Serial.begin(115200);
  Serial.println("Practica Modulo 1");

  dht.setup(dht_pin, DHTesp::DHT22);

  pinMode(red_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

  Serial.print("Connecting to wifi");
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }

  Serial.println("Connection Succesfull!!");

  espClient.setCACert(root_CA);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callBack);


}

void reconnect(){
  Serial.println("Establishing connection with MQTT server");
  while (!client.connected()) {
    Serial.print(".");
    bool conectado = client.connect(String(esp_random()).c_str(), mqtt_user, mqtt_pass);

    if (conectado) {
      Serial.println("Connection established!");
      client.subscribe("javierrdgzrz/config");
    } else {
      Serial.println("An error has occurred. Trying to reconnect...");
      Serial.println(client.state());
      delay(3000);
    }
  }
}

void callBack(char* topic, byte* payload, unsigned int length){

  if (payload[0] == 48) {
    
    Serial.println("Turning off both LEDs...");
    digitalWrite(blue_pin, LOW);
    digitalWrite(red_pin, LOW);

  }

  if (payload[0] == 49) {

    Serial.println("Turning on blue LED...");
    digitalWrite(blue_pin, HIGH);
    Serial.println("Blue LED on!");

  } 

  if (payload[0] == 50){

    Serial.println("Turning on red LED...");
    digitalWrite(red_pin, HIGH);
    Serial.println("Red LED on!");
  }
}

unsigned long lastRead = 0;

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  if (millis() - lastRead > 2000) { 

    lastRead = millis();

    float temp = dht.getTemperature();
    float humidity = dht.getHumidity();

    Serial.println("temperature: " + String(temp) + " °C");
    Serial.println("humidity: " + String(humidity) + " %");

    Serial.println("--------------------");

    client.publish("javierrdgzrz/temperature", String(temp, 2).c_str());
    client.publish("javierrdgzrz/humidity", String(humidity, 2).c_str());
  }

  delay(10);
}

