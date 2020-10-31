//http://www.youtube.com/user/greatscottlab 

#include "Tlc5940.h"  //don't forget to download the Tlc5940 lib from arduino playground ( http://tlc5940arduino.googlecode.com/files/Tlc5940_r014_2.zip )
#define LayerOne 14      //Gate of 1.MOSFET which connects to Layer "a" (anodes of all Strings in Layer "a")
#define LayerTwo 15      //Gate of 2.MOSFET which connects to Layer "b" (anodes of all StringS in Layer "b")
#define LayerThree 16    //Gate of 3.MOSFET which connects to Layer "c" (anodes of all Strings in Layer "c")
#define LayerFour 17     //Gate of 4.MOSFET which connects to Layer "d" (anodes of all Strings in Layer "d")


                   
int LayerDuration = 400;     // ON time of each Layer in microseconds  
int layer = 0;                // starting with Layer 0 (Layer "a")
unsigned long oldMicros = 0;// starting counter to decide when if(micros()>=oldMicros)is true to change active layer, count++1, MatrixUpdate(); don't know what micros() is? Look here: http://arduino.cc/de/Reference/Micros#.UygzAIWmWd4
int count=0;                // starting counter which is included in if() statement to change the shown picture, not necessary when using a static picture
int stretch=200;             // set timer for the change of the shown picture, if animation should move really fast then low stretch value, if animation should move really slow than big stretch value
int bright=3000;            // set brightness for all Strings(4095=20mA, 0=0ma, 1000=4.88mA,....). If you want individually brightness for a String you have to change the value in tab "function"


void setup()
{
  pinMode(LayerOne, OUTPUT);  // declare arduino nano pin A0(LayerOne) as OUTPUT
  pinMode(LayerTwo, OUTPUT);  // declare arduino nano pin A1(LayerTwo) as OUTPUT
  pinMode(LayerThree, OUTPUT);// declare arduino nano pin A2(LayerThree) as OUTPUT
  pinMode(LayerFour, OUTPUT); // declare arduino nano pin A3(LayerFour) as OUTPUT
  Tlc.init();               // configures the arduino to use the tlc5940, be sure to connect the arduino correctly to the tlc 
 
 /*          ARDUINO                                         TLC5940                            
              13|-> SCLK (pin 25)           OUT1 String "B" |1     28| OUT0 String "A"
              12|                           OUT2 String "C" |2     27|-> GND (VPRG)
              11|-> SIN (pin 26)            OUT3 String "D" |3     26|-> SIN (pin 11)
              10|-> BLANK (pin 23)          OUT4 String "E" |4     25|-> SCLK (pin 13)
               9|-> XLAT (pin 24)           OUT5 String "F" |5     24|-> XLAT (pin 9)
               8|                           OUT6 String "G" |6     23|-> BLANK (pin 10)
               7|                           OUT7 String "H" |7     22|-> GND
               6|                           OUT8 String "I" |8     21|-> VCC (+5V)
               5|                           OUT9 String "J" |9     20|-> 2K Resistor -> GND
               4|                          OUT10 String "K" |10    19|-> +5V (DCPRG)
               3|-> GSCLK (pin 18)         OUT11 String "L" |11    18|-> GSCLK (pin 3)
               2|                          OUT12 String "M" |12    17|-> SOUT (only used when you want to use more than one tlc5940)
               1|                          OUT13 String "N" |13    16|-> XERR (can be used as error report, but not necessary)
               0|                          OUT14 String "O" |14    15| OUT15 String "P"       */
  }

void loop(){
   
  if(micros()>= oldMicros){                                           // waits until LayerDuration is reached and than goes through cycle
    oldMicros=micros()+LayerDuration;                                 // updates oldMicros value by adding the micros() with LayerDuration
   if(count==0){reset();allred();}                                    // shows first static picture when count==0, so right at the beginng
   if(count==stretch){reset();allgreen();}                            // shows second static picture when count==2*stretch
   if(count==2*stretch){reset();allblue();}                           // .
   if(count==3*stretch){reset();allgreen();allblue();}                // .
   if(count==4*stretch){reset();allgreen();allred();}                 // .
   if(count==5*stretch){reset();allred();allblue();}                  // .
   if(count==6*stretch){reset();allgreen();allred();allblue();}       // .
   //if(count==7*stretch){reset();}                                   // .
   //if(count==8*stretch){reset();}                                   // .
   //if(count==9*stretch){reset();}                                   // shows 10. static picture when count ==9*stretch
   CubeUpdate(layer);                                                 // sets the values for the tlc5940 Outputs and puts all MOSFET Gates HIGH (not active) except for one MOSFET Low (active) -->this layer is ON, also look under tab "function"  
   layer++;                                                           // layer counter +1, so MatrixUpdate(layer) will affect the next layer during the next if() cycle 
   count++;                                                           // count counter +1, to change the picture in next if() cycle
   if(layer==5){layer = 1;}                                           // we only have 5 layers, so we start with layer 1 again if layer counter goes to 6
   if (count==7*stretch){count=0;}                                    // we have 8 static picture for the moving sine wave, so if count counter would hit the 9.(not existing) pic, it goes back to count=0
   }}
