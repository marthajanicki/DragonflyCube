//HUMAN-PRESENCE PIR VARIABLES
#include <Wire.h>
#include "SparkFun_AK975X_Arduino_Library.h" //Use Library Manager or download here: https://github.com/sparkfun/SparkFun_AK975X_Arduino_Library
AK975X movementSensor; //Hook object to the library
int ir1, ir2, ir3, ir4, temperature;


//NEOPIXEL COMPONENTS
#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    2
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 7


// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW);

//HAPTIC DRIVER COMPONENT
#include <Sparkfun_DRV2605L.h> //SparkFun Haptic Motor Driver Library 
#include <Wire.h> //I2C library 
SFE_HMD_DRV2605L HMD; //Create haptic motor driver object 
int distanceState = 2; 
int lastDistanceState = 0; 



void setup(){
  //Serial.begin(9600);
  
  //HUMAN-PRESENCE PIR SETUP
  Serial.println("AK975X Read Example");
  Wire.begin();
  //Turn on sensor
  if (movementSensor.begin() == false) {
    Serial.println("Device not found. Check wiring.");
    while (1);
  }


  //NEOPIXEL SETUP
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)

  //HAPTIC MOTOR DRIVER
    HMD.begin();
  Serial.begin(9600);
  HMD.Mode(0); // Internal trigger input mode -- Must use the GO() function to trigger playback.
  HMD.MotorSelect(0x36); // ERM motor, 4x Braking, Medium loop gain, 1.365x back EMF gain
  HMD.Library(2); //1-5 & 7 for ERM motors, 6 for LRA motors
  
}

void loop() {
  //HUMAN-PRESENCE PIR LOOP
  if (movementSensor.available()) {
    ir1 = movementSensor.getIR1();
    ir2 = movementSensor.getIR2();
    ir3 = movementSensor.getIR3();
    ir4 = movementSensor.getIR4();
    float tempF = movementSensor.getTemperatureF();
    movementSensor.refresh(); //Read dummy register after new data is read
    //Note: The observable area is shown in the silkscreen.
    //If sensor 2 increases first, the human is on the left
    Serial.print("1:DWN[");
    Serial.print(ir1);
    Serial.print("]\t2:LFT[");
    Serial.print(ir2);
    Serial.print("]\t3:UP[");
    Serial.print(ir3);
    Serial.print("]\t4:RGH[");
    Serial.print(ir4);
    Serial.print("]\ttempF[");
    Serial.print(tempF);
    Serial.print("]\tmillis[");
    Serial.print(millis());
    Serial.print("]");
    Serial.println();
  }
    delay(1);

    Serial.println();
    delay(500);




    if ((ir1>5000) || (ir2>5000) ||(ir3>5000) ||(ir4>5000)) {
      distanceState = 0;
      if (distanceState != lastDistanceState) {
        
        Serial.print("distance state changed");
        Serial.println();
        delay(500);
        int seq = 0; //There are 8 sequence registers that can queue up to 8 waveforms
        int wave = 12; 
        HMD.Waveform(seq, wave);
        HMD.go();
        Serial.print("Effect No.:      ");
        Serial.println(wave);
        delay(800);

        seq = 0; //There are 8 sequence registers that can queue up to 8 waveforms
        wave = 94; 
        HMD.Waveform(seq, wave);
        HMD.go();
        Serial.print("Effect No.:      ");
       Serial.println(wave);
        delay(800); 

//         seq = 0; //There are 8 sequence registers that can queue up to 8 waveforms
//        wave = 98; 
//        HMD.Waveform(seq, wave);
//        HMD.go();
//        Serial.print("Effect No.:      ");
//       Serial.println(wave);
//        delay(800); 
       
        lastDistanceState = distanceState;
      }
    
    }
    else if ((ir1>0) || (ir2>0) ||(ir3>0) ||(ir4>0)) {
      distanceState = 1;
            if (distanceState != lastDistanceState) {
        
        Serial.print("distance state changed");
        Serial.println();
        delay(500);
        int seq = 0; //There are 8 sequence registers that can queue up to 8 waveforms
        int wave = 13; 
        HMD.Waveform(seq, wave);
        HMD.go();
        Serial.print("Effect No.:      ");
        Serial.println(wave);
        delay(800); 

        seq = 0; //There are 8 sequence registers that can queue up to 8 waveforms
        wave = 13; 
        HMD.Waveform(seq, wave);
        HMD.go();
        Serial.print("Effect No.:      ");
        Serial.println(wave);
        delay(1000); 
        
        lastDistanceState = distanceState;
      }
    }
    else {
      distanceState = 2;
         if (distanceState != lastDistanceState) {
        lastDistanceState = distanceState;
        Serial.print("distance state changed");
        Serial.println();
        delay(500);
        int seq = 0; //There are 8 sequence registers that can queue up to 8 waveforms
        int wave = 47; 
        HMD.Waveform(seq, wave);
        HMD.go();
        Serial.print("Effect No.:      ");
        Serial.println(wave);
        delay(800); 

        seq = 0; //There are 8 sequence registers that can queue up to 8 waveforms
        wave = 47; 
        HMD.Waveform(seq, wave);
        HMD.go();
        Serial.print("Effect No.:      ");
        Serial.println(wave);
        delay(1000); 

        lastDistanceState = distanceState;
      }
    }
    Serial.print(distanceState);
    Serial.print(lastDistanceState);
    Serial.println();





  //NEOPIXEL LOOP

    if ((ir1>5000) || (ir2>5000) ||(ir3>5000) ||(ir4>5000)) {
      colorWipe(strip.Color( 0, 150,   150, 50), 255); // purple
    } else { 
        colorWipe(strip.Color(0,   0,   25, 50), 50); // less bright BLUE
  colorWipe(strip.Color( 0, 0,   150, 50), 255); // more bright BLUE
    }




 }

 void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
//    if ((ir1>5000) || (ir2>5000) ||(ir3>5000) ||(ir4>5000)) {
//      delay(225);
//      //Serial.println("touching");
//    }
//    else if ((ir1>200) || (ir2>200) ||(ir3>200) ||(ir4>200)) {
//      delay(100);
//      //Serial.println("close");
//    }
//    else {
//      delay(5);
//      //Serial.println("far");
//    }

  if (distanceState == 0) { 
    delay(225);
  } else if (distanceState == 1) { 
    delay(100);
  } else { 
    delay(5);
  }

  }
}
