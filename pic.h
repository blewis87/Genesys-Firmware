// This file contains all functions related to PIC functions and setup

//Function Definitions

// These defines are used in the oscillator setup subroutines below.
// Note the situations of concern:
//    4MHz osc: TMR2 prescale 16, postscale 6, PR2=99;         DIV16_POST6 
//       [PWM freq 625Hz, PWM resolution 400, TMR2 int rate: 9.6ms]
//    3.6864MHz osc: TMR2 prescale 16, postscale 6, PR2=99;    DIV16_POST6
//       [PWM freq 576Hz, PWM resolution 400, TMR2 int rate: 10.4ms]
//    1MHz osc: TMR2 prescale 4, postscale 6, PR2=99;          DIV4_POST6
//       [PWM freq 625Hz, PWM resolution 400, TMR2 int rate: 9.6ms]
//    250KHz osc: TMR2 prescale 16, postscale 6, PR2=99;       DIV1_POST6
//       [PWM freq 625Hz, PWM resolution 400, TMR2 int rate: 9.6ms]
//    32768Hz osc: TMR2 prescale 1, postscale 1, PR2=79;       DIV1_POST1
//       [no PWM, TMR2 int rate: 9.8ms]
//    31250Hz osc: TMR2 prescale 1, postscale 1, PR2=79;       DIV1_POST1
//       [no PWM, TMR2 int rate: 10.2ms]

// SEE p 177 of DS41412F [PIC18(L)F2X/4XK22 datasheet]
#define  DIV16_POST6    0b00101110
#define  DIV4_POST6     0b00101101
#define  DIV1_POST6     0b00101100
#define  DIV1_POST1     0b00000100

// Each of these subroutines
//    a. save INTCON (high and low global int enab bits)
//    b. disable global interrupts
//    c. select the oscillator
//    d. set up TMR2, which is used to produce the PWM waveforms
//       and to cause interrupts to checking, e.g., the valve motor current
//    e. set up delnum param used by the newdel_10ms routine,
//       which is oscillator-dependent
//    f. restore INTCON

// These routines simply set up OSCCON, T0CON, T2CON, PR2 and osctype.

// PR2 is selected to provide a resolution of 400 in the PWM duty cycle
//  when clock is 250KHz and above.
// Below 250KHz the duty cycle resolution is unimportant because we would 
//  be idling...
// The combination of PR2 and T2CON (T2CON includes prescale (1/4/16) 
//  and postscale (..6..) ensures that the TMR2 ints occur at the proper rate 
//  of approx 10ms regardless of the oscillator choice.
// Finally T0CON is set such that the timer 0 prescaler is selected to provide
//  TMR0 functionality and accuracy largely independent of oscillator choice:
//  e.g., at 31250/250k/1m/4m clock, T0CON is set such that the prescaler is 
//   set to 2/16/64/256 thus providing a consistent counting rate of 3906.25Hz
//   and a programmable overflow time ranging from 256us up to about 16.8s
//  Accuracy is a bit compromised when running with the 3.6864MHz primary 
//   crystal, but this error is not significant.... (an 8.5% error...)

//  Interrupts disabled to prevent reentrancy
void     fosc_pri(void)
{
   uint8_t     tmp_intcon;
   tmp_intcon = INTCON & 0xC0;
   disable_interrupts(GLOBAL);
   OSCCON = 0x00;       // select primary xtal oscillator
   T0CON = T0_DIV_256;  // 278usec per tick
   T2CON = 0b01110010;  // T2 scale factor 15*16 (248usec per tick equiv)
   T4CON = 0b01110010;  // T4 scale factor 15*16 (248usec per tick equiv)
   PR2 = 99;           
   INTCON |= tmp_intcon;
}

void     fosc_pri_ISR(void)
{
   uint8_t     tmp_intcon;
   tmp_intcon = INTCON & 0xC0;
   disable_interrupts(GLOBAL);
   OSCCON = 0x00;       // select primary xtal oscillator
   T0CON = T0_DIV_256;  // 278usec per tick
   T2CON = 0b01110010;  // T2 scale factor 15*16 (248usec per tick equiv)
   T4CON = 0b01110010;  // T4 scale factor 15*16 (248usec per tick equiv)
   PR2 = 99;
   INTCON |= tmp_intcon;
}

