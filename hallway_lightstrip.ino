#include "LPD8806.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma

//--------------------------------------
//VARIABLE PARAMETER, Debuggin, Zeiten, Helligkeiten, ...
//--------------------------------------
bool debugSensoren = true; //false is standard
const int SensorTEST_Pos = 50;
const int maxhelligkeit = 60;
const int minhelligkeit = 1;
const int ZEITGOONPHASE   = 25;
const int ZEITBEONPHASE   = 1000;
const int ZEITGOOFFPHASE  = 100;
const int ZEITCHECKSENSORPHASE = 100;

//--------------------------------------
//TEST ODER PRODUKTIV 
//--------------------------------------
//String System = "Chris"; //Default ist Thomas
String System = "Thomas"; //Default ist Thomas

//--------------------------------------
//FEST PARAMETER HARDWARE 
//--------------------------------------
int dataPin  = 2;
int clockPin = 3;
int interval = 100;
String Nachricht;
unsigned long ledMillis;
enum ZUSTAENDE {GOON, BEON, GOOFF, CHECKSENSOR};
byte zustand = CHECKSENSOR;
bool SensorWert1 = false;
bool SensorWert2 = false;
bool SensorWert3 = false;
uint32_t color;
int Red_Value = 0;
int Green_Value = 0;
int Blue_Value = 0;
int Red_Value_T = 0;
int Green_Value_T = 0;
int Blue_Value_T = 0;

//unsigned long previousMillis = 0;
//int iniziert = 0;
//int RGBW_Step = 1;
//bool nextStep = false;
//bool stripON = false;
//String Nachricht,Stripe;
/* //FOR NEXT PROJEKT VERSION
int WegSchritt1 = -1;
int WegSchritt2 = -1;
bool WegSchrittGetan1 = false;
bool WegSchrittGetan2 = false;
int Way = 0;
*/

//THOMAS
//LED ANZAHL 215
//SENSOR Buero 87
//SENSOR WC 121
//SENSOR Bad 199
//--------------------------------------
//CONSTANTEN SIND AUSZUKOMENTIEREN BEI TEST ODER PRODUKTIV 
//--------------------------------------
//CHRIS
/*
  // Number of RGB LEDs in strand:
  const int nLEDs = 76;
  //S1 = Wohnzimmer/Bad
  const int Sensor1 = 5;       // 7
  const int Sensor1_Pos = 20;  // 87
  //S2 = WC
  const int Sensor2 = 6;       // 6
  const int Sensor2_Pos = 40;  // 121
  //S3 = BUERO
  const int Sensor3 = 7;       // 5
  const int Sensor3_Pos = 60;  // 199
// */
//THOMAS FLUR

// Number of RGB LEDs in strand:
const int nLEDs = 216;
//S1 = Wohnzimmer/Bad
const int Sensor1 = 5;        // 7
const int Sensor1_Pos = 199;   // 87
//S2 = WC
const int Sensor2 = 6;        // 6
const int Sensor2_Pos = 121;  // 121
//S3 = BUERO
const int Sensor3 = 7;        // 5
const int Sensor3_Pos = 87;  // 199
// */

const int TEST_Pos = 210;  // Test Position

//--------------------------------------
//--------------------------------------

// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// Genutzte Funktionen
void sensorcheck ();
void fade_up_all ();
//uint32_t CHECH_STRIP (array ledstrip);

void setup() {
  pinMode(Sensor1, INPUT);
  digitalWrite(Sensor1, HIGH);
  pinMode(Sensor2, INPUT);
  digitalWrite(Sensor2, HIGH);
  pinMode(Sensor3, INPUT);
  digitalWrite(Sensor3, HIGH);

  // Start up the LED strip
  strip.begin();
  //Switch the STRIP OFF
  color = strip.Color(0, 0, 0);
  for (int s = 0; s < strip.numPixels(); s++) {
    strip.setPixelColor(s, color);
  }
  strip.show();

  Serial.begin(9600);
  Serial.println("Programm Gestartet: ");
}

