// EEPROM.h
// This contains all things related to the EEPROM
// -functions
// -#defines for memory locations
// -#defines conveted to eeprom values to load/store out of EEPROM

////////////////////EEPROM Locations Start///////////////////////////////////////////////////////////// 
                                                            
// start of EEPROM locations
#define EE_START_LOCATION     0x0000
// Location ID                                                                   
#define EE_SPRINKLER_NUM      0x0000
// Cause for shutdown/restart
#define EE_RESTART_CAUSE      0x0002                                              
// Last valve calibrations and time           
#define EE_VLV_TIME_TO_OPEN   0x0004                                     
#define EE_VLV_TIME_TO_CLOSE  0x0006                                            
#define EE_VLV_CAL_UTC_TIME   0x0008                                    
// Last valve position
#define EE_VLV_POSITION       0x000C                                                   
                 
// start of eeprom changable variables                                            
#define EE_VLV_NEAR_CLOSED_RANGE_MAX 0x000E
#define EE_VLV_NEAR_OPENED_RANGE_MAX 0x0010              
#define EE_DAC_LVL_OPENING_NEAR_OPENED 0x0012  
#define EE_DAC_LVL_OPENING_NEAR_CLOSED 0x0014  
#define EE_DAC_LVL_CLOSING_NEAR_OPENED 0x0016  
#define EE_DAC_LVL_CLOSING_NEAR_CLOSED 0x0018  
#define EE_DAC_LVL_IN_MIDDLE 0x001A
#define EE_DAC_LVL_INIT_CURRENT_BOOST 0x001C
#define EE_VLV_INIT_CURRENT_BOOST_TIME_1024THS 0x001E 
#define EE_VLV_NOT_SPIN_LIMIT 0x0020  
#define EE_VLV_NOT_SPIN_FIX 0x0022  
#define EE_NOT_SPINNING_MAG_DECOUPLING_RPM_FIXED 0x0024  
#define EE_VLV_MAGNETIC_COUPLING_LIMIT 0x0026  
#define EE_VLV_MAGNETIC_COUPLING_FIX 0x0028  
#define EE_RPM_TIMEOUT 0x002A
#define EE_MOTE_SEARCH_TIME 0x002C
#define EE_MOTE_NETWORK_SHUTDOWN_TIMEOUT 0x002E                                        
// end of eeprom changeable variables

// last byte of EEPROM used
#define EE_END_LOCATION       0x002F    
                                         

////////////////////EEPROM Locations End///////////////////////////////////////////////////////////////

 ////////////////////EEPROM defines->globals Start//////////////////////////////////////////////////////  
// Maximum value of "near closed" range
uint16_t VLV_NEAR_CLOSED_RANGE_MAX = 0x1000;
// Maximum value of "near opened" range
uint16_t VLV_NEAR_OPENED_RANGE_MAX =   0x7000;    
                                           
// DAC level while opening near the fully open end
uint16_t DAC_LVL_OPENING_NEAR_OPENED =      24;
// DAC level while opening near the fully closed end
uint16_t DAC_LVL_OPENING_NEAR_CLOSED =      20;   
// DAC level while closing near the fully open end
uint16_t DAC_LVL_CLOSING_NEAR_OPENED =      20;
// DAC level while closing near the fully closed end                        
uint16_t DAC_LVL_CLOSING_NEAR_CLOSED =      24; 
// DAC level while moving in the middle of the valve range and it's default
uint16_t DAC_LVL_IN_MIDDLE =                19; 
// DAC level for the inital current boost/lowered current limit  
uint16_t DAC_LVL_INIT_CURRENT_BOOST =       19;     
                                                           
// Amount of millispans that a current boost/lovered dac level is implemented.
//    This is done outside of the (NEAR_CLOSED/OPEN_RANGE)   
//    @ 45 seconds to close, 30 = 1.3 seconds
uint16_t VLV_INIT_CURRENT_BOOST_TIME_1024THS =  30;  
                                              
// valve not-spinning and magnetic de-coupling defines                                                    
// below this number is the regime where we watch for the not spinning case (low torque) 
uint16_t VLV_NOT_SPIN_LIMIT =               0x4000;
// where we send the valve when we want to fix it.  If it gets fixed before reaching this valve position
//    it stops. If it does not get fixed by the time it reaches this position, it sends an error and stops.
uint16_t VLV_NOT_SPIN_FIX =                 0x7000;
                                              
// above this rpm, consider magnetic coupling or not-spinning fixed
uint8_t NOT_SPINNING_MAG_DECOUPLING_RPM_FIXED =  8;       
                                                  
// above this number is the regime where we watch for the magnetic decoupling
uint16_t VLV_MAGNETIC_COUPLING_LIMIT =      0x5000;       
// where to move the valve at a time when trying to recover 
uint16_t VLV_MAGNETIC_COUPLING_FIX =        0x1000;             
// amount to move the valve at a time when trying to recover   
                                                                          
