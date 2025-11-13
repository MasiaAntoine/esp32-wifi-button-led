#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

// ====== CONFIG WIFI ======
const char* ssid     = "Ton_SSID";
const char* password = "Ton_MotDePasse";
const char* hostname = "esp32-led";    // nom mDNS -> esp32-led.local

// ====== LED ======
const int ledPin = 2;                  // adapte si besoin
bool ledOn = false;

WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain",
              String("OK\nState: ") + (ledOn ? "ON" : "OFF") +
              "\nMAC: " + WiFi.macAddress() +
              "\nIP: " + WiFi.localIP().toString() + "\n");
}

void handleToggle() {
  ledOn = !ledOn;
  digitalWrite(ledPin, ledOn ? HIGH : LOW);
  server.send(200, "text/plain", ledOn ? "ON\n" : "OFF\n");
  Serial.print("Toggle -> "); Serial.println(ledOn ? "ON" : "OFF");
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(hostname);
  WiFi.begin(ssid, password);

  Serial.print("WiFi -> "); Serial.println(ssid);
  for (int i = 0; i < 30 && WiFi.status() != WL_CONNECTED; i++) {
    delay(500); Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connecté");
    Serial.print("IP: ");  Serial.println(WiFi.localIP());
    Serial.print("MAC: "); Serial.println(WiFi.macAddress());
  } else {
    Serial.println("Échec WiFi (serveur non démarré)");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  connectWiFi();

  if (WiFi.status() == WL_CONNECTED) {
    if (MDNS.begin(hostname)) {
      Serial.print("mDNS: http://"); Serial.print(hostname); Serial.println(".local/");
    } else {
      Serial.println("mDNS init FAILED");
    }

    server.on("/", handleRoot);
    server.on("/toggle", HTTP_GET, handleToggle);
    server.begin();
    Serial.println("HTTP server démarré");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  }
}
