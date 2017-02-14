//  Last compiled with CCS Version 5.055 PICC compiler
//   Schematic versions:  PCB rev 6 (3.6864 MHz Primary crystal)
//                                  (32.768 KHz Secondary crystal)
// Rev D101
// Started March 9, 2016
// First work with latest PCB Rev8
//
// Rev D102
// Started March 13, 2016
// Demo for Mike DeFrank visit March 16/17
// Some Manager communications established for testing and demonstration
//
// Rev D103
// Started Mar 15, 2016
// Cleaned out unused messages, improved message processing flow
// Demo'd at team meeting Mar 18, 2016
// Includes valve open/close/freespin prototype code
//
// Rev D104
// Started Mar 20, 2016
// First attemp at valve adjust algorithms
//
// Rev D105
// Started Mar 24, 2016
// Quick test to ensure I2C communications with daughter board work... 
//
// Rev D106
// Started Mar28, 2016
// Debug v_current fl_move_valve, etc
//
// Rev D107
// Started Mar 28, 2016
// Brian Lewis re-write



//*********************Restart Info:********************
/*
// restart causes built into compiler:
#define WDT_TIMEOUT           0x07  // not used RCON = 0bxxx10111
#define MCLR_FROM_SLEEP       0x0B  // RCON = 0bxxx11011
                              //  like WARM_RESTART
#define MCLR_FROM_RUN         0x0F  // RCON = 0bxxx11111
                              //  like WARM_RESTART
#define NORMAL_POWER_UP       0x0C  // RCON = 0bxxx11100
                              // This may or MAY NOT have 
                              //  involved power cycling
                              //  because of the way the programmer
                              //  works in the lab during development
                              // Treat like WARM START but look
                              //  and look for condition requiring 
                              //  a mote reset.
#define BROWNOUT_RESTART      0x0E  // not used RCON = 0bxxx11110
                              //  use default WARM_RESTART
#define WDT_FROM_SLEEP        0x03  // not used RCON = 0bxxx10011
                              //  use default WARM_RESTART
// other restart_cause values are unexpected
                              // RCON3:0 = 1,2,4,5,6,8,9,A,D
                              //  use default WARM_RESTART
                        
#define RESET_INSTRUCTION     0x00  // RCON = 0bxxx0xxxx [all cases below]

// these are the reset_cause values, set before any reset_cpu instruction
// The WARM_RESTART process is the default
// Whenever SW1 is pressed, there is a search and rejoin process
//  to establish a new center pivot affiliation
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



//********************BATTERY HANDLING ON BOOT********************

// if the battery is too weak to work the valve, we will carry on, but
//  flag tells us that we CANNOT actually run
//  HOWEVER we will get out that first message... manager can decide to 
//   to shut her down.

// if we have gotten this far, the battery is at least good enough to
//  allow to abruptly reset the radio, command the radio to do a search 
//  for the loudest manager signal, store that network ID and then go 
//  into deep_sleep, from which only a MCLR or SPIN_RESTART gets us going
//  again.