// how many seconds it takes to consider the rpm to be zero (or infinite for higher valve settings)
// 8 = 7.5 RPM (8 may be max of what ccp5 can handle without overflow)   
// JG SAYS that RPM_TIMEOUT of 2 makes more sense because
//  any time which exceeds 1 second indicates that the RPM is less than 1.2,
//  a very low number which we can consider to equal 0.
uint16_t RPM_TIMEOUT =                     2;
                                       
// amount of seconds for mote to stay searching for advertisements                    
uint16_t MOTE_SEARCH_TIME = 120;       
// amount of time the mote will stay disconnected from the network before shutting down (1200 = 20 min)                                   
uint16_t MOTE_NETWORK_SHUTDOWN_TIMEOUT = 1200;                     

////////////////////EEPROM defines->globals End//////////////////////////////////////////////////////// 
 
////////////////////EEPROM Functions Start///////////////////////////////////////////////////////////// 
uint8_t read_ee1(uint16_t source)
{                                                                 
   return read_eeprom(source);
}                
uint16_t read_ee2(uint16_t source)
{
   return (make16(read_eeprom(source+1), read_eeprom(source)));
} 
uint32_t read_ee4(uint16_t source)
{
   return (make32(read_eeprom(source+3), read_eeprom(source+2), read_eeprom(source+1), read_eeprom(source)));
}

void write_ee1(uint16_t dest, uint8_t dat8)
{
   write_eeprom(dest, dat8);
}

void write_ee2(uint16_t dest, uint16_t dat16)
{
   write_eeprom(dest++, make8(dat16,0));
   write_eeprom(dest, make8(dat16,1));
}

void write_ee4(uint16_t dest, uint32_t dat32)
{
   write_eeprom(dest++, make8(dat32,0));
   write_eeprom(dest++, make8(dat32,1));
   write_eeprom(dest++, make8(dat32,2));
   write_eeprom(dest, make8(dat32,3));
}                                             

void store_all_eeprom_values(void)
{                                                                                                  

   // store valve calibration times in EEPROM
   write_ee2(EE_VLV_TIME_TO_OPEN, global_valve_time_to_open_1024th);
   write_ee2(EE_VLV_TIME_TO_CLOSE, global_valve_time_to_close_1024th);
   write_ee4(EE_VLV_CAL_UTC_TIME, global_valve_calibration_utc_time);
   write_ee2(EE_VLV_POSITION, global_valve_position);
   // save the shutdown/restart reason in EEPROM
   write_ee1(EE_RESTART_CAUSE, global_shutdown_cause);                                        
                                                                        
}


void read_all_eeprom_values(void)
{
   // if EEPROM is freshly flashed, overwrite sprinkler number with 0
   if (read_ee2(EE_SPRINKLER_NUM) == 0xFFFF) write_ee2(EE_SPRINKLER_NUM,0);     
   // pull the sprinkler number from EEPROM 
   global_sprinkler_num = read_ee2(EE_SPRINKLER_NUM);
   // grab the shutdown cause and overwrite it for next time
   global_previous_shutdown_cause = read_ee1(EE_RESTART_CAUSE);
   write_ee1(EE_RESTART_CAUSE,NO_ERR);
                                                   
   // read valve calibration times and position in EEPROM
   // if freshly flashed, valve position should default to 0xFFFF, which
   // is defined as VLV_POSITION_UNKNOWN
   global_valve_time_to_open_1024th = read_ee2(EE_VLV_TIME_TO_OPEN);
   global_valve_time_to_close_1024th = read_ee2(EE_VLV_TIME_TO_CLOSE);
   
   if ((global_valve_time_to_open_1024th < ERROR_VLV_CAL_TIME_LO) || \
       (global_valve_time_to_open_1024th > ERROR_VLV_CAL_TIME_HI))
   {
      global_valve_time_to_open_1024th = DEFAULT_VLV_TIME_TO_OPEN;
      write_ee2 (EE_VLV_TIME_TO_OPEN, DEFAULT_VLV_TIME_TO_OPEN);
   }
   
   if ((global_valve_time_to_close_1024th < ERROR_VLV_CAL_TIME_LO) || \
       (global_valve_time_to_close_1024th > ERROR_VLV_CAL_TIME_HI))
   {
      global_valve_time_to_close_1024th = DEFAULT_VLV_TIME_TO_CLOSE;
      write_ee2 (EE_VLV_TIME_TO_OPEN, DEFAULT_VLV_TIME_TO_OPEN);
   }

 // for cleared EEPROM, the following read will give us 0xFFFFFFFF, so
 //  add 1 so that it is 0.  This global is compared to current time
 //  to determine if the valve calibration is stale
   global_valve_calibration_utc_time = 1 + read_ee4(EE_VLV_CAL_UTC_TIME);
   global_valve_position = read_ee2(EE_VLV_POSITION);
   // store unknown valve position incase it doesn't get stored properly on shutdown/restart
   //    so the position isn't used from before
   write_ee2(EE_VLV_POSITION, VLV_POSITION_UNKNOWN);

}

