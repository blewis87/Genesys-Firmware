// required device includes/settings
#include <18F46K22.h>
#device adc=10 
#device high_ints=TRUE
                 
// standard libraries
#include <stdio.h>
#include <stdint.h>               
#include <stdlib.h>
#include <string.h>
#include <stdlibm.h>

// configuration files  
#include "pic_config.h"
#include "defines.h"
#include "function_headers.h"                  
#include "jack_dn2500.h"
#include "globals.h"                               
#include "eeprom.h"
 
// specific headers                   
#include "pic.h"
#include "dust.h"                                       
#include "periph.h"                                                      
#include "control.h"                     
#include "valve.h"       
#include "battery.h"                                                          
#include "stacks_queues.h"  
#include "util.h"
                  
void main()
<<<<<<< HEAD
{      

   // some change
   int i;
   
   // priority queue item that is to be executed
=======
{                                                         
   // priority queue item that is to be executed 
>>>>>>> parent of 136b246... E313 commit
   uint8_t     priority_queue_item_to_execute = EMPTY_PRIORITY_QUEUE;    
   // valve setting that is stored when free spin rate is performed (so you can return to it afterwards)
   uint16_t    pre_fsr_valve_setting;                                         
                                       
   // set system state to init
   write_system_state(SYSTEM_INIT);              
               
   // initalize oscillator and timing of rs232, i2c, delay
   osc_init();         

   // initalize all variables (not really needed as they are initalized properly when CPU is rebooted)                                                        
   //vars_init();                  
   
   // store string version of firmware version in global_firmware_version_string                                                           
   global_firmware_version_string[0] = hi2asc(make8(FIRMWARE_VERSION,1));
   global_firmware_version_string[1] = lo2asc(make8(FIRMWARE_VERSION,1));
   global_firmware_version_string[2] = hi2asc(make8(FIRMWARE_VERSION,0));      
   global_firmware_version_string[3] = lo2asc(make8(FIRMWARE_VERSION,0));   

   // grab all eeprom values (e.g. vlv cal, sprinkler number, vlv position)
   read_all_eeprom_values();   
   
   // load all changeable eeprom variables 
   read_all_eeprom_variables();        
                               
   // initalize all periphs, timers, ccps, ints   
   periph_init();
                                    
   // clear queues    
   clear_priority_queue();                                 
   clear_time_queue();
   clear_message_queue();      
   
   // command queue is not needed/used yet as it is used in conjunction with the "modulation" scheme
   //allocate_command_queue();
   //clear_command_queue();
   
   // startup rtc, turn on gen_rpm, enable dust chip (check for communication)
   start_rtc();   
   setup_gen_rpm();        
   mote_init();
                         
   // if cold start was done, reset mote   
   if (global_previous_shutdown_cause == COLD_RESTART_REQUEST)
   {
      mote_reset();          
   }         
                         
   // DELETE?                             
   enable_interrupts(GLOBAL);                     
                                      
   // if SW1 is asserted during boot, trigger searching alogrithm               
   if (!input(SW1n))             
   {                                            
      // set the system state up for searching for a network (not run)
      write_system_state(SYSTEM_SEARCHING_FOR_NETWORK);
      strcpy (global_temp_line_buff, "Setup Manager...");
      LCD_line1(global_temp_line_buff);
      strcpy (global_temp_line_buff, "  Release SW1   ");
      LCD_line2(global_temp_line_buff);     
      // wait for switch release 
      while (!input(SW1n));      
      // delay lcd update for a tiny bit                                                                                                                             
      global_skip_lcd_update_count = 1;
      // first part of search alorithm sequence                                                                                      
      PUSH_PRIORITY_QUEUE_MACRO(SEARCH_FOR_STRONGEST_1);                 
   }                                 
   
   // normal behavior, check mote status.  Join network if necessary.                                                                                                      
   else 
   {                                 
      // Fun startup splash screen (and clear startup screen)
      //LCD_startup_splash();       
      //LCD_clear();                  
      
      // enable RB4 button press functionality      
      setup_rb4_int();        
                                                                                   
      // dispaly battery voltage (line 0) and restart/shutdown cause (line 1) on screen for a second 
      LCD_display_battery_voltage(0);    
      LCD_display_shutdown_cause(1);            
       
      // global_skip_lcd_update_count is our way of letting things stay on the lcd for a moment
      //    You set it to the number of times to skip an LCD update + 1
      //    e.g. setting it to 2 means things stay on the screen for 1-2 seconds and then they are cleared
      //    e.g. setting it to 1 means things are cleared right away
      //    e.g. setting it to 0 (or not setting it) means the LCD screen is just overwritten by LCD update
      global_skip_lcd_update_count = 2;        
      
      // all periphs should be initalized (except mote), so system should be ready to run        
      write_system_state(SYSTEM_RUN);
                                      
      // flash the leds to help identify motes   
      happy_lites();
      
      // check mote state. If it needs to connect, it will react as it should           
      PUSH_TIME_QUEUE_MACRO(global_rtc_time + 5, CHECK_MOTE_STATE);  
      
      // check mote info
      PUSH_TIME_QUEUE_MACRO(global_rtc_time + 10, UPDATE_MOTE_NETWORK_INFO);
            
      // setup WDT for 1 second (plenty long for the control loop (~64ms) to   
      setup_wdt(WDT_1S);     
   }                                                                 
                                                             
   // start the queue/control loop timer
   setup_T2_int(T2_64MS);    
       
    
   // HACKY LAB STUFF TO REMOVE LATER
   global_valve_position = VLV_PRECALIBRAION_POSITION; 
   global_valve_position_set_value = VLV_PRECALIBRAION_POSITION;                        
   global_valve_time_to_close_1024th = DEFAULT_VLV_TIME_TO_CLOSE;                        
   global_valve_time_to_open_1024th = DEFAULT_VLV_TIME_TO_OPEN;                        
                           
   while(1)                   
   {               
      // restart the watchdog timer    
      restart_wdt();    
  
                
////////////////////Start of Priority Queue Handling///////////////////////////
                                                                      
      // Timer 2 is used to signal the priority queue to check for another item
      //    to execute. This is only done once every 64ms to NOT keep the 
      //    system in a state with the interrupts disabled all the time.
      // Timer 2 is also used to time the control loop.  How often the set point
      //    is re-evaluated and adjustments are made to the actual values is
      //    controlled by control_loop_delay_cycles_left within update_control_loop()
      
      // if it is time to check priority queue/control loop
      if (TMR2IF) 
      {  
         // reset the timer 2 queue loop interrupt
         setup_T2_int(T2_64MS);      
                            
         // update the control loop if the system isn't in the system wait (low-power) state
         if (read_system_state() != SYSTEM_IDLE) update_control_loop();           

         // Safely grab the next item in the priority queue to execute                
         // All interrupts that can modify the queue need to be disabled to                            
         //    ensure memory is not corrupted.      
         disable_interrupts(INT_CCP4);
         disable_interrupts(INT_EXT2);       
         priority_queue_item_to_execute = pop_priority_queue();   
         enable_interrupts(INT_EXT2);   
         enable_interrupts(INT_CCP4);             
                                                      
         // execute the priority queue item    
         switch (priority_queue_item_to_execute)                               
         {
            // Decode a new packet and react/respond appropriately
            case DEAL_WITH_NEW_PACKET:            
               // Display that you're dealing with a new packet
               LCD_clear();                                 
               strcpy (global_temp_line_buff, "Deal With Packet");
               LCD_line1(global_temp_line_buff);
               global_skip_lcd_update_count = 2;     
                                                 
               // disable ccp4 and mote interrupt so we don't overwrite payload_buff
               //    or have colliding unsolicited messages
               disable_interrupts(INT_CCP4);
               disable_interrupts(INT_EXT2_H2L);
               deal_with_packet(); 
               enable_interrupts(INT_EXT2_H2L);                       
               enable_interrupts(INT_CCP4);
               break;
               
            // Start the somewhat convoluted calibrate valve routine
            // 1. Open valve VLV_CAL_1_MOVEMENT w/ "starting current"
            // 2. Close valve fully w/ current being "normal closing current"
            // 3. Open valve fully w/ normal current regimes
            // 4. Do free spin rate (not done in here at the moment, just skipped). 
            // 5. Close valve fully w/ normal current regimes
            // 6. Send valve calibration response to mote   
            // steps and system states are handled in COMP and CCP3 ISR as well 
            //    as setting the calibration values      
                        
            case CALIBRATE_VALVE_1:
               // Change system state: initial open for calibrate valve routine
               write_system_state(SYSTEM_CAL_VLV_1);    
               // display calibration routine on screen     
               LCD_clear();                         
               strcpy (global_temp_line_buff, "VLV Calibration ");
               LCD_line1(global_temp_line_buff);
               strcpy (global_temp_line_buff, "Begining........");
               LCD_line2(global_temp_line_buff);
               global_skip_lcd_update_count = 2;  
               // setup brakes and charging for valve calibration
               // (maximum resistance with no RPM Control)
               global_control_loop_mechanism = NO_RPM_CONTROL_DYN_MPPC;
               global_charge_duty_set_value = MAX_CHARGE;
               global_brake_duty_set_value = MAX_BRK;
               // put values to default values (45 seconds open/close)                               
               global_valve_time_to_close_1024th = DEFAULT_VLV_TIME_TO_CLOSE; 
               global_valve_time_to_open_1024th = DEFAULT_VLV_TIME_TO_OPEN;              
               // Set the global valve position to the default value (middle)                 
               // this gives the valve a reference point to open a little from
               global_valve_position = VLV_PRECALIBRAION_POSITION;
               // Set valve position slightly more open than it is and move valve
               global_valve_position_set_value = (VLV_PRECALIBRAION_POSITION +                        \
               VLV_CAL_1_MOVEMENT);
               PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);
               break;                                                      
               
            case CALIBRATE_VALVE_2:                                                                                                       
               // Change system state: fully closed
               write_system_state(SYSTEM_CAL_VLV_2);
               // set valve position target to fully closed and move valve
               global_valve_position_set_value = VLV_POSITION_CLOSED;
               PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);                                 
               break;   
               
            case CALIBRATE_VALVE_3:
               // Change system state: fully closed to fully opened
               write_system_state(SYSTEM_CAL_VLV_3);
               // set valve position target to fully open and move valve
               global_valve_position_set_value = VLV_POSITION_OPENED;
               PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);
               break;     
            
           case CALIBRATE_VALVE_4: 
               // Change system state: FSR (not used at the moment)
               write_system_state(SYSTEM_CAL_VLV_4); 
               // move to the next calibration stage
               PUSH_PRIORITY_QUEUE_MACRO(CALIBRATE_VALVE_5);
               break;      
               
            case CALIBRATE_VALVE_5:
               // Change system state: fully opened to fully closed
               write_system_state(SYSTEM_CAL_VLV_5);
               // set valve position target to fully closed and move valve
               global_valve_position_set_value = VLV_POSITION_CLOSED;
               PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);
               break;     
            
            case CALIBRATE_VALVE_6:
               // Change system state: display calibration and send to manager
               write_system_state(SYSTEM_CAL_VLV_6);
               // display calibraion on screen
               LCD_clear();
               strcpy (global_temp_line_buff, "Close CCP=      ");
               LCD_line1(global_temp_line_buff);
               strcpy (global_temp_line_buff, "Open CCP =      ");
               LCD_line2(global_temp_line_buff);
               LCD_place_uint16(global_valve_time_to_close_1024th,0,11,5);
               LCD_place_uint16(global_valve_time_to_open_1024th,1,11,5);
               // put calibration stuff on screen for 4 seconds
               global_skip_lcd_update_count = 5;     
               // if valve calibration time is below the limit, it triggers an error
               //    and throws away the calibration, returning it to the run state.
               if ((global_valve_time_to_close_1024th < ERROR_VLV_CAL_TIME) || \
                  (global_valve_time_to_open_1024th < ERROR_VLV_CAL_TIME))
               {
                  // reset valve calibration times to the defaults
                  global_valve_time_to_open_1024th = DEFAULT_VLV_TIME_TO_OPEN;
                  global_valve_time_to_close_1024th = DEFAULT_VLV_TIME_TO_CLOSE;
                  // change valve position to unknown
                  global_valve_position = VLV_POSITION_UNKNOWN;
                  // set the error bitfield and send an error
                  global_error_message_bitfield |= ERR_MSG_VLV_CAL_FAIL;
                  PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_ERROR_MSG);
               }
               // successful/valid calibration time
               else
               {
                  // update the calibration time
                  global_valve_calibration_utc_time = global_utc_time;
                  // store calibrations in eeprom
                  store_vcal_eeprom_values();
                  // send an unsolicited valve report to the manager
                  PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_VALVE_REPORT);
               }
               // put system in run state
               write_system_state(SYSTEM_RUN);
               break;     
                           
            // FSR IS LARGELY UNTESTED EVEN THOUGH IT'S BASICALLY ALL WRITTEN!               
            // move valve to position specified by calibrate FSR routine   
            case CALIBRATE_FSR_1:
               // set system state                      
               write_system_state(SYSTEM_CAL_FSR_1);                                         
               // save the current valve, so we can return to it later
               pre_fsr_valve_setting = global_valve_position;     
               // move valve to the FSR position                         
               global_valve_position_set_value = global_calibrate_fsr_valve_position; 
               PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);                            
               break;         
                                                                                        
            case CALIBRATE_FSR_2:
               // set system state
               write_system_state(SYSTEM_CAL_FSR_2);   
               // disable ccp5 for a quick second
               disable_interrupts(INT_CCP5);
               // actually measure the FSR and store it 
               global_calibrate_fsr_period = global_current_period;   
               // restore ccp5
               enable_interrupts(INT_CCP5);     
               // store the fsr calibration time
               global_calibrate_fsr_utc_time = global_utc_time;   
                                    
               // move valve to the FSR position
               global_valve_position_set_value = pre_fsr_valve_setting;  
               PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);    
                                                 
               PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_VALVE_REPORT);
               write_system_state(SYSTEM_RUN);
               break;                                  
               
            case CALIBRATE_FSR_3:
               write_system_state(SYSTEM_CAL_FSR_3);
               break;        
            /*   
            case CALIBRATE_FSR_4:
               write_system_state(SYSTEM_CAL_FSR_4);
               LCD_clear();
               LCD_place_uint16(global_valve_position,0,0,5);
               LCD_place_uint16(global_valve_time_to_close_1024th,0,6,5);
               LCD_place_uint16(global_valve_time_to_open_1024th,0,11,5);
               LCD_place_uint16(global_valve_position,1,0,5);
               LCD_place_uint16(global_valve_position,1,11,5);
               LCD_place_uint16(global_valve_position,1,11,5);
               // put calibration stuff on screen for 4 seconds
               global_skip_lcd_update_count = 5;
               global_calibrate_fsr_utc_time
               
               write_system_state(SYSTEM_RUN);
               break;         
               */            
               
               
            case MOVE_VALVE_MAG_DECOUPLING_RECOVERY:
               // move the valve to VLV_MAGNETIC_COUPLING_FIX
               global_valve_position_set_value = VLV_MAGNETIC_COUPLING_FIX;
               PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);
               // change control loop scheme 
               global_control_loop_mechanism = MAG_DECOUPLING_RECOVERY;
               break;
               
            case MOVE_VALVE_NO_SPIN_RECOVERY:
               // move the valve to VLV_NOT_SPIN_FIX
               global_valve_position_set_value = VLV_NOT_SPIN_FIX;
               PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);
               // change control loop scheme
               global_control_loop_mechanism = NO_SPIN_RECOVERY;
               break;
                                              
            // Opens valve irrespective of the position. Used for shutting down the system (and leaving
            //    the valve open) and for test purposes.  Added very quickly in Nebraska.                 
            case OPEN_VALVE_UNKNOWN_STATE:                            
               // clear out the time in motion
               global_valve_time_in_motion_1024ths = 0;     
               
               global_valve_position_set_value = VLV_POSITION_OPENED;
               CCP_3 = global_valve_time_to_open_1024th; 
               // start the valve opening
               mV_OPENm;         
               // setup/turn on the comparator interrupt (also sets up DAC)
               comparator_setup();
               
               // setup and enable CCP3 as well as it's respective timer
               setup_ccp3(CCP_USE_TIMER1_AND_TIMER2 | CCP_COMPARE_RESET_TIMER);
               setup_timer_1(T1_ENABLE_SOSC | T1_EXTERNAL_SYNC | T1_DIV_BY_1);
               set_timer1(0);
                                                                         
               // clear any ccp3 interrupt and enable ccp3 interrupt
               clear_interrupt(INT_CCP3);               
               enable_interrupts(INT_CCP3); 
               
               // JG: previously there was no "break;" here.  Rather the
               // following case was MOVE_VALVE. I believe this accomplishes
               // the same thing.
               PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);
               break;

            
            // Closes valve irrespective of the position. Used for shutting down the system (and leaving
            //    the valve closed) and for test purposes.
            case CLOSE_VALVE_UNKNOWN_STATE:                            
               // clear out the time in motion
               global_valve_time_in_motion_1024ths = 0;     
               
               global_valve_position_set_value = VLV_POSITION_CLOSED;
               CCP_3 = global_valve_time_to_open_1024th; 
               // start the valve opening
               mV_CLOSEm;         
               // setup/turn on the comparator interrupt (also sets up DAC)
               comparator_setup();
               
               // setup and enable CCP3 as well as it's respective timer
               setup_ccp3(CCP_USE_TIMER1_AND_TIMER2 | CCP_COMPARE_RESET_TIMER);
               setup_timer_1(T1_ENABLE_SOSC | T1_EXTERNAL_SYNC | T1_DIV_BY_1);
               set_timer1(0);
                                                                         
               // clear any ccp3 interrupt and enable ccp3 interrupt
               clear_interrupt(INT_CCP3);               
               enable_interrupts(INT_CCP3);   
               PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);
               break;                                    
                                           
                                           
            case MOVE_VALVE:              
               // if the system is in the SYSTEM_IDLE state, bump it into the SYSTEM_RUN state.
               if (read_system_state() == SYSTEM_IDLE) write_system_state(SYSTEM_RUN);     
                                                     
               // if system is in an undesireable state, don't move the valve
               //    and send an error message                                  
               // Undesirable states such as unkown or init
               if ((read_system_state() == SYSTEM_STATE_UNKNOWN)||            \
                  (read_system_state() == SYSTEM_INIT))
               {                                   
                  global_error_message_bitfield |= ERR_MSG_INCOMPATIBLE_STATE;
                  PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_ERROR_MSG);
                  break;                                 
               }
               // or if you're in the run state and the valve is uncalibrated
               else if ((read_system_state() == SYSTEM_RUN) &&                \
                  (global_valve_position == VLV_POSITION_UNKNOWN))
               {
                  global_error_message_bitfield |= ERR_MSG_VLV_NOT_CALIBRATED;
                  PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_ERROR_MSG);
                  break;
               }
                                           
               // if valve movement is not needed (less than one millispan away
               //    and in run state), break out of switch case
               if ((read_system_state() == SYSTEM_RUN) &&                     \
               (((global_valve_position_set_value - global_valve_position) < 0x20) || \
               ((global_valve_position - global_valve_position_set_value) < 0x20)))
               {               
                  break;   
               }             
                                      
               // clear out the time in motion
               global_valve_time_in_motion_1024ths = 0;  
               
               // increment the movements since hitting an endstop. Also, check if
               // enough valve movements have happened to warrant a recalibration of
               //    the position of the valve by going towards an endstop
               if ((global_valve_movements_since_endstop++) > VLV_MOVES_BEFORE_RECAL)
               {
#IFNDEF  DISABLE_RECAL_VLV                       
                  write_system_state(SYSTEM_RECAL_VLV_MOVES);                        
                  // quicker to go to the closed endstop. Start motion.
                  if ((global_valve_position_set_value + global_valve_position) >\
                     VLV_POSITION_OPENED)
                  {
                     CCP_3 = global_valve_time_to_close_1024th;
                     mV_CLOSEm;                                                                   
                  }
                  // quicker to go to the open endstop. Start motion.
                  else   
                  {
                     CCP_3 = global_valve_time_to_open_1024th;
                     // don't go to the open endstop to avoid magnetic decoupling
                     //mV_OPENm;
                     mV_CLOSEm;
                  }
 #ENDIF
               }
               
               // regular valve move (without recalibration)
               else
               {
                  // setup CCP3 and put the valve into motion based on target value
                  if (global_valve_position_set_value > global_valve_position)
                  {
                     CCP_3 = global_valve_time_to_open_1024th;
                     mV_OPENm; 
                  }
                  else if (global_valve_position_set_value < global_valve_position)
                  {
                     CCP_3 = global_valve_time_to_close_1024th;
                     mV_CLOSEm;
                  }
                  else if (global_valve_position_set_value == VLV_POSITION_OPENED)
                  {
                     CCP_3 = global_valve_time_to_open_1024th;
                     mV_OPENm;  
                  }
                  else if (global_valve_position_set_value == VLV_POSITION_CLOSED)
                  {
                     CCP_3 = global_valve_time_to_close_1024th;
                     mV_CLOSEm;
                  }                 
               }   
               
               // setup/turn on the comparator interrupt (also sets up DAC)
               comparator_setup();                                         
               
               // setup and enable CCP3 as well as it's respective timer
               setup_ccp3(CCP_USE_TIMER1_AND_TIMER2 | CCP_COMPARE_RESET_TIMER);
               setup_timer_1(T1_ENABLE_SOSC | T1_EXTERNAL_SYNC | T1_DIV_BY_1);
               set_timer1(0);
               
               // clear any ccp3 interrupt and enable ccp3 interrupt
               clear_interrupt(INT_CCP3);
               enable_interrupts(INT_CCP3);           
               break;
               
            // Start GPS aquisition
            case START_GPS_AQUISITION:
               break;
                                                    
            // Check GPS for lock
            case CHECK_GPS_FOR_LOCK:    
               break;                   
                           
            case TAKE_XCDR_MESUREMENT_1:
               // turn on XDCR power (must give ~50ms to settle)
               output_high(XDCR_PWR);                    
               // queue up the actual meaurement a second later.                   
               PUSH_TIME_QUEUE_MACRO(global_rtc_time + 1, TAKE_XCDR_MESUREMENT_2);
               break;
                                                                                    
            case TAKE_XCDR_MESUREMENT_2:   
               // take the actual xdcr measurement via the adc (stores the result in global_xdcr_output)    
               adc_XDCR_measure();    
               // add xdcr message to queue                      
               PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_XDCR_READ_MSG); 
               break;                                            
                                       
            // Update the LCD
            // In the future, we're going to have "pages" controlled by the switch.
            case LCD_UPDATE:          
               // instructed to skip this update (to show other things on screen)         
               
               // disable button press interrupt while accessing shared global lcd variables
