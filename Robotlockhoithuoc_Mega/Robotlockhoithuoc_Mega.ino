#include <Wire.h>
#include "SoftwareSerial.h"
#include "SharpGP2Y10.h"
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 13
#define MQ135Pin A6
#define MP2Pin A7
#define MQ2Pin A8
#define voPin A9
#define ledPin 11
#define CoiPin 12

#define btn1 8
#define btn2 9
#define btn3 10
#define NGUONG_GAS 500
#define NGUONG_KHOI 500

String data[9];
#define trig1 A0
#define echo1 A1
#define trig2 A2
#define echo2 A3
#define trig3 A4
#define echo3 A5
#define in1 6
#define in2 5
#define in3 4
#define in4 3
#define ENA 7
#define ENB 2
bool check;
int count = 0;
DHT dht(DHTPIN, DHTTYPE);
int LeftSpeed = 125;
int RightSpeed = 125;
long leftDistance = 0, middleDistance = 0, rightDistance = 0;
long duration, distance;

int DIS = 20;
int modeCar = 1;
bool StateRelay1 = false;
bool StateRelay2 = true;
const int relayPins[] = { 15, 16 };
int ModeCar_state, ModeSys_state, PhunSuong_state, QuatHut_state, speed;
int ModeCar__state, ModeSys__state, PS__state, QH__state;
int btn1State = LOW;
int btn2State = LOW;
int btn3State = LOW;
int lastBtn1State = LOW;
int lastBtn2State = LOW;
int lastBtn3State = LOW;
unsigned long btn1PressTime = 0;
unsigned long btn2PressTime = 0;
unsigned long btn3PressTime = 0;
unsigned long lastSendData = 0;
bool manualMode = false;                       // thủ công
const unsigned long longPressDuration = 1000;  // 3 giây
const unsigned long longPressDuration1 = 100;

SharpGP2Y10 dustSensor(voPin, ledPin);
//SoftwareSerial Serial1(6, 7);
LiquidCrystal_I2C lcd(0x27, 16, 2);
float dustDensity = 0;
float t, h;
int giatri_mtr, giatri_khoi, giatri_gas, displayCount = 0;
unsigned long btn4PressTime = 0, btn5PressTime = 0, btn6PressTime = 0, btn7PressTime = 0;
bool lastBtn4State = LOW, lastBtn5State = LOW, lastBtn6State = LOW, lastBtn7State = LOW;
int btn4State, btn5State, btn6State, btn7State, len = 0, xuong = 0, trai = 0, phai = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(echo3, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  moveStop();
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(CoiPin, OUTPUT);
  for (int i = 0; i < 2; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[0], LOW);

    digitalWrite(relayPins[1], HIGH);
  }
  // Serial.println("MQ3 warming up!");
}

