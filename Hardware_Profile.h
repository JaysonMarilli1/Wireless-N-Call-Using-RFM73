/* 
 * File:   Hardware_Profile.h
 * Author: Jayson Marilli
 * Dependencies:    
 * Processor:       PIC16F1827
 * Compiler:        HI-TECH-PICC v9.81 or higher
 * Company:         Micro Nursecall Systems
 * 
 * Comments:        
 * 
 * Created on 29 July 2015, 12:01 PM
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

#ifndef HARDWARE_PROFILE_H
#define	HARDWARE_PROFILE_H

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <htc.h>        /* HiTech General Include File */




// Choose which hardware profile to compile for here.  See 
// the hardware profiles below for meaning of various options.
//#define BED_HEAD          // I still need to see if this would be the "SLAVE" point (more research)
#define OVER_DOOR         // I still need to see if this would be the "SLAVE / RELAY" point (more research)
//#define DUTY_STATION        // I still need to see if this would be the "MASTER" point (more research)
//
//#ifdef BED_HEAD
//
//#endif
//
//#ifdef OVER_DOOR
//
//#endif
//
//#ifdef DUTY_STATION
//   
//#endif

// Defines to setup TRISA input / output
#define CANCEL_IN()             TRISA0 = 1         
#define CALL_IN()               TRISA1 = 1 
#define LED_RED_OUT()           TRISA2 = 0 
#define LED_GREEN_OUT()         TRISA3 = 0 
#define LED_YELLOW_OUT()        TRISA4 = 0 
#define NOT_USED_OUT()          TRISA5 = 0 
#define PROFILE_SELECT_IN()     TRISA6 = 1 
#define KEY_PAD_LED_OUT()       TRISA7 = 0 

// I/O Pins   
#define CANCEL_IO           RA0
#define CALL_IO             RA1
#define LED_RED_IO          RA2
#define LED_GREEN_IO        RA3
#define LED_YELLOW_IO       RA4
#define NOT_USED_IO         RA5 
#define PROFILE_SELECT_IO  	RA6
#define KEY_PAD_LED_IO      RA7

#endif	/* HARDWARE_PROFILE_H */

