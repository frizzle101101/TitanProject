#define LOW(value) ((value) & 0xFF)
#define SET_BITS(reg,mask) ((reg) |= (mask))
#define CLR_BITS(reg,mask) ((reg) &= LOW(~mask))
#define FLIP_BITS(reg,mask) ((reg) ^= (mask))
#define FORCE_BITS(reg,mask,value) ((reg) = ((reg) & LOW(~mask)) | ((value) & (mask)))
#define ENABLE_PWM_CHNL(chnl) SETBITS(pwme,0x01<<(chnl))
#define DISABLE_PWM_CHNL(chnl) CLR_BITS(pwme,0x01<<(chnl))
#define FOREVER for(;;)
#define BIT_IS_CLR(val,mask) (!((val)&(mask)))
#define BIT_IS_SET(val,mask) ((val)&(mask))

int map(int value, int fromLow, int fromHigh, int toLow, int toHigh);
long lmap(long value, long fromLow, long fromHigh, long toLow, long toHigh);
int power(int base, int powerRaised);