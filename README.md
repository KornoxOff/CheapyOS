# 🚀 CheapyOS — Le système d'exploitation ESP32 style Android

> **Un OS complet, moderne et fluide pour ESP32 CYD (Cheap Yellow Display)**
> Inspiré d'Android 16, conçu pour les makers, les développeurs et les passionnés d'embarqué.

![Version](https://img.shields.io/badge/version-4.0-blue)
![Platform](https://img.shields.io/badge/platform-ESP32-orange)
![License](https://img.shields.io/badge/license-MIT-green)
![Language](https://img.shields.io/badge/langage-C%2B%2B-blue)
![Status](https://img.shields.io/badge/status-actif-brightgreen)

---

## 📖 Présentation

**CheapyOS** est un système d'exploitation complet pour **ESP32-2432W328R (CYD)** et variantes. Il transforme une simple carte à 15€ en un mini-téléphone fonctionnel avec :

- 🎨 Une **interface tactile moderne** inspirée d'Android 16
- 📱 Un **launcher** avec dock, pages multiples et widgets
- 🔒 Un **écran de verrouillage** avec cadenas animé
- 👋 Un **Welcome Screen** de configuration au premier démarrage
- 🎬 Des **animations fluides** partout (transitions, zoom, fade, ripple)
- 📦 **20 applications** préinstallées, prêtes à l'emploi
- ⚙️ Des **paramètres complets** (WiFi, Bluetooth, Système, Affichage, Batterie, Stockage)
- 📊 **Barre de statut** avec heure, WiFi, batterie
- 🧭 **Barre de navigation** style Android (Home, Back, Recent)
- 🌐 **Connexion WiFi automatique** en arrière-plan
- 🎨 **3 thèmes** (Bleu, Violet, Vert)

---

## ✨ Fonctionnalités détaillées

### 🎬 Système de démarrage

| Étape | Description |
|-------|-------------|
| **1. Boot Logo** | Logo "C" fixe pendant 7 secondes, style Apple/Android |
| **2. Splash Screen** | Animation "CheapyOS v4.0" avec barre de chargement |
| **3. Welcome Screen** | Configuration initiale en 4 étapes (1er boot uniquement) |
| **4. Lock Screen** | Écran de verrouillage avec cadenas animé |
| **5. Home** | Launcher Android avec dock et pages multiples |

### 🎨 Interface utilisateur

- **Design Material You** : coins arrondis, ombres douces, dégradés
- **Animations** : zoom, fade, ripple, slide, transitions fluides
- **Thème dynamique** : 3 palettes au choix (Bleu/Violet/Vert)
- **Barre de statut** complète : heure, WiFi, batterie
- **Barre de navigation** : Home, Back, Recent apps
- **Icônes dessinées** : aucune dépendance à des images externes

### 📱 Applications incluses (20)

#### 🛠️ Outils essentiels
| App | Description |
|-----|-------------|
| 🧮 **Calculatrice** | Calculs avec +, -, ×, ÷, %, effacement |
| ⚙️ **Paramètres** | Configuration complète du système |
| 🕐 **Horloge** | Heure et date en temps réel |
| 📝 **Notes** | Prise de notes simple |
| 📁 **Fichiers** | Explorateur SPIFFS + SD |

#### 🌐 Connectivité
| App | Description |
|-----|-------------|
| 📶 **WiFi** | Scan des réseaux disponibles |
| ☀️ **Météo** | Affichage météo (Paris par défaut) |

#### 🎨 Multimédia
| App | Description |
|-----|-------------|
| 🎵 **Musique** | Lecteur avec barre de progression |
| 🖼️ **Galerie** | Visualiseur d'images |
| 📖 **Livre** | Lecteur de texte multi-pages |

#### 🎮 Jeux
| App | Description |
|-----|-------------|
| 🐍 **Snake** | Jeu classique avec score |
| 🏓 **Pong** | Pong contre IA |
| ⭕ **Morpion** | Tic-Tac-Toe contre IA |

#### 🧰 Utilitaires
| App | Description |
|-----|-------------|
| 📅 **Calendrier** | Calendrier mensuel |
| ⏰ **Réveil** | Chrono et minuteur |
| 📷 **Caméra** | Simulateur d'appareil photo |
| 🎨 **Dessin** | Tableau blanc tactile |
| 📊 **Système** | Infos CPU, RAM, Flash, WiFi |
| 🔢 **Convertisseur** | Temp/Longueur/Poids |
| 🎲 **Dés** | Lancer de dé |

### ⚙️ Paramètres Android 16 style

Menu complet avec sous-menus :
- 📶 **Réseau et Internet** → Scan WiFi + connexion
- 📡 **Appareils connectés** → Bluetooth
- 📱 **Applications** → Liste des apps
- 🔔 **Notifications** → Gestion
- 🔋 **Batterie** → État détaillé
- 💾 **Stockage** → Utilisation Flash/SPIFFS
- ☀️ **Affichage** → Luminosité, rotation
- 🔊 **Son** → Volume
- 🖥️ **Système** → Informations détaillées (CPU, RAM, MAC, SDK...)
- ℹ️ **À propos** → Version et MAJ

---

## 🔧 Matériel requis

| Composant | Détail |
|-----------|--------|
| **Carte** | ESP32-2432W328R (Cheap Yellow Display) |
| **Écran** | ILI9341 2.8" 240×320 |
| **Tactile** | XPT2046 résistif |
| **CPU** | ESP32-WROVER (recommandé) |
| **RAM** | 4 MB Flash |
| **Alimentation** | USB-C ou batterie 18650 |

### Alternatives compatibles
- ESP32-2432S028R (CYD classique)
- ESP32-2432S024R (2.4")
- Clones CYD avec ILI9341

---

## 🚀 Installation

### 1. Prérequis

- **Arduino IDE 2.x** ([télécharger](https://www.arduino.cc/en/software))
- **Support ESP32** (v3.x minimum)
- **Bibliothèque TFT_eSPI** (Bodmer)

### 2. Bibliothèques nécessaires

Installation via **Croquis → Inclure une bibliothèque → Gérer les bibliothèques** :

TFT_eSPI (Bodmer)

ESP32Time (FBiego)

ArduinoJson (Benoit Blanchon)

Preferences (incluse avec ESP32)


### 3. Configuration TFT_eSPI

**Éditez** `Documents/Arduino/libraries/TFT_eSPI/User_Setup.h` :

```cpp
#define USER_SETUP_INFO "CYD-2432W328R"

#define ILI9341_2_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

#define TFT_RGB_ORDER TFT_BGR

#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST  -1

#define TFT_BL   27
#define TFT_BACKLIGHT_ON HIGH

#define TOUCH_CS 33

#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY  16000000
#define SPI_TOUCH_FREQUENCY  2500000

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

#define USE_HSPI_PORT
```cpp

ESP32OS/
├── ESP32OS.ino                 # Point d'entrée
├── App.h                       # Classe de base des apps
├── System.h                    # Noyau du système
├── Launcher.h                  # Home screen
├── StatusBar.h                 # Barre de statut
│
├── CalculatorApp.h             # 🧮 Calculatrice
├── SettingsApp.h               # ⚙️ Paramètres
├── ClockApp.h                  # 🕐 Horloge
├── NotesApp.h                  # 📝 Notes
├── WiFiApp.h                   # 📶 WiFi
├── MusicApp.h                  # 🎵 Musique
├── GalleryApp.h                # 🖼️ Galerie
├── WeatherApp.h                # ☀️ Météo
├── GameApp.h                   # 🐍 Snake
├── FileManagerApp.h            # 📁 Fichiers
├── CalendarApp.h               # 📅 Calendrier
├── AlarmApp.h                  # ⏰ Réveil
├── CameraApp.h                 # 📷 Caméra
├── PaintApp.h                  # 🎨 Dessin
├── PongApp.h                   # 🏓 Pong
├── TicTacToeApp.h              # ⭕ Morpion
├── SysInfoApp.h                # 📊 Système
├── ConverterApp.h              # 🔢 Convertisseur
├── DiceApp.h                   # 🎲 Dés
└── BookApp.h                   # 📖 Livre

🎯 Roadmap
✅ Fait (v4.0)
☑ Boot logo fixe 7s
☑ Splash animé
☑ Welcome Screen 4 étapes
☑ Lock Screen avec cadenas
☑ Launcher avec dock + 4 pages
☑ Barre de navigation (Home/Back/Recent)
☑ Paramètres Android 16 style
☑ WiFi en arrière-plan
☑ 20 applications
☑ 3 thèmes de couleur
🚧 En cours (v4.1)
□ Clavier virtuel pour saisie WiFi
□ Application Météo avec API réelle
□ Bluetooth (scan + connexion)
□ Lecteur SD complet
□ Notifications panneau déroulant
🔮 Futur (v5.0)
□ Multi-utilisateurs
□ Apps récentes (Recent apps réel)
□ Widget météo/heure sur le home
□ Personnalisation des icônes
□ Lecteur audio MP3 (I2S)
□ Mode économie batterie
□ Support SD pour apps externes
□ OTA Update depuis GitHub
