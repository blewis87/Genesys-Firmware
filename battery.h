// JG
uint16_t get_vbatt(int1 charge_state)
{
   /* Samples the V_EXT_REF to get the battery voltage
    * and returns a uint16_t of the sum of 4 samples
    * charge_state defines if you want the LTC3105 to be:
    * 0 - original state
    * 1 - not charging
    */
                                                         
   uint8_t nn;
   uint16_t v_batt_value = 0;
   int1     tmp_RB3;
   uint8_t  tmp_ccp2con;
   
   // save state of RB3, which includes the LTC3105_CTRL line
   tmp_ccp2con = CCP2CON;       
   
   // save state of CCP2, which controls charging aggressiveness
   // note that CCP2 will have control of the pin unless it is OFF
   tmp_RB3 = LTC3105_CTRL_pinstate; 
   
   // turn on ADC
   ADON = TRUE;         
   
   // set adc to VREF
   set_adc_channel(V_EXT_REF);
   
   // alter the charge state if necessary
   // charge state off
   if (charge_state == 1)
   {
      setup_ccp2(CCP_OFF);   
      output_low(LTC3105_CTRL);
   }
   
   // sum up 4 samples from the adc
   for (nn=0; nn<4; nn++)
   {
      v_batt_value += read_adc();
   }
   
   // Now return charger to its previous state
   LTC3105_CTRL_pinstate = tmp_RB3;
   CCP2CON = tmp_ccp2con;
   
   // turn off adc
   ADON = FALSE;
   
   // return the voltage
   return v_batt_value;
}

uint16_t get_vbatt_ISR(int1 charge_state)
{
   /* Samples the V_EXT_REF to get the battery voltage
    * and returns a uint16_t of the sum of 4 samples
    * charge_state defines if you want the LTC3105 to be:
    * 0 - original state
    * 1 - not charging
    */
    
   uint8_t nn;
   uint16_t v_batt_value = 0;
   int1     tmp_RB3;
   uint8_t  tmp_ccp2con;
   
   // save state of RB3, which includes the LTC3105_CTRL line
   tmp_ccp2con = CCP2CON;
   
   // save state of CCP2, which controls charging aggressiveness
   // note that CCP2 will have control of the pin unless it is OFF
   tmp_RB3 = LTC3105_CTRL_pinstate; 
   
   // turn on ADC
   ADON = TRUE;         
   
   // set adc to VREF
   set_adc_channel(V_EXT_REF);
   
   // alter the charge state if necessary
   // charge state off
   if (charge_state == 1)
   {
      setup_ccp2(CCP_OFF);   
      output_low(LTC3105_CTRL);
   }
   
   // sum up 4 samples from the adc
   for (nn=0; nn<4; nn++)
   {
      v_batt_value += read_adc();
   }
   
   // Now return charger to its previous state
   LTC3105_CTRL_pinstate = tmp_RB3;
   CCP2CON = tmp_ccp2con;
   
   // turn off adc
   ADON = FALSE;
   
   // return the voltage
   return v_batt_value;
}

uint16_t get_vgen(int1 charge_state)
{
   /* Samples the V_GEN to get the battery voltage
    * and returns a uint16_t of the sum of 4 samples
    * charge_state defines if you want the LTC3105 to be:
    * 0 - original state
    * 1 - not charging
    */
    
   uint8_t nn;
   uint16_t vgen_value = 0;
   int1     tmp_RB3;
   uint8_t  tmp_ccp2con;
   
   // save state of RB3, which includes the LTC3105_CTRL line
   tmp_ccp2con = CCP2CON;
   
   // save state of CCP2, which controls charging aggressiveness
   // note that CCP2 will have control of the pin unless it is OFF
   tmp_RB3 = LTC3105_CTRL_pinstate; 
   
   // turn on ADC
   ADON = TRUE;         
   
   // set adc to VREF
   set_adc_channel(V_GEN);
   
   // alter the charge state if necessary
   // charge state off
   if (charge_state == 1)
   {
      setup_ccp2(CCP_OFF);   
      output_low(LTC3105_CTRL);
   }
   
   // sum up 3 samples from the adc
   for (nn=0; nn<3; nn++)
   {
      vgen_value += read_adc();
   }
   
   // Now return charger to its previous state
   LTC3105_CTRL_pinstate = tmp_RB3;
   CCP2CON = tmp_ccp2con;
   
   // turn off adc
   ADON = FALSE;
   
   // save vgen to global value
   global_vgen = vgen_value;
   
   // return the voltage
   return vgen_value;
}

