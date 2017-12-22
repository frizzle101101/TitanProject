#include "timer.h"

static int delayinprogress = 0;
static volatile unsigned long systemTimeMs = 0; 
void timer_init(void) 
{
  
  TSCR1 	= 0x90;  		/* enable timer counter, enable fast flag clear*/
	TSCR2 	= 0x03; 		/* enable TCNT overflow interrupt, set prescaler to 8 */
	
	//initilize encoder timer chanels
	CLR_BITS(TIOS,(TIOS_IOS0_MASK|TIOS_IOS1_MASK)); 	/* enable input-capture 0,1 by clearing the bit */
	TCTL4 	= 0x0A; 		/* capture the falling edge of the PT0, and PT1 pin */
	TFLG1 	= 0x03;			/* clear the C0F,C1F flag – in case anything pending */
  TIE = 0x7F;         /* enable intrupts for timer 0,1,2,3,4,5,6*/

  //enable system timer channel
  TC6 	= TCNT + MSCONST;		/* preset TC6 for first OC event */
	TIOS   |= TIOS_IOS6_MASK;	/* ready to go - enable TC6 as OC */
}
unsigned long getSystemTimeMs(void){
  unsigned long temp;
  DisableInterrupts;
  temp = systemTimeMs;
  EnableInterrupts;
  return(temp);
}
interrupt VectorNumber_Vtimch6 void TIMCH6handler (void) {
  systemTimeMs++;
  TC6 	= TCNT + MSCONST;		/* preset TC6 for first OC event */
}
void delay_ms( int k )
{
  unsigned long inittime = getSystemTimeMs();
  while(k > (getSystemTimeMs()-inittime));
}
