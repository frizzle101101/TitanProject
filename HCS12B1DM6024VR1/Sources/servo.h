#include "derivative.h"      /* derivative-specific definitions */
#include "myUtils.h"

#define SET  1U                       //define for ones
#define ZERO 0U                       //define for zeros

#define TCTL2_LOW  0x20               //defined for the TCTL2 register to be set low
#define TCTL2_HIGH 0x30               //defined for the TCTL2 register to be set high
#define ENABLE_TIMER_RTI  0x04        //defined for the enabling interrupts for timer channel 2
#define PRESCALER_VAL  0x03           //defined for the TSCR2 to set the prescaler to be 8 
#define TSCR_INIT   0xB0              //defined for the TSCR1 to enable the TCNT and to clear the timer flag

#define RC_GAIN     9
#define OFFSET_TIME 650
#define RC_PERIOD   20000U            //period of the RC servo 

#define SERVO_MAX         180
#define SERVO_MID         90
#define SERVO_MIN         55
#define SERVO_SPEED_MAX   2500
#define SERVO_SPEED_MIN   20000

void servo_init(void);
void degreesToTime(unsigned int degrees);
void Drive_Camera_Servo(int );


