#include <Wire.h>
#define MPU_ADDR 0x68
// ===== Internal variables =====
int16_t gyro_z;
float angleZ = 0.0;
unsigned long prevTime = 0;

// ===== INIT FUNCTION =====
void mpu_init() {
  Wire.begin();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // Wake up MPU6050
  Wire.write(0);
  Wire.endTransmission(true);

  prevTime = millis();
}

// ===== UPDATE FUNCTION =====
void mpu_update() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x47); // GYRO_ZOUT_H register
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 2, true);

  gyro_z = Wire.read() << 8 | Wire.read();

  // Time calculation
  unsigned long currentTime = millis();
  float dt = (currentTime - prevTime) / 1000.0;
  prevTime = currentTime;

  // Convert to deg/s
  float gz_dps = gyro_z / 131.0;

  // Integrate
  angleZ += gz_dps * dt;
}

// ===== GETTERS =====
float mpu_getYaw() {
  return angleZ;
}

float mpu_getGyroZ() {
  return gyro_z / 131.0;
}

// ===== OPTIONAL: RESET ANGLE =====
void mpu_resetYaw() {
  angleZ = 0;
}


void setup(){
  mpu_init();
  Serial.begin(9600);
}


void loop(){
  mpu_update();
  float angle = mpu_getYaw();
  Serial.println(angle);
  delay(10); // small delay only
  char data=Serial.read();
  if (data=='D'){
    mpu_resetYaw();
  }}