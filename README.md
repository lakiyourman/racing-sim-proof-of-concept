#    racing-sim-proof-of-concept


A 1-DOF (one degree of freedom) vehicle motion simulator prototype built with **SimTools** and an **ESP32** microcontroller. The system extracts real-time telemetry (position and acceleration vectors) from racing games, transmits axis position values over USB serial, actuates a servo motor to simulate vehicle surge, and displays live axis values on a 16×2 character LCD

---

## System Overview

1. **Telemetry Extraction (PC):** **SimTools** hooks into the game's memory (demonstrated with *Live for Speed*) to read vehicle position and acceleration vectors.
2. **Serial Transmission (USB):** SimTools formats the 10-bit axis position (`0`–`1023`) as a semicolon-delimited decimal string (`<Axis1a>;`) and sends it over USB serial.
3. **Actuation & Display (ESP32):** The ESP32 parses incoming values, maps the 10-bit range to servo angles (`20°`–`173°`), drives an SG90 micro servo via PWM, and outputs the current raw position value onto a 16×2 LCD.

[![racing-sim-proof-of-concept](https://img.youtube.com/vi/kdlv3q1Omgo/hqdefault.jpg)](https://www.youtube.com/watch?v=kdlv3q1Omgo)

[Watch racing-sim-proof-of-concept on YouTube](https://www.youtube.com/watch?v=kdlv3q1Omgo)

---

## SimTools Configuration

### 1. Interface Setup (Serial Output)
Configure **Motion Output 1** under **Interface Setup** using the **Serial** plugin:

| Parameter | Value | Description |
| :--- | :--- | :--- |
| **Selected Plugin** | `Serial` | Standard USB-UART serial communication |
| **ComPort** | `COMx` | Assigned ESP32 USB serial port (e.g., `COM1`) |
| **BitsPerSec** | `115200` (or `9600`) | Must match `Serial.begin(...)` in the ESP32 sketch |
| **Data Bits / Parity / Stop Bits** | `8` / `None` / `1` | Standard 8N1 serial framing |
| **Output - Bit Range** | `10` | 10-bit resolution (`0` to `1023`, center at `511`) |
| **Output - Type** | `Decimal` | ASCII decimal numbers |
| **Startup - Output** | `:511;` | Centers the actuator on session start |
| **Interface - Output** | `<Axis1a>;` | Delimits each axis frame with `;` |
| **Shutdown - Output** | `:511;` | Returns actuator to center on exit |
| **Output Rate** | `1 ms` | High-frequency telemetry update interval |

### 2. Axis Assignments
Under **Motion Settings > Axis Assignments** (`Generic 2D Standard` plugin), **Axis1a** is mapped to **Surge** at **100%** to simulate longitudinal acceleration and braking forces. Additional forces (such as Pitch, Roll, or Sway) can be mixed into `Axis1a` or assigned to additional axes (`Axis2a`–`Axis6a`).

---

## Hardware Architecture & Wiring

### Components
* **Microcontroller:** ESP32 development board
* **Actuator:** SG90 9g micro servo motor (50 Hz PWM)
* **Display:** 16×2 character LCD (HD44780 compatible, 4-bit parallel mode)
* **Contrast Adjustment:** $10\,\text{k}\Omega$ potentiometer connected to LCD `VO`

### Pinout (ESP32 ↔ Peripherals)

| Component | Pin / Signal | ESP32 GPIO | Notes |
| :--- | :--- | :--- | :--- |
| **LCD 16×2** | `RS` | `GPIO 13` | Register Select |
| **LCD 16×2** | `EN` | `GPIO 12` | Enable |
| **LCD 16×2** | `D4` | `GPIO 14` | 4-bit data bus |
| **LCD 16×2** | `D5` | `GPIO 27` | 4-bit data bus |
| **LCD 16×2** | `D6` | `GPIO 26` | 4-bit data bus |
| **LCD 16×2** | `D7` | `GPIO 25` | 4-bit data bus |
| **Servo Motor** | `PWM Signal` | `GPIO 32` | 50 Hz PWM, pulse width `500–2400 µs` |

> **Note:** Ensure the serial baud rate in SimTools (`BitsPerSec`) matches the `Serial.begin(115200)` rate configured in the firmware.

---

## Firmware (`firmware/1DOF_LCD_ESP32/1DOF_LCD_ESP32.ino`)

The firmware is written in C++ using the Arduino framework for ESP32:
* **`LiquidCrystal.h`**: Drives the 16×2 display in 4-bit mode.
* **`ESP32Servo.h`**: Allocates hardware PWM timers (`0` and `1`) to generate a 50 Hz servo control signal.
* **Serial Parser**: Buffers incoming ASCII characters until the `;` delimiter is received, converts the string to an integer (`0`–`1023`), maps it to the physical servo range (`20`–`173` degrees), updates the servo position, and refreshes the LCD readout.

### Dependencies
Install the following libraries via the **Arduino Library Manager** (or PlatformIO):
1. `LiquidCrystal`
2. `ESP32Servo`

### Flashing Instructions
1. Clone this repository:
   ```bash
   git clone [https://github.com/your-username/simtools-esp32-1dof-motion.git](https://github.com/your-username/simtools-esp32-1dof-motion.git)
   ```
2. Open `firmware/1DOF_LCD_ESP32/1DOF_LCD_ESP32.ino` in the **Arduino IDE**.
3. Select your ESP32 board profile (e.g., **ESP32 Dev Module**) and the corresponding COM port.
4. Click **Upload**.
5. Launch **SimTools**, verify the matching COM port and baud rate (`115200`), and test output using **Output Testing** or *Live for Speed*.

---

## References
* SimTools Motion Simulation Software: [https://simtools.us/](https://simtools.us/)
