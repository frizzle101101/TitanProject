#include "rs232.h"

char buffer[7]= {'~','A','?'};

int main(){
	int uart0_filestream = initRs232();
	for(;;)
	{
		sendBytes(uart0_filestream, buffer);
	}
	closeRs232(uart0_filestream);
}