void loop() {
  //digitalWrite(relayPins[0], HIGH);
  // put your main code here, to run repeatedly:
  check = count % 2;
  btn1State = digitalRead(btn1);  // nhan tren 3s ==> doi mode // nhan duoi 3s doi che do xem man hinh
  btn2State = digitalRead(btn2);
  btn3State = digitalRead(btn3);

  if (btn1State != lastBtn1State) {
    if (btn1State == LOW) {
      btn1PressTime = millis();
    } else {
      if (millis() - btn1PressTime < longPressDuration) {
        // Xử lý btn 1 nhấn ngắn (< 3 giây)
        manualMode = !manualMode;  // dao Mode
      } else {
        // Xử lý btn 1 nhấn lâu (>= 3 giây)
        // Thêm chức năng tại đây
        /// CHỖ NÀY ĐỂ ĐẢO MODE TỰ ĐỘNG/THỦ CÔNG CỦA ĐẾ XE
        modeCar = !modeCar;
      }
    }
  }

  if (manualMode) {
    // Serial.print("\tmanual");
    read_sensor();
    display();
    if (btn2State != lastBtn2State) {
      if (btn2State == LOW) {
        btn2PressTime = millis();
      } else {
        if (millis() - btn2PressTime < longPressDuration) {
          // Xử lý btn 1 nhấn ngắn (< 3 giây)
          displayCount--;
          if (displayCount == 0) {
            displayCount = 3;
          }
        } else {
          // Xử lý btn 1 nhấn lâu (>= 3 giây)
          // Thêm chức năng tại đây

          digitalWrite(relayPins[0], HIGH);
          delay(200);
          digitalWrite(relayPins[0], LOW);
          delay(200);
          count++;
        }
      }
    }


    if (btn3State != lastBtn3State) {
      if (btn3State == LOW) {
        btn3PressTime = millis();
      } else {
        if (millis() - btn3PressTime < longPressDuration) {
          // Xử lý btn 1 nhấn ngắn (< 3 giây)
          displayCount++;
          if (displayCount == 4) {
            displayCount = 1;
          }
        } else {
          // Xử lý btn 1 nhấn lâu (>= 3 giây)
          // Thêm chức năng tại đây
          StateRelay2 = !StateRelay2;
        }
      }
    }


    digitalWrite(relayPins[1], StateRelay2);



  }

  else {  // che do tu dong
    // Serial.print("\t auto");
    read_sensor();
    if (btn2State != lastBtn2State) {
      if (btn2State == LOW) {
        btn2PressTime = millis();
      } else {
        if (millis() - btn2PressTime < longPressDuration) {
          // Xử lý btn 1 nhấn ngắn (< 3 giây)
          displayCount--;
          if (displayCount == 0) {
            displayCount = 3;
          }
        }
      }
    }
    if (btn3State != lastBtn3State) {
      if (btn3State == LOW) {
        btn3PressTime = millis();
      } else {
        if (millis() - btn3PressTime < longPressDuration) {
          // Xử lý btn 1 nhấn ngắn (< 3 giây)
          displayCount++;
          if (displayCount == 4) {
            displayCount = 1;
          }
        }
      }
    }

    display();
    if (t > 30 && h > 50 && count % 2 == 0) {
      digitalWrite(relayPins[0], HIGH);
      delay(200);
      digitalWrite(relayPins[0], LOW);
      delay(200);
      count++;
    } else if (count % 2 == 1 && t < 30 && h < 50) {
      digitalWrite(relayPins[0], LOW);
      delay(200);
      digitalWrite(relayPins[0], HIGH);
      delay(200);
      count++;
    }


    if (giatri_gas > 600 || giatri_khoi > 600 || giatri_mtr > 600 || dustDensity > 50) {
      digitalWrite(CoiPin, HIGH);
      digitalWrite(relayPins[1], LOW);
    } else {
      digitalWrite(CoiPin, LOW);
      digitalWrite(relayPins[1], HIGH);
    }
  }



  if (modeCar == 0) {
    manualCar();

  } else {
    autoCar();
  }
  Control_Received();

  // Serial.print("\t displayCount:");
  // Serial.print(displayCount);
  // Serial.print("\tStateRelay1:");
  // Serial.print(StateRelay1);
  // Serial.print("\tStateRelay2:");
  // Serial.println(StateRelay2);

  if (millis() - lastSendData > 2000) {
    lastSendData = millis();
    sendUart();
    //   Serial.print("update done !!!");
  }

  lastBtn1State = btn1State;
  lastBtn2State = btn2State;
  lastBtn3State = btn3State;
}
// /gas, nhiệt, độ ẩm, bụi, mq135, khói

