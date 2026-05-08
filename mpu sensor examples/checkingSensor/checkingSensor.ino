#include "Wire.h"

const int MPU_ADDR = 0x68;

int16_t accelerometer_x, accelerometer_y, accelerometer_z;
int16_t gyro_x, gyro_y, gyro_z;
int16_t temperature;

float angleZ = 0.0;
unsigned long prevTime = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Wake up MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  prevTime = millis();
}

void loop() {
  // Read sensor data
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  accelerometer_x = Wire.read()<<8 | Wire.read();
  accelerometer_y = Wire.read()<<8 | Wire.read();
  accelerometer_z = Wire.read()<<8 | Wire.read();
  temperature     = Wire.read()<<8 | Wire.read();
  gyro_x          = Wire.read()<<8 | Wire.read();
  gyro_y          = Wire.read()<<8 | Wire.read();
  gyro_z          = Wire.read()<<8 | Wire.read();

  // ===== Time calculation =====
  unsigned long currentTime = millis();
  float dt = (currentTime - prevTime) / 1000.0; // seconds
  prevTime = currentTime;

  // ===== Convert gyro Z to deg/s =====
  float gz_dps = gyro_z / 131.0;

  // ===== Integrate to get angle =====
  angleZ += gz_dps * dt;

  // ===== Print results =====
  Serial.print("Angle Z (Yaw): ");
  Serial.print(angleZ);

  Serial.print(" | gZ: ");
  Serial.print(gz_dps);

  Serial.println();

  delay(10); // fast sampling
}