#include "derivative.h"      /* derivative-specific definitions */
#include "myUtils.h"


#define EIGHT_BIT_MODE 0x00
#define PWM_RESET 0x00
#define PWM_ECLK 0x00
#define PRESCALE_0 0x03
#define SA_DIVIDE_2 0x01
#define PERIOD_22KHZ_LEFT_ALINGED 182U
#define PWM_DTY_10 18U
#define PWM_DTY_60 108U
#define PWM_DTY_75 135U
#define PERIOD_22KHZ_CENTER_ALINGED 182U
#define SB_DIVIDE_128 0x60
#define PERIOD_300HZ_LEFT_ALINGED 208U
#define RTI_PERIOD 0x49 // Sets RTI period to 5.12mS (Look up tabel in slides)

#define MAX_DRIVE_VALUE 250
#define MAX_STEER_DRIVE_VALUE 126//0.7*MAX_DRIVE_VALUE
#define MIN_DRIVE_VALUE 20

#define MAX_PEROID 275
#define MIN_PEROID 600
#define MAX_SPEED_ERROR 10000
#define MIN_SPEED_ERROR -10000
#define P_GAIN 300
#define I_GAIN 250
#define GAIN_DIVISOR 10000
#define FEEDBACK_SCALE_FACTOR 65536

void DC_motor_init(void);
void DC_motor_drive(int lr, int fb);
void left_f_right_f(void);
void left_f_right_r(void);
void left_r_right_f(void);
void left_r_right_r(void);