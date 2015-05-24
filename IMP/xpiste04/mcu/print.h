//
//  print.h
//  IMPVoltmetr
//
//  Created by Radek Pistelak, xpiste04
//  Original
//  -------------------------------------------------
//
//

#ifndef __IMPVoltmetr__print__
#define __IMPVoltmetr__print__

#include <stdint.h>
#include "queue.h"
#include "print.h"
#include "vga_block.h"

#define SetPixel1(X,Y)	VGA_SetPixelXY(X,Y,50);
#define SetPixel0(X,Y)  VGA_SetPixelXY(X,Y,0);

static inline void reDraw(uint16_t pos)
{
        uint8_t it = 0; // iterator 
        
	queueAddPoint(pos);
       
	if (!queueIsFull) {
		for (it = 1; it < Q.end ; it++) {
        	      	SetPixel0(it, 50 - queueReadPoint(it-1));
                	SetPixel1(it, 50 - queueReadPoint(it));
		}
	} else {
		for (it = 1; it < LEN - 1; it++) {
              		SetPixel0(it, 50 - queueReadPoint(it - 1));
        	        SetPixel1(it, 50 - queueReadPoint(it));
		}	
	}
}

#endif /* defined(__IMPvoltmetr__print__) */
