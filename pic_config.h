//IF NO primary oscillator is used, no need for FCMEN or PRIMARY_SW
// or IESO; also for debug purposes place INTRC at IO pin RA6
// THINK ABOUT PUT config setting! and transitions out of sleep that require
// accurate timing for UART

/////////////////////Fuses Start///////////////////////////////////////////////
#FUSES CCP2B3 //CCP2 input/output multiplexed with RB3
#FUSES CCP2D2 //CCP2 input/output multiplexed with RD2
#FUSES CCP3E0 //CCP3 input/output multiplexed with RE0
#FUSES HFOFST //High Frequency INTRC starts clocking CPU immediately 
#FUSES HSH //High speed Osc, high power 16MHz-25MHz
#FUSES IESO //Internal External Switch Over mode enabled 
#FUSES MCLR //Master Clear pin enabled 
#FUSES NOBROWNOUT //No brownout reset 
#FUSES NOCPB //No Boot Block code protection 
#FUSES NOCPD //No EE protection 
#FUSES NODEBUG //No Debug mode for ICD
#FUSES NOEBTR //Memory not protected from table reads 
#FUSES NOEBTRB //Boot block not protected from table reads 
#FUSES NOFCMEN //Fail-safe clock monitor disabled 
#FUSES NOLVP //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O 
#FUSES NOPBADEN //PORTB pins are configured as digital I/O on RESET
#FUSES NOPLLEN //4X HW PLL disabled, 4X PLL enabled in software  
#FUSES NOPROTECT //Code not protected from reading 
#FUSES NOWRT //Program memory not write protected 
#FUSES NOWRTB //Boot block not write protected 
#FUSES NOWRTC //configuration not registers write protected 
#FUSES NOWRTD //Data EEPROM not write protected 
#FUSES NOXINST //Extended set extension and Indexed Addressing mode disabled (Legacy mode) 
#FUSES PRIMARY_SW //Primary clock can be disabled in software 
#FUSES PUT //Power Up Timer
#FUSES STVREN //Stack full/underflow will cause reset
#FUSES TIMER3B5 //Timer3 Clock In is on pin B5 
#FUSES WDT32768 //Watch Dog Timer uses 1:32768 Postscale 
#FUSES WDT_SW //No Watch Dog Timer, enabled in Software 
//#FUSES WDT //Watch Dog Timer enabled. 
/////////////////////Fuses End/////////////////////////////////////////////////
                                          
/////////////////////Serial and I2C Timing Setup Start/////////////////////////
#use delay(clock=3,686,400) 
#use RS232 (uart1, baud=115200)       
#use delay(clock=250KHZ)
// setup i2c for use on 250khz main clock
#use i2c(master, I2C1, NOINIT, FORCE_HW, SLOW)
#use  fast_io(all)
/////////////////////Serial and I2C Timing Setup End///////////////////////////

/////////////////////Special Function Registers Start//////////////////////////

#byte    WREG     =  0x0FE8
#byte    STATUS   =  0x0FD8
#BIT     CARRY       =  STATUS.0

#byte    STKPTR   =  0x0FFC
#BIT     STKUNF      = STKPTR.6
#BIT     STKFUL      = STKPTR.7

#byte    RCON     =  0x0FD0
#BIT     IPEN        = RCON.7

#byte    INTCON   =  0x0FF2
#BIT     RBIF        = INTCON.0
#BIT     INT0IF      = INTCON.1
#BIT     TMR0IF      = INTCON.2                                                                                                          
#BIT     RBIE        = INTCON.3
#BIT     INT0IE      = INTCON.4
#BIT     TMR0IE      = INTCON.5                                                                                                   
#byte    INTCON2  =  0x0FF1
#BIT     RBPU        = INTCON2.7  
// RBPU lo to allow individual PORTB pullups
#byte    INTCON3  =  0x0FF0  
#BIT     INT1IF      =  INTCON3.0
#BIT     INT2IF      =  INTCON3.1

#byte    T0CON    =  0x0FD5
#BIT     TMR0ON      = T0CON.7

#byte    OSCCON   =  0x0FD3
#BIT     SCS0        = OSCCON.0
#BIT     SCS1        = OSCCON.1
#BIT     HFIOFS      = OSCCON.2
#BIT     OSTS        = OSCCON.3
#BIT     IDLEN       = OSCCON.7
#byte    OSCCON2  =  0x0FD2
#BIT     LFIOFS      = OSCCON2.0
#BIT     MFIOFS      = OSCCON2.1
#BIT     PRISD       = OSCCON2.2
#BIT     SOSCGO      = OSCCON2.3
#BIT     MFIOSEL     = OSCCON2.4
#BIT     SOSCRUN     = OSCCON2.6
#BIT     PLLRDY      = OSCCON2.7
#byte    OSCTUNE  =  0x0F9B
#BIT     PLLEN       = OSCTUNE.6
#BIT     INTSRC      = OSCTUNE.7

// Timer registers
#byte    TMR1H    =  0x0FCF
#byte    TMR1L    =  0x0FCE
#byte    T1CON    =  0x0FCD
#BIT     TMR1ON      = T1CON.0
#byte    T1GCON   =  0x0FCC
#BIT     T1GGO       = T1GCON.3

#byte    T2CON    =  0x0FBA
#BIT     TMR2ON      = T2CON.2
#BIT     T2CKPS0     = T2CON.0
#BIT     T2CKPS1     = T2CON.1
#byte    PR2      =  0x0FBB

#byte    TMR3H    =  0x0FB3
#byte    TMR3L    =  0x0FB2
#byte    T3CON    =  0x0FB1
#BIT     TMR3ON      =  T3CON.0
#byte    T3GCON   =  0x0FB4
#BIT     T3GGO       = T3GCON.3

#byte    T4CON    =  0x0F51
#BIT     TMR4ON      = T4CON.2
#byte    PR4      =  0x0F52


#byte    TMR5H    =  0x0F50
#byte    TMR5L    =  0x0F4F     
#byte    T5CON    =  0x0F4E
#BIT     TMR5ON      = T5CON.0
#byte    T5GCON   =  0x0F4D
#BIT     T5GGO       = T5GCON.3

#byte    T6CON    =  0x0F4A
#BIT     TMR6ON      = T6CON.2
#byte    PR6      =  0x0F4B

// ADC registers
#word    ADRES    =  0x0FC3
#byte    ADCON0   =  0x0FC2
#BIT     ADON        =  ADCON0.0
#BIT     ADGO        =  ADCON0.1
#byte    ADCON2   =  0x0FC0
#define  ADC_SETUP_DEF  0xFF   // right justified read, 20TAD acquisition time (insurance), FRC

// CCP registers      
#byte    CCPR1H   =  0x0FBF
#byte    CCPR1L   =  0x0FBE
#byte    CCP1CON  =  0x0FBD
#byte    CCPR2H   =  0x0F68
#byte    CCPR2L   =  0x0F67
#byte    CCP2CON  =  0x0F66
#byte    CCPR3H   =  0x0F5F
#byte    CCPR3L   =  0x0F5E
#byte    CCP3CON  =  0x0F5D
#byte    CCPR4H   =  0x0F59
#byte    CCPR4L   =  0x0F58
#byte    CCP4CON  =  0x0F57
#byte    CCPR5H   =  0x0F56
#byte    CCPR5L   =  0x0F55
#byte    CCP5CON  =  0x0F54

#byte    CCPTMRS0 =  0x0F49
#byte    CCPTMRS1 =  0x0F48
#byte    PSTR1CON =  0x0FB9
#byte    BAUDCON1 =  0x0FB8

#byte    SPBRGH1  =  0x0FB0
#byte    SPBRG1   =  0x0FAF
#byte    RCREG1   =  0x0FAE
#byte    TXREG1   =  0x0FAD
#byte    TXSTA1   =  0x0FAC
#BIT     TRMT1       = TXSTA1.1     // 1: empty xmit reg, 0: xmit in progress
#byte    RCSTA1   =  0x0FAB
#BIT     FERR1       = RCSTA1.2

#byte    PIR1     =  0x0F9E
#BIT     TMR1IF      = PIR1.0
#BIT     TMR2IF      = PIR1.1
#BIT     SSP1IF      = PIR1.3
#BIT     ADIF        = PIR1.6
#byte    PIR2     =  0x0FA1
#BIT     CCP2IF      = PIR2.0
#BIT     TMR3IF      = PIR2.1
#BIT     BCL1IF      = PIR2.3
#BIT     C1IF        = PIR2.6
#BIT     OSCFIF      = PIR2.7
#byte    PIR3     =  0x0FA4
#BIT     TMR5GIF     = PIR3.2
#BIT     TMR3GIF     = PIR3.1
#BIT     TMR1GIF     = PIR3.0
#byte    PIR4     =  0x0F7B
#BIT     CCP3IF      = PIR4.0
#BIT     CCP4IF      = PIR4.1
#BIT     CCP5IF      = PIR4.2
#byte    PIR5     =  0x0F7E
#BIT     TMR4IF      = PIR5.0
#BIT     TMR5IF      = PIR5.1
#BIT     TMR6IF      = PIR5.2

#byte    PIE1     =  0x0F9D
#BIT     TMR2IE      =  PIE1.1
#byte    PIE2     =  0x0FA0
#BIT     CCP2IE      =  PIE2.0
#byte    PIE3     =  0x0FA3
#byte    PIE4     =  0x0F7A
#byte    PIE5     =  0x0F7D

#byte    IPR1     = 0x0F9F
#byte    IPR2     = 0x0FA2
#byte    IPR3     = 0x0FA5
#byte    IPR4     = 0x0F7C
#byte    IPR5     = 0x0F7F

#byte    LATA     =  0x0F89
#byte    PORTA    =  0x0F80
#byte    TRISA    =  0x0F92
#byte    LATB     =  0x0F8A
#byte    PORTB    =  0x0F81
#byte    TRISB    =  0x0F93
#byte    LATC     =  0x0F8B
#byte    PORTC    =  0x0F82
#byte    TRISC    =  0x0F94
#byte    LATD     =  0x0F8C
#byte    PORTD    =  0x0F83
#byte    TRISD    =  0x0F95
#byte    LATE     =  0x0F8D
#byte    PORTE    =  0x0F84
#byte    TRISE    =  0x0F96

#byte    PSTR2CON =  0x0F63
#byte    IOCB     =  0x0F62
#byte    WPUB     =  0x0F61
#byte    PSTR3CON =  0x0F5A

