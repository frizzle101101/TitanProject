#include "derivative.h"      /* derivative-specific definitions */
#include "myUtils.h"

//stepper look up table
#define MOTOR_A                  0b10000000
#define MOTOR_AC                 0b10100000
#define MOTOR_C                  0b00100000
#define MOTOR_BC                 0b01100000
#define MOTOR_B                  0b01000000
#define MOTOR_BD                 0b01010000     
#define MOTOR_D                  0b00010000
#define MOTOR_AD                 0b10010000

#define REVERSE                  (-1)
#define STEPPER_MASK             0x07
#define PORT_T_WRITE_MASK        0xF0
#define RTI_PERIOD               0x49                                                   // Sets RTI period to 5.12mS or 200Hz (Look up tabel in slides)
#define LIMIT_SWITCH_MASK_L      0x40
#define LIMIT_SWITCH_MASK_R      0x80
#define CALIB_STEP_SPEED         5000
#define STEP_SPEED_MAX           2000
#define STEP_SPEED_MIN           4000

void stepper_init(void);
void Drive_Camera_Stepper(int v);