void store_vcal_eeprom_values(void)
{  
   // store valve calibration times in EEPROM
   write_ee2(EE_VLV_TIME_TO_OPEN, global_valve_time_to_open_1024th);
   write_ee2(EE_VLV_TIME_TO_CLOSE, global_valve_time_to_close_1024th);
   write_ee4(EE_VLV_CAL_UTC_TIME, global_valve_calibration_utc_time);
}                                                              

void read_all_eeprom_variables(void)    
{                                       
   uint16_t temp_u16;

   temp_u16 = read_ee2(EE_VLV_NEAR_CLOSED_RANGE_MAX);       
   if (temp_u16 != 0xFFFF) VLV_NEAR_CLOSED_RANGE_MAX = temp_u16; 
   
   temp_u16 = read_ee2(EE_VLV_NEAR_OPENED_RANGE_MAX);       
   if (temp_u16 != 0xFFFF) VLV_NEAR_OPENED_RANGE_MAX = temp_u16;
   
   temp_u16 = read_ee2(EE_DAC_LVL_OPENING_NEAR_OPENED);       
   if (temp_u16 != 0xFFFF) DAC_LVL_OPENING_NEAR_OPENED = temp_u16;
   
   temp_u16 = read_ee2(EE_DAC_LVL_OPENING_NEAR_CLOSED);       
   if (temp_u16 != 0xFFFF) DAC_LVL_OPENING_NEAR_CLOSED = temp_u16;
   
   temp_u16 = read_ee2(EE_DAC_LVL_CLOSING_NEAR_OPENED);       
   if (temp_u16 != 0xFFFF) DAC_LVL_CLOSING_NEAR_OPENED = temp_u16;
   
   temp_u16 = read_ee2(EE_DAC_LVL_CLOSING_NEAR_CLOSED);       
   if (temp_u16 != 0xFFFF) DAC_LVL_CLOSING_NEAR_CLOSED = temp_u16;
   
   temp_u16 = read_ee2(EE_DAC_LVL_IN_MIDDLE);       
   if (temp_u16 != 0xFFFF) DAC_LVL_IN_MIDDLE = temp_u16;
   
   temp_u16 = read_ee2(EE_DAC_LVL_INIT_CURRENT_BOOST);       
   if (temp_u16 != 0xFFFF) DAC_LVL_INIT_CURRENT_BOOST = temp_u16;
   
   temp_u16 = read_ee2(EE_VLV_INIT_CURRENT_BOOST_TIME_1024THS);       
   if (temp_u16 != 0xFFFF) VLV_INIT_CURRENT_BOOST_TIME_1024THS = temp_u16;
   
   temp_u16 = read_ee2(EE_VLV_NOT_SPIN_LIMIT);       
   if (temp_u16 != 0xFFFF) VLV_NOT_SPIN_LIMIT = temp_u16;
   
   temp_u16 = read_ee2(EE_VLV_NOT_SPIN_FIX);       
   if (temp_u16 != 0xFFFF) VLV_NOT_SPIN_FIX = temp_u16;  
   
   temp_u16 = read_ee2(EE_NOT_SPINNING_MAG_DECOUPLING_RPM_FIXED);       
   if (temp_u16 != 0xFFFF) NOT_SPINNING_MAG_DECOUPLING_RPM_FIXED = temp_u16;  
   
   temp_u16 = read_ee2(EE_VLV_MAGNETIC_COUPLING_LIMIT);       
   if (temp_u16 != 0xFFFF) VLV_MAGNETIC_COUPLING_LIMIT = temp_u16;  
   
   temp_u16 = read_ee2(EE_VLV_MAGNETIC_COUPLING_FIX);       
   if (temp_u16 != 0xFFFF) VLV_MAGNETIC_COUPLING_FIX = temp_u16;
   
   temp_u16 = read_ee2(EE_RPM_TIMEOUT);                      
   if (temp_u16 != 0xFFFF) RPM_TIMEOUT = temp_u16;  
   
   temp_u16 = read_ee2(EE_MOTE_SEARCH_TIME);       
   if (temp_u16 != 0xFFFF) MOTE_SEARCH_TIME = temp_u16;  
   
   temp_u16 = read_ee2(EE_MOTE_NETWORK_SHUTDOWN_TIMEOUT);       
   if (temp_u16 != 0xFFFF) MOTE_NETWORK_SHUTDOWN_TIMEOUT = temp_u16;   
}
   

////////////////////EEPROM Functions End/////////////////////////////////////////////////////////////// 
                                                                                                   

 
