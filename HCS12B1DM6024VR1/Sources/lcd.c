#include "lcd.h"

void initLCD(void) {
    INIT_LCD_DDR_OUT;
  // run through sync sequence from datasheet to start 4-bit interface    
    LCD_E_HI;
    LCD_BUS( 0x03 );      // wake up display & sync
    LCD_E_LO;
    
    delay_ms( 5 );

    LCD_E_HI;
    LCD_BUS( 0x03 );      // wake up display & sync
    LCD_E_LO;

    delay_ms( 1 );   
    
    LCD_E_HI;
    LCD_BUS( 0x03 );      // wake up display & sync
    LCD_E_LO;
    
    LCD_E_HI;
    LCD_BUS( 0x02 );      // wake up display & sync - go to 4-bit mode
    LCD_E_LO;

	  delay_ms( 2 );
// now that we're sync'd and in 4-bit mode, issue commands to configure the display
    
    LCDcmd( LCD_CMD_FUNCTION | LCD_FUNCTION_4BIT | LCD_FUNCTION_2LINES | LCD_FUNCTION_5X8FONT);
    LCDcmd( LCD_CMD_DISPLAY | LCD_DISPLAY_OFF );
    LCDclear();
}
void LCDcmd(unsigned char c) 
{
  LCD_E_LO;
  LCD_IR;
  LCD_RW_WRITE;
  LCD_E_HI;
  LCD_BUS(HI_NYBBLE(c));
  LCD_E_LO;
  LCD_E_HI;
  LCD_BUS(LO_NYBBLE(c));
  LCD_E_LO;
  delay_ms(2);
}
void LCDdata(unsigned char c) 
{
  LCD_E_LO;
  LCD_DR;
  LCD_RW_WRITE;
  LCD_E_HI;
  LCD_BUS(HI_NYBBLE(c));
  LCD_E_LO;
  LCD_E_HI;
  LCD_BUS(LO_NYBBLE(c));
  LCD_E_LO;
  delay_ms(2);
}
void LCDputc(char c) 
{
  switch(c) 
  {
    case 13:
      LCDcmd(LCD_HOME_MASK);
      break;
    case 10:
      LCDcmd(LCD_NEW_LINE);
      break;
    default:
      LCDdata(c);
  }
}
void LCDputs(char *s) 
{
  int count = 0;
  
  while(*s) 
  {
    if(count == 16)
    LCDcmd(LCD_NEW_LINE);
    if(count == 32)
    break;
    LCDputc(*s);
    s++;
    count++;
  }
}
void LCDprintf(char *format, ...) 
{
  va_list myArgs;
  char Buffer[MAX_CHARACTERS];
  va_start(myArgs, format);
  (void)vsprintf(Buffer,format,myArgs);
  va_end(myArgs);
  LCDputs(Buffer);  
}
void LCDclear(void) 
{
  LCDcmd(LCD_CLEAR_MASK);
  delay_ms(16);
  LCDcmd( LCD_CMD_ENTRY | LCD_ENTRY_MOVE_CURSOR | LCD_ENTRY_INC );
  LCDcmd( LCD_CMD_DISPLAY | LCD_DISPLAY_ON | LCD_DISPLAY_NOCURSOR | LCD_DISPLAY_NOBLINK );
}
void BusyFlagWait (void) 
{
  
}