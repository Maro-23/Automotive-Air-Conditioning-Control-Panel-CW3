void DIO_Init();
void DIO_SetPinDirection(unsigned char port, int pin, unsigned char direction);
void DIO_SetPinState(unsigned char port, int pin, unsigned char state);
int DIO_GetPinState(unsigned char port, int pin);
