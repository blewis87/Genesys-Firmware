void  comparator_setup(void)
{
// Sets up and enables the comparator for current limit detection when moving
//    valve
   
   // turns off comparator
   setup_comparator(NC_NC_NC_NC);
   
   // Sets up the DAC for a range of VDD-VSS
   setup_DAC(DAC_VSS_VDD);
   
   // sets up an appropriate dac level depending on movement direction and 
   //    valve position
   set_comp_dac_level();
   
   // delay for comparator to setup?
   delay_cycles(5);
   
   // CP1 compares VLV_I at its neg input to the DAC output
   // CP2 off at power up and stays off
   setup_comparator(CP1_A1_DAC);
                                    
   // clear the comparator interrupt and enable it
   clear_interrupt(INT_COMP);
   enable_interrupts(INT_COMP);
}


uint16_t calc_valve_movement_time(uint16_t current_location, uint16_t destination)
{
// calculates the amount of time it takes to make the move from 
//    current location to destination and returns it in seconds (7.9 format)

   uint32_t temp_u32;
   
   // if valve is going to open
   if (current_location < destination)
   {
      temp_u32 = ((uint32_t)(destination - current_location) * (uint32_t)(global_valve_time_to_open_1024th)); 
   }  
   // if valve is going to close or not move at all
   else if (current_location >= destination)
   {
      temp_u32 = ((uint32_t)(current_location - destination) * (uint32_t)(global_valve_time_to_close_1024th)); 
   }  
   
   // now 8.24 format; convert to 16 bit 7.9 format (seconds)
   if (bit_test(temp_u32,14))
   {
      temp_u32 += 0x8000;
   }
   // return the time in 7.9 format (seconds)
   return(uint16_t)(temp_u32 >> 15);
}

void set_comp_dac_level(void)
{
// sets the dac level for the comparator for different situations/vlv positions

   // if valve is just begining to move and outside of the "almost close/opened"
   //    regime.  DAC level is lowered to allow for more current draw.
   if ((global_valve_time_in_motion_1024ths <= VLV_INIT_CURRENT_BOOST_TIME_1024THS)    \
      && (!(global_valve_position < VLV_NEAR_CLOSED_RANGE_MAX)) &&                     \
      (!(global_valve_position > VLV_NEAR_OPENED_RANGE_MAX)))
   {
      dac_write(DAC_LVL_INIT_CURRENT_BOOST);
   }
   
   // initial valve open (for a few seconds)
   else if (read_system_state() == SYSTEM_CAL_VLV_1)
   {
      dac_write(DAC_LVL_IN_MIDDLE);
   }
   
   // inital valve close (don't know where we are exactly)
   else if (read_system_state() == SYSTEM_CAL_VLV_2)
   {
      // normally, during this state, the valve position should be unknown

      // if the valve is almost (or fully) open after the inital open push aka
      //    it hit the open endstop
      if (global_valve_position > VLV_NEAR_OPENED_RANGE_MAX)
      {
         dac_write(DAC_LVL_CLOSING_NEAR_OPENED);
      }
      // otherwise, position is unknown and will be assumed to be closing near
      //    the closed endstop.  DAC level will be set as such to get a 
      //    consistent calibration
      else
      {
         dac_write(DAC_LVL_CLOSING_NEAR_CLOSED);
      }
   }
   
   // valve closing
   else if (IS_VLV_CLOSING)
   {
      // valve is almost closed (or closed) and closing
      if (global_valve_position < VLV_NEAR_CLOSED_RANGE_MAX)
      {
         dac_write(DAC_LVL_CLOSING_NEAR_CLOSED);
      }
      // valve is almost open (or open) and closing
      else if (global_valve_position > VLV_NEAR_OPENED_RANGE_MAX)
      {
         dac_write(DAC_LVL_CLOSING_NEAR_OPENED);
      }
      // valve is in the middle somewhere
      else
      {
         dac_write(DAC_LVL_IN_MIDDLE);
      }
   }
   
   // valve opening
   else if (IS_VLV_OPENING)
   {
      // valve is almost closed (or closed) and opening
      if (global_valve_position < VLV_NEAR_CLOSED_RANGE_MAX)
      {
         dac_write(DAC_LVL_OPENING_NEAR_CLOSED);
      }
      // valve is almost open (or open) and opening
      else if (global_valve_position > VLV_NEAR_OPENED_RANGE_MAX)
      {
         dac_write(DAC_LVL_OPENING_NEAR_OPENED);
      }
      // valve is in the middle somewhere
      else
      {
         dac_write(DAC_LVL_IN_MIDDLE);
      }
   }
   
   // other cases (like if this accidentally gets called before valve is
   //    actually moving, although it should get picked up by the initial 
   //    boost case
   else
   {
      dac_write(DAC_LVL_IN_MIDDLE);
   }
}


