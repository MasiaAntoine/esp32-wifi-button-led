# ESP32 – Bouton WiFi → LED distante

Contrôler une LED distante via WiFi avec deux cartes ESP32 : l’une agit comme bouton connecté, l’autre héberge un mini-serveur HTTP qui commute la LED. Ce dépôt fournit les deux sketches Arduino prêts à téléverser.

---

## ⚡️ Aperçu rapide

- `esp32-led` : serveur HTTP qui expose l’endpoint `/toggle`. Chaque appel inverse l’état de la LED et renvoie `ON` ou `OFF`.
- `esp32-bouton` : client HTTP qui détecte les pressions, applique un anti-rebond logiciel et déclenche `GET /toggle` via mDNS (`esp32-led.local`).
- 🎥 Démo rapide en vidéo : [ESP32 WiFi Bouton → LED (YouTube Shorts)](https://www.youtube.com/shorts/JkvxhKgQAlg)

[![Miniature de la démo vidéo](https://img.youtube.com/vi/JkvxhKgQAlg/maxresdefault.jpg)](https://www.youtube.com/shorts/JkvxhKgQAlg)

---

## 🧰 Matériel requis

- 2 × cartes ESP32 DevKit (ou équivalentes)
- 1 × bouton poussoir (module ou simple switch)
- 1 × LED rouge + résistance 150–330 Ω
- Fils dupont, breadboard, alimentation USB

---

## 🔌 Câblage

### 💡 ESP32-LED

| Composant                 | ESP32                    |
| ------------------------- | ------------------------ |
| LED (anode, patte longue) | `GPIO2`                  |
| LED (cathode)             | Résistance 220 Ω → `GND` |

![Schéma de câblage ESP32-LED](https://raw.githubusercontent.com/MasiaAntoine/esp32-wifi-button-led/refs/heads/main/schema/esp-led.png)

### 🔘 ESP32-Bouton

| Pin module bouton | ESP32    |
| ----------------- | -------- |
| `VCC`             | `3V3`    |
| `GND`             | `GND`    |
| `OUT`             | `GPIO16` |

> Le code active l’`INPUT_PULLUP`; selon votre module, adaptez le câblage (contact à la masse ou au VCC).

![Schéma de câblage ESP32-Bouton](https://raw.githubusercontent.com/MasiaAntoine/esp32-wifi-button-led/refs/heads/main/schema/esp-button.png)

---

## 🧩 Structure du projet

```
esp32-wifi-button-led/
├── esp32-led/
│   └── esp32-led.ino
├── esp32-bouton/
│   └── esp32-bouton.ino
└── README.md
```

---

## 🛠️ Préparation de l’environnement

1. **Arduino IDE** → _Fichier ▸ Préférences_ → ajouter<br>`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` dans _URL de gestionnaire de cartes supplémentaires_.
2. _Outils ▸ Type de carte ▸ Gestionnaire de cartes_ → rechercher « esp32 » → installer **Espressif Systems**.
3. Dans _Outils_ sélectionner :
   - `Board` : **ESP32 Dev Module**
   - `Upload Speed` : 921600 (ou 115200 si instable)
   - `CPU Frequency` : 240 MHz (WiFi/BT)
   - `Flash Frequency` : 80 MHz
   - `Flash Mode` : QIO
   - `Flash Size` : 4 MB (32 Mb)
   - `Partition Scheme` : Default 4MB with spiffs (1.2MB APP / 1.5MB SPIFFS)
   - `Core Debug Level` : None
   - `Port` : le port série correspondant à chaque carte

---

## 🧾 Configuration des sketches

1. Ouvrir `esp32-led/esp32-led.ino`.
   - Renseigner votre SSID et mot de passe WiFi (lignes 6-7 : `const char* ssid`, `const char* password`).
   - Facultatif : définir `LED_PIN` si vous utilisez une autre broche.
2. Ouvrir `esp32-bouton/esp32-bouton.ino`.
   - Renseigner les mêmes identifiants WiFi (également lignes 6-7).
   - Ajuster `BUTTON_PIN` et `HOSTNAME` si besoin.

> Astuce : stocker les identifiants dans un fichier `secrets.h` ignoré par Git, et l’inclure dans chaque sketch (`#include "secrets.h"`).

---

## 🚀 Téléversement

1. Connecter uniquement l’ESP32-LED → téléverser `esp32-led.ino`.
2. Ouvrir le Moniteur Série (115200 bauds) → vérifier l’acquisition IP et le message `mDNS responder started`.
3. Connecter l’ESP32-Bouton → téléverser `esp32-bouton.ino`.
4. Vérifier dans le Moniteur Série du bouton que la résolution mDNS (`esp32-led.local`) aboutit et que chaque pression génère `GET /toggle`.

---

## ✅ Tests de fonctionnement

1. Appuyer sur le bouton : la LED doit changer d’état instantanément.
2. Depuis un navigateur : `http://esp32-led.local/toggle` doit retourner `ON` ou `OFF`.
3. Pour tester sans bouton : envoyer un `curl http://esp32-led.local/toggle`.

---

## 🧪 Dépannage

- **💤 La LED reste éteinte**  
  Vérifier la polarité, la résistance, la broche `GPIO2` ou modifier `LED_PIN`.

- **🙈 Bouton inactif**  
  Confirmer le câblage sur `GPIO16`, vérifier l’anti-rebond, tester avec un simple pont vers `GND`.

- **📶 mDNS indisponible**  
  Certaines box bloquent `.local`. Utiliser l’adresse IP affichée dans le Moniteur Série, ou réserver une IP fixe via la box.

- **🌪️ WiFi instable**  
  Réduire la vitesse de téléversement, vérifier l’alimentation USB, rapprocher les modules du routeur.

---

## 🔭 Pour aller plus loin

- Remplacer HTTP par MQTT ou WebSocket pour le temps réel.
- Ajouter plusieurs boutons / LEDs avec gestion d’états.
- Utiliser ESP-NOW pour supprimer la dépendance au WiFi.
- Intégrer Home Assistant via ESPHome.

---

## 📜 Licence

Projet librement réutilisable et modifiable. Attribution appréciée.
