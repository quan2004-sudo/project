/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6ZVRJ0kDl"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "1TfCvZisf698r2OYbgdqdYqjh_Mcbogl"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D6, D5);
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "AmericanStudy T1";
char pass[] = "66668888";
 unsigned long lasttimedata=0;
BlynkTimer timer;
int giatri_mtr, giatri_khoi, giatri_gas, dustDensity, manualMode, h, t, modeCar, PS_state, QH_state;
int len = 0, xuong = 0, trai = 0, phai = 0, ModeCar = 0, ModeSys = 0, PhunSuong = 0, QuatHut = 0, speed = 0;
int ModeCar_state = 0, ModeSys_state = 0, PhunSuong_state = 0, QuatHut_state = 0;
String Terminal, Mode_Sys, Mode_Car, PS__state, QH__state, data[10];
WiFiManager wifiManager;
// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  len = param.asInt();
}
BLYNK_WRITE(V1) {
  // Set incoming value from pin V0 to a variable
  xuong = param.asInt();
}
BLYNK_WRITE(V2) {
  // Set incoming value from pin V0 to a variable
  trai = param.asInt();
}
BLYNK_WRITE(V3) {
  // Set incoming value from pin V0 to a variable
  phai = param.asInt();
}
BLYNK_WRITE(V4) {
  // Set incoming value from pin V0 to a variable
  ModeCar = param.asInt();
}
BLYNK_WRITE(V5) {
  // Set incoming value from pin V0 to a variable
  ModeSys = param.asInt();
}
BLYNK_WRITE(V6) {
  // Set incoming value from pin V0 to a variable
  Terminal = param.asInt();
}
BLYNK_WRITE(V7) {
  // Set incoming value from pin V0 to a variable
  speed = param.asInt();
}
BLYNK_WRITE(V8) {
  // Set incoming value from pin V0 to a variable
  PhunSuong = param.asInt();
}
BLYNK_WRITE(V9) {
  // Set incoming value from pin V0 to a variable
  QuatHut = param.asInt();
}


// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED() {
  Blynk.virtualWrite(V6, "Xin chào!!!");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V6, "TimeUp:" + String(millis() / 1000));
  Blynk.virtualWrite(V6, "GAS:" + String(giatri_gas) + "  KK:" + String(giatri_mtr) + "  KHOI:" + String(giatri_khoi) + "  BUI:" + String(dustDensity) + "  NHIET:" + String(t) + "  DOAM:" + String(h));
  Blynk.virtualWrite(V6, "ModeSys:" + String(Mode_Sys) + "  ModeCar:" + String(Mode_Car) + "  PHUNSUONG:" + String(PS__state) + "  QUATHUT:" + String(QH__state));
}

void setup() {
  // Debug console
  Serial.begin(115200);
  mySerial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(3000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

  // chỗ này update dữ liệu
  if (millis() - lasttimedata > 2000) {lasttimedata= millis();  
sendUart();}
  nhandulieu();
}


void sendUart() {
  // float data1[3] = { mode_1, state1_1, state2_2 };  // Dữ liệu của 3
  String data1[9] = { String(ModeCar), String(ModeSys), String(PhunSuong), String(QuatHut), String(len), String(xuong), String(trai), String(phai), String(speed) };



  String combinedData1 = "";  // Khởi tạo chuỗi kết quả

  combinedData1 = data1[0] + "," + data1[1] + "," + data1[2] + "," + data1[3] + "," + data1[4] + "," + data1[5] + "," + data1[6] + "," + data1[7] + "," + data1[8] + "\n";
  // Kết hợp các dữ liệu vào chuỗi

  // Gửi chuỗi dữ liệu qua UART
  mySerial.print(combinedData1);
  Serial.print(combinedData1);
}


void nhandulieu() {
  if (mySerial.available()) {                              // Kiểm tra xem có dữ liệu đang được gửi đến không
    String receivedData = mySerial.readStringUntil('\n');  // Đọc chuỗi dữ liệu cho đến khi gặp ký tự '\n'

    // Tách dữ liệu bằng dấu phẩy

    int startIndex = 0;
    int commaIndex;

    for (int i = 0; i < 10; i++) {
      commaIndex = receivedData.indexOf(',', startIndex);
      if (commaIndex != -1) {
        data[i] = receivedData.substring(startIndex, commaIndex);
        startIndex = commaIndex + 1;
      } else {
        data[i] = receivedData.substring(startIndex);
        break;
      }
    }
    // for (int i = 0; i < 10; i++) {
    //   Serial.print("Data " + String(i + 1) + ": ");
    //   Serial.println(data[i]);
    // }
    giatri_gas = data[0].toInt();
    giatri_mtr = data[1].toInt();
    giatri_khoi = data[2].toInt();
    dustDensity = data[3].toInt();
    t = data[4].toInt();
    h = data[5].toInt();
    manualMode = data[6].toInt();
    modeCar = data[7].toInt();
    PS_state = data[8].toInt();
    QH_state = data[9].toInt();
    if (manualMode == 1) {
      Mode_Sys = "Thủ công";
    } else {
      Mode_Sys = "Tự động";
    }
    if (modeCar == 0) {
      Mode_Car = "Tự động";
    } else {
      Mode_Car = "Thủ công";
    }

    if (PS_state == 1) {
      PS__state = "Bật";
    } else {
      PS__state = "Tắt";
    }
    if (QH_state == 1) {
      QH__state = "Bật";
    } else {
      QH__state = "Tắt";
    }
  }
}