#byte    CM1CON0  =  0x0F79
#bit     C1ON        = CM1CON0.7
#bit     C1POL       = CM1CON0.4
#byte    CM2CON0  =  0x0F78
#byte    CM2CON1  =  0x0F79

#byte    VREFCON0 =  0x0F42
#bit     FVREN    =  VREFCON0.7     //fixed voltage ref enable
#bit     FVRST    =  VREFCON0.6     //fixed voltage ref stable
#bit     FVRS1    =  VREFCON0.5     //fixed voltage select 1
#bit     FVRS0    =  VREFCON0.4     //fixed voltage select 0
#byte    VREFCON1 =  0x0F41
#byte    VREFCON2 =  0x0F40

#byte    PMD0     =  0x0F3F
#byte    PMD1     =  0x0F3E
#byte    PMD2     =  0x0F3D
#byte    CTMUICON =  0x0F43
#byte    CTMUCONL =  0x0F44
#byte    CTMUCONH =  0x0F45
#byte    SRCON1   =  0x0F46
#byte    SRCON0   =  0x0F47

#byte    SSP1CON2 =  0x0FC5
#BIT     SEN         = SSP1CON2.0
#BIT     PEN         = SSP1CON2.2
#byte    SSP1CON1 =  0x0FC6
#BIT     WCOL        = SSP1CON1.7
#byte    SSP1STAT =  0x0FC7
#BIT     I2CBUSY     = SSP1STAT.0

#define  RCON_INIT    0b00010011
// OR with RCON to SET BORn, PORn and RIn bits

/////////////////////Special Function Registers End////////////////////////////

/////////////////////I/0 Lines and Initialization Start////////////////////////
 
//  PORTA/B/C _ COLD and TRISA/B/C _ NORM are fine for lowest_pwr() I/O setup

////////////////////ADC channel choices
#define V_EXT_REF    0        // AN0 = RA0, 1.2V accurate reference
                              // use 1.2V accurate ref for measurements
#define V_VLVI       1        // AN1 = RA1
#define V_GEN        12       // AN12 = RB0 (INT0)
#define V_XDC        13       // Extra IO 1, now used for XDCR measurements
     
#define V_DAC        30       // DAC output may be read by ADC
#define V_FIXED      31       // 1.024/2.048/4.096V Fixed Reference (internal)


////////////////////PORT A/////////////////////

//    V_EXT_REF    PIN_A0      // external reference, analog AN0
//    V_VLVI       PIN_A1      // sense line, valve motor current, analog AN1
#define LED2n        PIN_A2      // RA2 output, active lo
#define MOTE_RX_CTSn PIN_A3      // RA3 input active lo
                                 //  mote asserts to tell micro it is
                                 //  clear to rcv data from micro;
                                 //  was _DUST_MTCTS
                                 //  set as output, init hi
#define GPS_PWRn     PIN_A4      // RA4 output, active lo
#define MOTE_TX_CTSn PIN_A5      // RA5 output, active lo, 
                                 //  assert to initiate rcv data from MOTE,
                                 //  byte level handshake;
                                 //  was _DUST_SPCTS                               
//      OSC2         PIN_A6      // xtal osc output connection
//      OSC1         PIN_A7      // xtal osc input connection

#define TRISA_NORM   0b10001011
                                 // A0 analog
                                 // A1 analog
                                 // A2 output
                                 // A3 input
                                 // A4 output
                                 // A5 output
                                 // A6 output
                                 // A7 input
#define PORTA_INIT   0b01110100
                                 // A0 analog (0)
                                 // A1 analog (0) 
                                 // A2 output active lo (1)
                                 // A3 input (0)
                                 // A4 output active lo (1)
                                 // A5 output active lo (1)
                                 // A6 output (1)
                                 // A7 input (0)
#define PORTA_IDLE   PORTA_INIT
#define PORTA_DEEP   PORTA_INIT


////////////////////PORT B////////////////////

#define V_GEN_INT    PIN_B0      // RB0 input, high for significant v_gen;
                                 //  generally analog, but may be input;
                                 //  INT0
                                 
#define GPS_TXRDY    PIN_B1      // RB1 input, high indicates GPS has data
                                 //  ready to transmit to micro
                                 
#define MOTE_TX_RTSn PIN_B2      // RB2 input, active lo
                                 //  mote asserts to tell micro it is
                                 //   ready to send data to micro;
                                 //  was _DUST_MTRTS; INT2   
                                 
#define LTC3105_CTRL PIN_B3      // output, active hi, P2A-pwm,
                                 //  shdn/enab input of 3105
                                 //  hi for max chg aggressiveness (lower RPM)

#BIT    LTC3105_CTRL_pinstate   = LATB.3


#define SW1n         PIN_B4      // usually an input, active lo
                                 //  note can be IOC
#define LED1n        PIN_B4      // may become an output, active lo

#define ADC_XDCR     PIN_B5      // wired as input (ADC) to analog XDCR signal
                                                                
#define SPARE2       PIN_B6      // consider as an input (PGC);
                                 //  wired to VLV_XDCR header    
#define SPARE3       PIN_B7      // consider as an input (PGD)

#define TRISB_NORM   0b11110111
                                 // B0 input (INT0)
                                 // B1 input (INT1)
                                 // B2 input (INT2)
                                 // B3 output
                                 // B4 input (usually, may become an output)
                                 // B5 input (spare)
                                 // B6 input (PGC)
                                 // B7 input (PGD)                                            
#define TRISB_LED    0b11100111
                                 // same as TRISB_NORM, but PIN_B4 is output
                                                                               
#define PORTB_INIT   0b00011000
                                 // B0 input (0)
                                 // B1 input (0)
                                 // B2 input (0)
                                 // B3 output (LTC3105_CTRL) hi=fully charge (1)
                                 // B4 usually an input, 
                                 //   may be output active lo (1)
                                 // B5 input unused (0)
                                 // B6 may be spare I/O (PGC) 
                                 // B7 may be spare I/O (PGD)
#define PORTB_IDLE   0b00010000  // only difference here is that LTC3105 is
                                 //  shut down             
#define PORTB_DEEP   PORTB_IDLE

                                  
#define PORTB_PULLUPS            0b00010010
#define PORTB_PULLUPS_WITH_XDCR  0b00110010            
                                 // only MOTE_TX_RTSn (pin B2) 
                                 // and SW1/LED1 are pulled up by defualt.
                                 // ADC_XDCR (pin B5) is turned on for a second to detect 
                                 //    if the XDCR is plugged in or not during XDCR_init()  
                                                                       
                               
                                                                                           
////////////////////PORT C////////////////////

//      SOSCO        PIN_C0      // xtal osc output connection
//      SOCSI        PIN_C1      // for debug, we can look at frequency here
#define BRK_CTRLn    PIN_C2      // P1A-pwm, to gates of FETs --
                                 //  lo for max braking force, hi for no brake

#BIT     BRK_pinstate   = LATC.2                              

//      SCL1         PIN_C3      // I2C clock
//      SDA1         PIN_C4      // I2C data
#define XDCR_PWR     PIN_C5      // output, active hi
//      MOTE_RX      PIN_C6      // UART1 TX output
//      MOTE_TX      PIN_C7      // UART1 RX input

#define TRISC_NORM   0b10011010
                                 // C0 output
                                 // C1 input
                                 // C2 output
                                 // C3/C4 inputs (I2C)
                                 // C5 output
                                 // C6 output (UART TX / MOTE_RX)
                                 // C7 input  (UART RX / MOTE_TX)

#define PORTC_INIT   0b01000101
                                 // C0 output (1)
                                 // C1 input (0)
                                 // C2 output (SPD_CTRL hi=no braking) (1)
                                 // C3/4 I2C, init as inputs (0)
                                 // C5 output active hi (0)
                                 // C6 output (1)
                                 // C7 input (0)
#define PORTC_IDLE   PORTC_INIT
#define PORTC_DEEP   PORTC_INIT

////////////////////PORT D////////////////////
#define GPS_ON       PIN_D0      // output, active hi
#define MOTE_TIMEn   PIN_D1      // output, active lo, gets time from mote
#define LCD_RESETn   PIN_D2      // output, active lo
#define AUX_PWR      PIN_D3      // output, active hi, turns on AUX pwr
#define MOTE_RESETn  PIN_D4      // output, active lo
#define MOTE_RX_RTSn PIN_D5      // output, active lo
                                 //  micro asserts to tell mote it is 
                                 //   ready to send data to mote
#define GPS_RX       PIN_D6      // UART2 TX output
#define GPS_TX       PIN_D7      // UART2 RX input

#define TRISD_NORM   0b10000000
                                 // D0 output
                                 // D1 output
                                 // D2 output (spare)
                                 // D3 output
                                 // D4 output
                                 // D5 output
                                 // D6 output
                                 // D7 input

#define PORTD_INIT   0b01110110
                                 // D0 output active hi (0)
                                 // D1 output active lo (1)
                                 // D2 output active lo (1)
                                 // D3 output active hi (0)
                                 // D4 output active lo (1)
                                 // D5 output active lo (1)
                                 // D6 output (1)
                                 // D7 input (0)
#define PORTD_IDLE   PORTD_INIT
#define PORTD_DEEP   PORTD_INIT

////////////////////PORT E////////////////////
#define VLV_CTRL1    PIN_E0      // P3A-pwm, to H-bridge re valve 
#define VLV_CTRL2    PIN_E1      // P3B-pwm, to H-bridge re valve
#define GEN_RPM      PIN_E2      // CCP5 input
//       MCLRn       PIN_E3

#define TRISE_NORM   0b00001100
                                 // E0 output
                                 // E1 output
                                 // E2 input
                                 // E3 master clear input

#define PORTE_INIT   0b00000000
                                 // E0 output (E0,E1 = 00 for coast state)
                                 // E1 output
                                 // E2 input (0)
                                 // E3 input (0)
#define PORTE_IDLE   PORTE_INIT  // TURNS off the power supply to 
                                 //  the valve motor
#define PORTE_DEEP   PORTE_INIT

/////////////////////I/0 Lines and Initialization End//////////////////////////


/////////////////////Timer 0 Start/////////////////////////////////////////////

// THESE VALUES ARE DEPENDENT UPON CRYSTAL FREQUENCY
// Modify with care
#define  T0_OSC    4340
// After calibration of 1MHz HFINTOSC timebase to 921.6KHz: 4/Fosc = 4340 nsec

