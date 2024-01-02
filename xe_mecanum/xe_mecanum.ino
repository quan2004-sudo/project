#include<AFMotor.h>

AF_DCMotor motor1(1); 
AF_DCMotor motor2(2); 
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
 
 #define trig1 A0
#define echo1 A1
#define trig2 A2
#define echo2 A3
#define trig3 A4
#define echo3 A5
long leftDistance = 0, middleDistance = 0, rightDistance = 0;
int LeftSpeed = 125;
int RightSpeed = 125;
long duration, distance;
int DIS = 20;

char command; 
 
void setup(){      
  Serial.begin(9600);  
   pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(echo3, INPUT);
}
void loop() {
  Serial.println(command);
  if(Serial.available() > 0){ 
    command = Serial.read(); 
    Stop();

    switch(command){
    case 'F':  
      forward();
      break;
    case 'B':  
       back();
      break;
    case 'L':  
      left();
      break;
    case 'R':
      right();
      break;
    case 'G':
      forward_leftturn();
      break;
    case 'I':
      forward_rightturn();
      break;
    case 'H':
      backward_leftturn();
      break;
    case 'J':
      backward_rightturn();
      break;
    case 'q':
      rotate();
      break;
    }
  } 
}
void autoCar() {
  leftDistance = leftMeasurement();
  delay(10);
  middleDistance = middleMeasurement();
  delay(10);
  rightDistance = rightMeasurement();
  delay(10);
  // Serial.print("leftDistance = ");
  // Serial.print(leftDistance);
  // Serial.print("cm /");
  // Serial.print("middleDistance = ");
  // Serial.print(middleDistance);
  // Serial.print("cm /");
  // Serial.print("rightDistance = ");
  // Serial.print(rightDistance);
  // Serial.println("cm");

  //  Obstacle on left side
  if (leftDistance < DIS && middleDistance > DIS && rightDistance > DIS) {
    turnRight(125);
    delay(25);  //  turn lightly right
  }
  //  Obstacle on front
  else if (leftDistance > DIS && middleDistance < DIS && rightDistance > DIS) {
    if (leftDistance > rightDistance) {
      turnLeft(125);
      delay(400);  //  turn 90 degree left
    }
    if (leftDistance < rightDistance) {
      turnRight(125);
      delay(400);  //  turn 90 degree right
    }
  }
  //  Obstacle on right side
  else if (leftDistance > DIS && middleDistance > DIS && rightDistance < DIS) {
    turnLeft(125);
    delay(25);  //turn lightly left
  }
  //  Obstacle on front & left side
  else if (leftDistance < DIS && middleDistance < DIS && rightDistance > DIS) {
    turnRight(125);
    delay(400);  //  turnRight 90 degree
  }
  //  Obstacle on front & right side
  else if (leftDistance > DIS && middleDistance < DIS && rightDistance < DIS) {
    turnLeft(125);
    delay(400);  //  turnLeft 90 degree
  }
  //  Obstacle on left & right side
  else if (leftDistance < DIS && middleDistance > DIS && rightDistance < DIS) {
    if (leftDistance > rightDistance) {
      turnLeft(125);
      delay(50);  //  shift left
    }
    if (leftDistance < rightDistance) {
      turnRight(125);
      delay(50);  //  shift right
    }
  }
  //  Obstacle on all 3 side
  else if (leftDistance < DIS && middleDistance < DIS && rightDistance < DIS) {
    moveStop();
    delay(1000);
    moveBackward(125);
    delay(1000);
    turnLeft(125);
    delay(800);  //turn 180 degrees
  } else {
    moveForward(125);
  }
}

void right_turn()
{
  motor1.setSpeed(225); 
  motor1.run(FORWARD); 
  motor2.setSpeed(225); 
  motor2.run(FORWARD); 
  motor3.setSpeed(225); 
  motor3.run(BACKWARD); 
  motor4.setSpeed(225); 
  motor4.run(BACKWARD); 
} 
void left_turn()
{
  motor1.setSpeed(225); 
  motor1.run(BACKWARD); 
  motor2.setSpeed(225); 
  motor2.run(BACKWARD); 
  motor3.setSpeed(225); 
  motor3.run(FORWARD); 
  motor4.setSpeed(225); 
  motor4.run(FORWARD); 
} 


void forward()
{
  motor1.setSpeed(255); 
  motor1.run(FORWARD); 
  motor2.setSpeed(255); 
  motor2.run(FORWARD); 
  motor3.setSpeed(255);
  motor3.run(FORWARD); 
  motor4.setSpeed(255);
  motor4.run(FORWARD); 
}
 
void back()
{
  motor1.setSpeed(255); 
  motor1.run(BACKWARD); 
  motor2.setSpeed(255); 
  motor2.run(BACKWARD); 
  motor3.setSpeed(255); 
  motor3.run(BACKWARD); 
  motor4.setSpeed(255); 
  motor4.run(BACKWARD); 
}
 
void left()
{
  motor1.setSpeed(225); 
  motor1.run(BACKWARD); 
  motor2.setSpeed(225); 
  motor2.run(FORWARD); 
  motor3.setSpeed(225);
  motor3.run(FORWARD);  
  motor4.setSpeed(125); 
  motor4.run(BACKWARD); 
}
 
void right()
{
  motor1.setSpeed(225); 
  motor1.run(FORWARD); 
  motor2.setSpeed(225); 
  motor2.run(BACKWARD); 
  motor3.setSpeed(225); 
  motor3.run(BACKWARD); 
  motor4.setSpeed(225); 
  motor4.run(FORWARD); 
} 

void forward_leftturn()
{
  motor1.setSpeed(0); 
  motor1.run(RELEASE); 
  motor2.setSpeed(175); 
  motor2.run(FORWARD); 
  motor3.setSpeed(175); 
  motor3.run(FORWARD); 
  motor4.setSpeed(0); 
  motor4.run(RELEASE); 
}

void forward_rightturn()
{
  motor1.setSpeed(175); 
  motor1.run(FORWARD); 
  motor2.setSpeed(0); 
  motor2.run(RELEASE); 
  motor3.setSpeed(0); 
  motor3.run(RELEASE); 
  motor4.setSpeed(175); 
  motor4.run(FORWARD); 
}

void backward_leftturn()
{
  motor1.setSpeed(175); 
  motor1.run(BACKWARD); 
  motor2.setSpeed(0); 
  motor2.run(RELEASE); 
  motor3.setSpeed(0); 
  motor3.run(RELEASE); 
  motor4.setSpeed(175); 
  motor4.run(BACKWARD); 
}

void backward_rightturn()
{
  motor1.setSpeed(0); 
  motor1.run(RELEASE); 
  motor2.setSpeed(175); 
  motor2.run(BACKWARD); 
  motor3.setSpeed(175); 
  motor3.run(BACKWARD); 
  motor4.setSpeed(0); 
  motor4.run(RELEASE); 
}

void rotate()
{
  motor1.setSpeed(175); 
  motor1.run(FORWARD); 
  motor2.setSpeed(175); 
  motor2.run(BACKWARD); 
  motor3.setSpeed(175); 
  motor3.run(FORWARD); 
  motor4.setSpeed(175); 
  motor4.run(BACKWARD); 
}


void Stop()
{
  motor1.setSpeed(0); 
  motor1.run(RELEASE); 
  motor2.setSpeed(0); 
  motor2.run(RELEASE); 
  motor3.setSpeed(0); 
  motor3.run(RELEASE); 
  motor4.setSpeed(0); 
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