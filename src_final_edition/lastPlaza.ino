#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN 150
#define SERVOMAX 600
#define MPU_ADDR 0x68
int sPos = 120, ePos = 100, gPos = 110;
int stepSize = 2;

int16_t gyro_z;
float angleZ = 0.0;
unsigned long prevTime = 0;
float angle;
//===== Update Arm =====
int angleToPulse(int ang) {
  return map(ang, 0, 210, SERVOMIN, SERVOMAX);
}
void updateArm() {
  pwm.setPWM(0, 0, angleToPulse(sPos));
  pwm.setPWM(1, 0, angleToPulse(220 - sPos));
  pwm.setPWM(4, 0, angleToPulse(ePos));
  pwm.setPWM(5, 0, angleToPulse(220-ePos));
  // pwm.setPWM(6, 0, angleToPulse(wPos));
  pwm.setPWM(7, 0, angleToPulse(gPos));
}
float front,left,right;
// =====Motors=====
int in1L =2 , in2L = 3;
int in1R =4, in2R =5;
// int pwmL=10,pwmR=11;
// int pwmSpeed = 255;

// =====UltraSonic=====
int trigF = 8, echoF = 9;
int trigL = 11, echoL = 10;
int trigR = 6, echoR = 7;

// =====Variables=====
int mode = 0;
char data = 'S';

// ====== STATES ======
enum State { IDLE, TURN_RIGHT, TURN_LEFT };
State currentState = IDLE;

// ====== TIME CONTROL ======
unsigned long previousTime = 0;
int actionDuration = 0;
//------mpu-------
void mpu_init() {
  Wire.begin();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // Wake up MPU6050
  Wire.write(0);
  Wire.endTransmission(true);

  prevTime = millis();
}
// تحريك أي محور إلى زاوية معينة بشكل تدريجي
void moveTo(int &pos, int target) {
  while (pos < target) {
    pos += 2;
    updateArm();
    delay(10);
  }
  while (pos > target) {
    pos -= 2;
    updateArm();
    delay(10);
  }
}

// وضع النزول: S = 10 ، E = 100
void armIsdown() {
  moveTo(sPos, 10);
  moveTo(ePos, 40);
}

// وضع الطلوع: S = 120 ، E = 120
void armIsup() {
  moveTo(sPos, 140);
  moveTo(ePos, 260);
}

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
float mpu_getYaw() {
  return angleZ;
}
void mpu_resetYaw() {
  angleZ = 0;
}

// =====Functions=====
float readDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 20000);
  float distance = duration * 0.034 / 2;
  return distance;
}

// ===== Movement Functions =====
void moveLeft() {
  digitalWrite(in1L, HIGH); digitalWrite(in2L, LOW);
  digitalWrite(in1R, 0); digitalWrite(in2R, 0);
}
void moveRight() {
  digitalWrite(in1L, 0); digitalWrite(in2L, 0);
  digitalWrite(in1R, 1); digitalWrite(in2R, 0);
}

void moveForward() {
  digitalWrite(in1L, HIGH); digitalWrite(in2L, LOW);
  digitalWrite(in1R, HIGH); digitalWrite(in2R, LOW);
  // analogWrite(pwmL, pwmSpeed); 
  // analogWrite(pwmR, pwmSpeed);
}
void moveBack() {
  digitalWrite(in1L, LOW); digitalWrite(in2L, HIGH);
  digitalWrite(in1R, LOW); digitalWrite(in2R, HIGH);
  // analogWrite(pwmL, pwmSpeed); 
  // analogWrite(pwmR, pwmSpeed);
}


void returnLeft() {
  digitalWrite(in1L, LOW); digitalWrite(in2L, HIGH);
  digitalWrite(in1R, HIGH); digitalWrite(in2R, LOW);
  // analogWrite(pwmL, pwmSpeed); 
  // analogWrite(pwmR, pwmSpeed);
}
void returnRight() {
  digitalWrite(in1L, HIGH); digitalWrite(in2L, LOW);
  digitalWrite(in1R, LOW); digitalWrite(in2R, HIGH);
  // analogWrite(pwmL, pwmSpeed); 
  // analogWrite(pwmR, pwmSpeed);
}
void stopRobot() {
  digitalWrite(in1L,0);
  digitalWrite(in2L,0); 
  digitalWrite(in1R,0); 
  digitalWrite(in2R,0); 
}

// ===== Setup =====
void setup() {
  mpu_init();
  Serial.begin(9600);
  // pinMode(pwmL, OUTPUT); 
  pinMode(in1L, OUTPUT); 
  pinMode(in2L, OUTPUT);
  // pinMode(pwmR, OUTPUT); 
  pinMode(in1R, OUTPUT); 
  pinMode(in2R, OUTPUT);
  pinMode(trigF, OUTPUT); pinMode(echoF, INPUT);
  pinMode(trigL, OUTPUT); pinMode(echoL, INPUT);
  pinMode(trigR, OUTPUT); pinMode(echoR, INPUT);

  pwm.begin();
  pwm.setPWMFreq(60);
  updateArm();
}

// ===== Main Loop =====
void loop() {

  // Read from Bluetooth first
  // if (BT.available()) {
  //   data = BT.read();
  // }
  // If no Bluetooth, read from Serial
  if (Serial.available()) {
    data = Serial.read();
  }
  if(data=='D'){
    mode=1;
  }

  else{
    mode=0;
  }

  
if (mode == 1) {

  mpu_update();   // keep it running

  float front = readDistance(trigF, echoF);
  delay(20);
  float left  = readDistance(trigL, echoL);
  delay(20);

  float threshold = 18.0;

  if (front < threshold) {

    stopRobot();
    delay(150);

    mpu_resetYaw();

    if (readDistance(trigL, echoL) < threshold) {
      // ===== TURN RIGHT =====
      while (true) {
        mpu_update();
        angle = mpu_getYaw();
        if (abs(angle) >=83) break;
        returnLeft();
      }
    stopRobot();
    delay(300);
    }
    else {
      // ===== TURN LEFT =====
      while (true) {
        mpu_update();
        angle = mpu_getYaw();

        if (abs(angle) >= 83) break;
        returnRight();
      }
    mpu_resetYaw();
    stopRobot();
    delay(300);
    }
    stopRobot();
    mpu_resetYaw();
  }
  else {
    moveBack();
  }
}

  else if(mode==0){
  // Execute command
  if (data == 'F') moveForward();
  else if (data == 'B') moveBack();
  else if (data == 'L') moveLeft();
  else if (data == 'R') moveRight();
  else if (data == 'Q') returnLeft();
  else if (data == 'E') returnRight();
  else if (data == 'S') stopRobot();
  else if (data == 'u') sPos += stepSize;
  else if (data == 'k') sPos -= stepSize;
  // Elbow
  else if (data == 'e') ePos += stepSize;
  else if (data == 'c') ePos -= stepSize;
  // Wrist
  // else if (data == 'w') wPos += stepSize;
  // else if (data == 'x') wPos -= stepSize;
  // // Gripper
  else if (data == 'g') gPos += stepSize;
  else if (data == 'h') gPos -= stepSize;
  else if (data =='1')armIsdown();
  else if (data =='2')armIsup();
  // ===== Limits =====
  sPos = constrain(sPos, 0, 220);
  ePos = constrain(ePos, 0, 220);
  // wPos = constrain(wPos, 0, 220);
  gPos = constrain(gPos, 0,220);
  updateArm();
  delay(20);
  }

}