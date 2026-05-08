# Smart Robotic Arm & Mobile Robot

This project is a smart mobile robot equipped with a robotic arm that can be controlled manually through Bluetooth/Serial communication or operate autonomously using ultrasonic sensors and an MPU6050 gyroscope.

The system is designed for embedded systems and mechatronics applications, combining robot navigation, obstacle avoidance, servo motor control, and robotic arm manipulation in a single platform.

The robot can:
- Move in different directions
- Avoid obstacles automatically
- Control a multi-joint robotic arm
- Perform accurate turning using gyro data
- Execute predefined arm positions

---

# Features

- Mobile robot movement control
- Robotic arm manipulation
- Autonomous obstacle avoidance
- MPU6050 gyro-based turning
- Bluetooth/Serial communication
- Servo control using PCA9685

---

# Hardware Requirements

- Arduino UNO / Mega
- PCA9685 Servo Driver
- MPU6050 Gyroscope
- HC-SR04 Ultrasonic Sensors
- DC Motors + Motor Driver
- Servo Motors
- Bluetooth Module (HC-05)
- External Power Supply

---

# Pin Configuration

## Motor Pins

| Function | Pin |
|---|---|
| Left Motor IN1 | 2 |
| Left Motor IN2 | 3 |
| Right Motor IN1 | 4 |
| Right Motor IN2 | 5 |

## Ultrasonic Sensors

| Sensor | Trigger | Echo |
|---|---|---|
| Front | 8 | 9 |
| Left | 11 | 10 |
| Right | 6 | 7 |

---

# Software Libraries

```cpp
#include <Wire.h>                      // I2C communication library
#include <Adafruit_PWMServoDriver.h>  // PCA9685 servo driver library
