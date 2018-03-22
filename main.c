/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
#include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
#include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

//#include "system.h"        /* System funct/params, like osc/peripheral config */
//#include "user.h"          /* User funct/params, such as InitApp */
#include "RFM73.h"
#include "Hardware_Profile.h"
#include "main.h"

#define _XTAL_FREQ 8000000

// PIC16F1827 Configuration Bit Settings
__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_OFF &
        CPD_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
__CONFIG(WRT_OFF & PLLEN_OFF & STVREN_OFF & LVP_OFF);

UINT8 count_50hz;

FlagType Flag;  // Defined in "main.h"

UINT8 test_data;

/******************************************************************************/
/* Main Program                                                               */

/******************************************************************************/
void main(void) {
    unsigned char i, c, chksum;
    unsigned int callPoint;
    
    power_on_delay();
    init_mcu();
    count_50hz = 0;
    Flag.reach_1s = 0;
    
    Profile_Check();

    // Below could be moved to Hardware_Profile.h
    #ifdef BED_HEAD  // Hard coded define for the trade show to seperate call points
       #define BED_HEAD_1
    //   #define BED_HEAD_2
    //   #define BED_HEAD_3
    #endif
    #ifdef OVER_DOOR  // Hard coded define for the trade show to seperate call points
    //   #define OVER_DOOR_1
        #define OVER_DOOR_2
    #endif
    #ifdef BED_HEAD_1
       callPoint = 1;
    #endif
    #ifdef BED_HEAD_2
       callPoint = 2;
    #endif
    #ifdef BED_HEAD_3
      callPoint = 3;
    #endif

    INTCON = 0xc0; // enable interrupt # Bit5 - TMR0IE = 0 
    RFM73_Initialize();

 //   LED_YELLOW_IO = 1; // Commented ii for BED HEAD ONLY
    
    while (1) {
        NOP();
        
        Receive_Packet();

        #ifdef BED_HEAD
        NOP();
        if (CALL_IO == 0 && call_FLAG == '1') {
            sub_program_1hz(send_CALL, callPoint);
            call_FLAG = '0';
            cancel_FLAG = '1';
            KEY_PAD_LED_IO = 1;
        }
        if (CANCEL_IO == 0 && cancel_FLAG == '1') {
            sub_program_1hz(send_CANCEL, callPoint);
            call_FLAG = '1';
            cancel_FLAG = '0';
            KEY_PAD_LED_IO = 0;
        }
        #endif
        
        #ifdef OVER_DOOR
           NOP();
           #ifdef OVER_DOOR_1
           if (rx_Call_Point == '1' && rx_Call_Status == '1'){
               LED_RED_IO = 1;
               LED_GREEN_IO = 1;
               LED_YELLOW_IO = 1;
               KEY_PAD_LED_IO = 1;
           }
           if (rx_Call_Point == '1' && rx_Call_Status == '0'){
               LED_RED_IO = 0;
               LED_GREEN_IO = 0;
               LED_YELLOW_IO = 0;
               KEY_PAD_LED_IO = 0;
           }
           if (rx_Call_Point == '2' && rx_Call_Status == '1'){
               LED_RED_IO = 1;
               LED_GREEN_IO = 1;
               LED_YELLOW_IO = 1;
               KEY_PAD_LED_IO = 1;
           }
           if (rx_Call_Point == '2' && rx_Call_Status == '0'){
               LED_RED_IO = 0;
               LED_GREEN_IO = 0;
               LED_YELLOW_IO = 0;
               KEY_PAD_LED_IO = 0;
           }
           #endif
           #ifdef OVER_DOOR_2
           if (rx_Call_Point == '3' && rx_Call_Status == '1'){
               LED_RED_IO = 1;
               LED_GREEN_IO = 1;
               LED_YELLOW_IO = 1;
               KEY_PAD_LED_IO = 1;
           }
           if (rx_Call_Point == '3' && rx_Call_Status == '0'){
               LED_RED_IO = 0;
               LED_GREEN_IO = 0;
               LED_YELLOW_IO = 0;
               KEY_PAD_LED_IO = 0;
           }
           #endif
        #endif
        
        #ifdef DUTY_STATION
           NOP();
           if (rx_Call_Point == '1' && rx_Call_Status == '1'){
               LED_RED_IO = 1;
           }
           if (rx_Call_Point == '1' && rx_Call_Status == '0'){
               LED_RED_IO = 0;
           }
           if (rx_Call_Point == '2' && rx_Call_Status == '1'){
               LED_GREEN_IO = 1;
           }
           if (rx_Call_Point == '2' && rx_Call_Status == '0'){
               LED_GREEN_IO = 0;
           }
           if (rx_Call_Point == '3' && rx_Call_Status == '1'){
               LED_YELLOW_IO = 1;
           }
           if (rx_Call_Point == '3' && rx_Call_Status == '0'){
               LED_YELLOW_IO = 0;
           }
           
        #endif

//        if (rx_Call_Point == '+'){
//            KEY_PAD_LED_IO = 1;
//        }
//        if (rx_Call_Point == '#'){
//            KEY_PAD_LED_IO = 0;
//        }

//        if (CALL_IO == 0 && call_FLAG == '1') {
//            sub_program_1hz(send_CALL);
//            call_FLAG = '0';
//            cancel_FLAG = '1';
//        }
//        if (CANCEL_IO == 0 && cancel_FLAG == '1') {
//            sub_program_1hz(send_CANCEL);
//            call_FLAG = '1';
//            cancel_FLAG = '0';
//        }
    }

}
/*********************************************************
Function: init_mcu();

Description:
        initialize mcu.
 *********************************************************/