//               disable_interrupts(INT_RB);   
               
               // check if long press has exceeded time limit and indicate
               //  to user if so
               
               if (sw1_start_time && ((global_rtc_time - sw1_start_time) > SW1_ISR_LONG_PRESS_TIME))
               {
                  flash_led_2(4);
                  sw1_start_time = 0;
               }

               else if (global_skip_lcd_update_count > 1)  
               {                  
                  global_skip_lcd_update_count--;    
               }
               // either normal update or screen clear and update via function call (for multiple pages)    
               else                 
               {       
                  LCD_update_diplay();            
               }
               
               // disable button press interrupt while accessing shared global lcd variables
//               enable_interrupts(INT_RB); 
                
               break;                                         
               
            // recovers from an i2c bus collision interrupt
            case BUSCOL_RESET:                   
               BCL1IF = FALSE;
               SSP1IF = TRUE;
               i2c_init(TRUE);            
               if (global_lcd_enabled) LCD_init();  
               enable_interrupts(GLOBAL);         
               break;                                                   
               
            // Reset the mote (triggers a wait for boot event)
            case RESET_MOTE:     
               // display on screen what is happening
               LCD_clear();            
               strcpy (global_temp_line_buff, "Resetting Mote!!");
               LCD_line1(global_temp_line_buff);
               global_skip_lcd_update_count = 2;   
               // disable mote enabled flag
               global_dust_enabled = 0;     
               // reset the mote   
               mote_reset(); 
               break;                           
            
            // Make sure the mote is responding to a boot or shutdown
            case WAIT_FOR_BOOT_EVENT:    
               // if the mote didn't respond, set error, open valve, shutdown
               if (global_dust_enabled == 0)
               {        
                  // 
                  LCD_clear();   
                  strcpy (global_temp_line_buff, "Mote is Dead    ");  
                  LCD_line1(global_temp_line_buff);                  
                  global_skip_lcd_update_count = 2;  
                  // save shutdown cause and queue shutdown
                  global_shutdown_cause = ERR_FAIL_ON_MOTE_RESET;

                  PUSH_PRIORITY_QUEUE(OPEN_VALVE_UNKNOWN_STATE);  
                  PUSH_PRIORITY_QUEUE(SHUTDOWN_SYSTEM);
               }           
               break;
               
            // Check the mote status and react appropriately
            case CHECK_MOTE_STATE:
               mote_state_check();
               break;
               
            case CHECK_MOTE_INFO:
               mote_info_check();
               break;
              
            case UPDATE_MOTE_TIME:
               mote_time_update();
               break;   
               
            case UPDATE_MOTE_NETWORK_INFO:
               get_mote_net_info();
               break;
               
            // Check's battery voltage, decides to charge, not charge, tell manager
            // about a low voltage state, or to go to deep sleep
            case CHECK_BATTERY_STATE:
               /*
               LCD_clear();
               LCD_display_battery_voltage(0);
               LCD_place_uint16(get_vbatt(0),1,0,5);
               global_skip_lcd_update_count = 2;
               */        
               check_and_deal_with_battery();
               break;
            
            // Query the mote for the temp and store it
            case CHECK_MOTE_TEMP:
               mote_temp_check();
               /*
               LCD_clear();
               strcpy (global_temp_line_buff, "Temp =         C");
               LCD_line1(global_temp_line_buff);
               LCD_place_uint8(global_mote_temperature,0,7,3);
               global_skip_lcd_update_count = 2;
               */
               break;
               
            // First part of the search for strongest algorithm
            case SEARCH_FOR_STRONGEST_1:
               LCD_clear();
               strcpy (global_temp_line_buff, "Search Strong 1 ");
               LCD_line1(global_temp_line_buff);
               global_skip_lcd_update_count = 2;
               // set the state of the system appropriately
               global_system_state = SYSTEM_SEARCHING_FOR_NETWORK;
               PUSH_PRIORITY_QUEUE_MACRO(RESET_MOTE);
               PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), SEARCH_FOR_STRONGEST_2);
               break;
               
            // Second part of the search for strongest algorithm   
            case SEARCH_FOR_STRONGEST_2:
               LCD_clear();
               strcpy (global_temp_line_buff, "Search Strong 2 ");
               LCD_line1(global_temp_line_buff);
               global_skip_lcd_update_count = 2;
               search_for_strongest();
               break;
            
            // Initalizes a mote join
            case INIT_JOIN:
               LCD_clear();
               strcpy (global_temp_line_buff, "  Init Join     ");
               LCD_line1(global_temp_line_buff);
               global_skip_lcd_update_count = 2;  
               initiate_join();                                     
               break;
            
            // not used anymore
            case OPTIMIZE_MPPC:
               adjust_mppc();  
               break;            
               
            // resets the cpu (if all pending messages are sent)   
            case CPU_RESET:
               // if the dust network is operational and the message queue is not empty (location at 255)
               //    wait for the message to be ack'd/resent and reschedule the shutdown. 
               if ((global_dust_operational == TRUE) && (global_current_message_queue_location != 255))
               {
                  PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), CPU_RESET);
               }
               // if the valve is moving, check again later
               else if (!IS_VLV_COASTING)
               {                      
                  PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), CPU_RESET);
               }
               // All messages are sent
               else
               {
                  // if it does, restart the entire system
                  store_all_eeprom_values();
                  reset_cpu();
               }
               break;
               
            // Shuts down the pic for a variety of reasons
            case SHUTDOWN_SYSTEM:               
               // if the dust network is operational and the message queue is not empty (location at 255)
               //    wait for the message to be ack'd/resent and reschedule the shutdown.
               // I suppose this has potential to be problamatic, but the network should eventually show
               //    up as non-operational in mote-check or get ack'd at some point, I would hope.
                                                             
               global_brake_duty_set_value = NO_BRK;
               global_charge_duty_set_value = NO_CHARGE;
               global_control_loop_mechanism = NO_RPM_CONTROL_DYN_MPPC;
               
               // if the system hasn't moved the brake and charge settings to 0, wait
               if ((global_brake_duty != NO_BRK) || (global_charge_duty != NO_CHARGE))
               {
                  break;
               }               
               // if there are still messages to be sent and the mote is working, wait
               else if ((global_dust_operational == TRUE) && (global_current_message_queue_location != 255))
               {                                                
                  PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), SHUTDOWN_SYSTEM);
               }
               // if the valve is moving, wait                          
               else if (!IS_VLV_COASTING)
               {
                  PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), SHUTDOWN_SYSTEM);
               }                                
               // All messages are sent or system is shutting down due to no network connection 
               //    and system is in SYSTEM_RUN or SYSTEM_IDLE state 
               else if ((read_system_state() == SYSTEM_RUN) || ((read_system_state() == SYSTEM_IDLE)))
               {
                  // try to put the mote to sleep
                  if (mote_sleep() == NO_ERR)         
                  {
                     // if it does, put the entire system to sleep
                     store_all_eeprom_values();
                     deep_sleep();
                  }
                  // otherwise, try again in 10 seconds   
                  else
                  {
                     PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), SHUTDOWN_SYSTEM);
                  }
               }
               // Anything else, just wait
               else
               {
                  PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), SHUTDOWN_SYSTEM);
               }
               break;
               
            // updates 
            // Default case (nothing to do)               
            case EMPTY_PRIORITY_QUEUE:
               //LCD_place_uint32(global_rtc_time, 1, 0, 10);
               break;
         }
         
      }
