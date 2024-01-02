//Viral Science www.viralsciencecreativity.com www.youtube.com/c/viralscience
//Arduino NRF24L01 Transmitter - Joystick Controller Test
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>  //https://github.com/tmrh20/RF24/
#include "MPU6050.h"   
#include "I2Cdev.h"

// MPU control/status vars

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

#define joybtn1 3  // Joystick button 1
#define btn1 8     // Button 1
#define btn2 9     // Button 2
#define buzz 10    //Buzzer

RF24 radio(5, 6);                 // nRF24L01 (CE, CSN)
const byte address[6] = "00001";  // Address

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte joy1_X;
  byte joy1_Y;
  byte j1Button;
  byte button1;
  byte button2;
  byte pitch;
  byte roll;
  byte yaw;
};

Data_Package data;  //Create a variable with the above structure

void setup() {
  Serial.begin(9600);

  Serial.println("Initialize MPU6050");

  mpu.initialize();

  // radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);

  pinMode(joybtn1, INPUT_PULLUP);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(buzz, OUTPUT);
  analogWrite(buzz, 100);
  delay(100);
 analogWrite(buzz, 0);
 
  // initial default values
  data.joy1_X = 127;
  data.joy1_Y = 127;
  data.j1Button = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.pitch = 127;
  data.roll = 127;
  data.yaw = 127;
}


void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Read all analog inputs and map them to one Byte value
  data.joy1_X = map(analogRead(A2), 0, 1023, 0, 255);  // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.joy1_Y = map(analogRead(A3), 0, 1023, 0, 255);
  // Read all digital inputs
  data.j1Button = digitalRead(joybtn1);
  data.button1 = digitalRead(btn1);
  data.button2 = digitalRead(btn2);
  data.pitch = map(ay, -17000, 17000, 0, 254);
  data.roll = map(ax, -17000, 17000, 0, 254);
  data.yaw = map(az, -17000, 17000, 0, 254);
  radio.write(&data, sizeof(Data_Package));  // Send the whole data from the structure to the receiver
  if(data.j1Button== LOW){
     analogWrite(buzz, 50);
  delay(100);
 analogWrite(buzz, 0);
  }
}
