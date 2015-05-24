/*******************************************************************************
   vga_block.h: Include file
   Copyright (C) 2009 Brno University of Technology,
                      Faculty of Information Technology
   Author(s): Zdenek Vasicek <vasicek AT fit.vutbr.cz>
              Karel Slany    <slany AT fit.vutbr.cz>

   LICENSE TERMS

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   3. All advertising materials mentioning features or use of this software
      or firmware must display the following acknowledgement: 

        This product includes software developed by the University of
        Technology, Faculty of Information Technology, Brno and its 
        contributors. 

   4. Neither the name of the Company nor the names of its contributors
      may be used to endorse or promote products derived from this
      software without specific prior written permission.
 
   This software is provided ``as is'', and any express or implied
   warranties, including, but not limited to, the implied warranties of
   merchantability and fitness for a particular purpose are disclaimed.
   In no event shall the company or contributors be liable for any
   direct, indirect, incidental, special, exemplary, or consequential
   damages (including, but not limited to, procurement of substitute
   goods or services; loss of use, data, or profits; or business
   interruption) however caused and on any theory of liability, whether
   in contract, strict liability, or tort (including negligence or
   otherwise) arising in any way out of the use of this software, even
   if advised of the possibility of such damage.
   
   $Id$

*******************************************************************************/

#ifndef _VGA_H_
#define _VGA_H_


// Bazova Adresa komponenty DISPLAY
#define BASE_ADDR_VRAM 0x8000

//#define SPI_FPGA_ENABLE_WRITE 0x01
#define SPI_FPGA_ENABLE_READ  0x02

//sloupcu
#define VGA_COLUMNS 80
//radku
#define VGA_ROWS    60
//pocet znaku na radek
#define VGA_STRIDE  128

void VGA_Clear(void);
void VGA_GotoXY(unsigned char x, unsigned char y);
void VGA_SetPixel(char val);
void VGA_SetPixelXY(unsigned char x, unsigned char y, char val);

void VRAM_Init(void);
#endif  // _VGA_H_

