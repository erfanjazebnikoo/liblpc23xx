
/*
 * lpc23xx-vic.c
 */

#include "lpc23xx.h"

#ifdef OLIMEXLPCP2378REVA
    #include "olimexlpcp2378reva.h"
#elif LPC2368PSASGFE 
    #include "lpc2368-PSAS-GFE"
#else
    #error ***** No board configuration defined. ******
#endif

#include "lpc23xx-vic.h"