uint16_t calc_vbatt_BCD(uint16_t vref_count)
{
   /* Takes the voltage count as an input (summed from ADC)
    * and outputs the battery integer and fractional as a single uint32_t
    * It's output is BCD as follows:
    * volts (.) tenths hundredths thousandths
   */
   uint8_t volts, tenths, hundredths, thousandths;

   uint32_t tempo_u32, frac_part;
   uint16_t tempo_u16;  
   
   uint16_t    bat_frac_part;    // in units of mV
   
   tempo_u32 = VAL32_FVR_BAT/vref_count;   // = 1 count in volts in 12.20 format
   tempo_u32 *= 1023;                        // bat voltage in 12.20 format
   tempo_u16 = (uint16_t)(tempo_u32 >> 8);
                                             // bat voltage in 4.12 format
   volts = (uint8_t)(tempo_u16 >> 12);       // 2 or 3.....   
   
   frac_part = (uint32_t)(tempo_u16 & 0x0fff);
                                       // one count of frac_part is 1/4096 volt
                                       // no rounding, full resolution
   tempo_u32 = (frac_part * 1000);     // We ignore the mV rounding issue
                                       //  cuz battery measurement doesn't
                                       //  support even mV resolution
   tempo_u32 >>= 12;
   tempo_u16 = (uint16_t)(tempo_u32);
   tenths = tempo_u16/100;
   tempo_u16 -= (uint16_t)(tenths)*100;
   hundredths = tempo_u16/10;
   thousandths = tempo_u16 % 10;

   return ((uint16_t)(volts)*4096 + (uint16_t)(tenths)*256 + (uint16_t)(hundredths)*16 + (uint16_t)(thousandths));
}

uint16_t calc_vgen_BCD(uint16_t vref_count)
{
   /* Takes the voltage count as an input (summed from ADC)
    * and outputs the vgen integer and fractional as a single uint32_t
    * It's output is BCD as follows:
    * volts (.) tenths hundredths thousandths
   */
   uint8_t volts, tenths, hundredths, thousandths;

   uint32_t tempo_u32, frac_part;
   uint16_t tempo_u16;  
   
   uint16_t    bat_frac_part;    // in units of mV
   
   // First calculate a single count of the ADC in volts in 12.20 format?
   tempo_u32 = VAL32_FVR_BAT/get_vbatt(0);   // = 1 count in volts in 12.20 format
   tempo_u32 *= vref_count;                  // vgen in 12.20 format
   tempo_u16 = (uint16_t)(tempo_u32 >> 8);
                                             // bat voltage in 4.12 format
   volts = (uint8_t)(tempo_u16 >> 12);       // 2 or 3.....   
   
   frac_part = (uint32_t)(tempo_u16 & 0x0fff);
                                       // one count of frac_part is 1/4096 volt
                                       // no rounding, full resolution
   tempo_u32 = (frac_part * 1000);     // We ignore the mV rounding issue
                                       //  cuz battery measurement doesn't
                                       //  support even mV resolution
   tempo_u32 >>= 12;
   tempo_u16 = (uint16_t)(tempo_u32);
   tenths = tempo_u16/100;
   tempo_u16 -= (uint16_t)(tenths)*100;
   hundredths = tempo_u16/10;
   thousandths = tempo_u16 % 10;

   return ((uint16_t)(volts)*4096 + (uint16_t)(tenths)*256 + (uint16_t)(hundredths)*16 + (uint16_t)(thousandths));
}

