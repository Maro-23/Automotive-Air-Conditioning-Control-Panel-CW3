#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "LCD.h"
#include "DIO.h"
#include "ADC.h"
#include "UART.h"
#include "PWM.h"
#include "AC_functions.h"
#include <EEPROM.h>

//ENUMs for clarity of code and easier operations
enum display_state{
  target_temp = 0,
  display_temp = 1,
  direction_display = 2,
  OFF = 0,
  ON = 1,
  increase = 1,
  decrease = 0
};

int main(void){

  app_init(); //initializing all created APIs

  int AC_switch = OFF; //variable for control ON/OFF of AC switch
  DIO_SetPinState('b',5,'h'); //red led to show that AC is off

  //button initializations to allow for edge detection
  int display_button;
  int prev_display_button = DIO_GetPinState('c',4);
  int switch_button;
  int prev_switch_button = DIO_GetPinState('c',3);
  int increase_button;
  int prev_increase_button = DIO_GetPinState('c',5);
  int decrease_button;
  int prev_decrease_button = DIO_GetPinState('c',2);

  unsigned short ADC_input;
  float Temp_voltage;
  float current_temp;//three variables for ADC measurement and conversion
  unsigned char display_buffer[5];
  unsigned char target_buffer[5]; //display buffers for displaying on LCD screen

  int LCD_state = target_temp;//state off the screen, what is currently displayed

  char receive_string[7]; //empty string to store received characters
  int counter = 0; //counter for measuring length of string
  char received; //empty char variable to receive from the serial monitor input

  int duty3;
  int duty11;

  int eeprom_address = 0; 
  int compare_temp; //eeprom variable inits

  while(1){

    //ON AND OFF BLOCK
    do{
      switch_button = DIO_GetPinState('c',3);
      if(switch_button == HIGH && switch_button != prev_switch_button){
        AC_switch = AC_switch_toggle(AC_switch);
      }
      prev_switch_button = switch_button;
    }while(!AC_switch);

    //read the value stored in the eeprom
    compare_temp = EEPROM.read(eeprom_address);

    //COMMUNICATING WITH THE AC
    if(uart_receive_ready()){//only enter if there is a bit to receive
      received = uart_receive();//store the received bit in a variable
      uart_transmit(received);
      if(received != '\n'){//if the received bit isn't a new line, meaning not the end of the command as after the command is typed, enter is pressed
        receive_string[counter++] = received;//place the received bit into the string in position (counter) which starts at 0 and increments each time a char is added to the string
      }else{//once the received character is a newline, enter the comparison branch
        if(counter == 2 && strncmp(receive_string,"cr",2)==0 ){
          uart_transmit_array(display_buffer,2);
          uart_transmit('\n');
        }else if(counter == 2 && strncmp(receive_string,"tr",2)==0){
          uart_transmit_array(target_buffer,2);
          uart_transmit('\n');
        }else if(counter == 2 && strncmp(receive_string,"in",2)==0){
          target_inc_dec(increase,compare_temp,eeprom_address);
        }else if(counter == 2 && strncmp(receive_string,"dc",2)==0){
          target_inc_dec(decrease,compare_temp,eeprom_address);
        }else{
          uart_transmit_array("incorrect command\n",18); 
        }
        counter = 0; //reset the counter for checking the word length after finishing the loop
      }
    }

    

    //INCREASE COMPARE BUTTON
    increase_button = DIO_GetPinState('c',5);
    if(increase_button == HIGH && increase_button != prev_increase_button){
      target_inc_dec(increase,compare_temp,eeprom_address);  
    }
    prev_increase_button = increase_button;

    //DECREASE COMPARE BUTTON
    decrease_button = DIO_GetPinState('c',2);
    if(decrease_button == HIGH && decrease_button != prev_decrease_button){
      target_inc_dec(decrease,compare_temp,eeprom_address);
    }
    prev_decrease_button = decrease_button;

    //ADC READING AND CONVERSION
    ADC_input = Adc_ReadChannel(1);
    Temp_voltage=(ADC_input*5000UL)/1024UL;
    current_temp = (Temp_voltage)/10;

    //converting the integers from reading and comparing to strings to display on LCD
    IntToStr(current_temp,display_buffer);
    IntToStr(compare_temp,target_buffer);

    //MOTOR direction and speed control using comparisons
    if(current_temp > compare_temp){
      duty11 = 0;
      if(current_temp-compare_temp>5){
        duty3 = 255;
      }else{
        duty3 = 180;
      }
    }else if(current_temp < compare_temp){
      duty3 = 0;
      if(compare_temp-current_temp>5){
        duty11 = 255;
      }else{
        duty11 = 180;
      }
    }else{
      duty11 = 0;
      duty3 = 0;
    }
    Set_DutyCycle(duty11,11);
    Set_DutyCycle(duty3,3);

  //SWITCH DISPLAY BLOCK BUTTONS
    display_button = DIO_GetPinState('c',4);
    if(display_button == HIGH && display_button != prev_display_button){ 
      if(LCD_state != 2){
        LCD_state++;
        if(LCD_state == 1){
          uart_transmit_array("current_temperature\n",20);
        }else{
          uart_transmit_array("direction_display\n",18);
        }
      }else{
        LCD_state = 0;
        uart_transmit_array("target_temperature\n",19);
      }
    }
    prev_display_button = display_button;
    

    //SWITCH DISPLAY BLOCK LCD WRITE
    LCD_Command(0x80); //set cursor to original position
    switch(LCD_state){
      case target_temp:
        LCD_String("target temp");	 
        LCD_Command(0xC0);
        LCD_String(target_buffer);	 	
        break;
      case display_temp:
        LCD_String("current temp");
        LCD_Command(0xC0);	
        LCD_String(display_buffer); 	 
        break;
      case direction_display:
        LCD_String("fan direction");
        LCD_Command(0xC0);	
        if(duty3 == 0){
          LCD_String("CCW");
        }else{
          LCD_String("CW");
        }
        break;
    }
    LCD_String("  ");
    _delay_ms(400);    
  }
  return 0;
}

