/* Simulate two balls in SHM
   WS2812 LED strip with HC-05 Bluetooth adaptor
   Hang the first LED strip like a catenary
   Hang a shorter strip from the centre
   
   Audio output from digital pin, connect to extnl amp
   
   Bluetooth control viz:
   u - up the brightness
   d - dim the brightness
   S - toggle sound output
   f - faster
   s - slower

  Geo - 20.11.2018
*/

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // RXin, TXout

#include <FastLED.h>
#include <Math.h>
#define NUM_LEDS 120
#define NUM_LEDS2 28
#define DATA_PIN 5      // to LED strip1 control wire
#define DATA_PIN2 6     // to LED strip2 control wire
#define tonepin 3       // connect via resistor chain to grnd
#define COLOR_ORDER GRB
#define mindel 2        // min value of DELAY
#define LED 13          // Pin 13 is connected to the Arduino LED
char rxChar = 0;        // rxChar holds the received command.
int j = 200;
int k = 200;
int m = 0;
int BRIGHTNESS = 24;
int DELAY = 10;         // extra delay at end of LED run

bool toggle = false;
bool stoploop = false;
bool debug = false;
bool sound = true;
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];

void setup() {
  Serial.begin(9600);   // Open serial port
  FastLED.addLeds<WS2812, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, DATA_PIN2, COLOR_ORDER>(leds2, NUM_LEDS2);
  FastLED.setBrightness(BRIGHTNESS);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
  }
  for (int i = 0; i < NUM_LEDS2; i++) {
    leds2[i].setRGB(0, 0, 0);
    FastLED.show();
  }
  BTSerial.begin(9600);
  BTSerial.flush();
}

void loop() {
  static int loopcnt = 0;
  while (!stoploop) {
    if (BTSerial.available() > 0 ) {        // Check receive buffer.
      rxChar = BTSerial.read();            // Save character received.
      BTSerial.flush();                    // Clear rx buffer
      Serial.println(rxChar);
      BT(rxChar);
    }
   
    for (int i = loopcnt; i < NUM_LEDS / 2; i++) {  //in to centre
      leds[i].setRGB(j, k, m);                      //eg 0->59
      leds[NUM_LEDS - i - 1].setRGB(k, j, m);       //eg 119->59
      FastLED.show();
      leds[i] = CRGB::Black;
      leds[NUM_LEDS - i - 1] = CRGB::Black;

      int n = 100 * (2 * i) / NUM_LEDS;
      float value = DELAY * (1 - sin(n * PI / 200) );
      delay (mindel + int(value));

      if (debug) {
        Serial.print (" * ");
        Serial.print (loopcnt);
        Serial.print (" \t ");
        Serial.print(i);
        Serial.print (" \t ");
        Serial.println(NUM_LEDS - i - 1);
        //  Serial.print (" \t ");
        // Serial.println(value, 4);
      }
      if (sound) {
        blip(1);
      }
    }

    for (int i = (NUM_LEDS / 2); i > loopcnt; i--) {
      leds[i - 1].setRGB(k, j, m);          //eg 59->0
      leds[NUM_LEDS - i].setRGB(j, k, m); //eg. 59->119
      FastLED.show();
      leds[i - 1] = CRGB::Black;
      leds[NUM_LEDS - i] = CRGB::Black;

      int n = 100 * (2 * i) / NUM_LEDS;
      float value = DELAY * (1 - sin(n * PI / 200) );
      delay (mindel + int(value));

      if (debug) {
        Serial.print (" ** ");
        Serial.print (loopcnt);
        Serial.print (" \t ");
        Serial.print(i - 1);
        Serial.print (" \t ");
        Serial.println(NUM_LEDS - i);
      }
      if (sound) {
        blip(1);
      }
    }

    if (sound) {
      tone(tonepin, 200 + (loopcnt * 10), 150);
      delay(150);
    }

    toggle = !toggle;
    if (toggle == true) {
      j = 255;
      k = 0;
    }
    else {
      j = 0;
      k = 255;
    }
  
    loopcnt++;

    if (loopcnt == NUM_LEDS / 2) {
      loopcnt = 0;
      FastLED.show();
      if (sound) warble();

      for (int i = NUM_LEDS / 2; i > 0; i--) {      //return LEDs to top
        leds[i - 1].setRGB(0, 255, 0);
        leds[NUM_LEDS - i].setRGB(255, 0, 0);
        if (i > 1) {
          FastLED.show();
          leds[i - 1] = CRGB::Black;
          leds[NUM_LEDS - i] = CRGB::Black;
        }
        tone(tonepin, 200 + (NUM_LEDS / 2 - i) * 40, 40); delay(40);
      }

      for (int j = 1; j < 5; j++) {
        randomSeed(analogRead(0));
        int x = random(255); int y = random(255); int z = random(255);
        for (int i = NUM_LEDS2; i > 0; i--) {
          leds2[i].setRGB(x, y, z);
          FastLED.show();
          leds2[i] = CRGB::Black;
          if (sound) {
            tone(tonepin, 400 + (i * 20), 80);
            delay(10 + 20 * i / NUM_LEDS2);
          }
        }
        delay(30);
      }
    }
    if (debug && (loopcnt == 6)) {
      stoploop = true;
    }
  }
}

/////////////////////////////////////////////////////
char BT(char rx) {
  switch (rx) {
    case 'u':
      {
        if (BRIGHTNESS < 100) {
          BRIGHTNESS += 5;
          FastLED.setBrightness(  BRIGHTNESS );
          Serial.print("Brightness = ");
          Serial.println(BRIGHTNESS);
        }
      }
      break;
    case 'd':
      {
        if (BRIGHTNESS > 5) {
          BRIGHTNESS -= 5;
          FastLED.setBrightness(  BRIGHTNESS );
          Serial.print("Brightness = ");
          Serial.println(BRIGHTNESS);
        }
      }
      break;
    case 'S': {
        sound = !sound;
      }
      break;
    case 'f':
      {
        if (DELAY > 3) {
          DELAY -= 2;
          Serial.print("delay = ");
          Serial.println(DELAY);
        }
      }
      break;
    case 's':
      {
        if (DELAY < 20) {
          DELAY += 2;
          Serial.print("delay = ");
          Serial.println(DELAY);
        }
      }
      break;
  }
}

void warble() {
  for (int i = 0; i < 20; i++) {
    tone(tonepin, i * 70, 50);
    delay (50);
  }
  for (int i = 20; i > 0; i--) {
    tone(tonepin, i * 70, 50);
    delay (50);
  }
}

void blip(int t) {
  digitalWrite(tonepin, HIGH);
  delay(t);
  digitalWrite(tonepin, LOW);
}