void     fosc_4m(void)
{
   uint8_t     tmp_intcon;
   tmp_intcon = INTCON & 0xC0;
   disable_interrupts(GLOBAL);
   OSCCON = 0x52;       // select 4MHz internal oscillator
   T0CON = T0_DIV_256;  // 256usec per tick
   T2CON = 0b01111010;  // T2 scale factor 16x16 (256usec per tick equiv)
   T4CON = 0b01111010;  // T4 scale factor 16x16 (256usec per tick equiv)
   PR2 = 99;
   INTCON |= tmp_intcon;
}


void     fosc_1m(void)
{
   uint8_t     tmp_intcon;
   tmp_intcon = INTCON & 0xC0;
   disable_interrupts(GLOBAL);
   OSCCON = 0x32;       // select 1MHz internal oscillator
   T0CON = T0_DIV_64;   // 256usec per tick
   T2CON = 0b00011010;  // T2 scale factor 4*16 (256usec per tick equiv)
   T4CON = 0b00011010;  // T4 scale factor 4*16 (256usec per tick equiv)
   PR2 = 99;
   INTCON |= tmp_intcon;
}

void     fosc_250k(void)
{
   uint8_t     tmp_intcon;
   tmp_intcon = INTCON & 0xC0;
   disable_interrupts(GLOBAL);
   OSCCON = 0x12;       // select 250KHz internal oscillator
   T0CON = T0_DIV_16;   // 256usec per tick
   T2CON = 0b00000010;  // T2 scale factor 1*16 (256usec per tick equiv)
   T4CON = 0b00000010;  // T4 scale factor 1*16 (256usec per tick equiv)
   PR2 = 99;
   INTCON |= tmp_intcon;
}

void     fosc_250k_ISR(void) 
{
   uint8_t     tmp_intcon;
   tmp_intcon = INTCON & 0xC0;
   disable_interrupts(GLOBAL);
   OSCCON = 0x12;       // select 250KHz internal oscillator
   T0CON = T0_DIV_16;   // 256usec per tick
   T2CON = 0b00000010;  // T2 scale factor 1*16 (256usec per tick equiv)
   T4CON = 0b00000010;  // T4 scale factor 1*16 (256usec per tick equiv)
   PR2 = 99;
   INTCON |= tmp_intcon;
}

void     fosc_31250(void)
{
   uint8_t     tmp_intcon;
   tmp_intcon = INTCON & 0xC0;
   disable_interrupts(GLOBAL);
   OSCCON = 0x02;       // select 31250Hz internal oscillator
   T0CON = T0_DIV_2;    // 256usec per tick
   T2CON = 0b00001000;  // T2 scale factor 2*1 (256usec per tick equiv)
   T4CON = 0b00001000;  // T4 scale factor 2*1 (256usec per tick equiv)
   PR2 = 79;
   INTCON |= tmp_intcon;
}      

void     fosc_31250_ISR(void) 
{
   uint8_t     tmp_intcon;
   tmp_intcon = INTCON & 0xC0;
   disable_interrupts(GLOBAL);
   OSCCON = 0x02;       // select 31250Hz internal oscillator
   T0CON = T0_DIV_2;    // 256usec per tick
   T2CON = 0b00001000;  // T2 scale factor 2*1 (256usec per tick equiv)
   T4CON = 0b00001000;  // T4 scale factor 2*1 (256usec per tick equiv)
   PR2 = 79;
   INTCON |= tmp_intcon;
}      

void     fosc_sec(void)
{
   uint8_t     tmp_intcon;
   tmp_intcon = INTCON & 0xC0;
   disable_interrupts(GLOBAL);
   OSCCON = 0x01;       // select secondary xtal oscillator
   T0CON = T0_DIV_2;    // 244usec per tick
   T2CON = 0b00001000;  // T2 scale factor 2*1 (244usec per tick equiv)
   T4CON = 0b00001000;  // T4 scale factor 2*1 (244usec per tick equiv)
   PR2 = 79;
   INTCON |= tmp_intcon;
}

