/* 
 * File:   main.h
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

#ifndef MAIN_H
#define	MAIN_H

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <htc.h>        /* HiTech General Include File */

#include "RFM73.h"
#include "pic.h"

typedef struct {
    unsigned char reach_1s : 1;
    unsigned char reach_5hz : 1;
} FlagType;

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

void init_mcu(void);
void init_port(void);
void timer2_init(void);

void power_on_delay(void);
void delay_5s(void);
void delay_1s(void);
void delay_200ms(void);
void delay_50ms(void);
void delay_5ms(void);
void delay_1ms(void);
void delay_20us(void);
void sub_program_1hz(char status, int point);

void Send_Packet(UINT8 type, UINT8* pbuf, UINT8 len);
void Send_NACK_Packet(void);
void Receive_Packet(void);
void SPI_Bank1_Write_Reg(UINT8 reg, UINT8 *pBuf);
void SPI_Bank1_Read_Reg(UINT8 reg, UINT8 *pBuf);
void Carrier_Test(UINT8 b_enable); //carrier test

// JM Added 2015/07/29
void Profile_Check(void);
void Key_Pad_Flash(char select);
//

extern void RFM73_Initialize(void);
extern void SwitchToTxMode(void);
extern void SwitchToRxMode(void);

extern const UINT8 RX0_Address[];
extern const unsigned long Bank1_Reg0_13[];

/******************************************************************************/
/* Below are my Wireless Nursecall specific functions                         */
/******************************************************************************/

//const UINT8 tx_buf[17] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x78};
/// Jayson Testing
const UINT8 tx_buf_CALL1[17] = {0x30, 0x31, 0x31, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0xe8};
const UINT8 tx_buf_CANCEL1[17] = {0x30, 0x31, 0x30, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0xe7};
const UINT8 tx_buf_CALL2[17] = {0x30, 0x32, 0x31, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0xe9};
const UINT8 tx_buf_CANCEL2[17] = {0x30, 0x32, 0x30, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0xe8};
const UINT8 tx_buf_CALL3[17] = {0x30, 0x33, 0x31, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0xea};
const UINT8 tx_buf_CANCEL3[17] = {0x30, 0x33, 0x30, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0xe9};


UINT8 rx_buf[MAX_PACKET_LEN];

unsigned char rx_Call_Point = '.';
unsigned char rx_Call_Status = '.';

unsigned char call_FLAG = '1';
unsigned char cancel_FLAG = '1';

const char send_CALL = '1';
const char send_CANCEL = '0';

#endif	/* MAIN_H */

