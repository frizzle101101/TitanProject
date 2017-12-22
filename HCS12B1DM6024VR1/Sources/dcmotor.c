#include "dcmotor.h"
     
static unsigned int firstEdgeTCH0, firstEdgeTCH1;
static unsigned int edge0, edge1, period0, period1;
static int m1setpoint = 0;
static int m1PWMval = 0;
static int m1speedErrorIntegral = 0;
static int m1speedError = 0;

static int m2setpoint = 0;
static int m2PWMval = 0;
static int m2speedErrorIntegral = 0;
static int m2speedError = 0;



void DC_motor_init(void) {
  
  
  //Initilize PWM ports 4 & 5
  PWMCTL = EIGHT_BIT_MODE;                                                              // 8-bit mode 
  PWMPRCLK = PWM_ECLK;                                                                  // Counts E-clock directly
  PWMSCLA = SA_DIVIDE_2;                                                                // Set the SA divisor to devide the clock by 2 (Refer to table 8.4 in textbook)
  PWMCLK |= (PWMCLK_PCLK4_MASK | PWMCLK_PCLK5_MASK);                                    // Select clock SA as the clock source for PWM port 4 & 5 
  PWMPOL |= (PWMPOL_PPOL4_MASK | PWMPOL_PPOL5_MASK);                                    // Sets both PWM port 4 & 5 to output high at the start of a period (Positave Polarity), by setting the bits
  PWMCAE &= LOW(PWMCAE_CAE0_MASK);                                                      // Select left - aligned mode for PWM port 4
  PWMCAE &= LOW(PWMCAE_CAE1_MASK);                                                      // Select left - aligned mode for PWM port 5
  PWMPER4 = PERIOD_22KHZ_LEFT_ALINGED;                                                  // Sets period value to 22KHz
  PWMPER5 = PERIOD_22KHZ_LEFT_ALINGED;
  PWMDTY4 = 0;                                                                 // Duty cycle (0% of the PWM frequency for PWM channel 4)
  PWMDTY5 = 0;                                                                 // Duty cycle (0% of the PWM frequency for PWM channel 5)
  PWMCNT4 = PWM_RESET;                                                                  // Reset PWM4 & PWM5 counter
  PWMCNT5 = PWM_RESET;
  
  
  SET_BITS(DDRB, (DDRB_BIT0_MASK|DDRB_BIT1_MASK|DDRB_BIT2_MASK|DDRB_BIT3_MASK));         // configures PORTB 0,1,2,3 to outputs
  
  
  PWME |= (PWME_PWME4_MASK|PWME_PWME5_MASK);                                            // Enable the PWM0 and PWM1 ports
  
  //init pid loop
  TC4 	= TCNT + 20000;
  TIOS   |= TIOS_IOS4_MASK;//enable OC on tch4
}
void DC_motor_drive(int lr, int fb){
  if(fb == 0){
    if(lr == 0) {
      m1setpoint = 0;
      m2setpoint = 0;
    }else if(lr < 0){
      left_r_right_f();
      m1setpoint = map(lr,-1,-100,MIN_DRIVE_VALUE,MAX_STEER_DRIVE_VALUE);
      m2setpoint = m1setpoint;
    }else{
      left_f_right_r();
      m1setpoint = map(lr,1,100,MIN_DRIVE_VALUE,MAX_STEER_DRIVE_VALUE);
      m2setpoint = m1setpoint;
    }
  }else if(fb < 0) {
    left_r_right_r();
    if(lr == 0) {
      m1setpoint = map(fb,-1,-100,MIN_DRIVE_VALUE,MAX_DRIVE_VALUE);
      m2setpoint = m1setpoint;  
    }else if(lr < 0){
      m1setpoint = map(fb+lr-1,-1,-70,MIN_DRIVE_VALUE,MAX_STEER_DRIVE_VALUE);
      m2setpoint = map(fb,-1,-70,MIN_DRIVE_VALUE,MAX_STEER_DRIVE_VALUE);
    }else{
      m1setpoint = map(fb,-1,-100,MIN_DRIVE_VALUE,MAX_STEER_DRIVE_VALUE);
      m2setpoint = map(fb-lr-1,-1,-100,MIN_DRIVE_VALUE,MAX_STEER_DRIVE_VALUE);
    }
  }else {
    left_f_right_f();
    if(lr == 0) {
      m1setpoint = map(fb,1,100,MIN_DRIVE_VALUE,MAX_DRIVE_VALUE);
      m2setpoint = m1setpoint;
    }else if(lr < 0){
      m1setpoint = map(fb+lr+1,1,100,MIN_DRIVE_VALUE,MAX_STEER_DRIVE_VALUE);
      m2setpoint = map(fb,1,100,MIN_DRIVE_VALUE,MAX_STEER_DRIVE_VALUE);
    }else{
      m1setpoint = map(fb,1,100,MIN_DRIVE_VALUE,MAX_STEER_DRIVE_VALUE);
      m2setpoint = map(fb-lr+1,1,100,MIN_DRIVE_VALUE,MAX_STEER_DRIVE_VALUE);
    }
  }
                                                                   // Duty cycle (75% of the PWM frequency for PWM channel 4)
  PWMDTY4 = m1PWMval;
  PWMDTY5 = m2PWMval;
}
void left_f_right_f(void){
  SET_BITS(PORTB,(PORTB_BIT1_MASK|PORTB_BIT2_MASK));                                     // writes a 1 to bits 1 and 2
  CLR_BITS(PORTB,(PORTB_BIT0_MASK|PORTB_BIT3_MASK));                                     // writes a 0 to bits 0 and 3
}
void left_f_right_r(void){
  SET_BITS(PORTB,(PORTB_BIT0_MASK|PORTB_BIT2_MASK));                                     // writes a 1 to bits 0 and 2
  CLR_BITS(PORTB,(PORTB_BIT1_MASK|PORTB_BIT3_MASK));                                     // writes a 0 to bits 1 and 3
}
void left_r_right_f(void){
  SET_BITS(PORTB,(PORTB_BIT1_MASK|PORTB_BIT3_MASK));                                     // writes a 1 to bits 1 and 3
  CLR_BITS(PORTB,(PORTB_BIT0_MASK|PORTB_BIT2_MASK));                                     // writes a 0 to bits 0 and 2
}
void left_r_right_r(void){
  SET_BITS(PORTB,(PORTB_BIT0_MASK|PORTB_BIT3_MASK));                                     // writes a 1 to bits 0 and 3
  CLR_BITS(PORTB,(PORTB_BIT1_MASK|PORTB_BIT2_MASK));                                     // writes a 0 to bits 1 and 2
}
interrupt VectorNumber_Vtimch0 void TIMCH0handler (void){
  if(firstEdgeTCH0 != 1) {
    edge0 	= TC0;  		/* save the first captured edge and clear C0F flag */
    firstEdgeTCH0 = 1;
  }else{
    period0 	= TC0 - edge0;	/* calculate the period as the difference between edge timestamps */
    firstEdgeTCH0 = 0;
  }
}
interrupt VectorNumber_Vtimch1 void TIMCH1handler (void){
  if(firstEdgeTCH1 != 1) {
    edge1 	= TC1;  		/* save the first captured edge and clear C0F flag */
    firstEdgeTCH1 = 1;
  }else{
    period1 	= TC1 - edge1;	/* calculate the period as the difference between edge timestamps */
    firstEdgeTCH1 = 0;
  }
} 
interrupt VectorNumber_Vtimch4 void TIMCH4handler (void){
  long driveValue; 
  
   /* calculate error term */
  m1speedError = m1setpoint - (FEEDBACK_SCALE_FACTOR/period0);
  /* check for stupid speed error - spurious interrupt,etc. - don't run controller if garbage */
  if ((m1speedError < MAX_SPEED_ERROR) && (m1speedError > MIN_SPEED_ERROR)) {
  			 
     /* update integral term but only if drive is not on the rail */
     if (((m1PWMval > MIN_DRIVE_VALUE) && (m1PWMval < MAX_DRIVE_VALUE))||((m1PWMval == MIN_DRIVE_VALUE) && (m1speedError > 0))||((m1PWMval == MAX_DRIVE_VALUE) && (m1speedError < 0)))
        m1speedErrorIntegral += m1speedError;
     
   
     /* calculate the control law (NB: PI - no derivative term) */
     /* This is ALL integer math. */
  	driveValue = (((long)m1speedError * P_GAIN)+((long)m1speedErrorIntegral * I_GAIN)) / GAIN_DIVISOR;

     /* limit the controller output to range of PWM */
     if (driveValue > MAX_DRIVE_VALUE)
        driveValue = MAX_DRIVE_VALUE;
     else if (driveValue < MIN_DRIVE_VALUE)
        driveValue = MIN_DRIVE_VALUE;
     
      /* Save the motor drive value for next time.
      NB: This is where you write to the PWM hardware too */
      m1PWMval = driveValue;
      
  } // if speedError not stupid
  
  /* calculate error term */
  m2speedError = m2setpoint - (FEEDBACK_SCALE_FACTOR/period1);
  /* check for stupid speed error - spurious interrupt,etc. - don't run controller if garbage */
  if ((m2speedError < MAX_SPEED_ERROR) && (m2speedError > MIN_SPEED_ERROR)) {
  			 
     /* update integral term but only if drive is not on the rail */
     if (((m2PWMval > MIN_DRIVE_VALUE) && (m2PWMval < MAX_DRIVE_VALUE))||((m2PWMval == MIN_DRIVE_VALUE) && (m2speedError > 0))||((m2PWMval == MAX_DRIVE_VALUE) && (m2speedError < 0)))
        m2speedErrorIntegral += m2speedError;
     
   
     /* calculate the control law (NB: PI - no derivative term) */
     /* This is ALL integer math. */
  	driveValue = (((long)m2speedError * P_GAIN)+((long)m2speedErrorIntegral * I_GAIN)) / GAIN_DIVISOR;

     /* limit the controller output to range of PWM */
     if (driveValue > MAX_DRIVE_VALUE)
        driveValue = MAX_DRIVE_VALUE;
     else if (driveValue < MIN_DRIVE_VALUE)
        driveValue = MIN_DRIVE_VALUE;
     
      /* Save the motor drive value for next time.
      NB: This is where you write to the PWM hardware too */
      m2PWMval = driveValue;
      
  } // if speedError not stupid
  TC4 	= TCNT + 20000;
}