void osc_init(void)
{
// initalizes the oscillators/crystals, sets up delays, RS232, and i2c

   
   // set stack pointer to 0 in case things restart
   STKPTR = 0;
   
   #use delay(clock=3,686,400)
   #use RS232 (uart1, baud=115200, ERRORS)
   //#use RS232 (uart2, baud=9600)
   
   // start up 32.768KHz xtal oscillator
   SOSCGO = TRUE;
   // use medium freq internal osc (lower power) for 250KHz and 500KHz choices  
   MFIOSEL = TRUE;  
   
   // switch to main 250k internal osc and setup delay functions                                    
   fosc_250k();
   #use delay(clock=250KHZ)
   
   // setup i2c for use on 250khz main clock
   #use i2c(master, I2C1, FORCE_HW, NO_STRETCH, FAST=10000)
}

   
   // during initialization, except for servicing the mote, the fosc is
   //  always at 250KHz
   // The #use delay above ensures that delay_ms instructions are 
   //  compiled properly
   // Built-in compiler delay functionality works fine until mote ints
   //  occur because mote service speeds up the clock and screws up the
   //  compiler built-in delay function.....

   // The assumption is that I2C communications takes place when system 
   //  clock is set to 250KHz
   
void     setup_T0_int(uint16_t value)
{
// Use this routine to set up TMR0 such that its flag sets after 
//  the designated time.
// These are not functions that hang up processing.  They merely ensure
//  that something takes place before a programmable amount of time passes.
//
// Add more possibilities as required....
// The defines for "value" can be found in the .h file
   disable_interrupts(GLOBAL);
   TMR0ON = FALSE;
   set_timer0(value);
   TMR0IF = FALSE;
   TMR0ON = TRUE;
   enable_interrupts(GLOBAL);
}

void     setup_T2_int(uint8_t value)
{
// Use this routine during ISRs to set up TMR4 such that its flag sets
//  after the designated time.
   TMR2ON = FALSE;
   set_timer2(0);
   PR2 = value;
   TMR2IF = FALSE;
   TMR2ON = TRUE;
}

void     setup_T4_int(uint8_t value)
{
// Use this routine during ISRs to set up TMR4 such that its flag sets
//  after the designated time.
   TMR4ON = FALSE;
   set_timer4(0);
   PR4 = value;
   TMR4IF = FALSE;
   TMR4ON = TRUE;
}


