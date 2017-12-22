#include "myUtils.h"
#include "derivative.h"

// Defines for SPI Control Register (SPICR1)
#define LSB_FIRST 1U
#define MSB_FIRST 0U
#define SLAVE 0U
#define MASTER 16U
#define SPI_ON 64U
#define SCK_LOW_SAMPLE_RISE 0U
#define SCK_LOW_SAMPLE_FALL 4U
#define SCK_HIGH_SAMPLE_RISE 12U
#define SCK_HIGH_SAMPLE_FALL 8U
#define INTERRUPTS_ON 128U
#define INTERRUPT_OFF 0U
#define SPI_TRANS_INTERRUPT_ON 32U
#define SPI_TRANS_INTERRUPT_OFF 0U
#define SSO_ON 2U

//Defines for SPI Control Register (SPICR2)
#define OUTPUT_BUFFER_ON 8U
#define OUTPUT_BUFFER_OFF 0U
#define SPI_NORMAL 0U
#define SPI_WAITMODE 2U
#define MODE_FAULT_ON 16U
#define MODE_FAULT_OFF 0U  
#define SERIAL_PIN_CONTROL_ON 1U


//Defines for SPI Ports & DDR
#define SPI_PORT PTM
#define SPI_DDR  DDRM
#define MISO_DDR DDRM_DDRM2_MASK
#define SS_DDR   DDRM_DDRM3_MASK
#define MOSI_DDR DDRM_DDRM4_MASK
#define MISO     PTM_PTM2_MASK
#define SS       PTM_PTM3_MASK
#define MOSI     PTM_PTM4_MASK


#define WRITE_SS_DDR  SET_BITS(SPI_DDR,SS_DDR)
#define SET_SS  SET_BITS(PTM,SS)
#define CLR_SS  CLR_BITS(PTM,SS)


//Defines for DAC

//Control bits
#define LOAD_IREG_A  0x01
#define LOAD_IREG_B  0x02
#define LOAD_DREG_AB 0x08
#define LOAD_DREG_A  0x09
#define LOAD_DREG_B  0x0A
#define STANDBY      0x0C
#define VOLTAGE_REF  0x0D
#define SHUTDOWN     0x0E
#define DATA         0x0F



//Data
#define VREF_1_2V 0x000  
#define VREF_1_9V 0x040  
#define VREF_2_4V 0x080  
#define VREF_3_8V 0x0C0  

//ControlBits
#define SUB 0x00

void InitSPI(unsigned char clockMode, unsigned char bitMode, unsigned char spiMode,unsigned char interruptStatus,unsigned char spiPower, unsigned char biDirectionMode, unsigned char ModeFault, unsigned char baudR);
void putcSPI(unsigned char cx);
void writeDacSpi(unsigned char controlBits, unsigned char dataBits, unsigned char subBits);