////////////////////End of Priority Queue Handling/////////////////////////////
   // end of inifinite while loop
   }   
}


//#PRIORITY COMP, CCP5, EXT2, CCP3, CCP4, BUSCOL
#PRIORITY CCP5, COMP, CCP3, EXT2, TIMER0, CCP4, BUSCOL, INT_RB, EXT          
                                                                    
#INT_RB         
void RB_ISR(void)                               
{                          
   // Setup in periph init to be called only when RB4 has changed state
   // RB4 is connected to SW1n 
   
   // keeps track of when the switch was pressed so we can enable the long press functionality     

   if (!input(SW1n))
   {
      sw1_start_time = global_rtc_time;
      return;
   }           
   
   sw1_start_time = 0;
   // if you're incrementing past the last page, reset to the first page  
   if (global_lcd_page_number == TOTAL_LCD_PAGES)
   {                                            
      global_lcd_page_number = 1;   
   }     
   // otherwise, just increment the page number
   else    
   {
      global_lcd_page_number++;
   } 
   
   // trigger a full LCD update (including the contstant parts of the lcd)
   global_skip_lcd_update_count = 1;      
         
   return;
   
   
      
   // not quite ready for prime time long press code (causes boot-looping, but is probably close)
   // if SW1 is depressed (active), take note of the time 
   
/*  
   if (!input(SW1n))
   {
      sw1_start_time = global_rtc_time;  
   }                                 
   // when SW1 is coming back up 
   else                  
   {                                        
      // if the button has been pressed more than SW1_ISR_LONG_PRESS_TIME    
      //    do the long-press action
      if ((global_rtc_time - sw1_start_time) > SW1_ISR_LONG_PRESS_TIME)
      {                  
         // long-press action
         flash_leds_ISR(2); 
      }        
                                          
      // if the button had been pressed less than SW1_ISR_LONG_PRESS_TIME, increment the page   
      else
      {        
         // if you're incrementing past the last page, reset to the first page  
         if (global_lcd_page_number == TOTAL_LCD_PAGES)
         {                                            
            global_lcd_page_number = 1;   
         }     
         // otherwie, just increment the page number
         else    
         {
            global_lcd_page_number++;
         } 
                                           
         // trigger a full LCD update (including the contstant parts of the lcd)
         global_skip_lcd_update_count = 1;                 
      }
      return;
   }
 */
 
}


