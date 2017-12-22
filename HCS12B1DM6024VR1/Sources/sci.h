#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "myUtils.h"


#define BaudRate 9600
#define ClockRate 8000000
#define Divider (ClockRate/16/BaudRate)
#define COMMAND_SIZE 6
#define CMD_BUFFER_SIZE 50

typedef struct {
  char type;
  int val;
} CmdInput;

void initSCI(void);
void putcSCI( char cx );
void putsSCI( char *str );
char getcSCI( void );
char SCIdequeue(void);
int getCmdLength(char *p);
CmdInput updateCmdInput(void);





   