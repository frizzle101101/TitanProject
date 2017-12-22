#include "adc.h"

unsigned int getAnalog(void) {
   unsigned int analog;
   ATDCTL5 = ATD_CL5; //Set the register to read ad2
   while (!(ATDSTAT0 & SCF)); 	//Has A/D conversion completed?	 
   analog = ATDDR0H;	//read back and save results *
   return(analog);
}

void initADC(void) {
   SET_BITS(ATDCTL2, ATD_CL2); 
   SET_BITS(ATDCTL3, ATD_CL3);
   SET_BITS(ATDCTL4, ATD_CL4);
  
 }