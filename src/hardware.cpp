#include <hardware.h>

unsigned long chipID(void)
{
    unsigned long   id = 0;

    for (unsigned char i=0; i<17; i=i+8)
    {
	    id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	}
    return(id);
}