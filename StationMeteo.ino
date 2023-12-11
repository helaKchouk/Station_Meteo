#include <DHT.h>
#include <WiFi.h>
#include <FirebaseESP32.h>




#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID "Galaxy A21s8BEC"
#define WIFI_PASSWORD "rmhx8563"

FirebaseData firebaseData;
FirebaseJson json;

#define DHT11_PIN 18
#define DHTTYPE DHT11
DHT dht(DHT11_PIN, DHTTYPE);

float temp, hum;

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  dht.begin();

  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);



  // Start server
  server.begin();

  Serial.println("------------------------------------");
  Serial.println("Connected...");
  delay(5000);
}

void loop() {
  temp = dht.readTemperature();
  hum = dht.readHumidity();

  Serial.print("La température est ");
  Serial.print(temp);
  Serial.println("°C");

  Serial.print("L'humidité est ");
  Serial.print(hum);
  Serial.println("%");

  delay(1000);

  json.set("/la Température", temp);
  json.set("/l'humidité", hum);

  Serial.println("Data sent to Firebase");

  Firebase.updateNode(firebaseData, "/Sensor", json);
}