void loop() {
  //CHRIS
  if (System == "Chris") {
    // Wert ueberpruefen
    if (digitalRead(Sensor1) == HIGH) { SensorWert1 = true;
    } else { SensorWert1 = false; }
    if (digitalRead(Sensor2) == HIGH) { SensorWert2 = true;
    } else { SensorWert2 = false; }
    if (digitalRead(Sensor3) == HIGH) { SensorWert3 = true;
    } else { SensorWert3 = false; }
  }
  else {
  //Thomas FLUR
    // Wert ueberpruefen
    if (digitalRead(Sensor1) == LOW) { SensorWert1 = true; } 
    else { SensorWert1 = false; }
    if (digitalRead(Sensor2) == LOW) { SensorWert2 = true; } 
    else { SensorWert2 = false; }
    if (digitalRead(Sensor3) == LOW) { SensorWert3 = true; }
    else { SensorWert3 = false; }
  }

  Nachricht = "";
  
  if (debugSensoren) {
    uint32_t  R, G, B, N;

    N = strip.Color(0, 0, 0);
    R = strip.Color(maxhelligkeit, 0, 0);
    G = strip.Color(0, maxhelligkeit, 0);
    B = strip.Color(0, 0, maxhelligkeit);

    if (SensorWert1 == false) {
      Nachricht = Nachricht + "S1-Wo/Ba: AN ";
    }
    else {
      Nachricht = Nachricht + "S1-Wo/Ba: AUS";
    }
    if (SensorWert2 == false) {
      Nachricht = Nachricht + "  || S2-WC: AN ";
    }
    else {
      Nachricht = Nachricht + "  || S2-WC: AUS";
    }
    if (SensorWert3 == false) {
      Nachricht = Nachricht + "  || S3-BU: AN ";
    }
    else {
      Nachricht = Nachricht + "  || S3-BU: AUS";
    }

    //Funktion TEST
    Red_Value_T = ((strip.getPixelColor(TEST_Pos) & 0x00FF00) >> 8);
    Blue_Value_T = ((strip.getPixelColor(TEST_Pos) & 0xFF0000) >> 16);
    Green_Value_T = (strip.getPixelColor(TEST_Pos) & 0x0000FF);

    //Nachricht = Nachricht + " || TEST: R:" + R + " G:" + G + " B:" + B + " N:" + N;
    Nachricht = Nachricht + "  || T-LED:"+ TEST_Pos + " R:" + Red_Value_T + " || G:" + Green_Value_T + " || B:" + Blue_Value_T  ;
    }
  
    //sensorcheck();

  
   // STRIP ABLAUFSTEUERUNG ZUSTAENDE {GOON, BEON, GOOFF, CHECKSENSOR} 
   if (millis() - ledMillis >= interval) {
      switch (zustand){
      case GOON:
          Nachricht = Nachricht + "  || ZU:GOON";
          fade_up_all();
          switch_next_led_on ();
          if(strip_all_on()){
            zustand = BEON;
            Nachricht = Nachricht + "  || Strip now on!";
          }
          interval = ZEITGOONPHASE;
          ledMillis = millis();
          break;
      case BEON:
          Nachricht = Nachricht + "  || ZU:BEON";
          zustand = GOOFF;
          if (SensorWert1 == false || SensorWert2 == false || SensorWert3 == false) {
            zustand = GOON;
          }
          interval = ZEITBEONPHASE;
          ledMillis = millis();
          break;
      case GOOFF:
          Nachricht = Nachricht + "  || ZU:GOOFF";
          fade_down_all ();
          if (SensorWert1 == false || SensorWert2 == false || SensorWert3 == false) {
            zustand = GOON;
          }
          if(strip_all_off()){
            zustand = CHECKSENSOR;
            Nachricht = Nachricht + "  || Strip now OFF!";
          }
          interval = ZEITGOOFFPHASE;
          ledMillis = millis();
          break;
      default:
          Nachricht = Nachricht + "  || ZU:DEFAULT";
          //                  R  B  G
          color = strip.Color(1, 1, 1);
          
          if (SensorWert1 == false) {
            strip.setPixelColor(Sensor1_Pos, color);
            zustand = GOON;
          }
          if (SensorWert2 == false) {
            strip.setPixelColor(Sensor2_Pos, color);
            zustand = GOON;
          }
          if (SensorWert3 == false) {
            strip.setPixelColor(Sensor3_Pos, color);
            zustand = GOON;
          }

          interval = ZEITCHECKSENSORPHASE;
          ledMillis = millis();
          break;
      }

      Serial.print(Nachricht);
      Serial.println();
    }
    strip.show();
}

void sensorcheck (){
    uint32_t color_sensorcheck;
    //S1 = Wohnzimmer/Bad
    if(SensorWert1 == false){
    //                              R  B              G  
    color_sensorcheck = strip.Color(0, maxhelligkeit, 0);
    strip.setPixelColor(Sensor1_Pos, color_sensorcheck); // Set new pixel 'on'
    }else{ color_sensorcheck = strip.Color(0, 0, 0);
    strip.setPixelColor(Sensor1_Pos, color_sensorcheck);}
    //S2 = WC
    if(SensorWert2 == false){
    color_sensorcheck = strip.Color(0, 0, maxhelligkeit);
    strip.setPixelColor(Sensor2_Pos, color_sensorcheck); // Set new pixel 'on'
    }else{ color_sensorcheck = strip.Color(0, 0, 0);
    strip.setPixelColor(Sensor2_Pos, color_sensorcheck);}
    //S3 = BUERO
    if(SensorWert3 == false){
    color_sensorcheck = strip.Color(maxhelligkeit, 0, 0);
    strip.setPixelColor(Sensor3_Pos, color_sensorcheck); // Set new pixel 'on'
    }else{ color_sensorcheck = strip.Color(0, 0, 0);
    strip.setPixelColor(Sensor3_Pos, color_sensorcheck); }
}

