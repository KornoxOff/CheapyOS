# 🚀 CheapyOS — The Android-style ESP32 Operating System

> **A complete, modern and fluid OS for ESP32 CYD (Cheap Yellow Display)**
> Inspired by Android 16, designed for makers, developers and embedded enthusiasts.

![Version](https://img.shields.io/badge/version-4.0-blue)
![Platform](https://img.shields.io/badge/platform-ESP32-orange)
![License](https://img.shields.io/badge/license-MIT-green)
![Language](https://img.shields.io/badge/language-C%2B%2B-blue)
![Status](https://img.shields.io/badge/status-active-brightgreen)

---

## 📖 Overview

**CheapyOS** is a complete operating system for the **ESP32-2432W328R (CYD)** and its variants. It transforms a simple 15€ board into a functional mini-phone featuring:

- 🎨 A **modern touch interface** inspired by Android 16
- 📱 A **launcher** with dock, multiple pages and widgets
- 🔒 A **lock screen** with animated padlock
- 👋 A **Welcome Screen** for first-time setup
- 🎬 **Fluid animations** everywhere (transitions, zoom, fade, ripple)
- 📦 **20 pre-installed applications** ready to use
- ⚙️ **Complete settings** (WiFi, Bluetooth, System, Display, Battery, Storage)
- 📊 **Status bar** with time, WiFi, battery
- 🧭 **Navigation bar** Android-style (Home, Back, Recent)
- 🌐 **Automatic background WiFi** connection
- 🎨 **3 themes** (Blue, Purple, Green)

---

## ✨ Detailed features

### 🎬 Boot sequence

| Step | Description |
|------|-------------|
| **1. Boot Logo** | Static "C" logo for 7 seconds, Apple/Android style |
| **2. Splash Screen** | Animated "CheapyOS v4.0" with loading bar |
| **3. Welcome Screen** | First-time setup in 4 steps (first boot only) |
| **4. Lock Screen** | Lock screen with animated padlock |
| **5. Home** | Android launcher with dock and multiple pages |

### 🎨 User interface

- **Material You design** : rounded corners, soft shadows, gradients
- **Animations** : zoom, fade, ripple, slide, fluid transitions
- **Dynamic theme** : 3 color palettes (Blue/Purple/Green)
- **Complete status bar** : time, WiFi, battery
- **Navigation bar** : Home, Back, Recent apps
- **Hand-drawn icons** : no external images required

### 📱 Included applications (20)

#### 🛠️ Essential tools
| App | Description |
|-----|-------------|
| 🧮 **Calculator** | Calculations with +, -, ×, ÷, %, clear |
| ⚙️ **Settings** | Complete system configuration |
| 🕐 **Clock** | Real-time time and date |
| 📝 **Notes** | Simple note taking |
| 📁 **Files** | SPIFFS + SD explorer |

#### 🌐 Connectivity
| App | Description |
|-----|-------------|
| 📶 **WiFi** | Available networks scanner |
| ☀️ **Weather** | Weather display (Paris by default) |

#### 🎨 Media
| App | Description |
|-----|-------------|
| 🎵 **Music** | Player with progress bar |
| 🖼️ **Gallery** | Image viewer |
| 📖 **Book** | Multi-page text reader |

#### 🎮 Games
| App | Description |
|-----|-------------|
| 🐍 **Snake** | Classic game with score |
| 🏓 **Pong** | Pong against AI |
| ⭕ **Tic-Tac-Toe** | Tic-Tac-Toe against AI |

#### 🧰 Utilities
| App | Description |
|-----|-------------|
| 📅 **Calendar** | Monthly calendar |
| ⏰ **Alarm** | Stopwatch and timer |
| 📷 **Camera** | Camera simulator |
| 🎨 **Paint** | Touch whiteboard |
| 📊 **System** | CPU, RAM, Flash, WiFi info |
| 🔢 **Converter** | Temp/Length/Weight |
| 🎲 **Dice** | Dice roll |

### ⚙️ Android 16-style Settings

Complete menu with submenus :
- 📶 **Network & Internet** → WiFi scan + connection
- 📡 **Connected devices** → Bluetooth
- 📱 **Applications** → Apps list
- 🔔 **Notifications** → Management
- 🔋 **Battery** → Detailed status
- 💾 **Storage** → Flash/SPIFFS usage
- ☀️ **Display** → Brightness, rotation
- 🔊 **Sound** → Volume
- 🖥️ **System** → Detailed information (CPU, RAM, MAC, SDK...)
- ℹ️ **About** → Version and updates

---

## 🔧 Required hardware

| Component | Detail |
|-----------|--------|
| **Board** | ESP32-2432W328R (Cheap Yellow Display) |
| **Screen** | ILI9341 2.8" 240×320 |
| **Touch** | Resistive XPT2046 |
| **CPU** | ESP32-WROVER (recommended) |
| **RAM** | 4 MB Flash |
| **Power** | USB-C or 18650 battery |

### Compatible alternatives
- ESP32-2432S028R (classic CYD)
- ESP32-2432S024R (2.4")
- CYD clones with ILI9341

---

## 🚀 Installation

### 1. Prerequisites

- **Arduino IDE 2.x** ([download](https://www.arduino.cc/en/software))
- **ESP32 support** (v3.x minimum)
- **TFT_eSPI library** (Bodmer)

### 2. Required libraries

Install via **Sketch → Include Library → Manage Libraries** :

TFT_eSPI (Bodmer)

ESP32Time (FBiego)

ArduinoJson (Benoit Blanchon)

Preferences (included with ESP32)


### 3. TFT_eSPI configuration

**Edit** `Documents/Arduino/libraries/TFT_eSPI/User_Setup.h` :

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

# PROJECT STRUCTURE

ESP32OS/
├── ESP32OS.ino                 # Entry point
├── App.h                       # Base app class
├── System.h                    # System core
├── Launcher.h                  # Home screen
├── StatusBar.h                 # Status bar
│
├── CalculatorApp.h             # 🧮 Calculator
├── SettingsApp.h               # ⚙️ Settings
├── ClockApp.h                  # 🕐 Clock
├── NotesApp.h                  # 📝 Notes
├── WiFiApp.h                   # 📶 WiFi
├── MusicApp.h                  # 🎵 Music
├── GalleryApp.h                # 🖼️ Gallery
├── WeatherApp.h                # ☀️ Weather
├── GameApp.h                   # 🐍 Snake
├── FileManagerApp.h            # 📁 Files
├── CalendarApp.h               # 📅 Calendar
├── AlarmApp.h                  # ⏰ Alarm
├── CameraApp.h                 # 📷 Camera
├── PaintApp.h                  # 🎨 Paint
├── PongApp.h                   # 🏓 Pong
├── TicTacToeApp.h              # ⭕ Tic-Tac-Toe
├── SysInfoApp.h                # 📊 System
├── ConverterApp.h              # 🔢 Converter
├── DiceApp.h                   # 🎲 Dice
└── BookApp.h                   # 📖 Book

# 🎯 Roadmap

✅ Done (v4.0)
☑ Static 7s boot logo
☑ Animated splash
☑ 4-step Welcome Screen
☑ Lock Screen with padlock
☑ Launcher with dock + 4 pages
☑ Navigation bar (Home/Back/Recent)
☑ Android 16-style Settings
☑ Background WiFi
☑ 20 applications
☑ 3 color themes

🚧 In progress (v4.1)
□ Virtual keyboard for WiFi input
□ Real Weather API integration
□ Bluetooth (scan + connect)
□ Full SD card reader
□ Notification drawer

🔮 Future (v5.0)
□ Multi-user support
□ Real Recent apps
□ Weather/clock widget on home
□ Custom icon themes
□ MP3 audio player (I2S)
□ Battery saver mode
□ External apps via SD
□ OTA Update from GitHub

