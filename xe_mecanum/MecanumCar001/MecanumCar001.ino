//Viral Science www.viralsciencecreativity.com www.youtube.com/c/viralscience
//Arduino Bluetooth App Control Mecanum Wheel Robot

#include <AFMotor.h>  //Download and Install AFMotor.h Library

AF_DCMotor motor1(1);  //Front Left Wheel
AF_DCMotor motor2(2);  //Back Left Wheel
AF_DCMotor motor3(3);  //Front Right Wheel
AF_DCMotor motor4(4);  //Back Right Wheel

#define trig1 A0
#define echo1 A1
#define trig2 A2
#define echo2 A3
#define trig3 A4
#define echo3 A5
long leftDistance = 0, middleDistance = 0, rightDistance = 0;
long duration, distance;
int DIS = 20;

String readString;

void setup() {
  Serial.begin(9600);
  motor1.setSpeed(250);  //Set Motor Speed
  motor2.setSpeed(250);
  motor3.setSpeed(250);
  motor4.setSpeed(250);
  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(echo3, INPUT);
}

void loop() {
}
////////////////////////////////////

////////////////////////////////////
void manualCar_BLE() {
  while (Serial.available()) {
    delay(50);
    char c = Serial.read();
    readString += c;
  }
  if (readString.length() > 0) {
    Serial.println(readString);
    if (readString == "FORWARD") {  // MOVE FORWARD
      FORWARD();
    }
    if (readString == "BACKWARD") {  // MOVE BACKWARD
      BACKWARD();
    }
    if (readString == "LEFT") {  // MOVE LEFT SIDE
      LEFT();
    }
    if (readString == "RIGHT") {  // MOVE RIGHT SIDE
      RIGHT();
    }
    if (readString == "FORWARDLEFT") {  // MOVE FORWARD LEFT
      FORWARDLEFT();
    }
    if (readString == "FORWARDRIGHT") {  // MOVE FORWARD RIGHT
      FORWARDRIGHT();
    }
    if (readString == "BACKWARDLEFT") {  // MOVE BACKWARD LEFT
      BACKWARDLEFT();
    }
    if (readString == "BACKWARDRIGHT") {  // MOVE BACKWARD RIGHT
      BACKWARDRIGHT();
    }
    if (readString == "ROTATELEFT") {  // ROTATE LEFT SIDE
      ROTATELEFT();
    }
    if (readString == "ROTATERIGHT") {  // ROTATE RIGHT SIDE
      ROTATERIGHT();
    }
    if (readString == "STOP") {  // STOP
      STOP();
    }

    readString = "";
  }
}
////////////////////////////////////
void manualCar_WIFI() {
  // cần thay đổi

  if (len == 1) {
    FORWARD();
  } else if (xuong == 1) {
    BACKWARD();
  } else if (trai == 1) {
    LEFT();
    // moveStop();
  } else if (phai == 1) {
    RIGHT();
    //moveStop();
  } else if (lentrai == 1) {
    FORWARDLEFT();
  } else if (lenphai == 1) {
    FORWARDRIGHT();
  } else if (xuongtrai == 1) {
    BACKWARDLEFT();
  } else if (xuongphai == 1) {
    BACKWARDRIGHT();
  } else if (xoaytrai == 1) {
    ROTATELEFT();
  } else if (xoayphai == 1) {
    ROTATERIGHT();
  } else {
    moveStop();
  }
}

////////////////////////////////////
void autoCar() {
  leftDistance = leftMeasurement();
  delay(10);
  middleDistance = middleMeasurement();
  delay(10);
  rightDistance = rightMeasurement();
  delay(10);


  //  Obstacle on left side
  if (leftDistance < DIS && middleDistance > DIS && rightDistance > DIS) {
    ROTATERIGHT();
    delay(25);  //  turn lightly right
  }
  //  Obstacle on front
  else if (leftDistance > DIS && middleDistance < DIS && rightDistance > DIS) {
    if (leftDistance > rightDistance) {
      ROTATELEFT();
      delay(400);  //  turn 90 degree left
    }
    if (leftDistance < rightDistance) {
      ROTATERIGHT();
      delay(400);  //  turn 90 degree right
    }
  }
  //  Obstacle on right side
  else if (leftDistance > DIS && middleDistance > DIS && rightDistance < DIS) {
    ROTATELEFT();
    delay(25);  //turn lightly left
  }
  //  Obstacle on front & left side
  else if (leftDistance < DIS && middleDistance < DIS && rightDistance > DIS) {
    ROTATERIGHT();
    delay(400);  //  turnRight 90 degree
  }
  //  Obstacle on front & right side
  else if (leftDistance > DIS && middleDistance < DIS && rightDistance < DIS) {
    ROTATELEFT();
    delay(400);  //  turnLeft 90 degree
  }
  //  Obstacle on left & right side
  else if (leftDistance < DIS && middleDistance > DIS && rightDistance < DIS) {
    if (leftDistance > rightDistance) {
      ROTATELEFT();
      delay(50);  //  shift left
    }
    if (leftDistance < rightDistance) {
      ROTATERIGHT();
      delay(50);  //  shift right
    }
  }
  //  Obstacle on all 3 side
  else if (leftDistance < DIS && middleDistance < DIS && rightDistance < DIS) {
    STOP();
    delay(1000);
    BACKWARD();
    delay(1000);
    ROTATELEFT();
    delay(800);  //turn 180 degrees
  } else {
    FORWARD();
  }
}
////////////////////////////////////

void FORWARD() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
void BACKWARD() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}
void LEFT() {
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}
void RIGHT() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}
void FORWARDLEFT() {
  motor1.run(RELEASE);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(RELEASE);
}
void FORWARDRIGHT() {
  motor1.run(FORWARD);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(FORWARD);
}
void BACKWARDLEFT() {
  motor1.run(BACKWARD);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(BACKWARD);
}
void BACKWARDRIGHT() {
  motor1.run(RELEASE);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(RELEASE);
}
void ROTATELEFT() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
void ROTATERIGHT() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}
void STOP() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
long rightMeasurement() {
  digitalWrite(trig3, LOW);
  delayMicroseconds(2);
  digitalWrite(trig3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig3, LOW);
  duration = pulseIn(echo3, HIGH);
  distance = (duration / 2) / 29.1;
  return (distance);
}
long leftMeasurement() {
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  duration = pulseIn(echo1, HIGH);
  distance = (duration / 2) / 29.1;
  return (distance);
}

long middleMeasurement() {
  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  duration = pulseIn(echo2, HIGH);
  distance = (duration / 2) / 29.1;
  return (distance);
}