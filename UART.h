void uart_SetBaudRate(int BRate);
void UART_Init(int BRate);
void uart_transmit(unsigned char data);
unsigned char uart_receive(void);
int uart_receive_ready(void);
void uart_transmit_array(char* data, int size);

