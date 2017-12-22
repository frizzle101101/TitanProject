#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <string.h>

int initRs232(void);
void closeRs232(int uart0_filestream);
void sendBytes(int uart0_filestream, char *p_tx_buffer);
void recieveBytes(int uart0_filestream);
