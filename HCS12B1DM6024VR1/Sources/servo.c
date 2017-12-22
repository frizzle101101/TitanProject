 #include "servo.h"
 
 /*

output(us)=10*in(degrees) + 600us
600us is 0 degrees
1050us is 45 degrees
1500us is 90 degrees
1950us is 135 degrees
2400us is 180 degrees
*/
//do not go above 180 degrees or below 0 degrees
static unsigned int dutycycle = 600;
static int current_servo_input = 0;
static int servoSetPoint = SERVO_MID;
static int currentServoSpeed = 0;
 
 void servo_init(void){
  TCTL2 = TCTL2_LOW;        // initilize timer channel 2 to low
  TC2	= TCNT + dutycycle;	// preset TC2 for first OC event
  TIOS |= TIOS_IOS2_MASK;	// ready to go - enable TC2 as OC
  
  TC3	= TCNT + dutycycle;	// preset TC2 for first OC event
  TIOS |= TIOS_IOS3_MASK;	// ready to go - enable TC2 as OC
}

void degreesToTime(unsigned int degrees){
  dutycycle = (RC_GAIN*degrees) + OFFSET_TIME;  
}

void Drive_Camera_Servo(int fb){
  current_servo_input = fb;
}

interrupt VectorNumber_Vtimch3 void TIMCH3handler(void){
  if(current_servo_input < 0){
    currentServoSpeed = lmap(current_servo_input,-1,-100,SERVO_SPEED_MAX,SERVO_SPEED_MIN);
    servoSetPoint = map(current_servo_input, -1, -100, SERVO_MID, SERVO_MAX);
	   
  }else if(current_servo_input > 0){
    currentServoSpeed = lmap(current_servo_input,1,100,SERVO_SPEED_MAX,SERVO_SPEED_MIN);
    servoSetPoint = map(current_servo_input, 1, 100, SERVO_MID, SERVO_MIN);
	}else{
    servoSetPoint = SERVO_MID;
  }
  degreesToTime(servoSetPoint);
  TC3 = TCNT + currentServoSpeed;
}
interrupt VectorNumber_Vtimch2 void ISR_usDelay(void){
   
    if(TCTL2 == TCTL2_LOW){               // if the TCTL2 is low the it has completed the high toggle
    TC2 = TCNT + (RC_PERIOD - dutycycle); // rearm the OC register for low time
    TCTL2 = TCTL2_HIGH;                   // TCTL2 to high so it starts the period as high in the next interrupt 
    } 
                           
    else if(TCTL2 == TCTL2_HIGH){   // if the TCTL2 is high the it has completed the 20ms period or the low toggle 
    TC2 = TCNT + dutycycle;		      // rearm the OC register for high time
    TCTL2 = TCTL2_LOW;              // TCTL2 to low so it completes the 20ms period in the next interrupt
    }
    else{}

}