#INT_EXT
void vgen_wakeup_ISR(void)
{
// runs on wakeup from vgen int
   reset_cpu();                 
}                

#INT_BUSCOL
void BUSCOL_ISR(void)
{  
// Catches and recovers from an i2c bus collision 

   // are reenabled in MAIN       
   disable_interrupts(GLOBAL);
   
   // clear bus collision interrupt flag
   BCL1IF = FALSE;
   
   // schedule a bus collision reset
   PUSH_PRIORITY_QUEUE_ISR_MACRO(BUSCOL_RESET);

   // if the lcd is connected, reset it
   if (global_lcd_enabled)
   {  
      output_low(LCD_RESETn);
      delay_cycles(64);       // about 1 millisecond
      output_high(LCD_RESETn);
   }   
   
   // send the stack pointer to position 1 (perhaps sort of dangerous)
   STKPTR = 1;
}

#INT_EXT2 
void mote_interrupt(void)
{   
// interrupt called when mote rts line gets asserted   
   uint8_t     tmp_oscccon, tmp_t2con, tmp_pr2, tmp_t0con;
   
   // save the current oscillator setup
   tmp_oscccon = OSCCON;
   tmp_t2con = T2CON;
   tmp_pr2 = PR2;
   tmp_t0con = T0CON;
   
   // start primary (3.8Mhz) crystal for UART communication
   fosc_pri_ISR();
   
   // disable int_ccp4 as (unsolicited) message queue can overwrite payload_buff
   disable_interrupts(INT_CCP4);
                                  
   // recieve serial data, respond, and schedule deal with packet if necessary
   deal_with_mote_ISR();
   
   // renable int_ccp4 as we're done touching payload_buff      
   enable_interrupts(INT_CCP4);  
                                    
   // restore the current oscillator
   OSCCON = tmp_oscccon;
   T2CON = tmp_t2con;
   T0CON = tmp_t0con;
   PR2 = tmp_PR2;
}


