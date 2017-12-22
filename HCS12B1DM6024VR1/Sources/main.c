#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "lcd.h"
#include "adc.h"
#include "dcmotor.h"
#include "myUtils.h"
#include "sci.h"
#include "servo.h"
#include "spi.h"
#include "stepper.h"
#include "timer.h"

unsigned char cc = 0;
int rev = 0;
int x,y,v,w = 0;
char scic[4];
int i = 0;
int counter = 0;
int degrees = 55;//min:55, max:180
unsigned long startTime;
CmdInput currentInput;


void main(void) {
  /* put your own code here */
  
  timer_init();
  initLCD();
  //initADC();
  DC_motor_init();
  stepper_init();
  servo_init();
  //InitSPI(SCK_LOW_SAMPLE_RISE,MSB_FIRST,MASTER,INTERRUPT_OFF,SPI_ON,OUTPUT_BUFFER_ON,MODE_FAULT_OFF,0x02);
  
  //writeDacSpi(VOLTAGE_REF,VREF_3_8V,SUB);
  initSCI();
  
  EnableInterrupts;
  
  LCDprintf("Hello");
  startTime = getSystemTimeMs();
  delay_ms(1000);
  LCDclear();
  
	

  
  	
	
  
  for(;;) {
    currentInput = updateCmdInput();
    if(currentInput.val <= 200){
      if(currentInput.type == 'x') {
        x = map(currentInput.val,0,200,-100,100);   
      } else if(currentInput.type == 'y') {
        y = map(currentInput.val,0,200,-100,100);
      } else if(currentInput.type == 'v') {
        v = map(currentInput.val,0,200,-100,100);
      } else if(currentInput.type == 'w') {
        w = map(currentInput.val,0,200,-100,100);
      } else if(currentInput.type == 'E'){
        LCDprintf("Comunication lost from control server!");
      } 
    }else {
      //###send sci error 
    }
    DC_motor_drive(x,y);
    Drive_Camera_Stepper(v);
    Drive_Camera_Servo(w);
    //writeDacSpi(DATA,cc++,SUB);
    //LCDprintf("%d", counter++);
    //delay_ms(1000);

    //if(getSystemTimeMs() > startTime + 1000) {
    //  startTime = getSystemTimeMs();
    //  LCDclear();
    //}
      
        
    //degree2time(degrees);
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}