// Assume 16 bit timer0 input is FOSC div 128 (timer 0 prescaler)
// Thus one count every 128*T0_OSC nanoseconds                     
#define T0_SETUP(MS) {  setup_timer_0(T0_DIV_128); \
                        set_timer0(65536 - (1000000*MS + 64*T0_OSC)/(128*T0_OSC)); }
// Note roundoff fix                    
// For use with the routine setup_T0_int(*)

//Timer 0 options
#define  T0_500US    65534
#define  T0_1MS      65536 - 1000/256
#define  T0_10MS     65536 - 10000/256
#define  T0_20MS     65536 - 20000/256
#define  T0_50MS     65536 - 50000/256
#define  T0_100MS    65536 - 100000/256
#define  T0_175MS    65536 - 175000/256
#define  T0_250MS    65536 - 250000/256
#define  T0_500MS    65536 - 500000/256
#define  T0_1S       65536 - 1000000/256
#define  T0_2S       65536 - 2000000/256
#define  T0_3S       65536 - 3000000/256
#define  T0_4S       65536 - 4000000/256
#define  T0_6S       65536 - 6000000/256
#define  T0_16S      65536 - 16000000/256
// the max is actually 16.7 seconds

/////////////////////Timer 0 End///////////////////////////////////////////////

/////////////////////Timer 2 Start/////////////////////////////////////////////

// For use with the routine setup_T2_int(*)
// The equivalent tick duration for timer2 is
//  adjusted depending on Fosc to be approx 256usec
// Timer2 counts up from 0 to match PR2 in order to set int flag
#define T2_1MS       3
#define T2_2MS       7
#define T2_3MS       11
#define T2_4MS       15
#define T2_5MS       19
#define T2_6MS       23
#define T2_7MS       27
#define T2_8MS       31
#define T2_9MS       35
#define T2_10MS      39
#define T2_15MS      59
#define T2_20MS      79
#define T2_25MS      99
#define T2_30MS      119
#define T2_40MS      159
#define T2_50MS      199
#define T2_60MS      239
#define T2_64MS      255
// cannot set to higher than 64 milliseconds 

////////////////////Timer 2 End////////////////////////////////////////////////

/////////////////////Timer 4 Start/////////////////////////////////////////////

// For use with the routine setup_T4_int(*)
// The equivalent tick duration for timer4 is
//  adjusted depending on Fosc to be approx 256usec
// Timer4 counts up from 0 to match PR4 in order to set int flag
#define T4_500US     1
#define T4_1MS       3
#define T4_2MS       7
#define T4_3MS       11
#define T4_4MS       15
#define T4_5MS       19
#define T4_6MS       23
#define T4_7MS       27
#define T4_8MS       31
#define T4_9MS       35
#define T4_10MS      39
#define T4_15MS      59
#define T4_20MS      79
#define T4_25MS      99
#define T4_30MS      119
#define T4_40MS      159
#define T4_50MS      199
#define T4_60MS      239
#define T4_64MS      255
// cannot set to higher than 64 milliseconds 

////////////////////Timer 4 End////////////////////////////////////////////////


////////////////////Battery-Voltage Start//////////////////////////////////////
// magic number based upon 4 samples averages with 1.2V reference!
// 12.20 format (4.8V)
#define VAL32_FVR_BAT      4.8 * 1048576 // 5033164.8 = 4CCCCD

////////////////////Battery-Voltage End////////////////////////////////////////



////////////////////Restart Causes Start///////////////////////////////////////
/*
// reset_cause is set to one of these values before any reset_cpu instruction

#define WARM_RESTART          0x00  // commanded: joins if needed
#define SPIN_RESTART          0x10  // generator wakeup from Deep sleep 
                                    //  or idle sleep
                                    //  like WARM_RESTART                                        
#define FRC_RJOIN_RESTART     0x20  // commanded to duplicate behavior of
                                    //  unexpected mote state change during
                                    //  RUN; like WARM_RESTART; mote rejoins
                                    //  and informs manager
#define RJOIN_RESTART         0x30  // unexpected mote state change during
                                    //  RUN; like WARM_RESTART; mote rejoins
                                    //  and informs manager  
#define COLD_RESTART          0x50  // commanded: force join
*/
////////////////////Restart Causes End/////////////////////////////////////////

////////////////////Extra LCD Start////////////////////////////////////////////
/*
// New Haven display part number NHD-C0216CIZ-FSW-FBW-3V3 special characters
#define  CROSS_CHAR        0xF7
// small cross
#define  BOX_CHAR          0xDB
// rectangle
#define  TOPLINE_CHAR      0xFF
// top line designates battery full, no charging needed
#define  NOTEQU_CHAR       0xFD
// equ with slash
#define  NOCHG_CHAR        0xE4
// c with slash, cent symbol, indicates charger circuit OFF
#define  MAXCHG_CHAR       'C'
#define  MINBRAKE_CHAR     'b'
#define  MAXBRAKE_CHAR     'B'
// PFHI character, indicates maximum braking force
#define  NOVLV_CHAR        'v'
// a PERIOD indicates valve PWM set for NO MOTION
#define  MAXVLV_CHAR       'V'
// ! indicates valve PWM set for MAX speed

#define  APPOS_CHAR        0x27
// appostrophe character: '


//#define  APPROX_CHAR       0x00
// custom approx equal character loaded at LCD_INIT

// custom characters loaded during LCD_INIT
#define  LEV1_CHAR         0x00
#define  LEV2_CHAR         0x01
#define  LEV3_CHAR         '_'
#define  LEV4_CHAR         0x02
#define  LEV5_CHAR         0x03
#define  LEV6_CHAR         '-'
#define  LEV7_CHAR         0x04
#define  LEV8_CHAR         0x05
#define  LEV9_CHAR         0x06
#define  LEV10_CHAR        0xFF

#define  BIGBOX_CHAR       0x07
#define  WAYLOW_CHAR       0xFB
// <<
#define  WAYHI_CHAR        0xFC
// >>
#define  CIRCLE_CHAR       0xF2
// small circle

*/
////////////////////Extra LCD End//////////////////////////////////////////////
////////////////////EEPROM Locations Start/////////////////////////////////////

// EEPROM LOCATIONS
#define EE_ADDCHG_DUR      0x0002
   // Configures add-charging time in seconds
   // 2 bytes, low byte at 0x0002
#define ADDCHG_DUR_DEF     4
   // 4 minutes?
#define EE_PRECHG_DUR      0x0004
   // Configures precharging time in seconds
   // 2 bytes, low byte at 0x0004
#define PRECHG_DUR_DEF     10
   // 1.5 minutes? 
#define CHGLOOPS_PER_MIN   110
   // determined experimentally, each loop is about 545 msec
   //  which is a bit longer than I would prefer (!), but oh well.
   //  JG 2015 Jun16
   
// 0x00A thru 0x00F        reserved
#define EE_NETW_ID         0x0010
   // 2 bytes, low byte at 0x0010; for now, network ID defaults to 0x04CD.
#define EE_PRENETW_ID      0x0012
   // 2 bytes, low byte at 0x0012; for now, network ID defaults to 0x04CD.


// 0013 reserved
#define EE_LOCATION     0x0014
   // 2 bytes, low byte at 0x0014; associates sprinkler with particular
   //  physical location on CP
#define EE_PRELOCATION  0x0016
   // 2 bytes, low byte at 0x0016, previous association of sprinkler with
   //  particular physical location on CP

   // 0x018 thru 0x01F     reserved

// IDLE behavior specified at 0x0020 thru 0x0033
#define EE_IDLE_OPT        0x0020
   // Configures IDLE behavior.  Applies if battery low during operational
   //  state
   //  0x00  send out Panic Message once when panic level is reached according
   //         to voltage-based algorithm
   //  0x01  send out Panic Message once when panic level is reached according
   //         to charge-based algorithm
#define MAX_IDLE_OPT       1
   // If manager tries to write any idle_opt value larger than this to EEPROM
   //  a NACK occurs; see above for idle option choices
#define LEN_IDLE_BLOCK     15
   // There are 15 bytes in the idle block

#define EE_IDLE_PROFILE    0x0021
   // Saved only upon entering IDLE state;
   //  profile number which was running or had last been run.
   // 0 for default profile, 1, 2, 3 for controller-specified profile choices
   //  other numbers, use 0
#define EE_IDLE_STEP       0x0022
   // Saved only upon entering IDLE state;
   //  step number of profile which was running or had last been run.
   //  Step 0xFF indicates that we were not within a profile
   //  Step 0x10 thru 0xFE invalid.
#define EE_IDLE_STIME      0x0023
   // 2 bytes, low byte at 0x0023, high byte at 0x0024
   // Saved only upon entering IDLE state;
   //  amount of time in seconds that rate was controlled according
   //  to EE_IDLE_STEP at time when IDLE state entered;
   //  set to 0x0000 if EE_IDLE_STEP=0xFF.
#define EE_IDLE_LOOP       0x0025
   // 2 bytes, low byte at 0x0025, high byte at 0x0026
   // Saved only upon entering IDLE state;
   //  for looping profiles, loop number at time when IDLE state entered;
   //  set to 0x0000 for non-looping profiles, set to
   //  0x0000 if EE_IDLE_STEP=0xFF. (looping beyond 0xFFFF is recorded as
   //  0xFFFF)
#define EE_IDLE_CHG        0x0027
   // 2 bytes, low byte at 0x0027, high byte at 0x0028
   // Configures used charge in millicoulombs for
   //  charge-based shutdown method (TBD); applies if EE_IDLE.1 is set
#define EE_IDLE_SHDN_DLY   0x0029
   // 2 bytes, low byte at 0x0029, high byte at 0x002A
   // Configures time in units of 67 secs (TMR0 ints during IDLE)
   //  between sensing of low battery and self-shutdown

