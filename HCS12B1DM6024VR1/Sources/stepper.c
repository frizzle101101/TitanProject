#include "stepper.h"

static unsigned char stepper_array[] = {MOTOR_A,MOTOR_AC,MOTOR_C,MOTOR_BC,MOTOR_B,MOTOR_BD,MOTOR_D,MOTOR_AD};        // array with the different outputs required for the stepper motor.
static unsigned char state;                                                                                          // array index
static unsigned char step_type;                                                                                               // full step or halfstep, forwards or backwards
static unsigned char lsL, lsR;
static long currentStepSpeed;
static int totalStepCount;
static int step_count_calibp1;
static int step_count_calibp2;
static int calib_mode;
static int current_step;
static int stepperSetPoint;
static int current_input;

void stepper_init(void) {
  
  currentStepSpeed = 10000;
  state = 0;
  step_type = 1; 
  
  CLR_BITS(DDRAD,(DDRAD_DDRAD6_MASK|DDRAD_DDRAD7_MASK));//set pin ad6,ad7 as inputs                   
  
  SET_BITS(DDRP,DDRP_DDRP3_MASK); // enables the stepper motor by enabeling PP3 to an output
  SET_BITS(PTP,PTP_PTP3_MASK); // sets PP3 to a high
  
  SET_BITS(DDRT, (DDRT_DDRT7_MASK|DDRT_DDRT6_MASK|DDRT_DDRT5_MASK|DDRT_DDRT4_MASK));// configures Port T 4,5,6,7 to be outputs
  
  FORCE_BITS(PTT,PORT_T_WRITE_MASK,stepper_array[state]);
  
  SET_BITS(ATDDIEN, (PORTAD0_PTAD6_MASK|PORTAD0_PTAD7_MASK));
  
  TC5 = TCNT + CALIB_STEP_SPEED;
  TIOS   |= TIOS_IOS5_MASK;//enable OC on tch5
  
  calib_mode = 1;//set to calibrate camera steper first
}

void Drive_Camera_Stepper(int v){
    current_input = v; 
}

interrupt VectorNumber_Vtimch5 void TIMCH5handler (void) {
   lsL = (PTAD & LIMIT_SWITCH_MASK_L); //get ls Left
   lsR = (PTAD & LIMIT_SWITCH_MASK_R); //get ls Right
   
   if(calib_mode != 0) {
	    //calibration mode to get totalStepCount
	    switch(calib_mode) {
  	      case 1:
    	      if(lsL != 0){
      	       step_type = (step_type * (-1));//reverse stepper
      	       calib_mode = 2;
      	       step_count_calibp1 = 0;
               state = (state + step_type) & STEPPER_MASK;
               FORCE_BITS(PTT,PORT_T_WRITE_MASK,stepper_array[state]);//step
               step_count_calibp1++;
	          }else{
	            state = (state + step_type) & STEPPER_MASK;
              FORCE_BITS(PTT,PORT_T_WRITE_MASK,stepper_array[state]);//step 
	          }
  	        break;
  	      case 2:
  	        if(lsR != 0) {
               step_type = (step_type * (-1));//reverse stepper
               calib_mode = 3;
      	       step_count_calibp2 = 0;
               state = (state + step_type) & STEPPER_MASK;
               FORCE_BITS(PTT,PORT_T_WRITE_MASK,stepper_array[state]);//step
      	    } else {
      	      state = (state + step_type) & STEPPER_MASK;
              FORCE_BITS(PTT,PORT_T_WRITE_MASK,stepper_array[state]);//step
              step_count_calibp1++;
      	    }
  	        break;
  	      case 3:
  	        if(lsR != 0){
      	       calib_mode = 0;
      	       current_step = 0;
    	          totalStepCount = step_count_calibp1;
    	          stepperSetPoint = totalStepCount/2; 
	          }else{
	            calib_mode = 1;//calibration failed restart
	          }
  	        break;
  	    }
	    
	    TC5 = TCNT + CALIB_STEP_SPEED;
	 }else{
	    //Regular operation
	    
	    if(current_input < 0){
	      currentStepSpeed = lmap(current_input, -1, -100, STEP_SPEED_MAX, STEP_SPEED_MIN);
	      stepperSetPoint = map(current_input, -1, -100, (totalStepCount/2), totalStepCount);
	      if(lsL != 0 && (current_step < totalStepCount||current_step > totalStepCount)){
	        current_step = totalStepCount;
	        stepperSetPoint = current_step;   
	      }
	    }else if(current_input > 0){
	      currentStepSpeed = lmap(current_input, 1, 100, STEP_SPEED_MAX, STEP_SPEED_MIN);
	      stepperSetPoint = map(current_input, 1, 100, (totalStepCount/2), 0);
	      if(lsR != 0 && (current_step < 0||current_step > 0)){
	        current_step = 0;
	        stepperSetPoint = current_step;   
	      }
	    }else {
	      stepperSetPoint = totalStepCount/2;
	    }
	    if(current_step < stepperSetPoint){
	      state = (state + step_type) & STEPPER_MASK;
	      FORCE_BITS(PTT,PORT_T_WRITE_MASK,stepper_array[state]);//step
	      current_step++;
	    }else if(current_step > stepperSetPoint){
	      state = (state - step_type) & STEPPER_MASK;
	      FORCE_BITS(PTT,PORT_T_WRITE_MASK,stepper_array[state]);//step
	      current_step--;
	    }
	    TC5 = TCNT + currentStepSpeed;
	 }
   
}

