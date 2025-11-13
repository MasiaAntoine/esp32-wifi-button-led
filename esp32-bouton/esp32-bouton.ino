#include <WiFi.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>

// ====== CONFIG WIFI ======
const char* ssid     = "Ton_SSID";
const char* password = "Ton_MotDePasse";
const char* hostname = "esp32-btn";     // nom de CET ESP (optionnel)

// ====== CIBLE ======
const char* ledHost = "esp32-led";      // doit matcher l'autre sketch
IPAddress ledIP;                         // sera résolu via mDNS

// ====== BOUTON ======
const int boutonPin = 16;                // OUT de ton module
const unsigned long debounceMs = 50;
int lastStable = LOW;                    // adapte à ta logique (HIGH si repos=HIGH)
int lastRead = LOW;
unsigned long lastChange = 0;

bool resolveLedIP() {
  if (!MDNS.begin(hostname)) {
    // on tente malgré tout, mDNS requête ne dépend pas de begin() côté client,
    // mais begin() facilite l’annonce locale de CE device.
  }
  // Essaye de résoudre 10 fois au cas où
  for (int i = 0; i < 10; i++) {
    IPAddress ip = MDNS.queryHost(ledHost, 1000); // timeout 1s
    if (ip != INADDR_NONE) {
      ledIP = ip;
      Serial.print("Résolu: "); Serial.print(ledHost); Serial.print(".local -> ");
      Serial.println(ledIP);
      return true;
    }
    delay(200);
  }
  Serial.println("mDNS: impossible de résoudre l'hôte LED");
  return false;
}

bool sendToggle() {
  if (ledIP == IPAddress((uint32_t)0)) {
    if (!resolveLedIP()) return false;
  }
  HTTPClient http;
  String url = "http://" + ledIP.toString() + "/toggle";
  http.begin(url);
  int code = http.GET();
  if (code > 0) {
    String payload = http.getString();
    Serial.print("LED -> "); Serial.print(code); Serial.print(" | ");
    Serial.println(payload);
  } else {
    Serial.print("HTTP FAIL: "); Serial.println(code);
    // tenter de re-résoudre si échec (IP peut changer)
    ledIP = IPAddress((uint32_t)0);
  }
  http.end();
  return code == 200;
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
    Serial.println("Échec WiFi");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(boutonPin, INPUT); // ou INPUT_PULLUP selon ton module

  connectWiFi();
  if (WiFi.status() == WL_CONNECTED) {
    resolveLedIP();
  }
}

void loop() {
  // Lecture + anti-rebond
  int reading = digitalRead(boutonPin);
  if (reading != lastRead) {
    lastChange = millis();
    lastRead = reading;
  }

  if (millis() - lastChange > debounceMs) {
    if (reading != lastStable) {
      lastStable = reading;

      // Détection de front montant (appui)
      if (lastStable == HIGH) {
        Serial.println("Bouton: APPUI détecté -> toggle LED");
        sendToggle();
      }
    }
  }

  // Optionnel: reconnexion auto si WiFi tombe
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    // On re-résout l'IP quand on revient en ligne
    if (WiFi.status() == WL_CONNECTED) ledIP = IPAddress((uint32_t)0);
  }
}