void periph_init(void)
{ 
   /* Initalizes and configures all perhips, ports, timers, etc.
    * Items initalized include:
    * Interrupts
    * GPIOs
    * Pullup resistors (Port B)
    * Unused periphs in pic (turned off for lower power usage)
    * Timers
    * CCPs
    * 
    */

   // extra insurance that we aren't interrupted while setting up periph   
   disable_interrupts(GLOBAL);
   
   // get I/O lines and hardware in general into LOWEST POSSIBLE current draw
   setup_vref(VREF_OFF);
   setup_dac(DAC_OFF);
   setup_adc(ADC_OFF);
   
   // ensure that the CTMU is off
   CTMUICON = 0;
   CTMUCONL = 0;
   CTMUCONH = 0;
   
   // ensure that the SR latch is off
   SRCON0 = 0;
   SRCON1 = 0;

   // turns off comarator. Funny define from PICklkk
   setup_comparator(NC_NC_NC_NC);
   
   /* definitively establishes that TMR2 is used to generate PWM frequency
    * for P1x, P2x, P3x
    */
   CCPTMRS0 = 0;
   CCPTMRS1 = 0;    
   
   /* ADC setup (ports, configuration, reference)
    * to perform an acquisition, select the channel and then turn on the ADC
    * [ADON=TRUE]. Then do a read_adc()
    */
   setup_adc_ports(sAN0|sAN1|sAN12);
   ADCON2 = ADC_SETUP_DEF;
   setup_adc_reference(VSS_VDD);
   
   /* setup pullup resistors on Port B for Mote Uart flow and XDCR detection   
    * RB2 (MOTE_TX_RTSn)   
    */ 
    // JG: think about this in light of other port_b_pullups code!
   port_b_pullups(0b00000100);
                                          
   // turns uart off
   setup_uart(FALSE);
   
   // set the inital state and i/o characteristics for all ports
   output_a(PORTA_INIT);               
   output_b(PORTB_INIT);
   output_c(PORTC_INIT);
   output_d(PORTD_INIT);
   output_e(PORTE_INIT);
   set_tris_a(TRISA_NORM);
   set_tris_b(TRISB_NORM);
   set_tris_c(TRISC_NORM);
   set_tris_d(TRISD_NORM);
   set_tris_e(TRISE_NORM);
 
   //********************Timers setup start********************    
   
   // TMR0 for general timing functions
   setup_timer_0(T0_OFF);
   
   /* TMR1 is used in valve motion algorithm
    * It is timed by the external secondary (32.768kHz) xtal
    * Initalized in off mode
    */
   setup_timer_1(T1_ENABLE_SOSC | T1_EXTERNAL | T1_DIV_BY_1);
   TMR1ON = FALSE;
   set_timer1(0);
   
   /* TMR2 is used in all PWM generation and to produce
    * an interrupt event every approx 16msec if desired
    * TMR2 setup handled by oscillator setup macro above
    * Initalized in off mode
    */
   TMR2ON = FALSE;
   set_timer2(0);
   
   /* TMR3 is used to count seconds
    * It is timed by the external secondary (32.768kHz) xtal
    * It is set to 8000 and overflows FFFF in 1 second.
    * It is then reset to 8000 after counting 1 second after overflow
    * Initalized in on mode 125ms before overflow (F000)
    */
   setup_timer_3(T3_ENABLE_SOSC | T3_EXTERNAL | T3_DIV_BY_1);  
   TMR3ON = FALSE;
   set_timer3(0);
     
   // TMR4 is not used
   setup_timer_4(T4_DISABLED,0,1);
   
   /* TMR5 is dedicated to measurement of RPM
    * It is timed by the external secondary (32.768kHz) xtal
    * Initalized in off mode
    */
   setup_timer_5(T5_ENABLE_SOSC|T5_EXTERNAL|T5_DIV_BY_1);
   TMR5ON = FALSE;
   set_timer5(0);
   
   // TMR6 is not used
   //setup_timer_6(T6_DISABLED,0,1);
   TMR6ON = FALSE;
   set_timer6(0);
   
   // sets up TMR2, TMR0, TMR6 CON  
   T0CON = T0_DIV_16;  
   T2CON = DIV1_POST6;
   T6CON = DIV1_POST6;                 
   PR2 = 99;
   PR6 = 99;  
   
   //********************Timers setup end********************    
   
   //********************CCP setup start********************    
   
   //setup_ccp1(CCP_USE_TIMER1_AND_TIMER2 | CCP_PWM_L_L | CCP_PULSE_STEERING_A);
   // CCP1(pwm with TMR2) and P1A are specifically for braking control.
   // Set as above when in use.
   // When off, the BRK_CTRLn line can be set as an I/O for
   //  either max brake (lo) of min (hi) brake.
   // Note that duty cycle 0 for minimum braking, 
   //  duty cycle=400 for (almost) maximum braking.
   setup_ccp1(CCP_OFF);          // CCP1 off for now
   // P1A (BRAKE_CTRLn) is initialized to brakes off (1)
 
   //setup_ccp2(CCP_USE_TIMER1_AND_TIMER2 | CCP_PWM_H_H | CCP_PULSE_STEERING_A);
   // CCP2(pwm with TMR2) and P2A are specifically for control of charging.
   // Set above when in use.
   // When off, the LTC3105_CTRL line can be set as an I/O for 
   //  either max charging (hi) or min (lo) charging.
   // Note that duty cycle 0 for no charging,
   //  duty cycle=400 for (almost) max charging.
   setup_ccp2(CCP_OFF);          // CCP2 off for now
   // P2A (LTC3105_CTRL) is initialized to max charging aggressiveness (1)
   
   //setup_ccp3(CCP_USE_TIMER1_AND_TIMER2 | CCP_PWM_L_L | CCP_PULSE_STEERING_A);
   //setup_ccp3(CCP_USE_TIMER1_AND_TIMER2 | CCP_PWM_L_L | CCP_PULSE_STEERING_B);
   //setup_ccp3(CCP_USE_TIMER1_AND_TIMER2 | CCP_COMPARE_RESET_TIMER);
   // CCP3(compare)/TMR1 is for valve timing; 
   // CCP3(pwm)/TMR2 if for valve motor speed control
   //Steer output to P3A for closing, steer output to P3B for opening;
   // both VLV_CTRL1 (P3A) and VLV_CTRL2 (P3B) hi for braking;
   // both VLV_CTRL1 (P3A) and VLV_CTRL2 (P3B) lo for coasting;
   // pulse P3A lo while P3B hi for closing, pulse P3B while P3A hi for opening
   //Compare mode used to time valve motion after ramp is finished.
   //  Set CCPf_3 to 0x8000 and start TMR1 for 1sec interrupts; or use other
   //  values for different timings.  General purpose timing when valve not
   //  being timed.
   setup_ccp3(CCP_OFF);          // CCP3 off for now

   //setup_ccp4(CCP_USE_TIMER3_AND_TIMER4 | CCP_COMPARE_RESET_TIMER); 
   // CCP4/TMR3 serves only one purpose: the real-time-clock.
   //  Set CCP_4 to 0x8000 and start TMR1 for 1sec interrupts
   
   setup_ccp4(CCP_OFF);          // CCP4 off for now
   
   //setup_ccp(CCP_USE_TIMER5_AND_TIMER6 | CCP_CAPTURE_RE);
   // CCP5/TMR5 serves only one purpose: measurement of spin rate. 
   setup_ccp5(CCP_OFF);

   //********************CCP setup end********************   
   
   //********************INT setup start******************  
   
   /* Diables:
    * Global int (all ints)
    * All peripheral ints
    * TMR0 overflow int
    * INT0 ext int
    * Port B IOC int
    * Clears:
    * TMROIF, INT0IF, RBIF
    */
   INTCON = 0;
   
   // PORTB pullups are enabled
   // INTEDG0,1,2 on falling edge
   // TMR0 and RBIP priority  is low
   INTCON2 = 0;
   
   // External INT1, INT2 disabled/cleared and set to low priority                              
   INTCON3 = 0;
   
   // Int priority enabled (part of RCON)
   IPEN = TRUE;
   
   // Clear all peripheral interrupts
   PIR1 = 0;                           
   PIR2 = 0;
   PIR3 = 0;
   PIR4 = 0;
   PIR5 = 0;
   
   // disable all peripheral interrupts
   PIE1 = 0;                          
   PIE2 = 0;
   PIE3 = 0;
   PIE4 = 0;
   PIE5 = 0;
     
   //   IPR1 = 0;
   //   IPR2 = 0;
   //   IPR3 = 0;
   //   IPR4 = 0;
   //   IPR5 = 0;

   // setup IOC to call INT_RB only on RB4 change
   IOCB = 0b00010000;     
   
   //********************INT setup end******************** 
                               
   // initialize I2C
   i2c_init(TRUE);
                         
   // initalizes XDCR, sets global xdcr_enabled
   adc_XDCR_init();                                    
        
   // initalize LCD, sets global lcd_enabled
   LCD_init();
   
   // set mppc value to 0                 
   set_mppc(global_mppc_value);
   
   // set battery charge cycle to 100%  
   set_charge_duty(global_charge_duty);
   
   // initalize fixture, sets global fixture_enabled
   fixture_init();
   
   // Turn on LTC1541 and MCP4452 
   output_high(AUX_PWR);
   
   /* This iS for power supply settling etc..
    * pause for 125ms or longer; should not hang;
    */
   delay_ms(125);
   

    
}