#INT_COMP
void comp1_ISR(void)
{
// ISR routine that is called when the comparator current limit is reached
   uint16_t    temp16_frac;
   
    // if comarator 1 is tripped (INT_COMP is triggered by comp 1 or 2)
   if (C1OUT)
   {     
      // grab the extra time/2ndary osc ticks since the last 1024th interrupt  
      temp16_frac = get_timer1();
      
      // if statements for different calibration routines
      // valve opening a little bit to ensure we dont jam into close endstop
      if (global_system_state ==  SYSTEM_CAL_VLV_1)
      {
         global_valve_position = VLV_POSITION_OPENED;
         // queue up the next stage of the calibration
         push_time_queue_ISR(global_rtc_time + 3, CALIBRATE_VALVE_2);
      }
      // valve closing towards closing endstop pre-calibration
      else if (global_system_state == SYSTEM_CAL_VLV_2)
      {
         global_valve_position = VLV_POSITION_CLOSED; 
         // queue up the next stage of the calibration
         push_time_queue_ISR(global_rtc_time + 3, CALIBRATE_VALVE_3);
      }
      // valve opening fully from fully closed for calibration
      else if (global_system_state == SYSTEM_CAL_VLV_3)
      {
         // set the valve position to open
         global_valve_position = VLV_POSITION_OPENED; 
         // recalculate the valve opening time for each 1024th
         //    (valve movements (1024th) * the time it takes for each 1024th
         //    + the extra time) divided by 1024
         global_valve_time_to_open_1024th =                                   \
         ((((uint32_t) global_valve_time_in_motion_1024ths * (uint32_t) global_valve_time_to_open_1024th) \
         + temp16_frac) >> 10);
         // queue up the next stage of the calibration
         push_time_queue_ISR(global_rtc_time + 3, CALIBRATE_VALVE_5);
      }
      // valve closing fully from fully open for calibration
      else if (global_system_state == SYSTEM_CAL_VLV_5)
      {
         global_valve_position = VLV_POSITION_CLOSED; 
         // recalculate the valve closing time for each 1024th
         //    (valve movements (1024th) * the time it takes for each 1024th
         //    + the extra time) divided by 1024
         global_valve_time_to_close_1024th =                                   \
         ((((uint32_t) global_valve_time_in_motion_1024ths * (uint32_t) global_valve_time_to_close_1024th) \
         + temp16_frac) >> 10);
         // queue up the next stage of the calibration
         push_time_queue_ISR(global_rtc_time + 3, CALIBRATE_VALVE_6);
      }
      // we are doing a valve endstop detect that we have a known calibration for.
      // We want to check the positional error (if we are too far away from the
      // endstop in position when the endstop is detected).  This applies to 
      // normal moves as well as re-calibration moves.
      else
      {
         // valve has closed/opened fully in preparation of a recalibration of 
         //    valve position.  Trigger a move valve to move to the pending valve
         //    set position and set system status to run
         if (global_system_state == SYSTEM_RECAL_VLV_MOVES)
         {
            push_time_queue_ISR(global_rtc_time + 3, MOVE_VALVE);
            global_system_state = SYSTEM_RUN;
         }
         // valve is closing
         if (IS_VLV_CLOSING)
         {
            // error checking if valve movement was longer or shorter than expected
            // We accomplish this by seeing if the valve hit an endstop while it was
            //    outside the VLV_NEAR_CLOSED_RANGE_MAX/VLV_NEAR_OPENED_RANGE_MAX
            if (global_valve_position > VLV_NEAR_CLOSED_RANGE_MAX)
            {
               // put system into run mode (in case it's in valve cal routine)
               global_system_state = SYSTEM_RUN;
               // set valve to unknown position (uncalibrated)
               global_valve_position = VLV_POSITION_UNKNOWN;
               // set the error bitfield and send an error
               global_error_message_bitfield |= ERR_MSG_VLV_MOVE_FAIL;
               push_time_queue_ISR(global_rtc_time + 1, MSG_MOTE_ERROR_MSG);
            }
            // was an expected endstop, proceed as usual
            else
            {
               global_valve_position = VLV_POSITION_CLOSED; 
            }
         }
         // valve is opening
         else if (IS_VLV_OPENING)
         {
            // error checking if valve movement was longer or shorter than expected
            // We accomplish this by seeing if the valve hit an endstop while it was
            //    outside the VLV_NEAR_CLOSED_RANGE_MAX/VLV_NEAR_OPENED_RANGE_MAX
            if (global_valve_position < VLV_NEAR_OPENED_RANGE_MAX)
            {
               // put system into run mode (in case it's in valve cal routine)
               global_system_state = SYSTEM_RUN;
               // set valve to unknown position (uncalibrated)
               global_valve_position = VLV_POSITION_UNKNOWN;
               // set the error bitfield and send an error
               global_error_message_bitfield |= ERR_MSG_VLV_MOVE_FAIL;
               push_time_queue_ISR(global_rtc_time + 1, MSG_MOTE_ERROR_MSG);
            }
            // was an expected endstop, proceed as usual
            else
            {
               global_valve_position = VLV_POSITION_OPENED; 
            }
         }
      }
      
      // Update fixture setting

                 
      // Turn off comparator and dac
      setup_DAC(DAC_OFF);
      setup_comparator(NC_NC_NC_NC);
      
      // Turn off CCP3 interrupt     
      disable_interrupts(INT_CCP3);
      
      // reset valve movements counter
      global_valve_movements_since_endstop = 0;
      
      // turn off the motor
      mV_COASTm;

      // put clock speed down to 250khz again
      //fosc_250k_ISR();   
      
      // turn off comparator interrupt so it isn't triggered on stop
      clear_interrupt(INT_COMP);     
   }
}


