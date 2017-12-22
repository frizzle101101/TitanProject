#include "derivative.h"      /* derivative-specific definitions */
#include "myUtils.h"

//ATD Conversion 
#define ATD_CL2 0xE0
#define ATD_CL3 0x0A
#define ATD_CL4 0xA1
#define ATD_CL5 0x22

#define OC_OFF                0x00     //Turn bits off 
#define OC_TOGGLE             0x01     //toggle
#define OC_GO_LO              0x02     //Low State
#define OC_GO_HI              0x03     //High State
#define TIMING                20000
#define SCF ATDSTAT0_SCF_MASK


unsigned int getAnalog(void);
void initADC(void);