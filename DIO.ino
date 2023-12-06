#define Get_Bit(reg, bit)    ((reg >> bit) & 1)
#define Set_Bit(reg, bit)    (reg |= (1 << bit))
#define Clear_Bit(reg,bit)   (reg &= ~(1 << bit))

void DIO_Init(){
  DIO_SetPinDirection('b',5,'o');
  DIO_SetPinDirection('b',4,'o');
}

// set Pin to INPUT or OUTput
void DIO_SetPinDirection(unsigned char port, int pin, unsigned char direction){ 
  if (port == 'b' & pin <= 7 & pin >= 0){ // pin must be from 0-7
    if(direction == 'o'){
      DDRB = DDRB|(1<<pin); //bit set to 1 for output
    }else if(direction == 'i'){
      DDRB = DDRB&!(1<<pin); // bit cleared to 0 for input
    }
  }
  if (port == 'c' & pin <= 7 & pin >= 0){ // pin must be from 0-7
    if(direction == 'o'){
      DDRC = DDRC|(1<<pin); // bit set to 1 for output
    }else if(direction == 'i'){
      DDRC = DDRC&!(1<<pin); // bit cleared to 0 for input
    }
  }
  if (port == 'd' & pin <= 7 & pin >= 0){ // pin must be from 0-7
    if(direction == 'o'){
      DDRD = DDRD|(1<<pin); //bit set to 1 for output
    }else if(direction == 'i'){
      DDRD = DDRD&!(1<<pin); //bit cleared to 0 for input
    }
  }
}

// if pin is output it will set it to either HIGH or LOW
// if pin is input it will activate internal pull up resistor
void DIO_SetPinState(unsigned char port, int pin, unsigned char state){
  if (port == 'b' & pin <= 7 & pin >= 0){ // pin must be from 0-7
    if(state == 'h'){
      PORTB = PORTB|(1<<pin); //pin is set to 1 for high
    }else if(state == 'l'){
      PORTB &= ~(1<<pin); //pin is cleared to 0 for low
    }
  }
  if (port == 'd' & pin <= 7 & pin >= 0){ // pin must be from 0-7
    if(state == 'h'){
      PORTD = PORTD|(1<<pin); //pin is set to 1 for high
    }else if(state == 'l'){
      PORTD &= ~(1<<pin); //pin is cleared to 0 for low
    }
  }
  if (port == 'c' & pin <= 7 & pin >= 0){ // pin must be from 0-7
    if(state == 'h'){
      PORTC = PORTC|(1<<pin); //pin is set to 1 for high
    }else if(state == 'l'){
      PORTC &= ~(1<<pin); //pin is cleared to 0 for low
    }
  }
}

int DIO_GetPinState(unsigned char port, int pin){ //function to return the value of a bit from a register
  int state;
  switch(port){ //check which register to use
    case 'd':
      if((pin <= 7) && (pin >= 0)){
        state = Get_Bit(PIND, pin);
      }
      break;
    case 'b':
      if((pin <= 5) && (pin >= 0)){
        state = Get_Bit(PINB, pin);
      }
      break;
    case 'c':
      if((pin <= 6) && (pin >= 0)){
        state = Get_Bit(PINC, pin);
      }
  }
  return state;
}
  