/*
#INT_TIMER0
void tmr0_ISR(void)
{
// may not be enabled during normal operation.
// Need only for IDLING situation (long winters...) wherein every 35minutes
//  we wake up and check the battery and set a flag if there is a very low
//  battery, which then tells the idle_sleep routine to go into deep_sleep.
// this needs work.
//
// We can know if we were idling by simply checking the OSCCON for 31250Hz
//  operation, which occurs ONLY during the long winter....
//
   if (cur_state == DEV_IDLE)
   {
   uint8_t  nn;
   // this interrupt occurred during the sleep_idle state
   // check the battery, if it is so low that we need to turn off the radio,
   //  then be sure to set a flag to make that happen...
   // Deep_Sleep may be the result
      output_high(AUX_PWR);
      ADON = TRUE;         // turn on ADC
      delay_cycles(50);    // insurance
      set_adc_channel(V_MEAS_REF);  
      VfvrAD = 0;  
      for (nn=0; nn<4; nn++)
         VfvrAD += read_adc();
      ADON = FALSE;
      output_low(AUX_PWR);
      if (VfvrAD > FVR_NODUST)
         fl_batNODUST = TRUE;
   }
   else
   {
   // any other TIMER0 activities we may want
   }
}
*/

                                                         
#INT_CCP3 HIGH
void ccp3_ISR(void)
{
// For use in timing valve motion
//    -Updates realtime position of valve
//    -Keeps track of valve movement time (in 1024th of full scale)
//    -Turns off valve movement when position is reached
//    -updates DAC level according to position and direction of movement
// May be used for other functionality if valve is not moving
// TMR1 dedicated to CCP3
   
   //set_timer1(TIMER1_VLV_MOVE_INIT + get_timer1());
   //temp16_frac = get_timer1();            
         
   // If the valve is in motion (not in the braked or coast mode) 
   if (IS_VLV_CLOSING || IS_VLV_OPENING)
   {
      // add to the time in motion variable
      global_valve_time_in_motion_1024ths++;
      
      //check for valve movement timeout
      if (global_valve_time_in_motion_1024ths > VALVE_TIMEOUT)
      {
         // turn off comparator interrupt so it isn't triggered on stop
         disable_interrupts(INT_COMP);
               
         // turn off valve movement
         mV_COASTm;
         
         // Error handling for valve motion timeout
         // put system into run mode (in case it's in valve cal routine)
         global_system_state = SYSTEM_RUN;
         // set valve to unknown position (uncalibrated)
         global_valve_position = VLV_POSITION_UNKNOWN;
         // set the error bitfield and send an error
         global_error_message_bitfield |= ERR_MSG_VLV_MOVE_FAIL;
         push_time_queue_ISR(global_rtc_time + 1, MSG_MOTE_ERROR_MSG);
      }
      
      else if (IS_VLV_CLOSING)
      {  
         //set_timer1(TIMER1_VLV_MOVE_INIT);
         // 0x20 is equal to one 'millispan'
         if (global_valve_position >= 0x20)
         {
            global_valve_position -= 0x20;    
         }
         // valve is closed, unsigned int thing
         else 
         {
            global_valve_position = VLV_POSITION_CLOSED;
         }
         
         // if system is doing a valve recalibration, ignore valve set position
         //    as it is storing the next valve position to move to
         if (global_system_state == SYSTEM_RECAL_VLV_MOVES)
         {
            break;
         }                       
         // we have reached or exceeded the set value/target position and
         //    we aren't trying to reach the endstop
         else if ((global_valve_position <= global_valve_position_set_value)&&\
         (global_valve_position_set_value != VLV_POSITION_CLOSED))
         {
            // turn off comparator interrupt so it isn't triggered on stop
            disable_interrupts(INT_COMP);
            
            // turn off valve movement
            mV_COASTm;
         }
      }
      else if (IS_VLV_OPENING)
      {                  
         //0x20 is equal to one 'millispan'
         global_valve_position += 0x20;      
      
         if (global_valve_position > VLV_POSITION_OPENED)
         {
            global_valve_position = VLV_POSITION_OPENED;   
         }
         
         // if system is doing a valve recalibration, ignore valve set position
         //    as it is storing the next valve position to move to
         if (global_system_state == SYSTEM_RECAL_VLV_MOVES)
         {
            break;
         }
         // we have reached or exceeded the set value/target position and
         //    we aren't trying to reach the endstop
         else if ((global_valve_position >= global_valve_position_set_value)&&\
         (global_valve_position_set_value != VLV_POSITION_OPENED))
         {
            // turn off comparator interrupt so it isn't triggered on stop
            disable_interrupts(INT_COMP);
            
            // turn off valve movement
            mV_COASTm;
            
            // Special case: if we are opening during CALIBRATE_VALVE_1 and
            //    have reached our position, start the next calibration 
            //    sequence 
            if (global_system_state == SYSTEM_CAL_VLV_1)
            {
               push_time_queue_ISR(global_rtc_time + 3, CALIBRATE_VALVE_2);
            }
         }
      }   
      // update the dac setting
      set_comp_dac_level_isr();
      
      // update the fixture setting
      
      // if the valve is not moving anymore
      if (IS_VLV_COASTING)
      {
         // switch back to lower clock, turn off CCP3 interrupt
            //fosc_250k_ISR();
            //#use delay(clock=250KHZ)
         disable_interrupts(INT_CCP3);
                                                                                                       
      }
   }
}


