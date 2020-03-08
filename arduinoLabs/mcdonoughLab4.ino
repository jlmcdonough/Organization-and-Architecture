/*lab 8 */
const int tiltPin = 8;   //tilt sensor read pin
unsigned long previousTime = 0;  
int tiltState = 0;  //state of pin
int prevTiltState = 0;   //state pin previously was
int led = 2;        //starting pin of led
long interval = 500;  //delay time when starting

/*lab 5*/
#include <Servo.h>  //servo class
Servo myServo;  //servo object


void setup() {
  Serial.begin(9600);
  for(int x = 2; x<8; x++)  //configures the leds
  {
   pinMode(x, OUTPUT);
  }
  pinMode(11,OUTPUT);  //remaining leds
  pinMode(12,OUTPUT);
  pinMode(tiltPin, INPUT);  //tiltPin current serves as input

  myServo.attach(10);  //pin of servo
}

void loop() {
  unsigned long currentTime = millis();
  if(currentTime - previousTime > interval)
  {
   previousTime = currentTime;
   digitalWrite(led, HIGH);
   led++;
  if(led == 6)  //once the 4 primer leds are lit, the song begins
  {
     song();  //plays song
     shutLightsOff(); //shuts lights off
     moveServo();  //moves servo
   }
  }
  tiltState = digitalRead(tiltPin);  //reads tilt
  if(tiltState != prevTiltState)  //if tilt sensor was moved
  {
   shutLightsOff();  //shuts lights off
   led = 2;  //sets starting led back to first (on 2nd pin)
   previousTime = currentTime;
}
 prevTiltState = tiltState;
 int i = 0;

}

void shutLightsOff()  //shuts off all led lights
{
      for(int x = 2; x<8; x++)
     {
     digitalWrite(x, LOW);
     }
     digitalWrite(11, LOW);
     digitalWrite(12, LOW);
}

void moveServo()   //moves the servo
{
 int i = 0;
 while (i <50)
   {
     myServo.write(150);
     delay(15);
     myServo.write(40);
     delay(15);
     myServo.write(120);
     delay(15);
     myServo.write(10);
     delay(15);
     i++;
     Serial.println(i);
   }
}

void song()  //plays the song
{
  /* pitch followed by the duration in milliseconds*/
  int melody []= {370,263, 1,263, 440,263, 554,263, 1,263,440,263, 1,263, 370,263, 
  294,263, 294,263, 294,263, 1,263, 1,526, 1,263, 277,263,
  294,263, 370,263, 440,263, 554,263, 1,263, 440,263, 1,263, 349,263,
  659,790, 622,263, 587,263, 1,263, 1,526,
  
  415,263, 1,263, 554,263, 370,263, 1,263,554,263, 1,263, 415,263, 
  1,263, 554,263, 392,263, 370,263, 1,263, 330,263, 1,263,
  330,263, 330,263, 330,263, 1,263, 1,526, 330,263, 330,263,
  330,263, 1,263, 1,526, 311,263, 294,263, 

  277,263, 1,263, 440,263, 554,263, 1,263,440,263, 1,263, 370,263, 
  294,263, 294,263, 294,263, 1,263, 659,263, 659,263, 659,263, 1,263,
  1,263, 370,263, 440,263, 554,263, 1,263, 440,263, 1,263, 349,263,
  659,1052, 587,263, 1,263, 1,526,

  494,263, 392,263, 294,263, 277,526, 494,263, 392,263, 277,263, 
  440,263, 370,263, 262,263, 247,526, 349,263, 294,263, 247,263,
  330,263, 330,263, 330,263, 1,526, 1,526, 466,526,
  554,263, 587,263, 740,263, 880,263, 1,263, 1,526, 

  1,1052, 220,526, 233,526,  
  220,790, 220,263, 220,1052,
  1,526, 220,263, 233,263, 220,263, 349,526, 262,263,
  220,790, 220,263, 220,1052,

  1,1052, 247,526, 262,526, 
  277,790, 262,263, 277,1052,
  1,526, 277,263, 262,263, 277,263, 415,526, 311,263,
  277,790, 311,263, 247,2100,
  
  330,526, 330,526, 330,526, 1,263};

  int thisNote;
  for (thisNote = 0; thisNote < 286; thisNote = thisNote + 2) //286 is size of array
  {
      int pitch = melody[thisNote];
      int duration = melody[thisNote+1];

    if(pitch == 220 || pitch == 233 || pitch == 466 || pitch == 440 || pitch == 880)  //note is a variant of A
    {
      shutLightsOff();   //ensures all other lights are off
      digitalWrite(2, HIGH);  //turns on light attached to pin 2
    }
      
    else if(pitch == 247 || pitch == 494)   //note is a variant of B
    {
      shutLightsOff();
      digitalWrite(3, HIGH);  //turns on light attached to pin 3
    }
    
    else if(pitch == 277 || pitch == 262 || pitch == 554)    //note is a variant of C
    { 
      shutLightsOff();
      digitalWrite(4, HIGH);   //turns on light attached to pin 4
    }
    
    else if(pitch == 311 || pitch == 294 || pitch == 622 || pitch == 587)   //note is a variant of D
     {
      shutLightsOff(); 
      digitalWrite(5, HIGH);   //turns on light attached to pin 5
     }

    else if(pitch == 330 || pitch == 659)   //note is a variant of E
     {
      shutLightsOff(); 
      digitalWrite(6, HIGH);  //turns on light attached to pin 6
     }
      
    else if(pitch == 370 || pitch == 349 || pitch == 740)  //note is a variant of F
      {
        shutLightsOff();
      digitalWrite(7, HIGH);   //turns on light attached to pin 7
      }
      
    else if(pitch == 415 || pitch == 392)   //note is a variant of G
     {
      shutLightsOff();
      digitalWrite(11, HIGH);  //turns on light attached to pin 11
     }

    else if(pitch == 1)   //rest (no note)
    {
      shutLightsOff();
      digitalWrite(12, HIGH);  //turns on light attached to pin 12
    }
     
   
    tone(13, pitch, duration*.9);  //tone is (pin, pitch, duration)
    delay(duration);
    noTone(13);      // stop the waveform generation before the next note
    
  }
}