void read_sensor() {
  giatri_gas = analogRead(MQ2Pin);
  giatri_mtr = analogRead(MQ135Pin);
  giatri_khoi = analogRead(MP2Pin);  // read analog input pin 0
  dustDensity = random(1, 10);       //dustSensor.getDustDensity();
  t = dht.readTemperature();
  h = dht.readHumidity();
  // Serial.print("giatri_gas:");
  // Serial.print(giatri_gas);
  // Serial.print("\tgiatri_mtr:");
  // Serial.print(giatri_mtr);
  // Serial.print("\tgiatri_khoi:");
  // Serial.print(giatri_khoi);
  // Serial.print("\tt:");
  // Serial.print(t);
  // Serial.print("\th:");
  // Serial.print(h);
}
void display() {


  if (displayCount == 1) {
    lcd.setCursor(0, 0);
    lcd.print("PHUN SUONG:");
    lcd.setCursor(11, 0);
    if (count % 2 == 1) {
      lcd.print("ON");
    } else {
      lcd.print("OFF");
    }
    lcd.setCursor(0, 1);
    lcd.print("QUAT LOC KHI:");
    lcd.setCursor(13, 1);
    if (digitalRead(16) == false) {
      lcd.print("ON");
    } else {
      lcd.print("OFF");
    }


  } else if (displayCount == 2) {
    lcd.setCursor(0, 0);
    lcd.print("MODE: ");
    if (manualMode == false) {
      lcd.print(" Auto");
    } else {
      lcd.print(" Manual");
    }

    lcd.setCursor(0, 1);
    lcd.print("HUM:");
    lcd.print(round(h));
    lcd.setCursor(8, 1);
    lcd.print("TEMP:");
    lcd.print(round(t));


  } else {

    lcd.setCursor(0, 0);
    lcd.print("BUI:");
    lcd.print(round(dustDensity));
    lcd.setCursor(7, 0);
    lcd.print(" KHOI:");
    lcd.print(giatri_khoi);
    lcd.setCursor(0, 1);
    lcd.print("GAS:");
    lcd.print(giatri_gas);
    lcd.setCursor(9, 1);
    lcd.print("KK:");
    lcd.print(giatri_mtr);
  }

  delay(500);
  lcd.clear();
}
void moveForward(int Speed) {
  Serial.println("Move Forward");
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void moveBackward(int Speed) {
  Serial.println("Move Backward");
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnRight(int Speed) {
  Serial.println("Turn Right");
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft(int Speed) {
  Serial.println("Turn Left");
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void moveStop() {
  Serial.println("Move Stop");
  analogWrite(ENA, LOW);
  analogWrite(ENB, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
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
void manualCar() {
  // cần thay đổi

  if (len == 1) {
    moveForward(speed);
  } else if (xuong == 1) {
    moveBackward(speed);
  } else if (trai == 1) {
    turnLeft(speed);
    // moveStop();
  } else if (phai == 1) {
    turnRight(speed);
    //moveStop();
  } else {
    moveStop();
  }
}



void Control_Received() {

  if (Serial1.available()) {                              // Kiểm tra xem có dữ liệu đang được gửi đến không
    String receivedData = Serial1.readStringUntil('\n');  // Đọc chuỗi dữ liệu cho đến khi gặp ký tự '\n'

    // Tách dữ liệu bằng dấu phẩy

    int startIndex = 0;
    int commaIndex;

    for (int i = 0; i < 9; i++) {
      commaIndex = receivedData.indexOf(',', startIndex);
      if (commaIndex != -1) {
        data[i] = receivedData.substring(startIndex, commaIndex);
        startIndex = commaIndex + 1;
      } else {
        data[i] = receivedData.substring(startIndex);
        break;
      }
    }

    // Xử lý dữ liệu nhận được
    // for (int i = 0; i < 9; i++) {
    //   Serial.print("Data " + String(i + 1) + ": ");
    //   Serial.print(data[i]);
    //   Serial.print("\t");
    // }
    Serial.println();
    ModeCar_state = data[0].toInt();
    ModeSys_state = data[1].toInt();
    PhunSuong_state = data[2].toInt();
    QuatHut_state = data[3].toInt();
    len = data[4].toInt();
    xuong = data[5].toInt();
    trai = data[6].toInt();
    phai = data[7].toInt();
    speed = data[8].toInt();
    // if (ModeCar_state) {
    //   modeCar++;
    //   if (modeCar == 4) {
    //     modeCar = 1;
    //   }
    // }
    // if (ModeSys_state) {
    //   manualMode = !manualMode;  // dao Mode
    // }
    // if (manualMode) {
    //   if (PhunSuong_state) {
    //     StateRelay1 = !StateRelay1;
    //   }
    //   if (QuatHut_state) {
    //     StateRelay2 = !StateRelay2;
    //   }
    // }
    btn4State = ModeCar_state;
    btn5State = ModeSys_state;
    btn6State = PhunSuong_state;
    btn7State = QuatHut_state;

    if (btn4State != lastBtn4State) {
      if (btn4State == HIGH) {
        btn4PressTime = millis();
      } else {
        if (millis() - btn4PressTime > longPressDuration1) {
          // Serial.println("444444444444");
          // ModeCar__state = 1;
          modeCar = !modeCar;
        }
      }
    }

    if (btn5State != lastBtn5State) {
      if (btn5State == HIGH) {
        btn5PressTime = millis();
      } else {
        if (millis() - btn5PressTime > longPressDuration1) {
          // Serial.println("444444444444");
          // ModeSys__state = 1;
          manualMode = !manualMode;
        }
      }
    }

    if (btn6State != lastBtn6State) {
      if (btn6State == HIGH) {
        btn6PressTime = millis();
      } else {
        if (millis() - btn6PressTime > longPressDuration1) {
          // Serial.println("444444444444");
          // PS__state = 1;
          if (manualMode)
            digitalWrite(relayPins[0], HIGH);
          delay(200);
          digitalWrite(relayPins[0], LOW);
          delay(200);
          count++;
        }
      }
    }
    if (btn7State != lastBtn7State) {
      if (btn7State == HIGH) {
        btn7PressTime = millis();
      } else {
        if (millis() - btn7PressTime > longPressDuration1) {
          // Serial.println("444444444444");
          // QH__state = 1;
          StateRelay2 = !StateRelay2;
        }
      }
    }


    // if (ModeCar__state) {
    //   modeCar=!modeCar;
    // }
    // if (ModeSys__state){
    //  manualMode = !manualMode;
    // }
    // if(PS__state) {
    //  StateRelay1 = !StateRelay1;
    // }
    // if(QH__state) {
    //    StateRelay2 = !StateRelay2;
    // }


    lastBtn4State = btn4State;
    lastBtn5State = btn5State;
    lastBtn6State = btn6State;
    lastBtn7State = btn7State;
    // ModeCar__state = 0;
    // ModeSys__state = 0;
    // PS__state = 0;
    // QH__state = 0;
  }
}
void sendUart() {

  String data1[10] = { String(giatri_gas), String(giatri_mtr), String(giatri_khoi), String(dustDensity), String(t), String(h), String(manualMode), String(!modeCar), String(count % 2 == 1), String(!digitalRead(relayPins[1])) };



  String combinedData1 = "";  // Khởi tạo chuỗi kết quả

  combinedData1 = data1[0] + "," + data1[1] + "," + data1[2] + "," + data1[3] + "," + data1[4] + "," + data1[5] + "," + data1[6] + "," + data1[7] + "," + data1[8] + "," + data1[9] + "\n";
  // Kết hợp các dữ liệu vào chuỗi

  // Gửi chuỗi dữ liệu qua UART
  Serial1.print(combinedData1);
  //Serial.print(combinedData1);
}