#INT_CCP4
void ccp4_isr(void)                    
{
   // real time clock interrupts     
   // TMR3 dedicated to CCP4     
                                                                                     
   // increment global system uptime
   global_rtc_time++;
   // increment utc time if mote is connected
   if (global_dust_enabled) global_utc_time++;

   // check if a time queue item needs to be run
   // make sure it isn't polling an empty queue
   while((global_current_time_queue_location != 255) &&                       \
   (global_current_priority_queue_location != (MAX_PRIORITY_QUEUE_ITEMS - 1)) \
   && (global_time_queue[global_current_time_queue_location].time_to_execute  \
   <= global_rtc_time))
   {                              
      // pop an item off the time queue and push it into the priority queue
      pop_time_queue_ISR();                     
   }                        
   
   // check if a message queue item needs to be run
   // make sure it isn't polling an empty queue
   while((global_current_message_queue_location != 255) &&                      \
      (global_message_queue[global_current_message_queue_location].time_to_send \
      <= global_rtc_time))   
   {                    
      // disable mote interrupt so payload_buf doesn't get overwritten
      disable_interrupts(INT_EXT2_H2L);  
      // send message and requeue it at a later date if not ack'd
      pop_message_queue_and_send_ISR();
      // re-enable mote interrupt                    
      enable_interrupts(INT_EXT2_H2L);    
   }                                  
   
   
   // check if a sprinkler queue item needs to be run and make sure it isn't polling an empty queue       
   // Note: The queue contains things that are to be run, not the current item running.            
   while((global_current_sprinkler_queue_location != 255) &&                      \
      (global_sprinkler_queue[global_current_sprinkler_queue_location].start_time \
      <= global_utc_time))
   {  
      // if the system is in the SYSTEM_IDLE state, bump it into the SYSTEM_RUN state.
      if (global_system_state == SYSTEM_IDLE) global_system_state = SYSTEM_RUN;      
                                  
      // if system is not in run state, do not stop the item.  Send an error 
      //    stating that the stop time is delayed
      if (global_system_state != SYSTEM_RUN)                       
      {                      
         global_error_message_bitfield |= ERR_MSG_SPINKLER_CMD_DELAYED_INVALID_STATE;
         push_time_queue_ISR(global_rtc_time + 1, MSG_MOTE_ERROR_MSG);
      }
      else
      {
         // pop an item off the time queue and push it into the priority queue
         pop_sprinkler_queue_ISR();
      }
   }           
                                                                
   // check if a sprinkler queue item needs to be stopped
   if (global_current_sprinkler_settings_end_time <= global_utc_time)
   {                        
      // if the system is in the SYSTEM_IDLE state, bump it into the SYSTEM_RUN state.
      if (global_system_state == SYSTEM_IDLE) global_system_state == SYSTEM_RUN;  
                                                                                          
      // if system is not in run state, do not stop the item.  Send an error
      //    stating that the stop time is delayed
      if (global_system_state != SYSTEM_RUN)
      {
         global_error_message_bitfield |= ERR_MSG_SPINKLER_CMD_DELAYED_INVALID_STATE;
         push_time_queue_ISR(global_rtc_time + 1, MSG_MOTE_ERROR_MSG);
      }             
                                                   
      /* 
      // if you're stopping the current item and there are no more in the queue, turn off the control
      //    loop.                            
      else if (global_current_sprinkler_queue_location == 255)
      {  
         global_control_loop_mechanism = NO_SPRINKLER_CONTROL;
      }       
      */                                         
                             
      else
      {                  
         // stop the current sprinkler setting
         stop_current_spinkler_setting_ISR();
      }             
   }
    
    
    
   
   // check to see if system should enter SYSTEM_IDLE (low-power) state
   //
   // First condition for SYSTEM_IDLE:
   //    system already in SYSTEM_RUN   
   //    Closed valve (or unknown)       
   //    no valve movement
   //    rpm = 0    
   if (((global_valve_position == VLV_POSITION_CLOSED) ||                                              \     
      (global_valve_position == VLV_POSITION_UNKNOWN)) && (IS_VLV_COASTING) &&                         \
      ((global_rtc_time - global_last_rpm_value_time) > RPM_TIMEOUT))           
   {                                  
      // change processor speed?     
      // change state to idle       
       if (global_system_state == SYSTEM_RUN) 
       {
         global_system_state = SYSTEM_IDLE;
         //fosc_31250_ISR(); 
       }
   }                                        
   // Second condition for SYSTEM_IDLE:               
   //    system already in SYSTEM_RUN   
   //    global_control_loop_mechanism is in either NO_RPM_CONTROL (test command) or NO_SPRINKLER_CONTROL       
   //    no valve movement        
   //    brake, charge, and mppc values are at their set points
   else if (((global_control_loop_mechanism == NO_RPM_CONTROL)||                                      \
      (global_control_loop_mechanism == NO_SPRINKLER_CONTROL)) && (IS_VLV_COASTING) &&                \
      (global_brake_duty == global_brake_duty_set_value) &&                                           \
      (global_charge_duty == global_charge_duty_set_value) &&                                         \
      (global_mppc_value == global_mppc_set_value))
   {                                              
      // change processor speed? 
      // change state to idle
      if (global_system_state == SYSTEM_RUN)   
      {
         global_system_state = SYSTEM_IDLE;
         //fosc_31250_ISR(); 
      }               
   }                      
   // if above conditions are not met and system is in idle mode, put it into run mode
   else if (global_system_state == SYSTEM_IDLE)         
   {                                 
      // change processor speed?   
      // change state to run state      
       global_system_state = SYSTEM_RUN; 
       //fosc_250k_ISR(); 
   }                           
                                                         
    
   // update lcd every second                                
   PUSH_PRIORITY_QUEUE_ISR_MACRO(LCD_UPDATE);   

   // periodic system checkups (all in one to minimize divides)
   // - mote state (reacts as necessary)   
   // - battery state (turns off/on charging, sends warnings, etc.)
   // - mote temp (logs data, sends warnings, etc.)
   // - mote utc time (updates utc time if valid)
   if ((global_rtc_time % PERIODIC_CHECKS_TIME) == 0)     
   {                                                 
      PUSH_PRIORITY_QUEUE_ISR_MACRO(CHECK_MOTE_STATE); 
      PUSH_PRIORITY_QUEUE_ISR_MACRO(CHECK_BATTERY_STATE);   
      PUSH_PRIORITY_QUEUE_ISR_MACRO(CHECK_MOTE_TEMP);
      PUSH_PRIORITY_QUEUE_ISR_MACRO(UPDATE_MOTE_TIME);
   }                                                                                                                          
}