////////////////////EEPROM Locations End///////////////////////////////////////
//***************************************************************
/*
#define DAY_SECS           24*3600

#define VAL32_FVR_BAT      4.8 * 1048576 // 5033164.8 = 4CCCCD
// magic number based upon 4 samples averages with 1.2V reference!
// 12.20 format (4.8V)

#define NETWORK_ID_DEF  0x04CD
// 04CD flipped bytes to match received network ID (JG!~?)
// this is the net_id for BlueBox#0
#define NETWORK_ID_MAX  NETWORK_ID_DEF + 250
// We could actually go from 0 (0x04CD) to 254, with 255 being illegal
//  as what would be read from cleared EEPROM

#define MAX_SP_NUM      499
// based on maximum number of motes that can be supported by Blue Box
#define MAX_SP_CP       200
// highest sprinkler number which uses the WARNER VALVE parameters
//  otherwise uses the BENCH VALVE SIMULATOR parameters

// state of charge levels

#define R2T             23040
   // 0x00005A00
   // Divide this number by RPM to get the target CCP2 capture value
   //  representative of speed.
   // 23040 RPM/(CCP2-prev_count) = 19200Hz * 6/5, where 19.2KHz is fosc/4,
   //  (1 rising edge occurs per capture event) and 6/5 RPM/Hz is the
   //  generator-dependent relationship between its output frequency and its
   //  RPM.
   // For example, 10RPM results in 2304 counts in the capture register
#define R2Tx4      R2T*4

#define VMOTION_TIMEOUT    60
// if valve travels longer than this time, a timeout error occurs

#define RPM_MIN            4
#define RPM_MAX            400
// when target_RPM = 0, we are in a braking mode

#define PERIOD_5RPM        4608

#define SHUT_DEL_DEF       30
// default shutdown delay to ensure that all motes get broadcast msg: 30 sec
// also used to ensure that manager acknowledges errors and first_msg in a 
//  timely manner (else there will be a repeat
#define REPEAT_LIMIT       4
// number of times that an unsolicited msg will repeat waiting for an
//  ACK from the manager.

#define STARTUP_DELAY      4
// delay in units of 1/4 sec at startup before opening valve for
//  first time

#define RSSI_MIN           -64
// -64 dBm; minimum rssi of an advertising message required
//   for that network to be a viable net to join
// BRIAN, this value may be TOO SENSITIVE
#define LISTEN_TIME        20
// number of seconds to dwell in promiscuous hi-power listen mode
//  in search of networks to join when mote has been cold-booted
// MUST be 1-27 -- 15s is occassionally too short a time
#define LISTEN_T1          65536 - LISTEN_TIME*2400
// 76.8KHz clock (19.2KHz counting up at timer1 div8....


#define TIMER2_SETUP_XTAL        T2_DIV_BY_4, 199, 8
// for 3.6864MHz xtal, this is the value used in setup_timer_2 
// to achieve 1152Hz PWM frequency (for braking purposes)
// AND interrupt rate (used for valve algorithms) of 144Hz
//  (check valve current approx once every 7msec)
// Increments "RTC" by 7 when keeping track of time (about 0.8% fast!)
#define FAST_CLK  0
#define FAST_MS   7

#define TIMER2_SETUP_250K        T2_DIV_BY_1, 199, 16
// for 250KHz internal clock, this is the value used in setup_timer_2 
// to achieve 312.5KHz PWM frequency (for braking purposes)
// AND interrupt rate of ~104Hz
// Increments "RTC" by 51 when keeping track of time (about 0.4% slow if 
//    250KHZ internal clock is accurate, which it is not)
#define MED_CLK   1
#define MED_MS    51

#define TIMER2_SETUP_31K         T1_DIV_BY_1, 199, 4
// for 31KHz LFINTOSC (inaccurate), this is value used in setup_timner_2
// to achieve interrupt rate of 9.7Hz (103msec)
// Not meant to be accurate, no need for variable braking when running at 31KHz
#define SLOW_CLK  2
#define SLOW_MS   103

//#define TIMER3_CCP2_SETUP       T3_DIV_BY_8 | T3_CCP2 | T3_INTERNAL
#define TIMER3_CCP2_SETUP       T3_DIV_BY_8 | T3_INTERNAL
//  This is the value used by setup_timer_3 in order to monitor real time
//   and to provide a time reference (CCP_2) for measurement of sprinkler RPM.
// 3.6864MHz div4 div8 = 57.6KHz
//*****NOTE THIS IS AN ISSUE WITH 3.6864MHz clock

#define TIMER3_AD_SETUP          
//  This is the value used by setup_timer_3 in order to automatically
//   have the ADC monitor the valve current during the initial closure
//   of the valve.
//  1.832MHz clock here.  Checks ADC 

#define CCP1_BRK  CCP_PWM_H_H | CCP_PULSE_STEERING_A | CCP_PULSE_STEERING_SYNC
//  This is the value used by setup_ccp1 in order to enable PWM-controlled 
//   braking
#define CCP1_CHG  CCP_PWM_H_H | CCP_PULSE_STEERING_D | CCP_PULSE_STEERING_SYNC
//  This is value used by setup_cc1 in order to enable PWM-controlled
//   charging level
#define CCP1_BOTHn (CCP_PWM_H_H | CCP_PULSE_STEERING_A | CCP_PULSE_STEERING_D | CCP_PULSE_STEERING_SYNC)
//  This value is sued during transitions to/from full battery situation
//   CCP_BOTHn provides for charge enable pulse and braking pulsing IN PHASE
#define CCP1_BOTHi (CCP_PWM_H_L | CCP_PULSE_STEERING_A | CCP_PULSE_STEERING_D | CCP_PULSE_STEERING_SYNC)
//  CCP_BOTHi provides for charge enable pulse and braking pulsing OUT OF PHASE

#define IDLE_LOOP_31       T2_DIV_BY_16,160,1
// setup for timer2 determines length of lo-power sleeping between
//  mote message checks -- basically and roughly times the idle loop
// 31KHz clock, T2 div 16, 160 counts -> 3Hz
//  thus overflow in about 330 msec
// For calculating battery check and time check and rejoin timeout,
//  we assume that each idle loop takes ~400msec.  Longer than 330 because
//  because of the execution of the steps after timer2
//  wakes us up and before we go back into idle_sleep.  [150 loops per minute]
// And note that LFINTOSC is a +/-10% (or so) oscillator, so idle timing
//  is rough.

/*
#define IDLE_LOOP_76       T2_DIV_BY_16,197,2
// setup for timer 2 determines length of sleeping between mote
//  message checks while idling
// With 76.8KHz clock, timer increments every 833.3usec; 
//  overflows every 198*.833msec = 165msec; interrupts every 330msec
// (see idle_sleep routine)
#define IDLE_LOOPS_PERMIN   182
// This number based on discussion above; change IDLE_LOOP_76 and
//  IDLE_LOOPS_PERMIN together
//#define TIMECHK_IDLELOOPS  25

#define TIMECHK_IDLEMINS   300
// DEBUG SETTING FOR TEST: 5 hours....
#define TIMECHK_IDLELOOPS  TIMECHK_IDLEMINS*IDLE_LOOPS_PERMIN
// This amounts to checking network time once every 10 mins.

#define TIMCHK_INTERVAL    3600
// number of seconds between timechecks when knowtime_fl = TRUE
#define BATCHK_INTERVAL    900
// number of seconds between battery checks!
#define BATCHK_MARCH       840
// what this means is that when there is a marching order change
//  we preset the batchk_timer to this value...
// Thus 60 seconds after the valve has been adjusted, we THEN
//  check battery health (which could lead to a charging on or off 
//  transition.....

#define MAX_JOINTIME       1800
// SHOULD BE ABOUT 1/2 hour or 1800 secs!!!!!!
// seconds to wait for a join to happen
///  even at 1/2 hr, MAY BE TOO SHORT for a full 500 count mesh!

#define JOIN_MINSMAX       30
// Number of minutes that we wait for mote to become operational after 
//  the first join; then we say goodnight

#define FIRSTDANCE_INTERVAL   120
// retry the MSG_IDENTIFIER if no acknowledge within this interval

#define REJOIN_INTERVAL       1800
// number of seconds after which we give up on rejoining!
*/
/*
#define RUN_LOOPS_PERMIN   120
// assume we are consistently at 500msec per loop

#define REJOIN_TIMEOUT     1800
// number of seconds at which we decide to GIVE up on rejoining!
#define TIMECHK_RUNSECS    3600
// check net time every hour
#define BATCHK_RUNSECS     3600
// check battery at least every hour
#define REJOIN_RUNSECS     1800
// cannot join if have attempted for more than 1/2 hour
#define REJOIN_DISP_LIM    REJOIN_RUNSECS - 3
// display REJOIN message during run for 3 seconds after rejoin is
//  initiated


#define  _ACTIVE_MIN   102
// if Vdd is 3.3V, this represents .33 volt at the generator

#define OPER_BCHK_INTVL    36600
   // check the battery level every OPER_BCHK_INTVL tmr2 interrupts
   //  during operation; 36600 for once every 10 minutes

#define FSLEEP_DEL   2
   // wait this number of seconds after receipt of force_warm or force_sleep
   //  manager commands before acting... allows time for ACK
*/

