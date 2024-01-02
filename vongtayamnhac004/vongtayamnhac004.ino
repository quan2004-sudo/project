
#define BLYNK_TEMPLATE_ID "TMPL6wt3c9E6_"
#define BLYNK_TEMPLATE_NAME "vòng tay âm nhạc"

#define EEPROM_SIZE 512
#define BLYNK_FIRMWARE_VERSION "0.1.4"

#define BLYNK_PRINT Serial

                                                                                                                         
#define APP_DEBUG


#include <FastLED.h>
#include <MegunoLink.h>
#include <Filter.h>
#include "BlynkEdgent.h"
#include <EEPROM.h>
int save, Note1, Note2, Note3, Note4, Note5, nguong, totalTime, Note11, Note22, Note33, Note44, Note55, nguong11, totalTime11;


#define NUM_LEDS 8
#define MIC_PIN A0
#define LED_PIN 14  // labeled "D6" on the board
// the following parameters can be tweaked according to your audio levels
#define NOISE 550
#define TOP (NUM_LEDS + 2)  // allow the max level to be slightly off scale
#define LED_TYPE WS2811
#define BRIGHTNESS 24  // a little dim for recording purposes
#define COLOR_ORDER GRB
// declare the LED array
CRGB leds[NUM_LEDS];
int n, height, n_1;
// define the variables needed for the audio levels
int lvl = 0, minLvl = 0, maxLvl = 300;  // tweak the min and max as needed

unsigned long musicTime = 0, time1 = 0, lastOneSecond = 0;
unsigned long highNoteTimes[] = { 78, 90, 104, 197, 240 };
bool Score[5];