void init_mcu(void) {
    init_port();
//    timer0_init();
//    timer4_init();
    timer2_init();
}

/*********************************************************
Function: init_port();

Description:
        initialize port.
 *********************************************************/
void init_port(void) {
    OSCCON = 0x7f; // 8 MHz

    ANSELA = 0; // define port as digital
    ANSELB = 0; // define port as digital

    //TRISA = 0x43; // TRIS: 0 = output, 1=input. ** NOW SETUP IN HARDWARE_PROFILE.H
    TRISB = 0x22; // TRIS: 0 = output, 1=input

    WPUA = 0; //Weak Pull up on A disabled
    WPUB = 0; //Weak Pull up on B disabled

    // Define calls for the setup of PORTB
    CE_OUT();
    CSN_OUT();
    SCK_OUT();
    MISO_IN();
    MOSI_OUT();
    IRQ_IN();

    // Define calls for the setup of PORTA
    CANCEL_IN();
    CALL_IN();
    LED_RED_OUT();
    LED_GREEN_OUT();
    LED_YELLOW_OUT();
    NOT_USED_OUT();
    PROFILE_SELECT_IN();
    KEY_PAD_LED_OUT();   

    CE = 0;
    CSN = 1;
    SCK = 0;
    MOSI = 0;

    // Setting LEDs to a stable state (OFF) on startup
    LED_RED_IO = 0;
    LED_GREEN_IO = 0;
    LED_YELLOW_IO = 0;
    KEY_PAD_LED_IO = 0;
}

/*********************************************************
Function: timer0_init();

Description:
        initialize timer for Key pad LED Flashing.
 *********************************************************/
//void timer0_init(void) {
//    OPTION_REG = 0b11010011;
//    /*             1-------     Disables weak pull ups
//     *             -1------     Interrupt on rising edge of INT pin
//     *             --0-----     Internal instruction cycle clock (Fosc/4)
//     *             ---1----     increment on high to low transition on T0CKI pin
//     *             ----0---     Prescaler is assigned to timer0
//     *             -----011     Sets timer0 prescaler to 1:16
//     */
//    TMR0IF = 0; // Clear TMR0 flag
//    TMR0IE = 0; // Disable timer0 overflow interrupt (for now)
//
////    PEIE = 1; // 1 = enables all active peripheral interrupts
////    GIE = 1; //Global interrupt enable
//}

/*********************************************************
Function: timer2_init();

Description:
        initialize timer.
 *********************************************************/
void timer2_init(void) {
    T2CON = 0x7e; // timer2 on and 16 pre, postscale
    PR2 = 156; // 50hZ, 4m/4/16/16/50
    TMR2IE = 1;
}