void vars_init(void)
{
// initalizes all global variables to default values from globals.h

////////////////////Global Variables Table of Contents/////////////////////////
//Test Globals
//LCD Globals
//Time Globals
//Hardware Enabled Globals
//Control Related Globals
//Mote Globals
//Priority Queue
//Time Queue
//Sprinkler Stack
//Command Stack
//System State Globals

////////////////////Test Globals///////////////////////////////////////////////
   test_global = 0;
   test_global_2 = 0;
   test_start_time = 0;
   test_end_time = 0;
   test_valve_open_1 = 0;
   test_rpm_1 = 0;
   test_duty_cycle_1 = 0;
   test_valve_open_2 = 0;
   test_rpm_2 = 0;
   test_duty_cycle_2 = 0;
   test_valve_open_3 = 0;
   test_rpm_3 = 0;
   test_duty_cycle_3 = 0;
 
////////////////////LCD Globals////////////////////////////////////////////////
   memset(global_temp_line_buff, 0, sizeof global_temp_line_buff);
   global_extra_lcd_ptr = NULL;

////////////////////Time Globals///////////////////////////////////////////////
   // keeps track of time since microprocessor has been awake in seconds
   global_rtc_time = 0;
   // keeps track of the time within the mote system (taken from mote)
   global_utc_time = 0;

////////////////////Hardware Enabled Globals///////////////////////////////////
   global_lcd_enabled = 0;
   global_xcdr_type1_enabled = 0;
   global_xcdr_type2_enabled = 0;
   global_fixture_enabled = 0;
   global_dust_enabled = 0;

////////////////////Control Related Globals////////////////////////////////////
   // keeps track of rpms (updated via interrupt routine)
   global_current_period = 0;
   global_rpm_set_value = 100;
   // keeps track of duty cycles of brake and charge
   global_brake_duty = 0;
   global_charge_duty = MAX_CHARGE;
   // keeps track of mppc level
   global_mppc_value = 100;
   // keeps track of vgen
   global_vgen = 0;
   // keeps track of fixture dac value
   global_fixture_dac_value = NO_FIXTURE;

////////////////////Valve Related Globals//////////////////////////////////////
// keeps track of current valve position
   global_valve_position = VLV_POSITION_UNKNOWN;

////////////////////Mote Globals///////////////////////////////////////////////
   //event flags defined by mote
   event_flags = 0;

   // Note that bytes_from_mote union contains the stripped unstuffed data
   //  eg, bytes_from_mote.dust_rcvd[128]
   
   memset (&bytes_from_mote, 0, sizeof bytes_from_mote);
   p = NULL;

   // It just can't get bigger than this!
   // pkgen places message to be sent here
   memset(dust_send_buffer, 0, sizeof(dust_send_buffer));

   // this is only for doing acks and only needs to be 
   // MAX_USER_DATA_SIZE*2 + 2 + 4 + 12 + 2
   // MAX_USER_DATA_SIZE is 2 bytes for ack
   memset(dust_send_buffer_isr, 0, sizeof dust_send_buffer_isr);

   // THIS is the buffer for RAW rcvd mote
   //  mote packet data
   memset(dust_recive_buffer, 0, sizeof dust_recive_buffer);

   //  this is place
   //  where the packet to be sent by micro
   //  to NETWORK manager is placed prior
   //  to dust_send_data call
   memset(payload_buff, 0, sizeof payload_buff);

   // Keeps track of how many bytes to send and passed between pkgen and pksend
   num_of_bytes_to_send = 0;
   num_of_bytes_to_send_isr = 0;

   // This is specific to the subroutine
   //  pkgen_send() which creates the message
   //  to be sent from the mote to the net                     
   send_prefix[0] = CMD_SEND;
   send_prefix[1] = 0;
   send_prefix[2] = FLAG_SEND_REQUEST;
   send_prefix[3] = DESTADDR_HI;
   send_prefix[4] = DESTADDR_LO;
   send_prefix[6] = APPDOMAIN_MAINTENANCE;
   send_prefix[7] = PRIORITY_HI;
   send_prefix[8] = 0;
   send_prefix[9] = 0;
   send_prefix[10] = 0xFF;
   send_prefix[11] = 0;
   
   // send_prefix[11] will be set to number of bytes in packet   
      // The send_prefix structure is as follows:
      // [0]      CMD_SEND
      // [1]      length of request       - this is payload length + 9
      // [2]      FLAG_SEND_REQUEST
      // [3]/[4]  destination address     - always DESTADDR_HI, DESTADDR_LO
      // [5]      svc_index               - get this from service indication received
      //                                     at join time
      // [6]      APPDOMAIN_MAINTENANCE
      // [7]      PRIORITY_HIGH
      // [8][9]   reserved                - 0x00, 0x00 
      // [10]     sequence_num            - always 0xFF for mote-originated packets
      // [11]     payload length          - length of the payload in bytes


   // For ensuring a message was recieved properly from mote
   result_rcvd = 0;

   // Flag for going through deal with mote ISR and going to sleep afterwards
   fl_mote_dealt = 0;
   fl_alt_LCD = 0;

   // Flag for a sucessful get/set response eliminating a few other flags
   global_set_get_response = 0;

   // FLAGS TO GET RID OF!
   // Flag for receiving an advertisment
   advert_rcvd_fl = 0;
   setnetid_done_fl = 0;
   ack_sent_fl = 0;
   pkt_rcvd_fl = 0;
   joinduty_done_fl = 0;
   gettime_done_fl = 0;
   fl_batNOEE = 0;
   joinkey_done_fl = 0;
   joincmd_done_fl = 0;

   flags_rcvd = 0, param_type_rcvd = 0, svc_index_rcvd=0;
   mote_msgSEQ = 0;

   //Globals for when advertisements are received
   network_id_rcvd = 0;
   mote_id_rcvd = 0;
   rssi_rcvd = 0;
   hop_depth_rcvd = 0;

   // More globals that we might actually want to keep
   memset(serial_number_rcvd, 0, sizeof serial_number_rcvd);
   memset(macaddr_rcvd, 0, sizeof macaddr_rcvd);

////////////////////Priority Queue/////////////////////////////////////////////
   // Actual priority queue
   memset(global_priority_queue, 0, sizeof global_priority_queue);
   global_current_priority_queue_location = (0-1);

////////////////////Time Queue/////////////////////////////////////////////////
   // Actual time queue
   memset(global_time_queue, 0, sizeof global_time_queue);
   global_current_time_queue_location = (0-1);

////////////////////Sprinkler Queue////////////////////////////////////////////
   // Actual sprinkler queue
   memset(global_sprinkler_queue, 0, sizeof global_sprinkler_queue);

////////////////////Command Queue//////////////////////////////////////////////
   // Actual command queue
   memset(global_command_queue, 0, sizeof global_command_queue);

////////////////////System State Variables/////////////////////////////////////
   global_mote_state = MOTESTATE_UNKNOWN;
   global_system_state = SYSTEM_INIT;
}

