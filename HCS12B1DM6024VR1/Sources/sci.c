#include "sci.h"

static int in = 0;
static int cmdin = 0;
static int out = 0;
static int cmdout = 0;
static int bufFulFlg = 0;
static int begCmdFlg = 0;
static int endCmdFlg = 0;
static char *begCmd;
static int sciBufCount = 0;
static int cmdBufCount = 0;
static char sciBuf[COMMAND_SIZE*CMD_BUFFER_SIZE];
static char *cmdBuf[CMD_BUFFER_SIZE];


   
void initSCI(void) 
{
  
 SCISR1 = 0;          //reset status register to default flag states
 SCIBD = Divider;      //configures Baud rate at set value
 SCICR1 = 0;          //First sci control register     set to 0b80 for loop select enable
 SCICR2 = 0;          //Second controll register 
 SET_BITS(SCICR2, (SCICR2_RE_MASK | SCICR2_TE_MASK)); //enable transmit and recieve lines
 SET_BITS(SCICR2, SCISR1_RDRF_MASK);       //enables reciever full interrupts
 
}

void putcSCI( char cx ) {
  while (!(SCISR1 & SCISR1_TDRE_MASK));
  SCIDRL = cx;
}


void putsSCI( char *str ) {
  while( *str )
    putcSCI( *str++);
}


char getcSCI( void ) {
  while(!(SCISR1 & SCISR1_RDRF_MASK));
  return(SCIDRL);
}

char SCIdequeue(void) {
    char nextConsumed;
    
    while (sciBufCount == 0); // do nothing – nothing to get
		nextConsumed =  sciBuf[out];
		out = (out + 1) % COMMAND_SIZE;
		DisableInterrupts;
	  sciBufCount--;
    EnableInterrupts;
		/*  consume the item in nextConsumed */
    return(nextConsumed);
}
int getCmdLength(char *p){
    int len = 0;
    
    while(p[len] != '>'){
      len++; 
    }
    return(len);
}
CmdInput updateCmdInput(void){
    CmdInput nextConsumed;
    nextConsumed.type = '?';
    nextConsumed.val = 0;
    if(cmdBufCount>0){      
      int cmdlen = getCmdLength(cmdBuf[cmdout]);
      int i ;
      for(i = 1;i<cmdlen;i++) {
        if(i == 1){
          nextConsumed.type = *(cmdBuf[cmdout]+i);
        }else{
          int temp = (int)(*(cmdBuf[cmdout]+i));
          if(temp >= 48 && temp <= 57)
            nextConsumed.val += (temp - 48)*(power(10,(cmdlen-i-1)));
          else {
            nextConsumed.val = 100;
            putcSCI('$');  
          }
        }
      }
      cmdout = (cmdout + 1) % CMD_BUFFER_SIZE;
  		DisableInterrupts;
  	  sciBufCount -= cmdlen + 1;
  	  cmdBufCount--;
      EnableInterrupts;   
    }
    /*  consume the item in nextConsumed */
    return(nextConsumed);
}
int getCmdBufCount(void){
    return(cmdBufCount);  
}
void checkCmdBufFull(void) {
    if(bufFulFlg){
      ///if need for bufferfull action 
    }
}


// SCI interrupt handler
// Only caring about Receiver Full Interrupt Flag, to see if there's information that has been sent to us (bit 5)
// 
// SCI is interrupt #20
interrupt 20 void SCIhandler( void ){
     SCISR1=0;//reset flag register to default  
      if ((SCISR1 &= SCISR1_RDRF_MASK)>0)   //if the RDRF flag is set, Take RX info and save to ring buffer
      {                    // count this interupt -- if counted down to 0, toggle LEDs
          /*  produce an item and put in nextProduced  */
         if(sciBufCount < COMMAND_SIZE) { 
  	       sciBuf[in] = getcSCI();
           if(sciBuf[in] == '<') {
              //begining of command
              begCmdFlg = 1;
              begCmd = &sciBuf[in];
           } else if (sciBuf[in] == '>'){
              endCmdFlg = 1;
           }
           if(begCmdFlg == 1 && endCmdFlg == 1) {
              cmdBuf[cmdin] = begCmd;
              cmdin = (cmdin + 1) % CMD_BUFFER_SIZE;
              cmdBufCount++;
              begCmdFlg = 0;
              endCmdFlg = 0;
           }
          
  	       in = (in + 1) % (COMMAND_SIZE*CMD_BUFFER_SIZE);
  	       sciBufCount++;
  	       bufFulFlg = 0;
         } else {
           bufFulFlg = 1;
         }        
      }
      
               
      
} // end of SCIhandler()