#include <avr/io.h>
#include "PWM.h"

void PWM_Init(){
  DDRD |= (1<<PD3);
  DDRB |= (1<<PB3);

  TCCR2A |= (1<<COM2B1) | (1<<COM2A1) | (1<<WGM21) | (1<<WGM20); //set the mode to fast PWM and the mode to non inverting 
  TCCR2B |= (1<<CS20); //set to no prescalar, maximum freq possible

  Set_DutyCycle(0,3);
  Set_DutyCycle(0,11); //set the initial duty cycle to zero
}

void Set_DutyCycle(int duty, int pin){
  switch(pin){
    case 3:
      OCR2B = duty; //set the duty cycle of the comparison
      break;
    case 11:
      OCR2A = duty; //set the duty cycle of the comparison
      break;
  }

}