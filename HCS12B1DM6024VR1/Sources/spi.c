#include  "spi.h"


void InitSPI(unsigned char clockMode, unsigned char bitMode, unsigned char spiMode,unsigned char interruptStatus,unsigned char spiPower, unsigned char biDirectionMode, unsigned char ModeFault, unsigned char baudR)
{	
	SPICR1 = spiPower|clockMode|bitMode|spiMode|interruptStatus;
	SPICR2 = biDirectionMode | ModeFault;
	SPIBR = baudR;
}
void putcSPI(unsigned char cx)
{
	unsigned char temp;
	while (!(SPISR & SPISR_SPTEF_MASK)); 	/* wait until write is alowed */
	SPIDR = cx;              			/* output the byte to the SPI */

	while (!(SPISR & SPISR_SPIF_MASK));  	/* wait until write is complete */
	temp = SPIDR;			/* clear the SPIF flag and take read data */
}
void writeDacSpi(unsigned char controlBits, unsigned char dataBits, unsigned char subBits)
{

  static unsigned char data1 = 0;
  static unsigned char data2 = 0;
  
  //Configure DDR to drive SS line manually
  SET_BITS(DDRS, DDRS_DDRS2_MASK);
  
  
  data1 = (controlBits << 4) | (dataBits >> 4);
	data2 = (dataBits << 4) | (subBits);
	CLR_BITS(PTS, PTS_PTS2_MASK);	
	putcSPI(data1);//Send the controlBits and high nibble of dataBits
	putcSPI(data2);// Send the lower nibble of dataBits and subBits
	SET_BITS(PTS, PTS_PTS2_MASK);;
	
}
