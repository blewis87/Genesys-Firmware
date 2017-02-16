// globals.h
//
// This file contains all of the global variables used in the firmware
// 
// Add to this list carefull and keep it syncronized with vars_init().....               
          
////////////////////Global Variables Table of Contents/////////////////////////
//Test Globals
//LCD Globals
//Time Globals                                                                           
//System Parameter Globals
//Hardware Enabled Globals                               
//Control Related Globals                 
//Valve Related Globals                                        
//Mote Globals
//Priority Queue
//Time Queue                                                              
//Sprinkler Stack                                                                                                
//Command Stack                    
//System State Globals

//Where to put?                                               
uint32_t global_last_connected_time = 0;        
uint16_t global_xdcr_output = 0;                            
                                  
// global that keeps track of why the system restarted/shutdown
uint8_t global_shutdown_cause = 0;
uint8_t global_previous_shutdown_cause = 0;

char global_firmware_version_string[5] = {'A','0','0','0',0};      

////////////////////Test Globals///////////////////////////////////////////////
uint8_t test_global = 0;
uint8_t test_global_2 = 0;
uint32_t test_start_time = 0;
uint32_t test_end_time = 0;
uint16_t test_valve_open_1 = 0;
uint16_t test_rpm_1 = 0;
uint16_t test_duty_cycle_1 = 0;
uint16_t test_valve_open_2 = 0;
uint16_t test_rpm_2 = 0;
uint16_t test_duty_cycle_2 = 0;
uint16_t test_valve_open_3 = 0;
uint16_t test_rpm_3 = 0;
uint16_t test_duty_cycle_3 = 0;

 
////////////////////LCD Globals////////////////////////////////////////////////
unsigned char global_temp_line_buff[17];
char* global_extra_lcd_ptr;
                    
// LCD skip update flag
// 0 = don't skip
// 1 = Last update skipped, clear screen and proceed as normal
// 2-255 = how many skips (plus 1) to skip.  LCD is updated 1/sec, so 4 = 
//    3 seconds of skipping updates.
uint8_t     global_skip_lcd_update_count = 0;       

// Current LCD page number
// 0 = don't display anything?
// 1-TOTAL_LCD_PAGES = display that page number    
uint8_t     global_lcd_page_number = 1;      
                  
////////////////////Time Globals///////////////////////////////////////////////
// keeps track of time since microprocessor has been awake in seconds
uint32_t global_rtc_time = 0;
// keeps track of the time within the mote system (taken from mote)
uint32_t global_utc_time = 0;
// keeps track of the time at book (as taken from the mote the first time after
//  program startup
uint32_t global_first_utc_time = 0;
// keeps track of when SW1 was last depressed (for recognition of long press event)
uint32_t sw1_start_time = 0;  
////////////////////System Parameter Globals///////////////////////////////////
uint16_t global_sprinkler_num = 0;

// More globals that we might actually want to keep
uint8_t     serial_number_rcvd[8] = {0,0,0,0,0,0,0,0};
uint8_t     macaddr_rcvd[8] = {0,0,0,0,0,0,0,0};

////////////////////Hardware Enabled Globals///////////////////////////////////
uint8_t  global_hardware_enabled = 0;                                                               
   #BIT  global_lcd_enabled         = global_hardware_enabled.0
   #BIT  global_gps_enabled         = global_hardware_enabled.1
   #BIT  global_xcdr_type1_enabled  = global_hardware_enabled.2
   #BIT  global_xcdr_type2_enabled  = global_hardware_enabled.3
   #BIT  global_fixture_enabled     = global_hardware_enabled.4
   #BIT  global_temp1_enabled       = global_hardware_enabled.5
   #BIT  global_dust_enabled        = global_hardware_enabled.6
   #BIT  global_dust_operational    = global_hardware_enabled.7