int btn = 12;
int highTone = 500;
// instantiate the filter class for smoothing the raw audio signal
ExponentialFilter<long> ADCFilter(5, 0);

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V1) {
  Note1 = param.asInt();
}
BLYNK_WRITE(V2) {
  Note2 = param.asInt();
}
BLYNK_WRITE(V3) {
  Note3 = param.asInt();
}
BLYNK_WRITE(V0) {
  Note4 = param.asInt();
}
BLYNK_WRITE(V4) {
  Note5 = param.asInt();
}
BLYNK_WRITE(V5) {
  totalTime = param.asInt();
}
BLYNK_WRITE(V8) {
  nguong = param.asInt();
}
BLYNK_WRITE(V9) {
  save = param.asInt();
  if (save == 1) {
    saveTime();
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  EEPROM.begin(EEPROM_SIZE);
  BlynkEdgent.begin();

  Score[0] = false;
  Score[1] = false;
  Score[2] = false;
  Score[3] = false;
  Score[4] = false;
  pinMode(btn, INPUT_PULLUP);
  // put your setup code here, to run once
  // initialize the LED object
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  BlynkEdgent.run();


  Note11 = EEPROM.read(1) << 8;
  Note11 |= EEPROM.read(2);

  Note22 = EEPROM.read(3) << 8;
  Note22 |= EEPROM.read(4);

  Note33 = EEPROM.read(5) << 8;
  Note33 |= EEPROM.read(6);

  Note44 = EEPROM.read(7) << 8;
  Note44 |= EEPROM.read(8);

  Note55 = EEPROM.read(9) << 8;
  Note55 |= EEPROM.read(10);

  totalTime11 = EEPROM.read(11) << 8;
  totalTime11 |= EEPROM.read(12);
  nguong11 = EEPROM.read(13) << 8;
  nguong11 |= EEPROM.read(14);

  highNoteTimes[0] = Note11;
  highNoteTimes[1] = Note22;
  highNoteTimes[2] = Note33;
  highNoteTimes[3] = Note44;
  highNoteTimes[4] = Note55;



  // put your main code here, to run repeatedly:
  // read the audio signal and filter it
  if (millis() - lastOneSecond > 1000) {
    lastOneSecond = millis();
    time1++;
  }

resetMusic();
  if (time1 < totalTime11) {

    n_1 = analogRead(MIC_PIN);
    n = abs(1023 - n_1);
    Serial.print(Note11);
    Serial.print("\t");
    Serial.print(Note22);
    Serial.print("\t");
    Serial.print(Note33);
    Serial.print("\t");
    Serial.print(Note44);
    Serial.print("\t");
    Serial.print(Note55);
    Serial.print("\t");
    Serial.print(totalTime11);
    Serial.print("\t");
    Serial.print(nguong11);
    Serial.print("\t");
    Serial.print(n);
    Serial.print("\t");
    Serial.println(time1);

    Blynk.virtualWrite(V6, time1);
    // remove the MX9614 bias of 1.25VDC
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // // hard limit noise/hum
    // n = (n <= NOISE) ? 0 : abs(n - NOISE);
    // // apply the exponential filter to smooth the raw signal
    // ADCFilter.Filter(n);
    // lvl = ADCFilter.Current();
    // //  // plot the raw versus filtered signals
    // //  Serial.print(n);
    // //  Serial.print(" ");
    // //  Serial.println(lvl);
    // // calculate the number of pixels as a percentage of the range
    // // TO-DO: can be done dynamically by using a running average of min/max audio levels
    // height = TOP * (lvl - minLvl) / (long)(maxLvl - minLvl);
    // if (height < 0L) height = 0;
    // else if (height > TOP) height = TOP;
    // // turn the LEDs corresponding to the level on/off
    // for (uint8_t i = 0; i < NUM_LEDS; i++) {
    //   // turn off LEDs above the current level
    //   if (i >= height) leds[i] = CRGB(0, 0, 0);
    //   // otherwise, turn them on!
    //   else leds[i] = Wheel(map(i, 0, NUM_LEDS - 1, 30, 150));
    // }



    ////////////////////////////////////////////////////////////////////////////////////////////
    FastLED.show();
    for (int i = 0; i < 5; i++) {
      if ((time1 >= highNoteTimes[i]) && (time1 <= highNoteTimes[i] + 3)) {
        if (n >= nguong11) {

          Score[i] = true;
        }
      }
    }
  } else {
    leds[0] = CRGB(255, 255, 0);
    leds[6] = CRGB(255, 255, 0);  // l `ed vang
    FastLED.show();
    delay(3000);
    checkScore();
    resetMusic();
  }
  // Blynk.virtualWrite(V7,"N1:" + String(Note11) + " "+ "N2:"  + String(Note22) + " " + "N3:"+ String(Note33) + " "+ "N4:" + String(Note44) + " " + "N5:"+ String(Note55)+ " "+ "N~" + String(nguong11)+ " "+ "TT"  + String(totalTime11) + " "+ "Time:"  +  String(time1) + "I:"+ String(abs(1023 - n_1)) + "\t");
}

void saveTime() {
  EEPROM.write(1, Note1 >> 8);
  EEPROM.write(2, Note1);
  EEPROM.write(3, Note2 >> 8);
  EEPROM.write(4, Note2);
  EEPROM.write(5, Note3 >> 8);
  EEPROM.write(6, Note3);
  EEPROM.write(7, Note4 >> 8);
  EEPROM.write(8, Note4);
  EEPROM.write(9, Note5 >> 8);
  EEPROM.write(10, Note5);
  EEPROM.write(11, totalTime >> 8);
  EEPROM.write(12, totalTime);
  EEPROM.write(13, nguong >> 8);
  EEPROM.write(14, nguong);
}
// CRGB Wheel(byte WheelPos) {
//   // return a color value based on an input value between 0 and 255
//   if (WheelPos < 85)
//     return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
//   else if (WheelPos < 170) {
//     WheelPos -= 85;
//     return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
//   } else {
//     WheelPos -= 170;
//     return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
//   }
// }

void checkScore() {
  for (int i = 0; i < 5; i++) {
    if (Score[i] == true) {
      leds[i + 1] = CRGB(0, 255, 0);
    } else {
      leds[i + 1] = CRGB(255, 0, 0);
    }
  }
  FastLED.show();
}
void resetMusic() {
  if (digitalRead(btn) == LOW) {
    leds[0] = CRGB(255, 0, 0);
      FastLED.show();
    leds[1] = CRGB(255, 0, 0);
      FastLED.show();
    leds[2] = CRGB(255, 0, 0);
      FastLED.show();
    leds[3] = CRGB(255, 0, 0);
      FastLED.show();
    leds[4] = CRGB(255, 0, 0);
      FastLED.show();
    leds[5] = CRGB(255, 0, 0);
      FastLED.show();
    leds[6] = CRGB(255, 0, 0);
      FastLED.show();
    leds[7] = CRGB(255, 0, 0);
    FastLED.show();
    delay(1000);
    leds[0] = CRGB(255, 255, 0);
      FastLED.show();
    leds[1] = CRGB(255, 255, 0);
      FastLED.show();
    leds[2] = CRGB(255, 255, 0);
      FastLED.show();
    leds[3] = CRGB(255, 255, 0);
      FastLED.show();
    leds[4] = CRGB(255, 255, 0);
      FastLED.show();
    leds[5] = CRGB(255, 255, 0);
      FastLED.show();
    leds[6] = CRGB(255, 255, 0);
      FastLED.show();
    leds[7] = CRGB(255, 255, 0);
    FastLED.show();
    delay(1000);
    leds[0] = CRGB(0, 255, 0);
      FastLED.show();
    leds[1] = CRGB(0, 255, 0);
      FastLED.show();
    leds[2] = CRGB(0, 255, 0);
      FastLED.show();
    leds[3] = CRGB(0, 255, 0);
      FastLED.show();
    leds[4] = CRGB(0, 255, 0);
      FastLED.show();
    leds[5] = CRGB(0, 255, 0);
      FastLED.show();
    leds[6] = CRGB(0, 255, 0);
      FastLED.show();
    leds[7] = CRGB(0, 255, 0);
    FastLED.show();
    delay(1000);

    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    leds[6] = CRGB::Black;
    leds[7] = CRGB::Black;
    FastLED.show();

    Score[0] = false;
    Score[1] = false;
    Score[2] = false;
    Score[3] = false;
    Score[4] = false;
    // for (int i = 0; i < 5; i++) {
    //   leds[i + 1] = CRGB::Black;
    //   FastLED.show();
    // }
    time1 = 0;
  }
}