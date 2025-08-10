P5_I2C1_BMP280
📌 Project Overview
This project demonstrates basic I2C communication between the STM32F401RE microcontroller and a BMP280 temperature & pressure sensor.
It uses STM32 HAL functions to:

Detect the sensor by reading its chip ID (WHOAMI).

Read raw temperature and pressure values over I2C.

Output data over UART for debugging.

🛠 Hardware Used
STM32F401RE Nucleo board

BMP280 sensor module (I2C address: 0x76 or 0x77)

USB cable for programming & UART output

Jumper wires for I2C connection

🔌 Wiring (I2C)
BMP280 Pin	STM32 Pin (Nucleo-F401RE)
VCC	3.3V
GND	GND
SCL	PB8 (I2C1_SCL)
SDA	PB9 (I2C1_SDA)

📜 How It Works
Initialize I2C and UART peripherals.

Send WHOAMI command to BMP280 and verify the returned chip ID (0x58).

Read raw temperature and pressure registers.

Print values over UART in the format:

ini
Copy
Edit
addr=0x76 id=0x58
raw_temp=...
raw_press=...
▶ Running the Project
Open the project in STM32CubeIDE.

Build and flash it to the Nucleo board.

Open a serial monitor (e.g., PuTTY, Tera Term) at 115200 baud.

Observe the chip ID and raw sensor values being printed.

📂 File Structure
Core/Src & Core/Inc – Main application code and headers.

BMP280.c / BMP280.h – Driver for BMP280 sensor.

.ioc file – CubeMX configuration for peripherals.