void deep_sleep(void)
{  // zzzzzzz.......

// Ways out of deep sleep: MCLR button, V_GEN_INT (int1)
// SW1 DOES not wake it up...

   disable_interrupts(GLOBAL);
   strcpy (global_temp_line_buff, "Enter Deep Sleep");
   LCD_line1(global_temp_line_buff);
   strcpy (global_temp_line_buff, "Reason:         ");
   global_temp_line_buff[8] = hi2asc(global_shutdown_cause);
   global_temp_line_buff[9] = lo2asc(global_shutdown_cause);
   LCD_line2(global_temp_line_buff);     
   
   setup_T0_int(T0_6S);
   while (!TMR0IF);
   
   LCD_shutdown();
   output_a(PORTA_DEEP);
   output_b(PORTB_DEEP);
   output_c(PORTC_DEEP);
   output_d(PORTD_DEEP);
   output_e(PORTE_DEEP);
   set_tris_a(TRISA_NORM);
   set_tris_b(TRISB_NORM);
   set_tris_c(TRISC_NORM);
   set_tris_d(TRISD_NORM);
   set_tris_e(TRISE_NORM);

   setup_vref(VREF_OFF);
   setup_dac(DAC_OFF);
   setup_adc(ADC_OFF);
   setup_CCP1(CCP_OFF);
   setup_CCP2(CCP_OFF);
   setup_CCP3(CCP_OFF);   
   setup_CCP4(CCP_OFF);
   setup_CCP5(CCP_OFF);
   setup_comparator(NC_NC_NC_NC);
   
   clear_interrupt(INT_TIMER0);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_timer_3(T3_DISABLED);
   setup_timer_4(T4_DISABLED,0,1);
   setup_timer_5(T5_DISABLED);
   setup_timer_6(T6_DISABLED,0,1);
   
   setup_wdt(WDT_OFF);   

   setup_uart(FALSE);

   INTCON = 0;                         // TMR0, INT0. IOC ints disabled/cleared
                                       //  High and Low Priority ints disabled
   INTCON2 = 0;                        // RB port change int is low priority
                                       //  PORTB pullups are enabled
   INTCON3 = 0;                        // INT1, INT2 ints disabled/cleared
   IPEN = TRUE;                        // Int priority enabled (part of RCON)
   
   PIR1 = 0;                           // clear various interrupt flags
   PIR2 = 0;
   PIR3 = 0;
   PIR4 = 0;
   PIR5 = 0;
   
   PIE1 = 0;                           // disable various interuupts
   PIE2 = 0;
   PIE3 = 0;
   PIE4 = 0;
   PIE5 = 0;
     
   IPR1 = 0;
   IPR2 = 0;
   IPR3 = 0;
   IPR4 = 0;
   IPR5 = 0;  
   
   IOCB = 0;                     // disable all PORTB interrupt on change
                                 //  functionality
   setup_adc_ports(NO_ANALOGS);                              
   clear_interrupt(INT_EXT);
   //clear_interrupt(INT_EXT2);
   enable_interrupts(INT_EXT_L2H); 
   //enable_interrupts(INT_EXT2_H2L); 
   enable_interrupts(GLOBAL);
   delay_ms(1000); 
     
   sleep(SLEEP_FULL);

   //reset_cpu();
   // wake up because of generator spin or SW1/Hall effect
   if (input(V_GEN_INT))
   {
   //   reset_cause = SPIN_RESTART;
   // otherwise it was the SW1/Hall effect that woke it up
      reset_cpu();
   }
   // deep sleep is exited by a MCLR, or V_GEN_INT (SPIN RESTART)
   // If for some reason it woke up, but not because of a SPIN_RESTART
   //  then simply go back to deep_sleep
   
   
} 


