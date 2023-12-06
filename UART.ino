#include <avr/io.h>

void uart_SetBaudRate(int BRate){
  int ubrr = ((F_CPU)/(BRate*16UL)-1);  //function to change baudrate, = 103
  UBRR0H = (ubrr>>8);               //set the high baudrate
  UBRR0L = ubrr;                    //set the low baudrate
}

void UART_Init(int BRate){
  uart_SetBaudRate(BRate); //run the previous BaudRate setup function
  UCSR0B |= (1<<TXEN0)|(1<<RXEN0);  //enable the transmitter and receiver
  UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);//set format to 8-bit
}

void uart_transmit(unsigned char data){
  int udr_empty; //initialize the udr empty boolean-like variable
  do{
    udr_empty = (UCSR0A & (1<<UDRE0)) != 0; //check the data buffer if empty or not
  }while(!udr_empty); //loop if data buffer is full
  UDR0 = data; //send the data to the data bus
}

void uart_transmit_array(char* data, int size){ //parameters are character array(string) and length of array to be able to loop on it
  for(int i = 0; i < size; i++){ //for loop
    while(!(UCSR0A & (1<<UDRE0))); //while loop to wait for the character to be received before sending another one
    UDR0 = (int) data[i]; //sending the character bit
  }
}

unsigned char uart_receive(void){
  if(!uart_receive_ready()){
    return;
  }else {
    return UDR0;
  } 
}

int uart_receive_ready(void){
  int flag = ((UCSR0A)&(1<<RXC0)) != 0; //checks the 7th bit of the register which means the received completed successfully
  return flag;
}