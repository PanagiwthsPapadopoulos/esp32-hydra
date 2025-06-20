## 🧠 Core

- ESP32-S3 DevKit or ESP32-WROOM-32U (with U.FL connector)

## 📡 Antennas & RF

- 1x 2.4GHz Omni Antenna (SMA Male, ~5 dBi)
- 1x 2.4GHz Directional Antenna (SMA Male, ~8–12 dBi)
- 1x U.FL to SMA Female Pigtail (to connect ESP32 to RF switch)
- 1x RF SPDT Switch (e.g., PE4259, SKY13385, HMC349)
- 2x SMA Male-to-Male jumpers (RF switch OUT → antennas)

## ⚙️ Control & Wiring

- 1x GPIO output from ESP32 (e.g., GPIO16)
- Breadboard or PCB
- Dupont wires
- 1x 10k pull-down resistor (optional, for RF switch control line)

## 🔌 Power

- USB cable or LiPo + TP4056 (optional for mobile setup)

## 🔧 Wiring Diagram (Text Style)

### RF Signal Path 
                                    +-----------------+
                                    |     ESP32       |
                                    |  (Wi-Fi/BLE MCU)|
                                    +--------+--------+
                                           |
                                           | RF Output
                                           |
                                           ▼
                                +----------------------+
                                |   U.FL to SMA Cable  |
                                +----------------------+
                                            |
                                            | SMA Connector
                                            ▼
                                 +----------------------+
                                 |    RF SPDT Switch    |
                                 |  (e.g., PE4259)      |
                                 +----+-----------+-----+
                                      |           |
                             Output A |           | Output B
                                (Omni)▼           ▼(Directional)
                            +-----------------+   +--------------------+
                            | Omni Antenna    |   | Directional Antenna|
                            | (2.4 GHz, ~5dBi)|   | (2.4 GHz, ~10dBi)  |
                            +-----------------+   +--------------------+
                                        ▲
                                        |
                            +------------------------+
                            |   Control Line (GPIO)  |
                            | From ESP32 to RF Switch|
                            +------------------------+


## 🧠 How It Works

- ESP32 generates all Wi-Fi/BLE traffic.
- The U.FL-to-SMA pigtail routes RF to the RF SPDT switch.
- The RF SPDT switch routes the signal to:
    - Omni antenna (for scanning, wide coverage)
    - Directional antenna (for targeting, attack)
- The control line from ESP32 toggles the active antenna.