# ESP32 – Bouton WiFi → Contrôle d’une LED distante

Ce projet utilise **deux ESP32** connectés sur le même réseau WiFi pour contrôler une LED à distance :

- **ESP32-Bouton** : détecte un appui sur un bouton et envoie une requête HTTP.
- **ESP32-LED** : reçoit cette requête et bascule l’état d’une LED (ON/OFF).

---

# 🧩 Structure du projet
├── esp32-led/
│ └── esp32-led.ino
│
├── esp32-bouton/
│ └── esp32-bouton.ino
│
└── README.md

---

# 🔌 Câblage

## 🟥 ESP32-LED

| Composant | ESP32 |
|----------|-------|
| LED rouge (patte longue +) | GPIO2 |
| LED patte courte (–) | Résistance 220Ω → GND |


---

## 🟦 ESP32-Bouton

Module bouton → ESP32 :

| Pin bouton | ESP32 |
|-----------|--------|
| VCC | 3.3V |
| GND | GND |
| OUT | GPIO16 |

---

# 📡 Adresses MAC

| Appareil | Adresse MAC |
|----------|-------------|
| ESP32 LED | `CC:DB:A7:94:BC:54` |
| ESP32 Bouton | `5C:01:3B:68:9B:68` |

Recommandé : réserver des IP fixes dans la box Internet.

---

# 🛠️ Installation et configuration

## ⚙️ Configuration de l’IDE Arduino

### 1. Installer le support ESP32

1. Arduino IDE → **Fichier → Préférences**
2. Dans *URL de gestionnaire de cartes supplémentaires*, ajouter : https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
3. Aller dans **Outils → Type de carte → Gestionnaire de cartes**
4. Rechercher **esp32**
5. Installer le paquet officiel **Espressif Systems**

---

### 2. Paramètres de compilation

Dans **Outils**, sélectionner :

- **Type de carte** : ESP32 Dev Module  
- **CPU Frequency** : 240MHz (WiFi/BT)  
- **Flash Frequency** : 80MHz  
- **Flash Mode** : QIO  
- **Flash Size** : 4MB (32Mb)  
- **Partition Scheme** : Default 4MB with spiffs (1.2MB APP / 1.5MB SPIFFS)  
- **Core Debug Level** : None  
- **Port** : le port COM où est connecté l’ESP32  

---

# 📡 Fonctionnement réseau

## 🌐 ESP32-LED (serveur HTTP)

Expose l’URL : http://esp32-led.local/toggle


Chaque appel `/toggle` :

- change l’état de la LED
- renvoie "ON" ou "OFF"

## 📡 ESP32-Bouton (client HTTP)

- détecte les appuis (anti-rebond)
- résout `esp32-led.local` via mDNS
- envoie `GET /toggle` à chaque pression

---

# 🚀 Utilisation

1. Brancher l’ESP32-LED → ouvrir le Moniteur Série  
2. Attendre l’IP + MAC + confirmation mDNS  
3. Brancher l’ESP32-Bouton  
4. Appuyer sur le bouton → la LED change d’état instantanément

---

# 🧪 Dépannage

### La LED ne s’allume pas
- Vérifier polarité
- Vérifier résistance 150–330Ω

### Le bouton ne fait rien
- OUT bien sur GPIO16
- Tester `INPUT_PULLUP`
- Vérifier que les deux ESP sont bien connectés au même WiFi

### mDNS ne fonctionne pas
- Certaines box bloquent `.local`
- Solution : réserver une IP fixe ou utiliser l’IP dans le code

---

# 📦 Évolutions possibles

- Communication **WebSocket** (temps réel)
- Utilisation de **MQTT**
- Passage à **ESP-NOW** (ultra rapide, sans WiFi)
- Multi-boutons / multi-LEDs

---

# 📜 Licence

Libre utilisation et modification.




