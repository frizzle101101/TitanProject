#include "myUtils.h"

int pR = 0;
long temp;
int map(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
   temp = ((long)value - (long)fromLow) * ((long)toHigh - (long)toLow) / ((long)fromHigh - (long)fromLow) + (long)toLow;
  return ((int)temp);
}
long lmap(long value, long fromLow, long fromHigh, long toLow, long toHigh) {
    return ((value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow);
}
int power(int base, int powerRaised)
{
    pR = powerRaised;
    if (pR > 0)
        return (base*power(base, pR-1));
    else
        return 1;
}