void switch_next_led_on () {
  int Red_next_Value, Blue_next_Value, Green_next_Value,Red_prev_Value,Blue_prev_Value,Green_prev_Value;
  bool nextred, nextblue, nextgreen, prevred, prevblue, prevgreen;
  nextred = true; 
  nextblue = true; 
  nextgreen = true; 
  prevred = true; 
  prevblue = true; 
  prevgreen = true; 
  
  for (int i = 0; i <= strip.numPixels(); i++) {
    // Rotwert Auslesen
    Red_Value = ((strip.getPixelColor(i) & 0x00FF00) >> 8);
    // Blau Auslesen
    Blue_Value = ((strip.getPixelColor(i) & 0xFF0000) >> 16);
    // Gruen Auslesen
    Green_Value = (strip.getPixelColor(i) & 0x0000FF);
    
    //RED CHECK
    if (Red_Value == 2 && nextred){
      //CHECK NAECHSTE LED
      if(((strip.getPixelColor(i + 1) & 0x00FF00) >> 8) == 0 && (i+1 <= strip.numPixels())){
        //Start next LED RED = 1
        nextred = false; 
        Red_next_Value = 1;
      } else {
        Red_next_Value = ((strip.getPixelColor(i+1) & 0x00FF00) >> 8);
      }
    } else {Red_next_Value = ((strip.getPixelColor(i+1) & 0x00FF00) >> 8);}
    if (Red_Value == 2 && prevred){
      //CHECK VORHAENGER LED
      if(((strip.getPixelColor(i - 1) & 0x00FF00) >> 8) == 0 && (i-1 >= 0)){
        prevred = false;
        Red_prev_Value = 1;
      } else {
        Red_prev_Value = ((strip.getPixelColor(i-1) & 0x00FF00) >> 8);
      }
    } else {Red_prev_Value = ((strip.getPixelColor(i-1) & 0x00FF00) >> 8);}

    //Blue CHECK
    if (Blue_Value == 2 && nextblue){
      //CHECK NAECHSTE LED
      if(((strip.getPixelColor(i + 1) & 0xFF0000) >> 16) == 0 && (i+1 <= strip.numPixels())){
        nextblue = false; 
        Blue_next_Value = 1;
      } else {
        Blue_next_Value = ((strip.getPixelColor(i+1) & 0xFF0000) >> 16);
      }
    } else {Blue_next_Value = ((strip.getPixelColor(i+1) & 0xFF0000) >> 16);}
    if (Blue_Value == 2 && prevblue){
      //CHECK VORHAENGER LED
      if(((strip.getPixelColor(i - 1) & 0xFF0000) >> 16) == 0 && (i-1 >= 0)){
        prevblue = false;
        Blue_prev_Value = 1;
      } else {
        Blue_prev_Value = ((strip.getPixelColor(i-1) & 0xFF0000) >> 16);
      }
    } else {Blue_prev_Value = ((strip.getPixelColor(i-1) & 0xFF0000) >> 16);}

    //Green CHECK
    if (Green_Value == 2 && nextgreen){
      //CHECK NAECHSTE LED
      if((strip.getPixelColor(i + 1) & 0x0000FF) == 0 && (i+1 <= strip.numPixels())){
        nextgreen = false; 
        Green_next_Value = 1;
      } else {
        Green_next_Value = (strip.getPixelColor(i + 1) & 0x0000FF);
      }
    } else {Green_next_Value = (strip.getPixelColor(i + 1) & 0x0000FF);}
    if (Green_Value == 2 && prevgreen){
      //CHECK VORHAENGER LED
      if((strip.getPixelColor(i - 1) & 0x0000FF) == 0 && (i-1 >= 0)){
        prevgreen = false;
        Green_prev_Value = 1;
      } else {
        Green_prev_Value = (strip.getPixelColor(i - 1) & 0x0000FF);
      }
    } else {Green_prev_Value = (strip.getPixelColor(i - 1) & 0x0000FF);}
    
    strip.setPixelColor((i+1), strip.Color(Red_next_Value, Blue_next_Value, Green_next_Value));
    strip.setPixelColor((i-1), strip.Color(Red_prev_Value, Blue_prev_Value, Green_prev_Value));
  }
}

