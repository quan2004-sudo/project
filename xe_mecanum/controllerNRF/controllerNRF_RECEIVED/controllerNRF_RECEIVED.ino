//Viral Science www.viralsciencecreativity.com www.youtube.com/c/viralscience
//Arduino NRF24L01 Receiver - Joystick Controller Test
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(38, 39);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

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

Data_Package data; //Create a variable with the above structure

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();
}
void loop() {
  // Check whether there is data to be received
  if (radio.available()>4) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  Serial.println("co tin hieu");
  }
  else{
    Serial.println("mat tin hieu");
  }
  // Check whether we keep receving data
  // currentTime = millis();
  // if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
  //   resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  // }
  // Print the data in the Serial Monitor
  // Serial.print("joy1_X: ");
  // Serial.print(data.joy1_X);
  // Serial.print("; joy1_Y: ");
  // Serial.print(data.joy1_Y);
  // Serial.print("; button1: ");
  // Serial.print(data.button1);
  // Serial.print("; button2: ");
  // Serial.print(data.button2);
  // Serial.print("; j1Button: ");
  // Serial.print(data.j1Button);
  // Serial.print("; Pitch: ");
  // Serial.print(data.pitch);
  // Serial.print("; Roll: ");
  // Serial.print(data.roll);
  // Serial.print("; Yaw: ");
  // Serial.println(data.yaw);
}

void resetData() {
  // Set initial default values
  data.joy1_X = 127;
  data.joy1_Y = 127;
  data.j1Button = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.pitch = 127;
  data.roll = 127;
  data.yaw = 127;
}