////////////////////Control Related Globals////////////////////////////////////
// keeps track of rpms (updated via interrupt routine)
uint16_t global_current_period = 0;
uint16_t global_current_rpm;
uint16_t global_rpm_set_value = 100;
uint32_t global_last_rpm_value_time = 0;

// keeps track of duty cycles of brake and charge
uint16_t global_brake_duty = 0;
uint16_t global_charge_duty = MAX_CHARGE;
// keeps track of set points for brake and charge duty cycles
uint16_t global_brake_duty_set_value = 0;
uint16_t global_charge_duty_set_value = MAX_CHARGE;

// keeps track of mppc level
uint8_t  global_mppc_value = 100;
uint8_t  global_mppc_set_value = 100;
// keeps track of vgen
uint16_t global_vgen = 0;
// keeps track of fixture dac value
uint16_t global_fixture_dac_value = NO_FIXTURE;

// specifies how the rpm is being controlled (see defines for more info)
uint8_t global_control_loop_mechanism = BRAKE_AND_CHARGE_START;
// JG: set to brake and charge for testing purposes Feb 2, 2017
//uint8_t global_control_loop_mechanism = BRAKE_AND_CHARGE_START;

////////////////////Valve Related Globals//////////////////////////////////////
// keeps track of current valve position
uint16_t global_valve_position = VLV_PRECALIBRATION_POSITION;   
// valve position set point
uint16_t global_valve_position_set_value = VLV_PRECALIBRATION_POSITION;   
                               
// 1/1024th of the time to go from open to close calculated by calibrate valve   
//     routine in units of 2ndary oscillator ticks
uint16_t global_valve_time_to_close_1024th = DEFAULT_VLV_TIME_TO_CLOSE;
// 1/1024th of the time to go from close to open calculated by calibrate valve  
//    routine in units of 2ndary oscillator ticks
uint16_t global_valve_time_to_open_1024th = DEFAULT_VLV_TIME_TO_OPEN;
// amount of time the valve has been in motion in units of 1/1024ths of a
//    millispan.  So, not really time, but distance.
uint16_t global_valve_time_in_motion_1024ths = 0;
// when the last valve calibration was done, in utc time
uint32_t global_valve_calibration_utc_time = 0;
// number of movements since last calibration/endstop 
uint8_t  global_valve_movements_since_endstop = 0;

// calibrate FSR valve position (default is 0x2000, but should be set by message)
uint16_t global_calibrate_fsr_valve_position = 0x2000;
// calibrate FSR outcome in period (inverse of rpm).  Default/unknown is 0-1
uint16_t global_calibrate_fsr_period = 0-1;
// when the last fsr calibration was done, in utc time
uint32_t global_calibrate_fsr_utc_time = 0;

////////////////////Mote Globals///////////////////////////////////////////////
//event flags defined by mote
uint8_t  event_flags;
   #BIT  fl_boot_event           = event_flags.0
   #BIT  fl_alarm_event          = event_flags.1
   #BIT  fl_time_event           = event_flags.2
   #BIT  fl_joinfail_event       = event_flags.3
   #BIT  fl_disconnected_event   = event_flags.4
   #BIT  fl_operational_event    = event_flags.5
   #BIT  fl_config_event         = event_flags.6
   #BIT  fl_unknown_event        = event_flags.7

// Note that bytes_from_mote union contains the stripped unstuffed data
//  eg, bytes_from_mote.dust_rcvd[128]
mote_to_micro  bytes_from_mote;
mote_to_micro* p;

// It just can't get bigger than this!
// pkgen places message to be sent here
char        dust_send_buffer[MAX_SEND_BUFFER];

// this is only for doing acks and only needs to be 
// MAX_USER_DATA_SIZE*2 + 2 + 4 + 12 + 2
// MAX_USER_DATA_SIZE is 2 bytes for ack
char        dust_send_buffer_isr[2*2 + 2 + 4 + 12 + 2];