/*

#define NO_BRK             0
#define MAX_BRK            400
// MAX_BRK for absolute minimum speed
   
// min and max charging aggressiveness values (PWM)   
#define MIN_CHGAGGR        0
#define MAX_CHGAGGR        400

#DEFINE MAX_CHGDUTY        0x320

#define MIN_PWM            0
#define MAX_PWM            400
 

#define FLAGS1_BNOCH       0x00
// no charge during BRK control as requested by operator

#define FLAGS1_NOFLO       0x01

#define FLAGS1_MANRPM      0x02
// manual set of MPPC (0x0 00-0x1FF), RPM CONTROL
#define FLAGS1_RPMOLD      0x03
// old table lookup for RPM control

// new adaptive algorith
#define FLAGS1_DYNLO       0x04
// most aggressive charging, PWM control of brake
#define FLAGS1_DYNMID      0x05
// variable MPPC 0-FF, no brake
#define FLAGS1_DYNHI       0x06
// variable PWM control of EN_3105, MPPC = 0xFF, no  brake

#define FLAGS1_RNOCH       0x07
// no charge during RPM control as requested by operator

#define FLAGS1_TRANS2F     0x08
#define FLAGS1_TRANS2N     0x09
// 08:09 for transitioning states to smooth out battery full situations while
//  dynamically charging 

#define FLAGS1_MANBRK      0x0A
// manual set of MPPC (0x000-0x1FF), NO RPM CONTROL
#define FLAGS1_BRKOLD      0x0B
// old formula calc for BRK control

#define FLAGS1_MAXCHG      0x0C
// defined max charging state

#define FLAGS1_FREE        0x0D
// defined state for free spin

#define FLAGS1_MANPWM      0x0E
// manual setting of MPPC and PWM control of 3105 enable

#define FLAGS1_DYNFULL     0x0F
// no charge during BRK control as requested by operator

  
//#define FLAGS_READY        0x00

#define VOPEN_FULL         0x8000
// Valve to max open
#define VCLOSE_FULL        0
#define VUNKNOWN           0xFFFF

// RPM increases as cur_MPPC goes from 0 to FF
#define MPPC_MAX           0x01FF
#define MPPC_DYNMAX        0xFF
// value of MPPC during dynamic control mode at which we shift into
//  hi speed mode wherein we duty cycle modulate the charging circuit!
#define MPPC_MIN           0x00a
#define MPPC_ADDCHG        0x0D1
// This may need to be adjusted if we change the manner in which MPPC is set
//  during braking. See prep_4_brake reoutine

#define MPPC_DUTYLIM       0x320
   // 0x320 = 800
   // maximum duty cycle for charger (special modes)
   
//#define MPPC_CHGOFF        0x1FF
// What should MPPC be set to when charging is NOT happening?  Does it matter?

#define MPPC_DYNCHG    0x0FF
// This is the default STARTING POINT for the MPPC setpt during dynamic
//  charging control
#define MPPC_STEP2F     1
// This is the MPPC value step change which takes place during transitions
// from _DYNMID to full battery situation
#define MPPC_STEP2N     1
// This is the MPPC value step change which takes place during transitions
// from full battery situation to Dynamic charging situation

#define TRANS_PERSTEP   1
// This is the target period value step change which takes place during 
// transitions from _DYNMID to full battery situation
#define TRANS_PWMSTEP   8
// This is the step change for the PWM when routed to the charger and while
//  transitioning from _DYNHI to full battery situation

#define CHRGING_THRESH     0x0011
   // A/D counts difference between UNLOADED and LOADED Vchg measurement
   // indicates whether or not we are (significantly) charging the battery now.
   // We compare this value to (VchgU-VchgL) where this is in units of ADC
   //  counts -- at 3mV per count, 50mv would be a CHRGING_THRESH of ~17.

#define NOSPIN_LIMIT       16
   // Number of ~half-seconds of low or no rotation that must occur in order to
   //  show the nospin display and alert the manager

#define NUM_4_STAT         64
   // This is number of rpm measurements to process prior to determination of
   //  AveRPM and Max/Min (~30sec interval)
   // Also signals reporting to the manager
#define SHIFT_4_STAT       5
   // This is intead of dividing by NUM_4_STAT; change these two values
   //  together

#define TEN_MIN            600
   // This is the number of seconds in ten minutes; used to time mandatory
   //  battery checks

// defines for prechg_opt
// TIME or Alternative (voltage-based?) Algorithm utiling regulated RPM
#define PRECHG_TIME_RPM    0x00


// defines for msg_id (Manager->Mote Payload message ID to Micro)
#define MSG_FULL              0x01
#define MSG_WATER_ON          0x02
#define MSG_WATER_OFF         0x03
  
#define MSG_FORCE_WARM        0x06

#define MSG_SHUT_DEL_RD       0x0E
#define MSG_SHUT_DEL_WR       0x0F

#define MSG_FIRMWARE_RD       0x24

#define MSG_FORCE_TIME        0x29

#define MSG_FORCE_COLD_DEL    0x2B
#define MSG_FORCE_COLD        0x2C

#define MSG_FORCE_REJOIN_DEL  0x2E
#define MSG_FORCE_REJOIN      0x2F

#define MSG_MGR_ACK           0x30
#define MSG_MGR_NACK          0x31

#define MSG_SP_NUM_WR         0x44

#define MSG_DEFINE_TEST       0x49
#define MSG_MARCH1            0x50
// for IMMEDIATE or future MARCHING ORDER, closed loop RPM, timed valve
#define MSG_MARCH2            0x51
// for IMMEDIATE or future MARCHING ORDER, closed RPM and FLOW
#define MSG_READY             0x52
#define MSG_TEST              0x53
#define MSG_DEMO              0x54
// for immediate open loop setting of valve, brake, charger, etc.

#define MSG_REPORT_VALVE      0x59

#define MSG_SLEEPNOW          0x5B
#define MSG_SHUTDOWNC_DEL     0x5C
#define MSG_SHUTDOWNC         0x5D
#define MSG_SHUTDOWNO_DEL     0x5E
#define MSG_SHUTDOWNO         0x5F

#define MSG_HEALTH            0x60
#define MSG_CAL_VALVE         0x61
// short form of MSG_STD solicited by manager to get LOCATION, STATE, ERRORS

// defines for msg_id (Micro to Mote)
#define MSG_ACK            0x80
#define MSG_NACK           0x81
#define MSG_ERROR          0x82
#define MSG_MICRO_RESPONSE 0x83
#define MSG_FIRST          0x84
#define MSG_VCAL           0x85

// defines for various unsolicited messages
#define BATTERRb           7
//Indicates to LV program that the startup was
//  rocky due to a DEAD BATTERY! (bit7 set)
#define LOSPINb            6
// Indicates to LV program that the freespin speed measured during the 
//  opening dance was less than FREESPIN_MIN (bit6 set)
#define OCDIFFb            5
// Indicates to LV program that the 1st open time and the 1st close
//  time differ by greater than 25% (bit5 set)
#define REJOINEDb          4
// Indicates that a rejoin occurred

// Note 3:2 carry information about 1st valve open
// and  1:0 carry information about 1st valve close
// as follows: 0b00   OK
//             0b01   timeout
//             0b10   TOO SHORT of a duration for valve motion
#define OPENSHORTb         3
#define OPENLONGb          2
#define CLOSESHORTb        1
#define CLOSELONGb         0

// defines for error codes as communicated to manager
#define NO_ERRCODE         0x00
#define BATOK_ERRCODE      0x03

#define FRC_SLEEP_ERRCODE  0x05
// manager forces system into a sleep state and decommissions the mote

#define LOC_ERRCODE        0x21
#define CPASSOC_ERRCODE    0x22
#define LOC_CP_ERRCODE     0x23

#define ADDCHG_WARNCODE    0x80
//#define LOWBAT_ERRCODE     0x80
#define PS_ERRCODE         0x81
         // Vdd too high
#define WEAKBAT_WARNCODE   0x82
#define DEADBAT_ERRCODE    0x83

// defines for state variable
#define  DEV_INIT          0x00
#define  DEV_JOINING       0x01
#define  DEV_1STEXCH       0x02
#define  DEV_READY         0x03
#define  DEV_RUN           0x04
#define  DEV_PAUSED        0x05
#define  DEV_IDLE          0x06
#define  DEV_SLEEP         0x07
#define  DEV_TEST          0x08
#define  DEV_DEMO          0x09

// defines for vlv_state variable
#define  VLV_COASTING   0x00
#define  VLV_CLOSING    0x01
#define  VLV_OPENING    0x02
#define  VLV_BRAKING    0x03

#define  TYPE_NOXDCR    0x01
#define  TYPE_XDCR      0x02

// defines for sys_state variable
// F009+
// 7:4 - major state indicator
// 3:0 - minor state indicator
//       .3 = joining_fl (also set if rejoining fails)
//       .2 = rejoining_fl
//       .1 = chgenab_fl
//       .0 = ctrlRPM_fl
#define INIT_STATE               0x00
   // OR with joining_fl when established
   // always leads to OPEN1_STATE;
   // NEVER accepts packets (NACK), even if joined
#define LSTATE_INIT              0x00

#define OPEN1_STATE              0x10
   // OR with joining_fl when established
   // always leads to PRECHG_STATE;
   // always from INIT_STATE;
   // May process report commands.
   // [Vmmmm:ss OPENING] countup display
#define LSTATE_OPEN1   0x01

#define PRECHG_STATE             0x22
   // OR with joining_fl when established
   // always leads to CLOSE_STATE; may be in and out quickly if no
   //  precharging is necessary;
   // always from OPEN1_STATE;
   // May process report commands..
   // [Pmmmm:ss        ] countdown display
#define LSTATE_PRECHG   0x02

#define CLOSE1_STATE             0x32
   // OR with joining_fl when established
   // always leads to JOINING_STATE or READY_STATE;
   // always from PRECHG_STATE;
   // May process report commands.
   // [Vmmmm:ss CLOSING] countup display
#define LSTATE_CLOSE1   0x03

#define JOINING_STATE            0x44
   // always leads to READY_STATE
   // always from CLOSE1_STATE
   // Bypassed if warm start
   // Cannot hear commands.
   // [Jmmmm:ss  moteid] display
#define LSTATE_JOIN     0x04

#define REJOINING_STATE          0x48
   // Leads to READY_ if rejoin is successful
   //  or JFAIL_ if rejoin is unsuccessful
   //  or ADDCHG_ if needed.
   // Drops into this state when we are rejoining
   //  AND there is no pending marching order
   //  Preceeding state READY_ or 
   //   short-lived READY_ after completion of marching orders.
   // [Rmmmm:ss  moteid] countup display
   // Cannot hear commands.
   // THINK ABOUT THIS

#define DISRUPT1_STATE           0x50
#define LSTATE_DIS1     0x05
#define DISRUPT2_STATE           0x60
#define LSTATE_DIS2     0x06
#define DISRUPT3_STATE           0x70
#define LSTATE_DIS3     0x07

#define READY_STATE              0x80
   // leads to OPENING_STATE or VADJ_STATE
   //  or to DISRUPT STATE;
   // valve is closed in READY_STATE;
   // from CLOSE1_STATE or or CLOSING_STATE
   // Processes all commands
   // ['yy:ddd:hh:mm:ss] display
#define LSTATE_READY    0x08

#define JFAIL_STATE              0x8C
   // Eventually leads to shutdown of sprinkler
   // Indicates cannot join and we have given up AND all marching orders
   //  have completed
   // Drops into this state from REJOINING_STATE
   // Cannot hear commands.
   // ['yy:ddd:hh:mm:ss] display
#define LSTATE_JFAIL    0x10
// band-aid
   
#define OPENING_STATE            0x90
   // OR with rejoining_fl (and joining_fl if failure to join)
   // May be brief if at open-valve end-stop;
   // Leads to either BRK_ or RPM_ or ADDCHG_;
   // From BRK_STATE or RPM_STATE or READY_STATE or PAUSE_STATE or REJOINING_
   // May process report, param and march commands.
   // [Vmmmm:ss OPENING] countup display
#define LSTATE_OPENING  0x09

#define CLOSING_STATE            0xB2
   // OR with rejoining_fl (and joining_fl if failure to join)
   // May be brief if at close-valve end-stop;
   // Leads to either READY_, PAUSE_.
   // From BRK_STATE or RPM_STATE.
   // May process report, param and march commands.
   // [Vmmmm:ss CLOSING] countup display
#define LSTATE_CLOSING  0x0B

#define VADJ_STATE               0xC0
   // OR with rejoining_fl (and joining_fl if failure to join)
   // May be brief if valve DOES NOT NEED adjustment;
   // Leads to either BRK_STATE or RPM_STATE;
   // From BRK_STATE or RPM_STATE or READY_STATE or PAUSE_STATE
   // May process report, param and march commands.
   // [Vmmmm:ss rrr!RRR] countup display shows current rpm ! desired RPM
#define LSTATE_VADJ     0x0C

#define PAUSE_STATE              0xD0
   // OR with rejoining_fl (and joining_fl if failure to join)
   // Leads to READY_ or OPENING_ or VADJ_
   // From READY_ or CLOSING_
   //  (CLOSING_ may have followed BRK_ or RPM_)
   // May process report, param and march commands.
   // either [ mmmm:ss  NOFLOW] countup display 
   //     or [Xmmmm:ss  NOFLOW] countdown display if pending order 
#define LSTATE_PAUSE    0x0D

#define BRK_STATE                0xE0
   // OR with rejoining_fl (and joining_fl if failure to join)
   //  and OR with chgenab_fl
   // Leads to CLOSING_ or OPENING_ or VADJ_ or READY_;
   // From VADJ_ or OPENING_
   // May process report, param and march commands.
   // either [ mmmm:ss rrrBbbb] countup display (current RPM B brake force)
   //     or [Xmmmm:ss rrrBbbb] countdown display if pending order  
#define LSTATE_BRK      0x0E

#define RPM_STATE       0xF1
   // OR with rejoining_fl (and joining_fl if failure to join)
   //  and OR with chgenab_fl
   // leads to CLOSING_ or OPENING_ or VADJ_ or READY_;
   // from VADJ_STATE or OPENING_STATE
   // May process report, param and march commands.
   // either [ mmmm:ss rrr*RRR] countup display (current RPM / desired RPM)
   //     or [Xmmmm:ss rrr*RRR] countdown display if pending order 
#define LSTATE_RPM      0x0F

#define JOIN_MASK       0x0C
// AND with sys_state to extract both fl_joining and fl_rejoining bits
#define STATE_MASK      0xF3
// AND with sys_state to clear both fl_joining and fl_rejoining bits
#define RPM_CH_MASK     0x03
// AND with sys_state to extract RPM control bit (0) and charging bit (1)


#define FREESPIN_MIN       60
// If freespin measured at startup is less than FREESPIN_MIN, then 
//  manager will be informed when UNSOLICITED message is sent
// See above regarding MSG_UNSOLICITED defines.

#define START_BRK          600
   // Some intermediate braking at cold start perior to beginning of precharge
#define IDLE_BRK           200
   // This is the braking force we apply during idling.... mostly important
   //  for debugging purposes


// IFDEF WARNER we are using Eric's valve
// otherwise we are using Jake's test fixture
//#IFDEF   WARNER
//#define VALVE_STOP         990
//#ELSE
//#define VALVE_STOP         960
//#ENDIF

#define VCURLIM            900
// guess during new board debug Feb 18 2016

#define VALVE_STOP_W       990
#define VALVE_STOP_J       960
   // This is the number of ADC counts below which we assume
   //  the valve motor has hit a stop.  NOTE hardware has a .25 ohm 
   //  resistor to Vbat
   // This value will depend on valve drive motor characteristics
   //  *J for test bench; *W for Warner valve
*/
//**************************************************************************
/*   
   Experimentation with prototype valves during March/April 2015 brought up
   problems with valve becoming STUCK at an endstop.
   Solutions include: 
      1. DO NOT CHECK for excessive current during first few
      nudges when COMING off an endstop
      2. Once valve is moving along (past the initial nudge which overcomes
      inertia) we can check for excessive current sooner in the nudge cycle
   Care must be taken to accomodate various situations which could interfere
      with this approach, such as when nudging is interrupted by the need to
      acknowledge activity at the mote.   
 
  VALVE_CHKDEL0 is the time in ms to wait after the drive motor is
  turned on before checking if we have excessive current as part of the
  very first nudge which specifically overcomes the startup current
  transient due to inertia

 VALVE_CHKDEL0 will be dependent upon whether we are using the "WARNER"
  valves or the in-house test fixture  
 
 VALVE_CHKDEL1 is the normal time in ms to wait after the drive motor is
  turned on before checking if we have excessive current.  The assumption
  for the choice of this value is that we have already overcome the startup
  current transient due to inertia

 VALVE_CHKNO is the number of nudges during which we DO NOT check for
  excessive current in the case where the valve is specially coming off
  of either the closed or open end stop


 Flags specify the situation at hand:
 atstop_fl     TRUE if we are currently at an endstop, this flag
               ensures that the algorithm utilizes VALVE_CHKNO
               
 firstnudge_fl TRUE if we are starting the valve from rest and we are NOT
               at an endstop; ensures that the algorithm utilizes VALVE_CHKDEL1
               
EXCEPTION and possible issue in the future is the first nudge after 
system startup.  We may or MAY NOT be at an endstop.  So we assume a
atstop_fl = TRUE.  

If we are actually FULL OPEN, and get stuck (!) we can
detect this because the number of steps during initial open will be too few
(less than 2 or 3) and then we can try again with the atstop_fl set.

If we are actually at FULL CLOSED, and get stuck, again the first opening will
be too few steps and we must then try again with the atstop_fl set.....

Thus if after the startup dance, either close_time or open_time are 
"unreasonable", we must simply do it again (although perhaps we could skip
the precharge if we have a good free-spin value).  This will be optimized
in a later version of the firmware. 

condx                      atstop_fl      firstnudge_fl
----------------------------------------------------
1st nudge after startup    TRUE           TRUE
(valve position unknown)
1st nudge from endstop     TRUE           FALSE
1st nudge norm             FALSE          TRUE
(or immediately after motion is interrupted by activity from mote)
subsequence nudges         FALSE          FALSE
*/
//***************************************************************************

