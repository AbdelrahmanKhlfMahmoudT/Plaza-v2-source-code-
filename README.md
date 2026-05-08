Smart Robotic Arm & Mobile Robot

A multifunctional robotic system that combines:

Mobile robot movement control
Robotic arm manipulation
Obstacle avoidance
MPU6050 gyro-based turning
Bluetooth/Serial command control
Servo motor control using PCA9685

The project is written using Arduino/C++ and designed for embedded and mechatronics applications.

Features
Mobile Robot Control

The robot can:

Move forward
Move backward
Move left/right
Rotate in place
Stop instantly

Motor movement is controlled using H-Bridge motor driver outputs.

Robotic Arm Control

The robotic arm includes:

Shoulder joint
Elbow joint
Gripper control

Servos are controlled using the PCA9685 PWM driver.

The arm supports:

Manual movement
Smooth motion interpolation
Predefined positions:
Arm Up
Arm Down
Autonomous Obstacle Avoidance

In autonomous mode, the robot:

Reads ultrasonic sensors
Detects obstacles
Measures turning angle using MPU6050
Turns automatically to avoid collision

The robot chooses left or right direction depending on free space availability.

MPU6050 Gyroscope Integration

The MPU6050 is used to:

Measure robot yaw angle
Perform accurate turns
Improve obstacle avoidance navigation

Yaw angle is calculated by integrating gyroscope Z-axis readings.

Hardware Requirements
Main Components
Arduino UNO / Mega
PCA9685 Servo Driver
MPU6050 Gyroscope
HC-SR04 Ultrasonic Sensors
DC Motors
Motor Driver (L298N or similar)
Servo Motors
Bluetooth Module (HC-05 recommended)
External Power Supply
Pin Configuration
Motor Pins
Function	Pin
Left Motor IN1	2
Left Motor IN2	3
Right Motor IN1	4
Right Motor IN2	5
Ultrasonic Sensors
Sensor	Trigger	Echo
Front	8	9
Left	11	10
Right	6	7
Servo Channels (PCA9685)
Servo	Channel
Shoulder	0
Shoulder Pair	1
Elbow	4
Elbow Pair	5
Gripper	7
Software Libraries

Install the following Arduino libraries:

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Required libraries:

Wire
Adafruit PWM Servo Driver Library
Operating Modes
Manual Mode

Default mode.

Robot is controlled using Serial/Bluetooth commands.

Movement Commands
Command	Action
F	Forward
B	Backward
L	Left
R	Right
Q	Rotate Left
E	Rotate Right
S	Stop
Arm Control Commands
Command	Action
u	Shoulder Up
k	Shoulder Down
e	Elbow Up
c	Elbow Down
g	Open Gripper
h	Close Gripper
1	Arm Down Position
2	Arm Up Position
Autonomous Mode

Send:

D

The robot will:

Detect obstacles
Turn automatically
Navigate without manual control
Code Structure
Main Functions
Servo Control
updateArm()

Updates all servo positions.

MPU6050 Functions
mpu_init()
mpu_update()
mpu_getYaw()
mpu_resetYaw()

Used for gyroscope initialization and yaw tracking.

Distance Measurement
readDistance()

Reads ultrasonic sensor distance.

Movement Functions
moveForward()
moveBack()
moveLeft()
moveRight()
returnLeft()
returnRight()
stopRobot()

Control robot motion.

System Workflow
Manual Mode
Receive serial command
Execute movement or arm action
Update motors/servos
Autonomous Mode
Read ultrasonic sensors
Detect obstacle
Reset MPU yaw
Rotate approximately 83°
Continue movement
Important Notes
Servo angles are constrained between 0° and 220°
MPU yaw calculation may drift over time because only gyroscope integration is used
External power supply is recommended for motors and servos
moveBack() is currently used during autonomous navigation instead of forward motion, which may not match expected behavior and could be intentional for your robot orientation setup
Future Improvements

Possible upgrades:

Add PID control
Add ROS2 integration
Add camera vision system
Use sensor fusion for better MPU accuracy
Add WiFi/ESP32 remote control
Add path planning algorithms
Abdulrahman Khalaf 

Developed for embedded systems and mechatronics applications using Arduino and sensor integration techniques.
