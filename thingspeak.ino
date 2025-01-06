#include <ESP8266WiFi.h>
#include <DHT.h>
#include <WiFiClient.h>

// WiFi settings
const char* ssid     = "IoT-Gateway";         // Replace with your WiFi SSID
const char* password = "password";     // Replace with your WiFi password

// ThingSpeak settings
const char* server = "api.thingspeak.com";
String apiKey = "writeApikeyFromthingspeak";            // Replace with your ThingSpeak Write API key

// DHT22 settings
#define DHTPIN D2                          // Pin connected to DHT22
#define DHTTYPE DHT22                      // DHT22 sensor type

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  // Start the Serial communication
  Serial.begin(115200);
  Serial.println("DHT22 sensor with ThingSpeak");

  // Initialize the DHT sensor
  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
}

void loop() {
  // Wait a few seconds between measurements
  delay(2000);

  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();  // Celsius by default

  // Check if any readings failed and exit early
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the results to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C | Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Connect to ThingSpeak and send the data
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temperature);
    postStr += "&field2=";
    postStr += String(humidity);
    postStr += "\r\n\r\n";

    // Send the HTTP request to ThingSpeak
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println("Data sent to ThingSpeak");

    // Close the connection
    client.stop();
  }

  // Wait 20 seconds to match ThingSpeak's rate limit (15 seconds minimum)
  delay(20000);
}