void fade_up_all () {
  for (int i = 0; i < strip.numPixels(); i++) {
    // Rotwert Auslesen
    if (((strip.getPixelColor(i) & 0x00FF00) >> 8) > 0){
      Red_Value = ((strip.getPixelColor(i) & 0x00FF00) >> 8) + 1 ;
    } else {Red_Value = 0;}
    
    // Blue Auslesen
    if (((strip.getPixelColor(i) & 0xFF0000) >> 16) > 0){
      Blue_Value = ((strip.getPixelColor(i) & 0xFF0000) >> 16) + 1 ;
    } else {Blue_Value = 0;}
    
    // Gruen Auslesen
    if ((strip.getPixelColor(i) & 0x0000FF) > 0){
      Green_Value = (strip.getPixelColor(i) & 0x0000FF) + 1 ;
    } else { Green_Value = 0; }
        
    // Werte Groesser 127 kruezen
    if (Red_Value > maxhelligkeit)   { Red_Value = maxhelligkeit;  }
    if (Green_Value > maxhelligkeit) { Green_Value = maxhelligkeit; }
    if (Blue_Value > maxhelligkeit)  { Blue_Value = maxhelligkeit; }
    
    strip.setPixelColor(i, strip.Color(Red_Value, Blue_Value,Green_Value));
  }
}

void fade_down_all () {
  for (int i = 0; i < strip.numPixels(); i++) {
    // Rotwert Auslesen
    if (((strip.getPixelColor(i) & 0x00FF00) >> 8) > 0){
      Red_Value = ((strip.getPixelColor(i) & 0x00FF00) >> 8) - 1 ;
    } else {Red_Value = 0;}
    
    // Blue Auslesen
    if (((strip.getPixelColor(i) & 0xFF0000) >> 16) > 0){
      Blue_Value = ((strip.getPixelColor(i) & 0xFF0000) >> 16) - 1 ;
    } else {Blue_Value = 0;}
    
    // Gruen Auslesen
    if ((strip.getPixelColor(i) & 0x0000FF) > 0){
      Green_Value = (strip.getPixelColor(i) & 0x0000FF) - 1 ;
    } else { Green_Value = 0; }
        
    // Werte kleiner min wert kruezen
    if (Red_Value < minhelligkeit)   { Red_Value = 0;  }
    if (Green_Value < minhelligkeit) { Green_Value = 0; }
    if (Blue_Value < minhelligkeit)  { Blue_Value = 0; }
    
    strip.setPixelColor(i, strip.Color(Red_Value, Blue_Value,Green_Value));
  }
}

bool strip_all_on(){
  bool CHECK_RED = true; 
  bool CHECK_BLUE = true; 
  bool CHECK_GREEN = true; 
  
  for (int i = 0; i < strip.numPixels(); i++) {    
    // Rotwert Auslesen
    if (((strip.getPixelColor(i) & 0x00FF00) >> 8) != maxhelligkeit){
      CHECK_RED = false;
    }
    
    // Blue Auslesen
    if (((strip.getPixelColor(i) & 0xFF0000) >> 16) != maxhelligkeit){
      CHECK_BLUE = false;
    }
    
    // Gruen Auslesen
    if ((strip.getPixelColor(i) & 0x0000FF) != maxhelligkeit){
      CHECK_GREEN = false;
    }
  }

  if(CHECK_RED && CHECK_BLUE && CHECK_GREEN){
    return true;
  }
  else {
    return false;  
  }
}

bool strip_all_off(){
  bool CHECK_RED = true; 
  bool CHECK_BLUE = true; 
  bool CHECK_GREEN = true; 
  
  for (int i = 0; i < strip.numPixels(); i++) {    
    // Rotwert Auslesen
    if (((strip.getPixelColor(i) & 0x00FF00) >> 8) != 0){
      CHECK_RED = false;
    }
    
    // Blue Auslesen
    if (((strip.getPixelColor(i) & 0xFF0000) >> 16) != 0){
      CHECK_BLUE = false;
    }
    
    // Gruen Auslesen
    if ((strip.getPixelColor(i) & 0x0000FF) != 0){
      CHECK_GREEN = false;
    }
  }

  if(CHECK_RED && CHECK_BLUE && CHECK_GREEN){
    return true;
  }
  else {
    return false;  
  }
}

// Chase one dot down the full strip.
void colorChase(uint32_t c, uint8_t wait) {
  int i;

  // Start by turning all pixels off:
  for (i = 0; i < strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    // strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    delay(wait);
  }

  strip.show(); // Refresh to turn off last pixel
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 384; j++) {     // cycle all 384 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 384));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r,g,b));
}