// THIS is the buffer for RAW rcvd mote
//  mote packet data
char        dust_recive_buffer[MAX_RCV_BUFFER + 1];

//  this is place
//  where the packet to be sent by micro
//  to NETWORK manager is placed prior
//  to dust_send_data call
char        payload_buff[MAX_USER_DATA_SIZE];

// Keeps track of how many bytes to send and passed between pkgen and pksend
uint8_t num_of_bytes_to_send = 0;
uint8_t num_of_bytes_to_send_isr = 0;

// This is specific to the subroutine
//  pkgen_send() which creates the message
//  to be sent from the mote to the net                     
uint8_t     send_prefix[12];

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
uint8_t result_rcvd = 0;

// JG Jan 2017
int1 fl_alt_LCD;

// Flag for going through deal with mote ISR and going to sleep afterwards
int1 fl_mote_dealt;

// Flag for a sucessful get/set response eliminating a few other flags
int1  global_set_get_response;

// FLAGS TO GET RID OF!
// Flag for receiving an advertisment
int1  advert_rcvd_fl;
int1  setnetid_done_fl;
int1  ack_sent_fl;
int1  pkt_rcvd_fl;
int1  joinduty_done_fl;
int1  gettime_done_fl;
int1  fl_batNOEE;
int1  joinkey_done_fl;
int1  joincmd_done_fl;

uint8_t flags_rcvd = 0 ,param_type_rcvd = 0, svc_index_rcvd=0;
uint16_t mote_msgSEQ = 0;

//Globals for when advertisements are received
uint16_t network_id_rcvd = NETWORK_ID_DEF;
uint16_t mote_id_rcvd = 0;
int8_t rssi_rcvd = 0;
uint8_t hop_depth_rcvd = 0;

// global variable that keeps track of the current temperature of the mote.  
//    It is updated every MOTE_CHECKS_TIME seconds
int8_t global_mote_temperature = 0;

////////////////////Priority Queue/////////////////////////////////////////////
// Actual priority queue
volatile uint8_t global_priority_queue[MAX_PRIORITY_QUEUE_ITEMS] = {0};
volatile uint8_t global_current_priority_queue_location = (0-1);

////////////////////Time Queue/////////////////////////////////////////////////
// Actual time queue
struct TimeQueueItem global_time_queue[MAX_TIME_QUEUE_ITEMS] = {0};
uint8_t global_current_time_queue_location = (0-1);

////////////////////Sprinkler Queue////////////////////////////////////////////
// Actual sprinkler queue
struct SprinklerQueueItem global_sprinkler_queue[MAX_SPRINKLER_QUEUE_ITEMS] = {0};
uint8_t global_current_sprinkler_queue_location = (0-1);
// when the current sprinkler command is supposed to end
uint32_t global_current_sprinkler_settings_end_time = (0-1);

////////////////////Command Queue//////////////////////////////////////////////
// Actual command queue             
struct CommandQueueItem global_command_queue[MAX_COMMAND_QUEUE_ITEMS] = {0};
uint8_t global_current_command_queue_location = (0-1);

////////////////////Message Queue//////////////////////////////////////////////
// Actual command queue                
struct MessageQueueItem global_message_queue[MAX_MESSAGE_QUEUE_ITEMS] = {0};
uint8_t global_current_message_queue_location = (0-1);
// global variable that stores the current errors encountered that will be sent
//    to the manager via an unsolicited error message
uint32_t global_error_message_bitfield = 0;

// the last error message's bitfield and send time for stopping reoccuring error messages
uint32_t last_error_message_sent_bitfield = 0;
uint32_t last_error_message_sent_rtc_time = 0;

////////////////////System State Variables/////////////////////////////////////
uint8_t global_mote_state = MOTESTATE_UNKNOWN;
uint8_t global_system_state = SYSTEM_STATE_UNKNOWN;

////////////////////Misc///////////////////////////////////////////////////////