void set_comp_dac_level_ISR(void)
{
// sets the dac level for the comparator for different situations/vlv positions

   // if valve is just begining to move and outside of the "almost close/opened"
   //    regime.  DAC level is lowered to allow for more current draw.
   if ((global_valve_time_in_motion_1024ths <= VLV_INIT_CURRENT_BOOST_TIME_1024THS) \
      && (!(global_valve_position < VLV_NEAR_CLOSED_RANGE_MAX)) &&            \
      (!(global_valve_position > VLV_NEAR_OPENED_RANGE_MAX)))
   {
      dac_write(DAC_LVL_INIT_CURRENT_BOOST);
   }
   
   // initial valve open (for a few seconds)
   else if (read_system_state() == SYSTEM_CAL_VLV_1)
   {
      dac_write(DAC_LVL_IN_MIDDLE);
   }
   
   // inital valve close (don't know where we are exactly)
   else if (read_system_state() == SYSTEM_CAL_VLV_2)
   {
      // normally, during this state, the valve position should be unknown

      // if the valve is almost (or fully) open after the inital open push aka
      //    it hit the open endstop
      if (global_valve_position > VLV_NEAR_OPENED_RANGE_MAX)
      {
         dac_write(DAC_LVL_CLOSING_NEAR_OPENED);
      }
      // otherwise, position is unknown and will be assumed to be closing near
      //    the closed endstop.  DAC level will be set as such to get a 
      //    consistent calibration
      else
      {
         dac_write(DAC_LVL_CLOSING_NEAR_CLOSED);
      }
   }
   
   // valve closing
   else if (IS_VLV_CLOSING)
   {
      // valve is almost closed (or closed) and closing
      if (global_valve_position < VLV_NEAR_CLOSED_RANGE_MAX)
      {
         dac_write(DAC_LVL_CLOSING_NEAR_CLOSED);
      }
      // valve is almost open (or open) and closing
      else if (global_valve_position > VLV_NEAR_OPENED_RANGE_MAX)
      {
         dac_write(DAC_LVL_CLOSING_NEAR_OPENED);
      }
      // valve is in the middle somewhere
      else
      {
         dac_write(DAC_LVL_IN_MIDDLE);
      }
   }
   
   // valve opening
   else if (IS_VLV_OPENING)
   {
      // valve is almost closed (or closed) and opening
      if (global_valve_position < VLV_NEAR_CLOSED_RANGE_MAX)
      {
         dac_write(DAC_LVL_OPENING_NEAR_CLOSED);
      }
      // valve is almost open (or open) and opening
      else if (global_valve_position > VLV_NEAR_OPENED_RANGE_MAX)
      {
         dac_write(DAC_LVL_OPENING_NEAR_OPENED);
      }
      // valve is in the middle somewhere
      else
      {
         dac_write(DAC_LVL_IN_MIDDLE);
      }
   }
   
   // other cases (like if this accidentally gets called before valve is
   //    actually moving, although it should get picked up by the initial 
   //    boost case
   else
   {
      dac_write(DAC_LVL_IN_MIDDLE);
   }
}

int1 is_valve_position_valid(uint16_t value)
// returns true if valid range for valve
{
   if (value > VLV_POSITION_OPENED) return FALSE;
   else if ((value < VLV_POSITION_CLOSED) && (VLV_POSITION_CLOSED != 0)) return FALSE;
   else return TRUE;
}

int1 is_valve_value_full_open_or_close(uint16_t value)
// return true if valve is either full open or vull close
{
   if ((value == VLV_POSITION_OPENED) || (value == VLV_POSITION_CLOSED)) 
      return TRUE;
   else return FALSE;
}
