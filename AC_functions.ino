#include <avr/io.h>
#include "LCD.h"
#include "DIO.h"
#include "ADC.h"
#include "UART.h"
#include "PWM.h"
#include <EEPROM.h>

void app_init(void){

  LCD_Init();
  ADC_Init();
  DIO_Init();
  PWM_Init();
  UART_Init(9600);

  return;
}

void target_inc_dec(int action,int& target, int address){
  switch(action){
    case increase:
      if(target < 60){
        target++;
        uart_transmit_array("compare increased\n",18);
        EEPROM.write(address, target);
      }
      return;
    case decrease:
      if(target > 0){
        target--;
        uart_transmit_array("compare decreased\n",18);
        EEPROM.write(address, target);
      }
      return;
  }
  
}

int AC_switch_toggle(int state){
  switch(state){
    case ON:
      state = OFF;
      DIO_SetPinState('b',5,'h');
      DIO_SetPinState('b',4,'l');
      LCD_Command(0x01);
      Set_DutyCycle(0,3);
      Set_DutyCycle(0,11);
      uart_transmit_array("off\n",4);
      return state;
    case OFF:
      state = ON;
      DIO_SetPinState('b',5,'l');
      DIO_SetPinState('b',4,'h');
      uart_transmit_array("on\n",3);
      return state;
  }
}