uint32_t calc_vbatt(uint16_t vref_count)
{
   /* Takes the voltage count as an input (summed from ADC)
    * and outputs the battery integer and fractional as a single uint32_t
    * It's output is (in binary) 00000000IIIIIIIIFFFFFFFFFFFFFFFF
    * Where the first 8 bits are 0, the next 8 represent the integer
    * part of the battery, and the next 16 are the fractional part
    */
   uint32_t tempo_u32, frac_part;
   uint16_t tempo_u16;
   
   uint16_t    bat_frac_part;    // in units of .1mV     
   uint8_t     bat_int_part;
   
   tempo_u32 = VAL32_FVR_BAT/vref_count;   // = 1 count in volts in 12.20 format
   tempo_u32 *= 1023;                        // bat voltage in 12.20 format
   tempo_u16 = (uint16_t)(tempo_u32 >> 8);
                                             // bat voltage in 4.12 format
                                             
   //This stores the integer part shifted into the final result                                    
   bat_int_part = (uint8_t)(tempo_u16 >> 12);      // 2 or 3.....
   frac_part = (uint32_t)(tempo_u16 & 0x0fff);
                                       // one count of frac_part is 1/4096 volt
   tempo_u32 = (frac_part * 10000);    //
   tempo_u32 >>= 12; // divide by 4096. temp32 now = frac part (0 to 9999)
   bat_frac_part = (uint16_t)(tempo_u32);
   
   return ((((uint32_t)bat_int_part) << 16) | (bat_frac_part));
   //return (uint32_t)bat_frac_part;
}

void check_and_deal_with_battery(void)
// checks the battery voltage (charging and not charging) and decides to turn on
// charger/off charger send a warning, or send a warning and shutdown.
{
   uint16_t batt_chrg, batt_no_chrg;
   
   // if the valve is not moving (otherwise, abort)
   if (IS_VLV_COASTING)
   {
      // grab a fresh battery reading
      batt_chrg = get_vbatt(0);
      batt_no_chrg = get_vbatt(1);
      
      // if the battery is dead and system needs to be shutdown
      if (batt_no_chrg > BATTERY_DEAD)
      {
         global_control_loop_mechanism = BRAKE_AND_CHARGE_START;
         // set the error bitfield and message
         global_error_message_bitfield |= ERR_MSG_BATT_DEAD_SLEEP;
         PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_ERROR_MSG);
         // set the shutdown message and schedule a shutdown
         global_shutdown_cause = ERR_BATT_DEAD_SLEEP; 
         PUSH_PRIORITY_QUEUE(OPEN_VALVE_UNKNOWN_STATE);  
         PUSH_TIME_QUEUE(global_rtc_time + 10, SHUTDOWN_SYSTEM);  
      }
      // if the battery is almost dead and the manager needs a warning
      else if (batt_no_chrg > BATTERY_WARN)
      {
         global_control_loop_mechanism = BRAKE_AND_CHARGE_START;
         // set the error bitfield and message
         global_error_message_bitfield |= ERR_MSG_BATT_LOW_WARN;
         PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_ERROR_MSG);
      }
      // if the battery charging needs to be turned off (is in brake + charge control)
      else if ((batt_chrg < BATTERY_STOP_CHARGING) &&                              \
         (global_control_loop_mechanism == BRAKE_AND_CHARGE))
      {
         global_control_loop_mechanism = BRAKE_ONLY;
      }
      // if the battery charging needs to be turned on (is in brake only control)
      else if ((batt_no_chrg > BATTERY_START_CHARGING) &&                     \
         (global_control_loop_mechanism == BRAKE_ONLY))
      {
         global_control_loop_mechanism = BRAKE_AND_CHARGE_START;
      }
   }
}
         



