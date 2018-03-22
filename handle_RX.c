/* 
* File:   handle_RX.c
* Author: Jayson Marilli
* Dependencies:    
* Processor:       PIC16F1827
* Compiler:        HI-TECH-PICC v9.81 or higher
* Company:         Micro Nursecall Systems
* 
* Comments:        
* 
* Created on 30 July 2015, 10:24 PM
* /*********************************************************
                 ---------------
     RED----    |RA2         RA1|    ----BUTTON CALL
   GREEN----    |RA3         RA0|    ----BUTTON CANCEL
  YELLOW----    |RA4         RA7|    ----KEY PAD LED
        ----    |RA5         RA6|    ----MASTER/SLAVE SELECT (High for master. Low for slave)
        ----    |VSS         VDD|    ----
      CE----    |RB0         RB7|    ----
    MISO----    |RB1         RB6|    ----
    MOSI----    |RB2         RB5|    ----IRQ
     CSN----    |RB3         RB4|    ----SCK
                 ---------------
                   PIC16F1827
********** ***************************************************
 */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <htc.h>        /* HiTech General Include File */

#include "Hardware_Profile.h"
#include "main.h"


/////////// TO DO ??????