/*

//#define  VALVE_CHKNO       3
#define VALVE_NOCHK_W     3
#define VALVE_NOCHK_J     1

//#IFDEF   WARNER
//#define VALVE_ONTIME       150
//#define VALVE_CHKDEL0      100
//#ELSE
//#define VALVE_ONTIME       100
//#define VALVE_CHKDEL0      50
//#ENDIF

// These values are in milliseconds
#define VALVE_ONTIME_W     150
#define VALVE_CHK0_W       100
#define VALVE_ONTIME_J     100
#define VALVE_CHK0_J       50

// assume 16 bit counter triggered by FOSC div 64 (timer 0 prescaler)
// Thus one count every 64*T0_OSC nanoseconds
// Setup timer 0 for div64 and load with the value below for it to time out                        

#define VALVE_TMR0SET0_W     (65536 - (1000000*(VALVE_ONTIME_W - VALVE_CHK0_W)+32*T0_OSC)/(64*T0_OSC))
#define VALVE_TMR0SET0_J     (65536 - (1000000*(VALVE_ONTIME_J - VALVE_CHK0_J)+32*T0_OSC)/(64*T0_OSC))
// THUS: if we delay CHK_DEL0 milliseconds, and then set a timer to timeout
//  according to VALVE_TMR0SET0, we have taken up a full CHUNK's worth of time

// once valve is in motion, only wait 20 msec before testing for endstop!
//  This is arbitrary and conservative.
#define VALVE_CHKDEL1      20
#define VALVE_TMR0SET1_W   (65536 - (1000000*(VALVE_ONTIME_W - VALVE_CHKDEL1)+32*T0_OSC)/(64*T0_OSC))
#define VALVE_TMR0SET1_J   (65536 - (1000000*(VALVE_ONTIME_J - VALVE_CHKDEL1)+32*T0_OSC)/(64*T0_OSC))
// THUS: if we delay CHK_DEL1 milliseconds, and then set a timer to timeout
//  according to VALVE_TMR0SET1, we have taken up a full CHUNK's worth of time

   // VALVE_ONTIME is the number of milliseconds that the motor runs between
   //  activity on mote checks
   // VALVE_CHKDEL0 is the number of milliseconds we wait when motor is
   //  first turned on to check for an endstop.  This accounts for the
   //  inertial transient at first turn-on.
   
#define VALVE_ATSTOPDEL    50


#define VALVE_TIM0         (VALVE_ONTIME-VALVE_CHKDEL0)
// Set TMR0 to overflow after this number of millisecondes
//  Note that each tick of TMR0 during the move_valve routine is
//  32/19200 or 1.666 msec or 1667 microseconds
//  This convenient number is represented by the fraction 3/5 in the 
//   define below
#define VALVE_TMR0SET0     256 - ((VALVE_TIM0*3)/5) 
// THUS: if we delay CHK_DEL0 milliseconds, and then set a timer to timeout
//  according to VALVE_TMR0SET0, we have taken up a full CHUNK's worth of time
 



//#IFDEF   WARNER
//#define VCTR_LIMIT         200
//#ELSE
//#define VCTR_LIMIT         175
//#ENDIF

#define VALVE_CTRLIM_W       200
#define VALVE_CTRLIM_J       175
// little motions of valve motor should definitely get it to a stop
//  or to its destination;  if not we time out.  This number is somewhat
//  arbitrary

#define VCLASS2_LIM        38     
// relative to 0 (full close) and 255 (full open) at what limit for 
//  valve desired do we force bouncing off the nearest enstop?
// 15% would be 38
#define BOUNCE_OPEN        256-VCLASS2_LIM
#define BOUNCE_CLOSE       VCLASS2_LIM
#define V_ENDCTR_LIM       5
// force an endstop at least every 5 excursions


//#IFDEF   WARNER
//#define V_SINCEEND_LIM     400 
//#ELSE
//#define V_SINCEEND_LIM     350
//#ENDIF
// force an endstop if cumulative motion has exceeded about 2 full paths
//  from open to closed
#define VALVE_SINCELIM_W   400
#define VALVE_SINCELIM_J   350


#define VFULL_TIM_LIM      7200
// if this number of seconds has passed since the full open2close or the
//  full close2open time has been remeasured, then force it.

// defines for vset_class variable
#define BOUNCE0_CL         0x01
#define BOUNCE0_OP         0x81
#define BOUNCE1_CL         0x02
#define BOUNCE1_OP         0x82
#define BOUNCE2_CL         0x03
#define BOUNCE2_OP         0x83
#define BOUNCE3_CL         0x04
#define BOUNCE3_OP         0x84

#define VTOOSHORT          60
// This is the minimum number of nudges for open to close or close to open
//  which is acceptable during startup dance.  If not VTOOSHORT, then
//  no problem is reported during first message after join
#define FREESPIN_LIM       120
// This is the minimum RPM which is 'acceptable' for free spin result!
// If not too slow, then no problem is reported during
//  first message after join

// EEPROM LOCATIONS
#define EE_ADDCHG_DUR      0x0002
   // Configures add-charging time in seconds
   // 2 bytes, low byte at 0x0002
#define ADDCHG_DUR_DEF     4
   // 4 minutes?
#define EE_PRECHG_DUR      0x0004
   // Configures precharging time in seconds
   // 2 bytes, low byte at 0x0004
#define PRECHG_DUR_DEF     10
   // 1.5 minutes? 
#define CHGLOOPS_PER_MIN   110
   // determined experimentally, each loop is about 545 msec
   //  which is a bit longer than I would prefer (!), but oh well.
   //  JG 2015 Jun16
   
// 0x00A thru 0x00F        reserved
#define EE_NETW_ID         0x0010
   // 2 bytes, low byte at 0x0010; for now, network ID defaults to 0x04CD.
#define EE_PRENETW_ID      0x0012
   // 2 bytes, low byte at 0x0012; for now, network ID defaults to 0x04CD.


// 0013 reserved
#define EE_LOCATION     0x0014
   // 2 bytes, low byte at 0x0014; associates sprinkler with particular
   //  physical location on CP
#define EE_PRELOCATION  0x0016
   // 2 bytes, low byte at 0x0016, previous association of sprinkler with
   //  particular physical location on CP

   // 0x018 thru 0x01F     reserved

// IDLE behavior specified at 0x0020 thru 0x0033
#define EE_IDLE_OPT        0x0020
   // Configures IDLE behavior.  Applies if battery low during operational
   //  state
   //  0x00  send out Panic Message once when panic level is reached according
   //         to voltage-based algorithm
   //  0x01  send out Panic Message once when panic level is reached according
   //         to charge-based algorithm
#define MAX_IDLE_OPT       1
   // If manager tries to write any idle_opt value larger than this to EEPROM
   //  a NACK occurs; see above for idle option choices
#define LEN_IDLE_BLOCK     15
   // There are 15 bytes in the idle block

#define EE_IDLE_PROFILE    0x0021
   // Saved only upon entering IDLE state;
   //  profile number which was running or had last been run.
   // 0 for default profile, 1, 2, 3 for controller-specified profile choices
   //  other numbers, use 0
#define EE_IDLE_STEP       0x0022
   // Saved only upon entering IDLE state;
   //  step number of profile which was running or had last been run.
   //  Step 0xFF indicates that we were not within a profile
   //  Step 0x10 thru 0xFE invalid.
#define EE_IDLE_STIME      0x0023
   // 2 bytes, low byte at 0x0023, high byte at 0x0024
   // Saved only upon entering IDLE state;
   //  amount of time in seconds that rate was controlled according
   //  to EE_IDLE_STEP at time when IDLE state entered;
   //  set to 0x0000 if EE_IDLE_STEP=0xFF.
#define EE_IDLE_LOOP       0x0025
   // 2 bytes, low byte at 0x0025, high byte at 0x0026
   // Saved only upon entering IDLE state;
   //  for looping profiles, loop number at time when IDLE state entered;
   //  set to 0x0000 for non-looping profiles, set to
   //  0x0000 if EE_IDLE_STEP=0xFF. (looping beyond 0xFFFF is recorded as
   //  0xFFFF)
#define EE_IDLE_CHG        0x0027
   // 2 bytes, low byte at 0x0027, high byte at 0x0028
   // Configures used charge in millicoulombs for
   //  charge-based shutdown method (TBD); applies if EE_IDLE.1 is set
#define EE_IDLE_SHDN_DLY   0x0029
   // 2 bytes, low byte at 0x0029, high byte at 0x002A
   // Configures time in units of 67 secs (TMR0 ints during IDLE)
   //  between sensing of low battery and self-shutdown


// 0x02F thru 0x033        reserved

// PROFILE behavior specified at 0x0034 thru 0x003E
#define EE_PROFILE_OPT     0x0034
   // Configures PROFILE behavior.
   //  0x00 start profile0 (0x0050-0x007F) from the beginning
   //  0x01 start profile1 (0x0090-0x00BF) from the beginning
   //  0x02 start profile2 (0x00D0-0x00FF) from the beginning
   //  0x03 start profile3 (0x0110-0x013F) from the beginning
   //  0x04 start last used profile (EE_IDLE_PROFILE) from the beginning
   //  0x05 start last used profile (EE_IDLE_PROFILE) from where it stopped
#define MAX_PROFILE_OPT    0x05
   // profile option value cannot exceed this value


#define EE_SHUT_DEL        0x003E
   // 2 bytes, low byte at 0x003E, high byte at 0x003F
   // number of seconds between MSG_FORCE_COLD_DEL or MSG_SHUTDOWNx_DEL
   //  and actual restart or shutdown
   // For broadcast messages, ensures that all of the motes get the
   //  message before they 'go away'

// GENERAL behavior specified at 0x0040 thru 0x004F
#define EE_GENERAL_OPT     0x0040
   // Configures GENERAL behavior.
   //  0x00 normal operation
   //  0x01 test mode A (debug)
   //  0x02 test mode B (time messages to DUST controller)
   //  0x03 test mode C (debug and time messages)
#define MAX_GENERAL_OPT    0x03
   // general option value cannot exceed this value

// 0x041 thru 0x04F        reserved

#define EE_REINFO          0x0050
   // hold last known restart_cause

// 0x051 thru 0x05F        reserved

#define EE_DEADPROFILE     0x0060
   // If death occurs during profiling, place profile number here (0,1,2,3)
   //  else set to 0xFF
#define EE_DEADSTEP        0x0061
   // Set to profile step number during which death occurred.
#define EE_DEADTIME        0x0062
   // 2 bytes indicates how far along we were into the profile step when
   //  dead battery was detected.
#define EE_DEADSTATE       0x0064
   // system state during which dead battery was detected

// 0x065 thru 0x07F        reserved

#define EE_ERRORID         0x0080
   // 2 bytes, low byte is the error_id and
   //  high byte is errors1 -- saves last reason for a deep event error

#define EE_VSET_CLASS      0x0082
#define EE_V_ENDCTR        0x0083
#define EE_V_SINCEEND      0x0084
#define EE_O_C             0x0086
#define EE_C_O             0x0088

#define EE_RESTART_INFO    0x00A0
// 0x090 thru 0x0EF        reserved

#define EE_PROFILE0        0x00F0
   // This is address of first byte of profile0 (default).
   // May extend to 0x011F.
   // 3 bytes per step, 16 steps.  Each step has 1 byte for RPM and 2 bytes
   //  for duration in seconds.  If duration = 0x0000, then this means
   //  "forever."
   //  If 0x00 <= RPM <= RPM_MIN, then interpret RPM as RPM_MIN.
   //  If RPM_MAX <= RPM <= 0xFF, then interpret RPM as RPM_MAX.
   //  thus RPM = 0x00 and duration = 0xFFFF means single step rpm_min speed forever.
   //  All bytes following duration = 0xFFFF in a profile are ignored.
   //   RPM = 0xFF and duration = 0xFFFF is the end of profile marker for
   //   "repeating" profiles".  0xFF / 0xFFFF cannot be the first step.
   //  for steps preceeding duration = 0xFFFF, duration cannot = 0x0000!
   //  Note that default profile is loaded into EEPROM by firmware
   //   at start if none is present.  Controller can read or write
   //   profiles.

// 0x120 thru 0x1EF        reserved

#define EE_VDD             0x1F0
   // Two bytes; ADC-based estimate of Vdd (4.12 fixed pt format)
#define EE_VBATT           0x1F2
   // Two bytes, ADC-based estimate of Vbatt
#define EE_VGEN            0x1F4
   // Two bytes, ADC-based estimate of Vgen
#define EE_VSOLAR          0x1F6
   // Two bytes, ADC-based estimate of Vsol


// SCRATCHPAD EEPROM

#define EE_DEBUG_1st       0x0200
#define EE_DEBUG_LAST      0x03F0

#define EE_FREE_1st        0x0200
#define EE_FREE_LAST       0x03BF      // free space never cleared
#define EE_SCRATCH_1ST     0x03F0
#define EE_SCRATCH_LAST    0x03F8      // scratch space cleared at start
                                       //  (warm, cold, or xcold)
   // This is the start and end of scrathpad memory in EEPROM.  To be used for
   //  for debug purposes or TBD.

#define EE_BOOT_RECORD     0x0200      // new events and time
#define EE_NETID_TIME      0x0208
#define EE_MOTEOPER_TIME   0x020C
#define EE_MOTEIDLE_TIME   0x0210
#define EE_MSG_RECORD_0    0x0220
#define EE_MSG_RECORD_END  0x03BF

#define EE_SYS_STATE       0x03C0
   // Address of state flags (echoes read only status)
   // Written to EEPROM at significant state transitions
#define EE_INT_CTR         0x03E8
   // Address of error/alarm flags (as many as 16 bytes)
   // Written to EEPROM at significant state transitions
#define EE_CUR_PROFILE     0x03F0
#define EE_MSGBYTES_DBG    0x03F8
#define EE_PORTB_BIT       0x03F9
#define EE_PORTB_DIR       0x03FA


   

   // for debug only

#define DUTYCYCLE_SET      0x40
// set join duty cycle for mote to this value at startup; approx 15%
#define SPECIALNV1_00      0x0000
   // for rapid join time during debug, use with pkgen_setnv_special1(**)
#define SPECIALNV2_00      0x0005
   // for rapid join time during debug, use with pkgen_setnv_special2(**)
   // RAPID: 5 seconds to 0 seconds is a timing range having to do with
   //  looking for advertisements during join
   // NORMAL: 90 to 60 seconds.....
#define SPECIALNV1_01      0x003C
   // for normal join time during debug, use with pkgen_setnv_special1(**)
#define SPECIALNV2_01      0x005A
   // for normal join time during debug, use with pkgen_setnv_special2(**)

*/


      /* signed int properties */
