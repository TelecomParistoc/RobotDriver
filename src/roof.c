#include <wiringPi.h>
#include "fToit.c"


// A modifier par le fichier de configuration
// d'Arnaud
#include "config.h"


void init()
{
  pinMode(29,INPUT);
  pinMode(32,INPUT);
  pinMode(24,INPUT);
  pinMode(26,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(28,OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}

void setRedLed(int val){
  if(val==1)
     digitalWrite(RED_LED, HIGH);
  else
     digitalWrite(RED_LED,LOW);
}

void setGreenLed(int val){
  if(val==1)
     digitalWrite(GREEN_LED, HIGH);
  else
     digitalWrite(GREEN_LED,LOW);
}

void setYellowLed(int val){
  if(val==1)
     digitalWrite(YELLOW_LED, HIGH);
  else
     digitalWrite(YELLOW_LED,LOW);
}

int getStartJack(){
  if(digitalRead(PIN_JACK)==HIGH)
     return 1;
  else
     return 0;
}

int getColor(){
   if(digitalRead(BUTTON_COLOR)==HIGH)
      return 1;
   else
      return 0;
}

int getMode(){
  if(digitalRead(BUTTON_MODE)==HIGH)
     return 1;
  else
     return 0;
}