/*********************************************************
Function: timer4_init();

Description:
        initialize timer.
 *********************************************************/
//void timer4_init(void) {
//    T4CON = 0x7a; // timer2 off for now and 16 pre, postscale
//    PR4 = 156; // 50hZ, 4m/4/16/16/50
//    TMR4IE = 0; // Disable timer4 overflow interrupt (for now)
//}

/*********************************************************
Function:  interrupt ISR_timer()

Description:

 *********************************************************/
void interrupt ISR_timer(void) {
    // Timer 1 stuff below
//    if (TMR0IF) {
//        TMR0IF = 0; // Clearing the timer0 flag
//        KEY_PAD_LED_IO = ~KEY_PAD_LED_IO;
//        //delay_200ms();
//    }
    
    // Timer 2 stuff below
    if (TMR2IF) {   //if (TMR2IF) {

        count_50hz++;
        if (count_50hz == 50) // REACH 1S
        {
            count_50hz = 0;
            Flag.reach_1s = 1;

        }
        else if (count_50hz == 5) {
            Flag.reach_5hz = 1;
        }

        TMR2IF = 0;
    }
    
////  Timer 4 stuff below
//    if (TMR4IF) {
//        TMR4IF = 0; // Clearing the timer0 flag
//        KEY_PAD_LED_IO = ~KEY_PAD_LED_IO;
//    }
}

/*********************************************************
Function:      power_on_delay()

Description:

 *********************************************************/
void power_on_delay(void) {
    unsigned int i;
    for (i = 0; i < 1000; i++) {
        delay_1ms();
    }
}

void delay_5s(void) {
    unsigned char j;
    for (j = 0; j < 8; j++) {
        delay_1s();
    }
}

/********************************************************

 *********************************************************/
void delay_1s(void) {
    unsigned char j;
    for (j = 0; j < 6; j++) {
        delay_200ms();
    }
    delay_50ms();
    delay_50ms();
}

/********************************************************

 *********************************************************/
void delay_200ms(void) {
    unsigned char j;
    for (j = 0; j < 40; j++) {
        delay_5ms();
    }
}

/*********************************************************
Function: delay_50ms();

Description:

 *********************************************************/
void delay_50ms(void) {
    unsigned char j;
    for (j = 0; j < 10; j++) {
        delay_5ms();
    }
}

/*********************************************************
Function: delay_5ms();

Description:

 *********************************************************/
void delay_5ms(void) {
    int i;
    for (i = 0; i < 650; i++) // 85*5
    {
        ;
    }
}

/*********************************************************
Function: delay_1ms();

Description:

 *********************************************************/
void delay_1ms(void) {
    unsigned char i;
    for (i = 0; i < 130; i++) {
        ;
    }
}

/*********************************************************
Function: delay_20us();

Description:

 *********************************************************/
void delay_20us(void) {
    unsigned char i;
    for (i = 0; i < 3; i++) {
        ;
    }
}

/*********************************************************
Function:  sub_program_1hz()

Description:

 *********************************************************/
void sub_program_1hz(char status, int point) {
    UINT8 i;
    UINT8 temp_buf[32];

    if (Flag.reach_1s) {
        Flag.reach_1s = 0;

        if (status == '1') {
            for (i = 0; i < 17; i++) {
                //temp_buf[i] = tx_buf[i];
                if (point == 1){
                    temp_buf[i] = tx_buf_CALL1[i];
                }
                if (point == 2){
                    temp_buf[i] = tx_buf_CALL2[i];
                }
                if (point == 3){
                    temp_buf[i] = tx_buf_CALL3[i];
                }
            }
        }
        if (status == '0') {
            for (i = 0; i < 17; i++) {
                //temp_buf[i] = tx_buf[i];
                if (point == 1){
                    temp_buf[i] = tx_buf_CANCEL1[i];
                }
                if (point == 2){
                    temp_buf[i] = tx_buf_CANCEL2[i];
                }
                if (point == 3){
                    temp_buf[i] = tx_buf_CANCEL3[i];
                }
            }
        }
//        for (i = 0; i < 17; i++) {
//            //temp_buf[i] = tx_buf[i];
//            temp_buf[i] = tx_buf[i];
//        }

        Send_Packet(W_TX_PAYLOAD_NOACK_CMD, temp_buf, 17);
        SwitchToRxMode(); //switch to Rx mode
    }
}

