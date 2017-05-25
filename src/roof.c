#include <wiringPi.h>
#include "fToit.c"

#include "roofdefs.h"


void initRoof()
{
  pinMode(22,INPUT);
  pinMode(26,INPUT);
  pinMode(21,INPUT);
  pinMode(30,OUTPUT);
  pinMode(31,OUTPUT);
  pinMode(11,OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  pullUpDnControl(22, PUD_UP);
  pullUpDnControl(26, PUD_UP);
  pullUpDnControl(21, PUD_UP);
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