#INT_CCP5 FAST
void CCP5_ISR(void)
{
// GEN_RPM event capture for determining speed of rotation
// We need the CCP5 interrupt routine to be very fast because 
//  the sprinkler can be spinning fast enough to generate 500 pulses per sec!
// With a 250KHz system clock, 2ms may be trouble with the full normal interrupt
//  overhead....
// TMR5 dedicated to CCP5
   
   static uint16_t ccp5_value = 0, ccp5_value_prev = 0;
   
   // save previous sample 
   ccp5_value_prev = ccp5_value;
   
   // get current sample     
   ccp5_value = CCP_5;         
                                  
   // disable INT_CCP4 to ensure safe grabbing of global_rtc_time                       
   disable_interrupts(INT_CCP4);             
   // check if a current period measurement is even valid
   if ((global_rtc_time - global_last_rpm_value_time) > RPM_TIMEOUT)  
   {
      // save the last time a measurement was take to figure out if system is spinning outside of this routine.    
      global_last_rpm_value_time = global_rtc_time;     
      break;                                         
   }                             
   else           
   {                                                               
      // calc current difference/global period    
      global_current_period = ccp5_value - ccp5_value_prev;      
   }                                          
   // save the last time a measurement was take to figure out if system is spinning outside of this routine.    
   global_last_rpm_value_time = global_rtc_time;  
   // re-enable INT_CCP4 after grabbing global rtc time       
   enable_interrupts(INT_CCP4); 
}
                                   