void start_rtc(void)
{
   global_rtc_time = 0;
   CCP_4 = 0x8000;
   setup_ccp4(CCP_USE_TIMER3_AND_TIMER4 | CCP_COMPARE_RESET_TIMER);
   CCP4IF = FALSE;
   setup_timer_3(T3_ENABLE_SOSC | T3_EXTERNAL_SYNC | T3_DIV_BY_1);
   set_timer3(0);   
   clear_interrupt(INT_CCP4);
   enable_interrupts(INT_CCP4);
}

void safeI2C_start(void)
{
   setup_T0_int(T0_10MS);
   while (I2CBUSY && !TMR0IF);
   if (!I2CBUSY)
      i2c_start();
   else
   {
      BCL1IF = FALSE;
      WCOL = FALSE;
      i2c_init(TRUE);
      i2c_start();
   }
      
}

void safeI2C_write(uint8_t value)
{
   // thinking that a spike on the data or clock line is f'ing up
   // i2c transmission, this routine simply looks for the ACK and if
   // it does not see it, tries ONE MORE TIME and one more time only!
   int1  unsuccess_fl;
   
   unsuccess_fl = i2c_write(value);
//   if (unsuccess_fl)
//      i2c_write(value);
}    

void setup_rb4_int(void)
{
   // sets up the RB4 interrupt
   RBIF = FALSE;
   enable_interrupts(INT_RB4);      
}     
     
void write_system_state(uint8_t system_state_to_change_to) 
{               
   // Modifies the global_system_state varible outside of the ISRs
   // Also deals with the transition from SYSTEM_IDLE to SYSTEM_RUN and back
                                                                   
   // If going from SYSTEM_IDLE to SYSTEM_RUN   
   if ((global_system_state == SYSTEM_IDLE) && (system_state_to_change_to == SYSTEM_RUN)) 
   {                           
      // change processor speed? 
      //fosc_250k(); 
   } 
   // If going from SYSTEM_RUN to SYSTEM_IDLE   
   else if ((global_system_state == SYSTEM_RUN) && (system_state_to_change_to == SYSTEM_IDLE))               
   {
      // change processor speed?
      //fosc_31250();            
   } 
   // If going from SYSTEM_IDLE to anything else   
   else if ((global_system_state == SYSTEM_IDLE) && (system_state_to_change_to != SYSTEM_IDLE))
   {     
      // change processor speed? 
      //fosc_250k();     
   }                         
   // actually change the state
   global_system_state = system_state_to_change_to;
}      

uint8_t read_system_state(void)       
{
   // safely read the system state outside of ISRs
   return global_system_state;
}
  
