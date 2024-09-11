#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

const char* ssid = "Intellier-KB";
const char* password = "IntellierKb@4321";

String apiKey = "QRE7EM6XAELIIN0T";
const char* server = "http://api.thingspeak.com/update";

#define DHTPIN 27         // No semicolon here
#define DHTTYPE DHT11     // No semicolon here
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey + "&field1=" + String(temperature) + "&field2=" + String(humidity);

    http.begin(url);
    int httpCode = http.GET(); 

    if (httpCode > 0) {
      String payload = http.getString(); 
      Serial.println("ThingSpeak Response: " + payload);
    } else {
      Serial.println("Error in sending request");
    }

    http.end();  
  }

  delay(1000);  
}