#define INT_MAX 32767         /* signed integer minimum*/
#define INT_MIN (-32768)      /*signed integer maximum*/

      /* signed long properties */
#define LONG_MAX 2147483647        /* signed long maximum*/
#define LONG_MIN (-2147483648) /* signed long minimum*/

/*

// BELOW are constants and defines used in battery check routines

#define FVR_NOEE        2455
// approx 2V, any lower, don't attempt to run the micro or write to EE
//    fl_bat_NOEE
//    (BOX_CHAR here and below)

#define FVR_NODUST      2332
// 2.2 V; any lower, don't use the dust module; fl_batNODUST
//    ('D' here down to FVR_NOEE)

#define FVR_TEST        1819       
// 2.7V threshold for testing purposes, display still visible

#define FVR_DEAD        1638
// 3.0V; dead battery, any lower, do not move valve; fl_batDEAD
//    ('d' here down to FVR_NODUST)

#define FVR_WEAK        1534
// 3.2V; below this point, battery dangerously weak 
//  but can add charge, shout out; fl_batSHOUT, fl_addchg 
//    ('-' here down to FVR_DEAD))

#define FVR_ADDCHG      1511
// 3.25V; battery weak... below this point add charge, shout out
//    (! here down to FVR_WEAK); fl_batSHOUT, fl_addchg
// above this point, battery OK; fl_batOK
//    ('+' here up to FVR_ADEQUATE)

#define FVR_ADEQUATE    1488
// 3.30V; adequately charged if at or above this point; fl_batOK 
//    ('f' here up to FVR_FULL)

#define FVR_FULL        1444
// 3.40V; battery full at or above this point; fl_batFULL
//    ('F' here up to FVR_TOOFULL)

#define FVR_TOOFULL     1327
// >=3.7 do not precharge, error condition?; fl_batTOOFULL
//    (TOPLINE_CHAR here and above)

// QCHK_FULL value is compared to 4x 1.2Vref as measured when sprinkler
//  is running and battery may be charging  (VfvrCAD)
// If the battery voltage measured during qchk_batt
//  is such that VfvrCAD < QCHK_FUL [indicates battery fully charged]
//  then charging will stop, hopefully in a smooth and controlled manner
//  before the charger hardware cannot charge due to its configuration.
// After this occurs, charging will not take place until the next marching
//  order, at which point a chk_batt happens and a different criterion applies.
// Note that qchk_batt() measures during running and charging!
// The flag fl_qbatFULL is set and not cleared until the next marching order
//  is processed.
#define QCHK_FULL       1391
// 1023/1391 * 4.8V = 3.53V
// charging never enabled (fl_qbatFULL set) when VfvrCAD < QCHK_FULL
//    (chargemark set to TOPLINE_CHAR whenever fl_qbatFULL is set)
//    (chargemark set to 'c' if !fl_qbatFULL and significant difference
//     between generator voltage while charging as compared to not charing)

#define BATT_HYST       12
// If the difference between the battery voltage at the start of a new
//  marching order as compared to the VfvrAD which was measured when
//  a bat full situation was last determined is greater than this amount,
//  then we turn charging back on; in other words, if the battery voltage
//  has dropped during the previous marching order to a great enough extent
//  AFTER we had already determined that the battery was full, then we turn
//  charging back on (Rev 030+)
// Approx a 30mV drop [somewhat arbitrary, but provides a form of hysteresis]

#define TM_BATCHK       7200
   // number of seconds maximum between chk_batt() calls;


#define TM_SPINRPT      0xA1
   // arbitrary 8 bit number which, when compared to low byte of seconds ctr
   //  initiates the possible reporting of a spin error (should be greater than
   //  32 seconds because that is the amount of time it takes for the variance
   //  determination to take place (sizeof periods[])
   // Possible error states include: NOSPIN_ERRCODE, SPEEDLO_ERRCODE,
   //  SPEEDHI_ERRCODE, SPEEDREG_ERRCODE
   // NOSPIN: manager has told us that water is flowing, but
   //  speed is too slow to detect (<~3RPM)
   // SPEEDLO/SPEEDHI: average speed over previous 64 measurements is beyond
   //  limit established (e.g., +/-3%)
   // SPEEDREG: max speed (or min speed) over previous 64 measurements
   //  exceeds limit establihsed (e.g. target+/-6%)
   // In this code, spin checks occur once every 256 seconds, or whenever
   //  requested by the manager (MSG_STATUS_RDONLY).
   // If a problem is detected, an error message immediately goes out one time.
   //  THUS spin errors will be sent at least every 256 seconds
   //  (change later?)

#define ONE_WEEK     2235
   // number of 4.51 minute intervals in a week (for DEAD BATT HANDLER)

#define DEAD_BATTERY    0xFF
   // indicates that restart event was due to dead battery detection

#define GET_A_CHAR(value)   { TMR1L = 0x78;  \
                              TMR1IF = FALSE;  \
                              output_low(_DUST_SPCTS);  \
                              while (!(kbhit() || TMR1IF));  \
                              errors1 |= RCSTA; \
                              dust_recive_buffer[value] = RCREG; \
                              output_high(_DUST_SPCTS); \
                            }
#define GET_1_CHAR      { set_timer1(0); \
                          output_low(_DUST_SPCTS);  \
                          while (!kbhit() && bit_test(TMR1H,7));  \                         
                          *mm = RCREG; \
                          output_high(_DUST_SPCTS); \ 
                          mm++; \
                        }
                        
#define GET_blah_CHAR      { output_low(_DUST_SPCTS);  \
                          while (!kbhit());  \                         
                          *mm = RCREG; \
                          output_high(_DUST_SPCTS); \ 
                          mm++; \
                        }

#define GET_32CHARS  {  GET_A_CHAR(0) \
                        GET_A_CHAR(1) \
                        GET_A_CHAR(2) \
                        GET_A_CHAR(3) \
                        GET_A_CHAR(4) \
                        GET_A_CHAR(5) \
                        GET_A_CHAR(6) \
                        GET_A_CHAR(7) \
                        GET_A_CHAR(8) \
                        GET_A_CHAR(9) \
                        GET_A_CHAR(10) \
                        GET_A_CHAR(11) \
                        GET_A_CHAR(12) \
                        GET_A_CHAR(13) \
                        GET_A_CHAR(14) \
                        GET_A_CHAR(15) \
                        GET_A_CHAR(16) \
                        GET_A_CHAR(17) \
                        GET_A_CHAR(18) \
                        GET_A_CHAR(19) \
                        GET_A_CHAR(20) \
                        GET_A_CHAR(21) \
                        GET_A_CHAR(22) \
                        GET_A_CHAR(23) \
                        GET_A_CHAR(24) \
                        GET_A_CHAR(25) \
                        GET_A_CHAR(26) \
                        GET_A_CHAR(27) \
                        GET_A_CHAR(28) \
                        GET_A_CHAR(29) \
                        GET_A_CHAR(30) \
                        GET_A_CHAR(31) }
                        
#define GET_REST_OF_CHARS  {  GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \
                              GET_1_CHAR \ }


// see MOTETIME.xlsx
// 1/1/2013 midnight .... 1/1/2062 midnight
UNSIGNED INT32 CONST YRLUKUP [50] = {
   0x50E22700,0x52C35A80,0x54A48E00,0x5685C180,0x58684680,
   0x5A497A00,0x5C2AAD80,0x5E0BE100,0x5FEE6600,0x61CF9980,
   0x63B0CD00,0x65920080,0x67748580,0x6955B900,0x6B36EC80,
   0x6D182000,0x6EFAA500,0x70DBD880,0x72BD0C00,0x749E3F80,
   0x7680C480,0x7861F800,0x7A432B80,0x7C245F00,0x7E06E400,
   0x7FE81780,0x81C94B00,0x83AA7E80,0x858D0380,0x876E3700,
   0x894F6A80,0x8B309E00,0x8D132300,0x8EF45680,0x90D58A00,
   0x92B6BD80,0x94994280,0x967A7600,0x985BA980,0x9A3CDD00,
   0x9C1F6200,0x9E009580,0x9FE1C900,0xA1C2FC80,0xA3A58180,
   0xA586B500,0xA767E880,0xA9491C00,0xAB2BA100,0xAD0CD480};
   

UNSIGNED INT8 CONST BYTE_2_PCT [256] = {
    0, 0, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 6, 
    6, 7, 7, 7, 8, 8, 9, 9, 9,10,10,11,11,11,12,12,
   13,13,13,14,14,15,15,15,16,16,16,17,17,18,18,18,
   19,19,20,20,20,21,21,22,22,22,23,23,24,24,24,25,
   25,25,26,26,27,27,27,28,28,29,29,29,30,30,31,31,
   31,32,32,33,33,33,34,34,35,35,35,36,36,36,37,37,
   38,38,38,39,39,40,40,40,41,41,42,42,42,43,43,44,
   44,44,45,45,45,46,46,47,47,47,48,48,49,49,49,50,
   50,51,51,51,52,52,53,53,53,54,54,55,55,55,56,56,
   56,57,57,58,58,58,59,59,60,60,60,61,61,62,62,62,
   63,63,64,64,64,65,65,65,66,66,67,67,67,68,68,69,
   69,69,70,70,71,71,71,72,72,73,73,73,74,74,75,75,
   75,76,76,76,77,77,78,78,78,79,79,80,80,80,81,81,
   82,82,82,83,83,84,84,84,85,85,85,86,86,87,87,87,
   88,88,89,89,89,90,90,91,91,91,92,92,93,93,93,94,
   94,95,95,95,96,96,96,97,97,98,98,98,99,99,100,100 };
   
UNSIGNED INT8 CONST BYTE99_2_10S [100] = {
   '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
   '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', 
   '2', '2', '2', '2', '2', '2', '2', '2', '2', '2', 
   '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', 
   '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', 
   '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', 
   '6', '6', '6', '6', '6', '6', '6', '6', '6', '6', 
   '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', 
   '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', 
   '9', '9', '9', '9', '9', '9', '9', '9', '9', '9' };

UNSIGNED INT8 CONST BYTE99_2_1S [100] = {
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
   
*/