/**************************************************
Function: Send_Packet
Description:
        fill FIFO to send a packet
Parameter:
        type: WR_TX_PLOAD or  W_TX_PAYLOAD_NOACK_CMD
        pbuf: a buffer pointer
        len: packet length
Return:
        None
 **************************************************/
void Send_Packet(UINT8 type, UINT8* pbuf, UINT8 len) {
    UINT8 fifo_sta;

    SwitchToTxMode(); //switch to tx mode

    fifo_sta = SPI_Read_Reg(FIFO_STATUS); // read register FIFO_STATUS's value
    if ((fifo_sta & FIFO_STATUS_TX_FULL) == 0)//if not full, send data (write buff)
    {
        LED_RED_IO = 1;

        SPI_Write_Buf(type, pbuf, len); // Writes data to buffer

        delay_50ms();
        LED_RED_IO = 0;
        delay_50ms();
    }
}

/**************************************************
Function: Receive_Packet
Description:
        read FIFO to read a packet
Parameter:
        None
Return:
        None
 **************************************************/
void Receive_Packet(void) {
    UINT8 len, i, sta, fifo_sta, value, chksum, aa;
    UINT8 rx_buf[MAX_PACKET_LEN];

    sta = SPI_Read_Reg(STATUS); // read register STATUS's value

    if ((STATUS_RX_DR & sta) == 0x40) // if receive data ready (RX_DR) interrupt
    {
        do {
            len = SPI_Read_Reg(R_RX_PL_WID_CMD); // read len

            if (len <= MAX_PACKET_LEN) {
                SPI_Read_Buf(RD_RX_PLOAD, rx_buf, len); // read receive payload from RX_FIFO buffer
            } else {
                SPI_Write_Reg(FLUSH_RX, 0); //flush Rx
            }

            fifo_sta = SPI_Read_Reg(FIFO_STATUS); // read register FIFO_STATUS's value

        } while ((fifo_sta & FIFO_STATUS_RX_EMPTY) == 0); //while not empty

        chksum = 0;
        for (i = 0; i < 16; i++) {
            chksum += rx_buf[i];
        }
        if (chksum == rx_buf[16] && rx_buf[0] == 0x30) {
//            LED_GREEN_IO = 1;
            delay_50ms();
            delay_50ms();
//            LED_GREEN_IO = 0;

            rx_Call_Point = rx_buf[1];
            rx_Call_Status = rx_buf[2];

            //Send_Packet(W_TX_PAYLOAD_NOACK_CMD,rx_buf,17);
            SwitchToRxMode(); //switch to RX mode
        }
    }
    SPI_Write_Reg(WRITE_REG | STATUS, sta); // clear RX_DR or TX_DS or MAX_RT interrupt flag
}

void Profile_Check(void) {
    if (PROFILE_SELECT_IO == 1) {
        NOP();
//#ifndef BED_HEAD
//#define BED_HEAD
//#endif
        Key_Pad_Flash('B');
    }
    if (PROFILE_SELECT_IO == 0) {
        NOP();
//#ifndef OVER_DOOR
//#define OVER_DOOR
//#endif
        Key_Pad_Flash('O');
    }
}

void Key_Pad_Flash(char select) {
    int a = 0;
    switch (select) {
        case 'B':
            while (a <= 25) {
                KEY_PAD_LED_IO = 1;
                delay_200ms();
                KEY_PAD_LED_IO = 0;
                delay_200ms();
                a++;
            }
            break;
        case 'O':
            KEY_PAD_LED_IO = 1;
            delay_5s();
            KEY_PAD_LED_IO = 0;
            break;
        case 'D':
            while (a <= 3) {
                KEY_PAD_LED_IO = 1;
                delay_1s();
                KEY_PAD_LED_IO = 0;
                delay_1s();
                a++;
            }
            break;
    }

}
