void mote_init(void)
{
   // sets the mote message sequence to a random number
   init_msgSEQ();
   // enable appropriate interrupts for mote response
   clear_interrupt(INT_EXT2_H2L);
   enable_interrupts(INT_EXT2_H2L);
}   
   
void mote_reset(void)
{
   // Resets the mote, enables interrupts for the mote to respond, and
   // schedules a WAIT_FOR_BOOT_EVENT to see if the mote responded
 
   // Force an reset
   output_low(MOTE_RESETn);
   // definitely long enough at any clock speed
   setup_T0_int(T0_250MS);
   while (!TMR0IF);   
   output_high(MOTE_RESETn);
     
   // add boot event checkup
   PUSH_TIME_QUEUE_MACRO((global_rtc_time + 6), WAIT_FOR_BOOT_EVENT)
}

uint8_t mote_sleep(void)
{
   uint8_t  result_byte;

   // set the join duty and timeout error handling
   global_set_get_response = FALSE;
   pkgen_sleep();
   result_byte = dust_hdlc_pksend();
   setup_T0_int(T0_250MS);
   while (!TMR0IF && !result_byte);
   if (TMR0IF || result_byte)
   {
      //return ERR_MOTE_UNRESPONSIVE;
   }
   else
   {
      return NO_ERR;
   }
   return NO_ERR;
}
void     search_for_strongest(void)
{
   uint8_t     result_byte;
   int8_t      rssi_best = -128;
   int16_t     net_id_best = 0, net_id_current = 0;;
   uint32_t    search_start_time = 0;
   int1        net_id_selected = 0;
   
   strcpy (global_temp_line_buff, " Searching ???? ");
   LCD_line1(global_temp_line_buff);
   strcpy (global_temp_line_buff, "nnnn iii sss ddd");
   LCD_line2(global_temp_line_buff);
  
   //prepare mote message for searching
   pkgen_search();
   result_byte = dust_hdlc_pksend();         
   
   // hangs until search begins or request times out;
   setup_T0_int(T0_250MS);
   while (!TMR0IF && (global_mote_state != MOTESTATE_SEARCHING) && !result_byte);
   
   //  immediately goes to sleep if ERR_MTNOTREADY
   if (TMR0IF || global_mote_state != MOTESTATE_SEARCHING)
   {
      global_shutdown_cause = ERR_NO_ADV_WHILE_SEARCHING;
      deep_sleep();      
   }
   
   
   // Search algorithm continues for MOTE_SEARCH_TIME or until net_id is selected
   search_start_time = global_rtc_time;
   while((global_rtc_time < (search_start_time + MOTE_SEARCH_TIME)) && !net_id_selected)
   {
      // use SW1 to exit early, give user a chance to select strongest netid
      setup_T0_int(T0_2S);
      while (!TMR0IF)
      {
         if (!input(SW1n))               
         {
            //wait for 250MS for debounce                   
            setup_T0_int(T0_250MS);
            while (!TMR0IF);                                                 
            //check button after 250ms to see if it's still depressed, if so, exit
            if (!input(SW1n))
            {
               net_id_selected = TRUE;
               break;
            }                                                               
         }
      }
      
      // if a new advertisement flag is triggered                     
      if (advert_rcvd_fl)
         {
            //saves current net id
            net_id_current = network_id_rcvd;
            //displays information from advertisement
            process_advert();
            //reset advertisement flag
            advert_rcvd_fl = FALSE;

            //if the advertisement came from a manager, give user a chance to 
            //select it and exit
            if (mote_id_rcvd == 1)
            {
               // if lcd is connected, give user a chance to select it
               if (global_lcd_enabled == TRUE)
               {
                  strcpy (global_temp_line_buff, "SW1 to Join     ");
                  global_temp_line_buff[12] = hi2asc(make8(network_id_rcvd,1));
                  global_temp_line_buff[13] = lo2asc(make8(network_id_rcvd,1));
                  global_temp_line_buff[14] = hi2asc(make8(network_id_rcvd,0));
                  global_temp_line_buff[15] = lo2asc(make8(network_id_rcvd,0));
                  LCD_line1(global_temp_line_buff);
                  
                  // give the user a chance to select the current NetID for 4sec
                  setup_T0_int(T0_4S);
                  while (!TMR0IF)
                  {                              
                     if (!input(SW1n))
                     {
                        net_id_best = net_id_current;
                        net_id_selected = 1;
                        break;
                     }
                  }
               }
               
               // store the strongest signal unless user selected network
               if ((rssi_rcvd > rssi_best) && (net_id_selected == FALSE))
               {
                  rssi_best = rssi_rcvd;
                  net_id_best = network_id_rcvd;
               }
               strcpy (global_temp_line_buff, "Strongest:      ");
               global_temp_line_buff[12] = hi2asc(make8(net_id_best,1));
               global_temp_line_buff[13] = lo2asc(make8(net_id_best,1));
               global_temp_line_buff[14] = hi2asc(make8(net_id_best,0));
               global_temp_line_buff[15] = lo2asc(make8(net_id_best,0));
               LCD_line1(global_temp_line_buff);
            }
         }
      }
               
   disable_interrupts(GLOBAL);
   
   net_id_best = __swap16(net_id_best);        // swap the bytes [why?]
   if (!net_id_best)
   {
      // send to deep sleep as no network was found
      global_shutdown_cause = ERR_NO_ADV_WHILE_SEARCHING;
      deep_sleep();
   }
  
   // now set the best network id into the motes NV memory
   pkgen_setnv_networkid(net_id_best);
   result_byte = dust_hdlc_pksend();
   setup_T0_int(T0_250MS);
   while (!TMR0IF && !setnetid_done_fl && !result_byte);
   
   if (TMR0IF || result_byte)
   {
      // send to deep sleep network couldn't be joined
      global_shutdown_cause = ERR_NETWORK_JOIN_FAIL;
      deep_sleep();
   }
   
   strcpy (global_temp_line_buff, " Search Success ");
   LCD_line1(global_temp_line_buff);
   strcpy (global_temp_line_buff, "Next join:      ");
   global_temp_line_buff[12] = hi2asc(make8(net_id_best,0));
   global_temp_line_buff[13] = lo2asc(make8(net_id_best,0));
   global_temp_line_buff[14] = hi2asc(make8(net_id_best,1));
   global_temp_line_buff[15] = lo2asc(make8(net_id_best,1));
   LCD_line2(global_temp_line_buff);

   // Let it sit on the screen for a couple seconds
   setup_T0_int(T0_2S);
   while (!TMR0IF);      
   
   happy_lites();                                           
                                                
   // send to deep sleep as network id is now set
   global_shutdown_cause |= NEW_NETWORK_ID_SET;
   deep_sleep();
   
   return;
}

void  process_advert(void)
{
   strcpy (global_temp_line_buff, "                ");

   global_temp_line_buff[0] = hi2asc(make8(network_id_rcvd,1));
   global_temp_line_buff[1] = lo2asc(make8(network_id_rcvd,1));
   global_temp_line_buff[2] = hi2asc(make8(network_id_rcvd,0));
   global_temp_line_buff[3] = lo2asc(make8(network_id_rcvd,0));
   // NOTE network_id_rcvd is swapped when rcvd to simplify
   //  the netID comparisons that take place later
   
   LCD_line2(global_temp_line_buff); 
   
   mote_id_rcvd = __swap16(mote_id_rcvd);    // swap the bytes [why?]
   
   LCD_place_uint16(mote_id_rcvd, 1, 5, 3);

   
      if (rssi_rcvd >= 0)
      {
         LCD_place_uint8(((uint8_t)(rssi_rcvd) + 128), 1, 9, 3);
      }
      else
      {
         LCD_place_uint8((128 - abs(rssi_rcvd)), 1, 9, 3);
      }
   
   LCD_place_uint8(hop_depth_rcvd, 1, 13, 3);
}

uint8_t     initiate_join(void)
{
// initiates a join by setting the joinduty to the max  
   uint8_t  result_byte;
   
   // set the join duty and timeout error handling
   global_set_get_response = FALSE;
   pkgen_set_joinduty(0xFF);
   result_byte = dust_hdlc_pksend();
   setup_T0_int(T0_250MS);
   while (!TMR0IF && !global_set_get_response && !result_byte);
   if (TMR0IF || result_byte)
      return ERR_MTJOINDUTY; 
   
   // set the joinkey and timeout error handling  
   global_set_get_response = FALSE;
   pkgen_setnv_joinkey(joinkey);
   result_byte = dust_hdlc_pksend();
   setup_T0_int(T0_250MS);
   while (!TMR0IF && !global_set_get_response && !result_byte);
   if (TMR0IF || result_byte)
      return ERR_MTJOINKEY;  
      
   // NOTE that we assume that the non-volatile memory of the mote
   //  contains a valid network ID....
   // The join could fail if blue box not found or if it is trying to join
   //  to a nonvalid network ID
   
   // actually send the join command
   global_set_get_response = FALSE;
   pkgen_join();
   result_byte = dust_hdlc_pksend();
   setup_T0_int(T0_250MS);
   while (!TMR0IF && !global_set_get_response && !result_byte);
   if (TMR0IF || result_byte)
      return ERR_NETWORK_JOIN_FAIL;
    
   return NO_ERR;
}

void mote_state_check(void)
{
// Asks mote what it's status is and responds accordingly (including following
// up on the mote status later)
// Restarts mote if needed, triggers join/rejoin if needed, and continues to 
// check status until mote is operational
   uint8_t result_byte;

   // send the mote status package to the mote
   global_set_get_response = FALSE;
   pkgen_get_motestatus();
   result_byte = dust_hdlc_pksend();                                              
   setup_T0_int(T0_250MS);
   while (!TMR0IF && !global_set_get_response && !result_byte);
   
   // if the mote status request fails, trigger a mote reboot
   if (TMR0IF || result_byte)
   {
      PUSH_PRIORITY_QUEUE_MACRO(RESET_MOTE);
      PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), CHECK_MOTE_STATE)
   }
   // if it has been more than MOTE_NETWORK_SHUTDOWN_TIMEOUT seconds since the mote 
   //    returned an operational state, and the mote is in the SYSTEM_RUN state or the SYSTEM_IDLE state
   //    (not looking for a network)
   else if (((global_rtc_time - global_last_connected_time) > MOTE_NETWORK_SHUTDOWN_TIMEOUT) && \
      ((read_system_state() == SYSTEM_RUN) || (read_system_state() == SYSTEM_IDLE)))   
   {                                      
         // set the shutdown cause and schedule a shutdown 10 seconds later
         global_shutdown_cause = ERR_NO_NETWORK_TIMEOUT;
         PUSH_PRIORITY_QUEUE(OPEN_VALVE_UNKNOWN_STATE);  
         PUSH_TIME_QUEUE(global_rtc_time + 10, SHUTDOWN_SYSTEM);   
   }
   
   // if mote is idle or disconnected, initiate a join and check on mote later
   else if ((global_mote_state == MOTESTATE_IDLE) ||                          \
      (global_mote_state == MOTESTATE_DISCONNECTED))  
   {
      PUSH_PRIORITY_QUEUE_MACRO(INIT_JOIN);
      PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), CHECK_MOTE_STATE)
//      LCD_place_char('D',1,15);    
   }                                                         
   
   // if mote is in one of these odd/unwanted modes, trigger reset
   // and a status check (to initiate a join) in the future
   else if ((global_mote_state == MOTESTATE_RADIOTEST) ||                     \
      (global_mote_state == MOTESTATE_UNKNOWN))                     
   {   
      PUSH_PRIORITY_QUEUE_MACRO(RESET_MOTE);
      PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), CHECK_MOTE_STATE)
//      LCD_place_char('U',1,15);
   }
   
   // if mote is in promiscuous listen mode but the system state is not 
   //    searching for the network, restart as it's unwanted.
   else if ((global_mote_state == MOTESTATE_PROMISCUOUS) &&                   \
      (!(read_system_state() == SYSTEM_SEARCHING_FOR_NETWORK)))
   {                                                    
      PUSH_PRIORITY_QUEUE_MACRO(RESET_MOTE);
      PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), CHECK_MOTE_STATE)
//      LCD_place_char('N',1,15);
   }
   
   // if the mote is booting or initiating a join, check up on it later
   else if ((global_mote_state == MOTESTATE_INIT) ||                          \
      (global_mote_state == MOTESTATE_SEARCHING) ||                           \
      (global_mote_state == MOTESTATE_NEGOTIATING) ||                         \
      (global_mote_state == MOTESTATE_CONNECTED))
   {                                                                               
      PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), CHECK_MOTE_STATE)  
//      LCD_place_char('I',1,15); 
   }
   
   // mote is operational, do nothing
   else if (global_mote_state == MOTESTATE_OPERATIONAL)
   { 
      global_last_connected_time = global_rtc_time;
//      LCD_place_char('O',1,15);     
      break;
   }
   
   // incase there is some other crazy mote status, restart mote and retry
   else
   {
      PUSH_PRIORITY_QUEUE_MACRO(RESET_MOTE);
      PUSH_TIME_QUEUE_MACRO((global_rtc_time + 10), CHECK_MOTE_STATE)
   }
   break;
}

void mote_info_check(void)
// queries the mote to send back information including mac address (serial no),
//  hardware model/rev, API version, etc.
{
   uint8_t result_byte;
   
   // if the mote is not enabled/ready exit!
   if (global_dust_enabled == FALSE) return;
      
   // set the response flag
   global_set_get_response = FALSE;
   // generate the message and send the message
   
   pkgen_get_moteinfo();
   result_byte = dust_hdlc_pksend();
   // set message timeout
   setup_T0_int(T0_250MS);
   while (!TMR0IF && !global_set_get_response && !result_byte);
   // if the message fails, send an error message (kinda pointless though)
   if (TMR0IF || result_byte)
   {
      global_error_message_bitfield |= ERR_MOTE_UNRESPONSIVE;
      PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_ERROR_MSG);
   }
   
}

void mote_temp_check(void)
// queries the mote to send the temperature back and stores it in the global
//    variable global_mote_temperature 
{
   uint8_t result_byte;
   
   // if the mote is not enabled/ready exit!
   if (global_dust_enabled == FALSE) return;
   
   // set the response flag
   global_set_get_response = FALSE;
   // generate the message and send the message
   pkgen_get_charge();
   result_byte = dust_hdlc_pksend();
   // set message timeout
   setup_T0_int(T0_250MS);
   while (!TMR0IF && !global_set_get_response && !result_byte);
   // if the message fails, send an error message (kinda pointless though)
   if (TMR0IF || result_byte)
   {
      global_error_message_bitfield |= ERR_MOTE_UNRESPONSIVE;
      PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_ERROR_MSG);
   }
   
   // if mote temp is outside of operating range, send an error message and queue up a shutdown
   if ((global_mote_temperature < MOTE_MIN_TEMP_SHUTDOWN) || (global_mote_temperature > MOTE_MAX_TEMP_SHUTDOWN))
   {
      // set message error bitfield and queue error message
      global_error_message_bitfield |= (ERR_MSG_MOTE_TEMP_OUT_OF_RANGE | ERR_MSG_MOTE_GOING_TO_DEEP_SLEEP);
      PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_ERROR_MSG);
      
      // set the shutdown cause and schedule a shutdown 10 seconds later
      PUSH_PRIORITY_QUEUE(OPEN_VALVE_UNKNOWN_STATE);  
      global_shutdown_cause = ERR_MOTE_OUT_OF_TEMP_RANGE;
      PUSH_TIME_QUEUE(global_rtc_time + 10, SHUTDOWN_SYSTEM);
   }
      // if mote temp is ALMOST outside of operating range, send an error message
   else if ((global_mote_temperature < MOTE_MIN_TEMP_WARNING) || (global_mote_temperature > MOTE_MAX_TEMP_WARNING))
   {
      // set message error bitfield and queue error message
      global_error_message_bitfield |= (ERR_MSG_MOTE_TEMP_OUT_OF_RANGE);
      PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_ERROR_MSG);
   }
      
}

void mote_time_update(void)
// queries the mote to for the utc time and update the micro's utc time
{
   uint8_t result_byte;
   
   // if the mote is not enabled/ready exit!
   if (global_dust_enabled == FALSE) return;
   
   // set the response flag
   global_set_get_response = FALSE;
   // generate the message and send the message
   pkgen_get_time();
   result_byte = dust_hdlc_pksend();
   // set message timeout
   setup_T0_int(T0_250MS);
   while (!TMR0IF && !global_set_get_response && !result_byte);
   // if the message fails, send an error message (kinda pointless though)
   if (TMR0IF || result_byte)
   {
      global_error_message_bitfield |= ERR_MOTE_UNRESPONSIVE;
      PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_ERROR_MSG);
   }     
}

void get_mote_net_info(void)
// queries the mote to get the macaddress
{
   uint8_t result_byte;
   
   // if the mote is not enabled/ready exit!
   if (global_dust_enabled == FALSE) return;
   
   // set the response flag
   global_set_get_response = FALSE;
   // generate the message and send the message
   pkgen_get_networkinfo();
   result_byte = dust_hdlc_pksend();
   // set message timeout
   setup_T0_int(T0_250MS);
   while (!TMR0IF && !global_set_get_response && !result_byte);
   // if the message fails, send an error message (kinda pointless though)
   if (TMR0IF || result_byte)
   {
      global_error_message_bitfield |= ERR_MOTE_UNRESPONSIVE;
      PUSH_MESSAGE_QUEUE_MACRO(MSG_MOTE_ERROR_MSG);
   }     
}

void init_msgSEQ(void)
{
   // uses the battery voltage and vgen to seed the random number generator
   srand((get_vbatt(0)<<16) || get_vgen(0));
   mote_msgSEQ = (uint16_t)rand();
}

void deal_with_mote_ISR(void)
{
   uint8_t     i;
   
   uint16_t    tmp2u16;
   
   uint8_t     cmd_rcvd = 0;
   int1        send_ack = FALSE;
   
   uint8_t     response_type = 0;
   uint8_t     len_data_rcvd = 0;
   uint32_t    utc_sec_rcvd = 0;
   uint32_t    utc_usec_rcvd = 0;
   uint32_t    new_events_rcvd = 0;
   
   // bytes grabbed by get_dustmsg and sent to pkdecode
   uint8_t      num_of_bytes_rcvd_from_mote = 0;

   // Actually grab the message from the mote
   // gets bytes rcvd(global), dust_recive_buffer, some errors
   num_of_bytes_rcvd_from_mote = get_dustmsg_ISR();
   
   // Add error handling from get_dustmsg_ISR
   // Send NACK?
  
   //decodes the message and returns how many bytes were recived
   dust_hdlc_pkdecode_ISR(num_of_bytes_rcvd_from_mote);
   
   // Add error handling from pkdecode
   // Send NACK?
   
   // get the command type
   cmd_rcvd = bytes_from_mote.dust_rcvd[0];        

   // set these for CMD_TIME, _SERVICE, _EVENT, and _DATA
   //   prefix_x[0] = cmd_rcvd;
   //   prefix_x[1] = 0x00;
   flags_rcvd = bytes_from_mote.dust_rcvd[2];

   response_type = RC_OK;
   
   // 
   if (cmd_rcvd == CMD_SET || cmd_rcvd == CMD_GET || \
       cmd_rcvd == CMD_SETNV || cmd_rcvd == CMD_GETNV || \
       cmd_rcvd == CMD_SEND || cmd_rcvd == CMD_JOIN || \
       cmd_rcvd == CMD_DISCONNECT || cmd_rcvd == CMD_RESET || \
       cmd_rcvd == CMD_SLEEP || cmd_rcvd == CMD_TESTRADIOTX || \
       cmd_rcvd == CMD_TESTRADIORX || cmd_rcvd == CMD_CLEARNV || \
       cmd_rcvd == CMD_SEARCH)
   {
       // we are hoping that result_rcvd = RC_OK)
       result_rcvd = bytes_from_mote.dust_rcvd[3];
   }   

   // Determine parameter type and if the response was ok for set and get commands
   if (cmd_rcvd == CMD_SET || cmd_rcvd == CMD_GET || \
       cmd_rcvd == CMD_SETNV || cmd_rcvd == CMD_GETNV)
   {     
       param_type_rcvd = bytes_from_mote.dust_rcvd[4];
       // this eliminates a bunch of flags for one response "ok" flag
       if (result_rcvd == RC_OK) global_set_get_response = TRUE;
   }
   
   // Main switch for all types of mote messages
   switch (cmd_rcvd)
   {  
      // receipt of response after a SET command
      case  CMD_SET:
         // byte4 is the param_type for all SET responses
         switch (param_type_rcvd)
         {  
            case  PARAM_JOINDUTY:
               break;
            case  PARAM_SERVICE:
               //    save info, no ack
               //    rem_svc_entries_rcvd = \
               //    bytes_from_mote.set_service_response.rem_svc_entries;
               break;
            default:          
         }
         
      // receipt of response after a GET command
      case  CMD_GET:
         // byte4 is the param_type for all GET responses
         switch (param_type_rcvd)
         {  
            case  PARAM_SERVICE:
               svc_index_rcvd = bytes_from_mote.get_service_response.svc_index;
               /*
               svc_state_rcvd = bytes_from_mote.get_service_response.svc_state;
               svc_flags_rcvd = bytes_from_mote.get_service_response.svc_flags;
               app_domain_rcvd = \
                  bytes_from_mote.get_service_response.app_domain;
               */
               //destaddr_rcvd = bytes_from_mote.get_service_response.destaddr;
               //time_rcvd = bytes_from_mote.get_service_response.time;
               break;

            case  PARAM_MOTEINFO:
               for (i=0; i<8; i++)
                  serial_number_rcvd[i] = \
                     bytes_from_mote.get_moteinfo_response.serial_number[i];
                  
               /*
               api_version_rcvd = \
                  bytes_from_mote.get_moteinfo_response.api_version;
               hw_model_rcvd = bytes_from_mote.get_moteinfo_response.hw_model;
               hw_version_rcvd = \
                  bytes_from_mote.get_moteinfo_response.hw_version;
               sw_major_ver_rcvd = \
                  bytes_from_mote.get_moteinfo_response.sw_major_ver;
               sw_minor_ver_rcvd = \
                  bytes_from_mote.get_moteinfo_response.sw_minor_ver;
               sw_patch_rcvd = bytes_from_mote.get_moteinfo_response.sw_patch;
               sw_build_rcvd = bytes_from_mote.get_moteinfo_response.sw_build;
               */
               break;

            case  PARAM_NETWORKINFO:
               for (i=0; i<8; i++)
                  macaddr_rcvd[i] = \
                     bytes_from_mote.get_networkinfo_response.macaddr[i];
               mote_id_rcvd = bytes_from_mote.get_networkinfo_response.mote_id;
               tmp2u16 = bytes_from_mote.get_networkinfo_response.network_id;
               network_id_rcvd = make16(make8(tmp2u16,0),make8(tmp2u16,1));

               break;

            case  PARAM_MOTESTATUS:
               global_mote_state = bytes_from_mote.get_motestatus_response.mote_state;
               
               // this is where I decide if the mote is alive or not
               global_dust_enabled = TRUE;
               
               // If the mote isn't fully connected (operational) set the global
               //    dust operational bit to false
               if (global_mote_state != MOTESTATE_OPERATIONAL)
               {
                  global_dust_operational = FALSE;
               }
               // If the mote is fully connected...
               else 
               {
                  // and it wasn't before...
                  if (global_dust_operational == FALSE)
                  {
 
                     // set it to be connected and send a join message!
                     global_dust_operational = TRUE;
                     PUSH_MESSAGE_QUEUE_ISR_MACRO(MSG_MOTE_JOIN_MSG);
                     PUSH_PRIORITY_QUEUE_ISR_MACRO(UPDATE_MOTE_TIME);
                     PUSH_PRIORITY_QUEUE_ISR_MACRO(CHECK_MOTE_TEMP);
                  }
               }   
               //mote_alarms_rcvd = \
               //   bytes_from_mote.get_motestatus_response.mote_alarms;
/*
               change_counter_rcvd = \
                  bytes_from_mote.get_motestatus_response.change_counter;
               number_of_parents_rcvd = \
                  bytes_from_mote.get_motestatus_response.number_of_parents;
*/
               break;

            case  PARAM_TIME:
               utc_sec_rcvd = bytes_from_mote.get_time_response.utc_sec;
               // JG: save first utc time reported from mote since boot
               if (!global_utc_time)
               {
                  global_first_utc_time = make32(make8(utc_sec_rcvd,0), \
                   make8(utc_sec_rcvd,1), make8(utc_sec_rcvd,2), \
                   make8(utc_sec_rcvd,3));     //  **fix byte order**
               }
               
               global_utc_time = make32(make8(utc_sec_rcvd,0), \
                make8(utc_sec_rcvd,1), make8(utc_sec_rcvd,2), \
                make8(utc_sec_rcvd,3));     //  **fix byte order**
                             
               utc_usec_rcvd = bytes_from_mote.get_time_response.utc_usec;
               if (mote_msgSEQ == 0)
               {
                  mote_msgSEQ = make16(make8(utc_usec_rcvd,2), \                  
                     make8(utc_usec_rcvd,3));
               }
               // This value is used to establish a "random" value for 
               //  the mote message Sequence ID
/*               
               for (i=0; i<5; i++)
                  asn_rcvd[i] = bytes_from_mote.get_time_response.asn[i];
               asn_offset_rcvd = bytes_from_mote.get_time_response.asn_offset;
*/               
               break;

            case  PARAM_CHARGE:
               //charge_mC_rcvd = bytes_from_mote.get_charge_response.charge_mC;
               //uptime_sec_rcvd = bytes_from_mote.get_charge_response.uptime_sec;
               global_mote_temperature = \
                  bytes_from_mote.get_charge_response.temp_celsius;
               //temp_frac_celsius_rcvd = \
               //   bytes_from_mote.get_charge_response.temp_frac_celsius;
               break;
/*
            case  PARAM_RADIOSTATSRX:
               num_of_received_packets_rcvd = \
                  bytes_from_mote.get_testrx_response.num_of_received_packets;
               num_of_recep_failures = \
                  bytes_from_mote.get_testrx_response.num_of_recep_failures;
               break;
*/
            default:
         }

      case  CMD_SETNV:
      // receipt of response after a SETNV command
          switch (param_type_rcvd)
         {  // byte4 is the param_type for all SETNV responses
            /*
            case PARAM_MACADDR: case  PARAM_TXPOWER: \
            case  PARAM_POWERINFO: case  PARAM_OTAPLOCK: \
            case PARAM_SPECIAL1: case PARAM_SPECIAL2:
               break;
            */
            case PARAM_JOINKEY: 
               break;
            case  PARAM_NETWORKID: 
               break;
            default:
         }

      case  CMD_GETNV:
      // receipt of response after a GETNV command
         switch (param_type_rcvd)
         {  // byte4 is the param_type for all GETNV responses
            case  PARAM_MACADDR:
               for (i=0; i<8; i++)
                  macaddr_rcvd[i] = \
                     bytes_from_mote.getNV_macaddr_response.macaddr[i];
               break;

            case  PARAM_NETWORKID:
               tmp2u16 = bytes_from_mote.getNV_networkid_response.network_id;
               network_id_rcvd = make16(make8(tmp2u16,0),make8(tmp2u16,1));
               break;

            default:
          }

      case  CMD_SEND:         case  CMD_DISCONNECT: \
      case  CMD_RESET:        case  CMD_SLEEP:     case  CMD_TESTRADIOTX: \
      case  CMD_TESTRADIORX:  case  CMD_CLEARNV:
         // info saved above; receipt of response after any of the
         // the above; no ack
         break;
      case CMD_SEARCH:
         if (result_rcvd == RC_OK)
         {
            global_mote_state= MOTESTATE_SEARCHING;
         }
         break;
      case CMD_JOIN:
         if (result_rcvd == RC_OK)
            global_mote_state = MOTESTATE_NEGOTIATING;
            joincmd_done_fl = TRUE;
         break;

      case  CMD_TIME:
      // receipt of response after a _TIME hardware interrupt
         utc_sec_rcvd = bytes_from_mote.time_indication.utc_sec;
         
         // JG: save first utc time reported from mote since boot
         if (!global_utc_time)
         {
            global_first_utc_time = make32(make8(utc_sec_rcvd,0), \
             make8(utc_sec_rcvd,1), make8(utc_sec_rcvd,2), \
             make8(utc_sec_rcvd,3));     //  **fix byte order**
         }

         global_utc_time = make32(make8(utc_sec_rcvd,0), \
          make8(utc_sec_rcvd,1), make8(utc_sec_rcvd,2), \
          make8(utc_sec_rcvd,3));     //  **fix byte order**
         utc_usec_rcvd = bytes_from_mote.time_indication.utc_usec;
         if (!mote_msgSEQ)
         {
            mote_msgSEQ = make16(make8(utc_usec_rcvd,2), \                  
               make8(utc_usec_rcvd,3));
         }

         /*
         for (i=0; i<5; i++)
            asn_rcvd[i] = bytes_from_mote.time_indication.asn[i];
         asn_offset_rcvd = bytes_from_mote.time_indication.asn_offset;
         */

//         dust_hdlc_pkgen(prefix_x, 2, (uint8_t*)&r_x, 2);   // type A
         send_ack = TRUE;
         break;

      case  CMD_SERVICE:
      // receipt of a service indication from from manager
         //event_code_rcvd = bytes_from_mote.service_indication.event_code;
         svc_index_rcvd = bytes_from_mote.service_indication.svc_index;
         /*
         mgr_code_rcvd = bytes_from_mote.service_indication.mgr_code;
         svc_state_rcvd = bytes_from_mote.service_indication.svc_state;
         svc_flags_rcvd = bytes_from_mote.service_indication.svc_flags;
         app_domain_rcvd = bytes_from_mote.service_indication.app_domain;
         */
         //destaddr_rcvd = bytes_from_mote.service_indication.destaddr;
         //time_rcvd = bytes_from_mote.service_indication.time;

 //        dust_hdlc_pkgen(prefix_x, 2, (uint8_t*)&r_x, 2);   // type A

         send_ack = TRUE;
         break;

      case  CMD_EVENT:
      // receipt of an event indication from the mote
         // this is where I decide if the mote is alive or not
         global_dust_enabled = TRUE;
         
         new_events_rcvd = bytes_from_mote.events_indication.new_events;
         global_mote_state = bytes_from_mote.events_indication.mote_state;
         
         // If the mote isn't fully connected (operational) set the global
         //    dust operational bit to false
         if (global_mote_state != MOTESTATE_OPERATIONAL)
         {
            global_dust_operational = FALSE;
         }
         // If the mote is fully connected...
         else 
         {                               
            // and it wasn't before...
            if (global_dust_operational == FALSE)
            {
               // set it to be connected and send a join message!
               global_dust_operational = TRUE;
               PUSH_MESSAGE_QUEUE_ISR_MACRO(MSG_MOTE_JOIN_MSG);
                                            
               PUSH_PRIORITY_QUEUE_ISR_MACRO(UPDATE_MOTE_TIME);
               PUSH_PRIORITY_QUEUE_ISR_MACRO(CHECK_MOTE_TEMP);   
            }
         }
         //mote_alarms_rcvd = bytes_from_mote.events_indication.mote_alarms;

         event_flags |= make8(new_events_rcvd,3);  // only last byte has
         // defined events for Eterna
   
         send_ack = TRUE;          
         break;

      case  CMD_ADVERT:
      // receipt of an event indication from the mote
         tmp2u16 = bytes_from_mote.advert_indication.network_id;
         network_id_rcvd = make16(make8(tmp2u16,0),make8(tmp2u16,1));
         mote_id_rcvd = bytes_from_mote.advert_indication.mote_id;
         
         rssi_rcvd = bytes_from_mote.advert_indication.rssi;
         hop_depth_rcvd = bytes_from_mote.advert_indication.hop_depth;
         
         advert_rcvd_fl = TRUE;
   //      dust_hdlc_pkgen(prefix_x, 2, (uint8_t*)&r_x, 2);   // type A

         send_ack = TRUE;
         break;

      case  CMD_DATA:
      // receipt of a data packet payload from the manager
         //sourceaddr_rcvd = bytes_from_mote.data_from_net.sourceaddr;
         //sequence_num_rcvd = bytes_from_mote.data_from_net.sequence_num;
         len_data_rcvd = bytes_from_mote.data_from_net.len_data;

         if (len_data_rcvd <= MAX_USER_DATA_SIZE)
         {
            for (i=0; i<len_data_rcvd; i++)
              payload_buff[i] = bytes_from_mote.data_from_net.data[i];
         }
         else
         {    
            response_type = RC_INVALID_LEN;
         }
         
         send_ack = TRUE;

         PUSH_PRIORITY_QUEUE_ISR_MACRO(DEAL_WITH_NEW_PACKET)         
         break;

      default:
   }

   // Send acks for appropriate messages
    if (send_ack)
   {
      // generates the dust message
      uint8_t pkgen_buf[2], cmd_rcvd_buf[2];
      pkgen_buf[0] = (flags_rcvd+1);
      pkgen_buf[1] = response_type;
      cmd_rcvd_buf[0] = cmd_rcvd;
      cmd_rcvd_buf[1] = 0;
      
      dust_hdlc_pkgen_ISR(cmd_rcvd_buf, 2, pkgen_buf , 2);
      
      // try 3 times to send the ack
      for (i = 0; i < 3; i++)
      {
         // successful ack send
         if (dust_hdlc_pksend_ISR() == NO_ERR) 
         {
            break;
         }
         // wait 50ms and try again
         else
         {
            setup_T4_int(T4_50MS);
            while(!TMR4IF);    
         }
      
      // Add error handling if no ack (not sure how)
      }
   }
}


uint8_t get_dustmsg_ISR(void)
{
// After being prompted by a hardware interrupts on the UART lines
// this routine is called to actually collect the serial data and 
// store it in dust_recive_buffer
//
// Returns the number of chars collected (1 char = 1 not 0) or some error code
// Error Codes:
// -ERR_GET_DUSTMSG_MESSAGE_LENGTH 
// -ERR_GET_DUSTMSG_FRAMING_ERROR

//  When the hardware UART is specified by #use rs232():
//  RS232_ERRORS is used only by the getc() function.
//  RS232_ERRORS is a copy of the RCSTA register except
//  that Bit 0 is used to indicate a parity error.
//  RS232 Errors are not yet implemented

   uint8_t     bytes_rcvd = 0;
   
   // Start the UART and pause for initialization
   setup_uart(TRUE);          
   setup_T4_int(T4_500US);
   while(!TMR4IF);       
   
   // recieve up to MAX_RCV_BUFFER 
   for (bytes_rcvd = 0; bytes_rcvd < MAX_RCV_BUFFER; bytes_rcvd++)
   {
      // clear to send a byte
      output_low(MOTE_TX_CTSn); 
      
      // setup individual byte timeout and wait for next char
      setup_T4_int(T0_10MS);   
      while (!kbhit() && !TMR4IF); 
      
      // not clear to send the next byte until we process this one
      output_high(MOTE_TX_CTSn);
      
      // return error on byte timeout
      if (TMR4IF) 
      {
         setup_uart(FALSE); 
         return ERR_DUSTMSG_TIMEOUT;
      }
      
      // grab the byte from the receive register
      dust_recive_buffer[bytes_rcvd] = RCREG1;        
      
      // done with getting message, break out of loop
      if ((dust_recive_buffer[bytes_rcvd] == FLAG_SEQUENCE) && (bytes_rcvd != 0))
      {
         break;         
      } 
   }
    
   // after the message is done, stop the uart
   setup_uart(FALSE); 
   
   // Check for errors
   // at this point only possible errors are framing and message length
   // Too long of a message (no ending flag sequence)
   if ((bytes_rcvd == MAX_RCV_BUFFER) && (dust_recive_buffer[bytes_rcvd] != FLAG_SEQUENCE))
   {
      return ERR_DUSTMSG_MESSAGE_LENGTH;
   }
   // Too small of a message, return error code
   else if (bytes_rcvd < 5)
   {
      return ERR_DUSTMSG_MESSAGE_LENGTH;
   }
   // Framing error, return error code
   else if ((dust_recive_buffer[0] != FLAG_SEQUENCE) || (dust_recive_buffer[bytes_rcvd] != FLAG_SEQUENCE))
   {
      return ERR_DUSTMSG_FRAMING_ERROR;
   }
   // all is well, exit normally
   else
   {          
      // return how many bytes recieved in actual (starting at 1) number      
      return (bytes_rcvd + 1);
   }
}

void dust_hdlc_pkgen(uint8_t* prefix, uint8_t size, \
                       uint8_t* buf, uint8_t size_buf)
{
// dust_hdlc_pkgen()
//
// Generate an HDLC packet ready for a mote.
//
// 1. Calc CRC-16
// 2. Add start delimiters (flag sequence)
// 3. Byte stuff 0x7e & 0x7d
// 4. Add end delimiters (flag sequence)
//
    
   uint16_t    fcs16       = 0;
   uint8_t     data8       = 0;
   uint8_t     i           = 0;
   uint8_t     j           = 0;

// Step 1
   fcs16 = FCS_INITIAL_FCS16;
   
   // FCS the prefix bytes
   for (i = 0; i < size; i++)
   {
      fcs16 = fcs_fcs16(fcs16, prefix[i]);
   }
   
   // FCS the data packet bytes
   for (i = 0; i < size_buf; i++)
   {
      fcs16 = fcs_fcs16(fcs16, buf[i]);
   }

   // add 1's complement
   fcs16 = ~(fcs16); 

// Step 2
   //Set initial delimiter
   j = 0;
   dust_send_buffer[j++]  = FLAG_SEQUENCE;

// Step 3
   // Stuff prefix buffer
   for (i = 0; i < size; i++)
   {
      STUFF_BYTE(prefix[i], dust_send_buffer, j);
   }

   // Stuff data buffer
   for (i = 0; i < size_buf; i++)
   {
      STUFF_BYTE(buf[i], dust_send_buffer, j);
   }

   // Add least significiant byte 1st (RFC 1622). Do byte stuffing.
   data8 = (uint8_t)(fcs16 & 0xff);
   STUFF_BYTE(data8, dust_send_buffer, j);

   // Add most significiant byte 2nd (RFC 1622). Do byte stuffing.
   data8 = (uint8_t)((fcs16>>8) & 0xff);
   STUFF_BYTE(data8, dust_send_buffer, j);

// Step 4
   // CHECK_BUF_SIZE(j+1);
   dust_send_buffer[j++]  = FLAG_SEQUENCE;

   // Set the number of bytes actually written
   num_of_bytes_to_send = j;
   
   return;
}

void dust_hdlc_pkgen_ISR(uint8_t* prefix, uint8_t size, \
                       uint8_t* buf, uint8_t size_buf)
{
// Generate an HDLC packet ready for a mote.
//
// 1. Calc CRC-16
// 2. Add start delimiters (flag sequence)
// 3. Byte stuff 0x7e & 0x7d
// 4. Add end delimiters (flag sequence)
//
    
   uint16_t    fcs16       = 0;
   uint8_t     data8       = 0;
   uint8_t     i           = 0;
   uint8_t     j           = 0;

// Step 1
   fcs16 = FCS_INITIAL_FCS16;
   
   // FCS the prefix bytes
   for (i = 0; i < size; i++)
   {
      fcs16 = fcs_fcs16(fcs16, prefix[i]);
   }
   
   // FCS the data packet bytes
   for (i = 0; i < size_buf; i++)
   {
      fcs16 = fcs_fcs16(fcs16, buf[i]);
   }

   // add 1's complement
   fcs16 = ~(fcs16); 

// Step 2
   //Set initial delimiter
   j = 0;
   dust_send_buffer_isr[j++]  = FLAG_SEQUENCE;

// Step 3
   // Stuff prefix buffer
   for (i = 0; i < size; i++)
   {
      STUFF_BYTE(prefix[i], dust_send_buffer_isr, j);
   }

   // Stuff data buffer
   for (i = 0; i < size_buf; i++)
   {
      STUFF_BYTE(buf[i], dust_send_buffer_isr, j);
   }

   // Add least significiant byte 1st (RFC 1622). Do byte stuffing.
   data8 = (uint8_t)(fcs16 & 0xff);
   STUFF_BYTE(data8, dust_send_buffer_isr, j);

   // Add most significiant byte 2nd (RFC 1622). Do byte stuffing.
   data8 = (uint8_t)((fcs16>>8) & 0xff);
   STUFF_BYTE(data8, dust_send_buffer_isr, j);

// Step 4
   // CHECK_BUF_SIZE(j+1);
   dust_send_buffer_isr[j++]  = FLAG_SEQUENCE;

   // Set the number of bytes actually written
   num_of_bytes_to_send_isr = j;
   
   return;
}

uint8_t dust_hdlc_pksend()
{
// This routine actually sends out the packet from the micro to the mote

   uint8_t     n;
   uint8_t     tmp_oscccon, tmp_t2con, tmp_pr2, tmp_t0con;
   
   // save the crystal status for later restore
   tmp_oscccon = OSCCON;
   tmp_t2con = T2CON;
   tmp_pr2 = PR2;
   tmp_t0con = T0CON;
   
   // turn on high-speed crystal for UART communication
   fosc_pri();

   // prepare mote to recieve data
   output_low(MOTE_RX_RTSn);
  
   // turn on uart and wait for it to initalize
   setup_uart(TRUE);                     
   setup_T0_int(T0_500US);
   while(!TMR0IF);   

   // loop for sending out bytes
   for (n = 0; n < num_of_bytes_to_send; n++)
   {
      // setup send timeout for error handling
      setup_T0_int(T0_10MS);

      // wait until a transmit is done or timeout error
      while (!TRMT1 && !TMR0IF);
      
      // if the timeout occured before the tranmit finished
      if (TMR0IF)
      {
         // tell mote that it is no longer going to recieve data
         output_high(MOTE_RX_RTSn);
         // turn off uart
         setup_uart(FALSE);
         // return error code
         return ERR_DUSTMSG_TIMEOUT;
      }
      // if transmit buffer is open
      else
      {
         // send byte to mote
         putc(dust_send_buffer[n]);
      }
   }
   
   // wait until the last transmit is done
   while (!TRMT1);

   // tell mote that it no longer is going to recieve data
   output_high(MOTE_RX_RTSn);
   
   // turn off the uart
   setup_uart(FALSE);
   
   //fosc_250k();
  
   // restore the crystal status
   OSCCON = tmp_oscccon;
   T2CON = tmp_t2con;
   T0CON = tmp_t0con;
   PR2 = tmp_PR2;
   
   // return no error
   return NO_ERR;
}

uint8_t dust_hdlc_pksend_ISR()
{
// We're now running at 3.6864MHz at 115.2K baud
// This routine actually sends out the packet from the micro to the mote
   
   uint8_t     n;
   uint8_t     tmp_oscccon, tmp_t2con, tmp_pr2, tmp_t0con;
   
   // save the crystal status for later restore
   tmp_oscccon = OSCCON;
   tmp_t2con = T2CON;
   tmp_pr2 = PR2;
   tmp_t0con = T0CON;
   
   // turn on high-speed crystal for UART communication
   fosc_pri();

   // prepare mote to recieve data
   output_low(MOTE_RX_RTSn);
  
   // turn on uart and wait for it to initalize
   setup_uart(TRUE);                     
   setup_T4_int(T4_500US);
   while(!TMR4IF);   

   for (n = 0; n < num_of_bytes_to_send_isr; n++)
   {
      // setup send timeout for error handling
      setup_T4_int(T4_10MS);

      // wait until a transmit is done or timeout error
      while (!TRMT1 && !TMR4IF);
      
      // if the timeout occured before the tranmit finished
      if (TMR4IF)
      {
         // tell mote that it is no longer going to recieve data
         output_high(MOTE_RX_RTSn);
         // turn off uart
         setup_uart(FALSE);
         // return error code
         return ERR_DUSTMSG_TIMEOUT;
      }
      // if transmit buffer is open
      else
      {
         // send byte to mote
         putc(dust_send_buffer_isr[n]);
      }
   }
   
   // wait until the last transmit is done
   while (!TRMT1);

   // tell mote that it no longer is going to recieve data
   output_high(MOTE_RX_RTSn);
   
   // turn off the uart
   setup_uart(FALSE);
   
   // restore the crystal status
   OSCCON = tmp_oscccon;
   T2CON = tmp_t2con;
   T0CON = tmp_t0con;
   PR2 = tmp_PR2;
   
   // return no error
   return NO_ERR;
}

uint8_t dust_hdlc_pkdecode_ISR(uint8_t bytes_to_decode)
{
/*
 * Decode an HDLC packet from Mote.
 *
 * NOTE: The destination buffer must contain an extra
 *       2 bytes to hold CRC data.  This requirement
 *       simplifies the code and improves performance.
 *
 * 1. Remove delimiters
 * 2. Unstuff *payload* and CRC bytes
 * 3. Assemble CRC bytes
 * 4. Check CRC
 * 
 * Returns error messages or the number of byte decoded
*/

//  Note changes to routine as supplied by Dust:
//  The buffer and buffer size for the storage of received packet
//   are hardwired to be dust_recive_buffer[128] for the raw data
//   and bytes_from_mote.dust_rcvd for the unstuffed and stripped data

   uint16_t    crc16       = 0;
   uint8_t*    src_ptr;
   uint8_t*    src_ptr_end;
   uint8_t*    dest_ptr;

   uint8_t      dust_bytes_rcvd = 0;

// 1 Check for start/end delimiters
    if ((FLAG_SEQUENCE != dust_recive_buffer[0]) ||
        (FLAG_SEQUENCE != dust_recive_buffer[bytes_to_decode-1])) 
    {
        // return framing error
        return ERR_DUSTMSG_FRAMING_ERROR;
    }

// 2 Unstuff *payload* and CRC data into destination buffer
    dest_ptr    = bytes_from_mote.dust_rcvd;
    src_ptr_end = &dust_recive_buffer[bytes_to_decode-1];
    for (src_ptr = &dust_recive_buffer[1]; src_ptr < src_ptr_end; src_ptr++)
    {
        if (*src_ptr == CONTROL_ESCAPE) {
           src_ptr++; /* skip to next byte, which contains the actual data */
            *dest_ptr++ = *src_ptr ^ XOR_BYTE;
        }
        else
        {
            *dest_ptr++ = *src_ptr;
        }
    }

// 3 Assemble CRC
    dest_ptr--;                 /* now points to MSBs */
    crc16 = *dest_ptr-- << 8;
    crc16 += *dest_ptr;         /* add LSBs */

    /* Fill in the number of bytes actually written */
   dust_bytes_rcvd = (uint8_t)(dest_ptr - bytes_from_mote.dust_rcvd);

// 4 Return error if CRCs don't match      
    if (fcs_calc_ISR(bytes_from_mote.dust_rcvd, dust_bytes_rcvd) != crc16 ) 
    {
       // indicate bad CRC
       return ERR_DUSTMSG_CRC;      
    }
   return dust_bytes_rcvd;
}

uint16_t fcs_fcs16(uint16_t fcs, uint8_t data)
{
   return (fcs >> 8) ^ fcstab[(fcs ^ data) & 0xff];
}

uint16_t fcs_fcs16_ISR(uint16_t fcs, uint8_t data)
{
   return (fcs >> 8) ^ fcstab[(fcs ^ data) & 0xff];
}

static uint16_t fcs_calc(uint8_t* p, uint16_t size)
{
/* fcs_calc()
 *
 * Calculate the 2 byte CRC-16 value given a buffer.
 *
 * Returns:
 *      2 byte CRC-16 value
 */
    uint16_t    fcs = 0;
    fcs = FCS_INITIAL_FCS16;
    while (size) {
        fcs = fcs_fcs16(fcs, *p++);
        size--;
    }
    return ~fcs; /* add 1's complement */
}

static uint16_t fcs_calc_ISR(uint8_t* p, uint16_t size)
{
    uint16_t    fcs = 0;
    fcs = FCS_INITIAL_FCS16;
    while (size) {
        fcs = fcs_fcs16_ISR(fcs, *p++);
        size--;
    }
    return ~fcs; /* add 1's complement */
}

uint8_t deal_with_packet(void)
{
   uint8_t  command_id_received;
   uint16_t mgr_msgSEQ;
   uint32_t temp_uint32;
   // clear out for use as NACK bitfield each time
   temp_uint32 = 0;
   static struct CommandQueueItem command_queue_item;
   static struct SprinklerZoneSettingsItem sprinkler_zone_settings_item;
   &command_queue_item.sprinkler_settings = &sprinkler_zone_settings_item;
   
   // This Dust-recommended prepend for messages (both from manager to mote and
   //    from mote to manager) ensures that we do not conflict with any future
   //    Hart-related aspects. 
   
   // This prepend is in the same spot for messages going either direction
   // Return with an error if any char is incorrect
   if ((payload_buff[0] != 0x00) || (payload_buff[1] != 0x00) ||              \
      (payload_buff[2] != 0xFC) || (payload_buff[3] != 0x12))
   {   
      return ERR_BAD_PAYLOAD;
   }
   
   // The mgr_msgSEQ, a sequence number used by the manager software to identify
   //    messages which are ACK'd or NACK'd always follows the prepend.
   mgr_msgSEQ = make16(payload_buff[5],payload_buff[4]);
   
   // Then comes the msg_rcvd identifier which is the packet's intent,
   //    such as 'give me a full report,' or 'do a warm start.'
   // We leave payload_buff[6] alone so the command ID is identical for the 
   //    response message
   command_id_received = payload_buff[6];
 
   // Switch for sorting through different messages recieved
   switch (command_id_received)
   {
   // Manager ACKs or NACKs unsolicited messages from micro to manager such as:
   //    a. Message at startup
   //    b. ERROR messages
   //    c. Valve calibrate response
   //    d. Bulk data dump
   
//////// Manager ACKs/NACKs////////
      // Manager ACKs
      case MSG_MGR_ACK:
         // clear off unsolicited message of proper msgSEQ
         REMOVE_MESSAGE_QUEUE_MACRO(mgr_msgSEQ);
         break;
                                                       
      // Manager NACKs   
      case MSG_MGR_NACK:
         // Do nothing. Messages will get resent as if it never got acked by
         //    manager
         break;   
      
//////// Report Requests////////
      // Manager requests a full report
      case MSG_MGR_FULL_REPORT:
         send_full_report(command_id_received, mgr_msgSEQ);  
         break;
      // Manager requests a health report
      case MSG_MGR_HEALTH_REPORT:
         send_health_report(command_id_received, mgr_msgSEQ);
         break;
      // Manager requests a valve report
      case MSG_MGR_VALVE_REPORT:
         send_valve_report(command_id_received, mgr_msgSEQ);
         break;
      
//////// Restart/Shutdown Commands////////                                           
      // Manager requests a warm restart (micro reboots)
      case MSG_MGR_WARM_RESTART:
         send_ack(command_id_received, mgr_msgSEQ);
         // set the shutdown cause
         global_shutdown_cause = WARM_RESTART_REQUEST;
         // save the shutdown/restart reason in EEPROM
         write_ee1(EE_RESTART_CAUSE, global_shutdown_cause);
         // restart micro                                                        
         PUSH_TIME_QUEUE_MACRO(global_rtc_time + 5, CPU_RESET); 
         break; 
      // Manager requests a cold restart (mote and micro reboot)  
      case MSG_MGR_COLD_RESTART:
         send_ack(command_id_received, mgr_msgSEQ);
         // set the shutdown cause
         global_shutdown_cause = COLD_RESTART_REQUEST;
         // save the shutdown/restart reason in EEPROM
         write_ee1(EE_RESTART_CAUSE, global_shutdown_cause);
         // restart micro
         PUSH_TIME_QUEUE_MACRO(global_rtc_time + 5, CPU_RESET);
         break;
      // Manager requests a rejoin (restart mote and rejoin) 
      case MSG_MGR_FORCE_REJOIN:
         send_ack(command_id_received, mgr_msgSEQ);
         // schedule rejoin/restart mote
         PUSH_TIME_QUEUE_MACRO(global_rtc_time + 5, RESET_MOTE);
         break;
      // Manager requests a shutdown (deep sleep, move valve to specified position
      //    and at a certain time)
      case MSG_MGR_SHUTDOWN:
         send_ack(command_id_received, mgr_msgSEQ);
         // set valve posisition and move valve
         global_valve_position_set_value = make16(payload_buff[12], payload_buff[11]);
         PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);
         // set the shutdown cause
         global_shutdown_cause = MANAGER_SHUTDOWN_REQUEST;
         // figure out the shutdown time in rtc time (given in utc time or 0 for NOW)
         // temp_uint32 is now the utc shutdown time
         temp_uint32 = make32(payload_buff[10], payload_buff[9],payload_buff[8], payload_buff[7]);
         
         // if shutdown isn't now (time = 0), utc_shutdown_time + rtc_time - utc_time = rtc_shutdown_time
         if (temp_uint32 != 0)
         {
            temp_uint32 += global_rtc_time - global_utc_time;
            // in some rare cases, this simple math might give a huge number (more than an hour past current rtc
            //    time.) In that case, time should be now.
            // The rare case would be when there is latency in the network more than the 
            //    elapsed rtc time.
            if (temp_uint32 > (global_rtc_time + 3600))
            { 
               temp_uint32 = 0;
            }
         }
         // shutdown time is now (sent time is 0)
         else
         {
            temp_uint32 = global_rtc_time;
         }                                  
         // schedule the shutdown
         PUSH_TIME_QUEUE_MACRO(temp_uint32, SHUTDOWN_SYSTEM);
         break;

//////// Erraneous Commands////////
      case MSG_MGR_UPDATE_SP_NUM:
         // update sprinkler number
         // check if number is in a valid range, NACK if no good.
         global_sprinkler_num = make16(payload_buff[8], payload_buff[7]);
         // write sprinkler number
         write_ee2(EE_SPRINKLER_NUM, global_sprinkler_num);
         send_update_sp_num_response(command_id_received, mgr_msgSEQ);
         break;
      case MSG_MGR_CAL_VALVE:
         send_ack(command_id_received, mgr_msgSEQ);            
         // schedule calibrate valve
         PUSH_PRIORITY_QUEUE_MACRO(CALIBRATE_VALVE_1);   
         break;
      case MSG_MGR_CAL_FSR:         
         if (read_system_state() != SYSTEM_RUN)   
         {       
            temp_uint32 |= NACK_BITFIELD_INCOMPATIBLE_STATE;
         }                                                    
         if (global_valve_position == VLV_POSITION_UNKNOWN)
         {
            temp_uint32 |= NACK_BITFIELD_VLV_POSITION_UNKNOWN;    
         }
         if (!is_valve_position_valid(make16(payload_buff[8], payload_buff[7])))   
         {                                                                      
            temp_uint32 |= NACK_BITFIELD_INVALID_VLV_SETTING;                    
         }
         // if the NACK bitfield contains anything, send the nack instead of proceeding
         if (temp_uint32 != 0)         
         {
            send_nack(command_id_received, mgr_msgSEQ, temp_uint32);
         }                                            
         else                                             
         {                 
            send_ack(command_id_received, mgr_msgSEQ);
            // schedule calibrate valve                   
            PUSH_PRIORITY_QUEUE_MACRO(CALIBRATE_FSR_1);  
         }
         break;
         
      case MSG_MGR_AQUIRE_GPS: 
         send_ack(command_id_received, mgr_msgSEQ);
         // start gps point aquisition                        
         PUSH_PRIORITY_QUEUE_MACRO(START_GPS_AQUISITION);              
         break;
                                                                         
      case MSG_MGR_UPDATE_EEPROM:                               
         LCD_clear();                                                
         LCD_place_uint16(make16(payload_buff[8], payload_buff[7]),0,0,5);  
         LCD_place_uint8(payload_buff[9],0,7,3);         
         LCD_place_uint32(make32(payload_buff[13],payload_buff[12],payload_buff[11],payload_buff[10]),1,0,8);
         global_skip_lcd_update_count = 3;        
                                                                                                
         // if eeprom location is outside of the valid range                                     
         //    (not in between start and end location (taking into account the number of bytes))   
         if ((make16(payload_buff[8], payload_buff[7]) < EE_START_LOCATION) ||                        \                     
            ((make16(payload_buff[8], payload_buff[7]) + payload_buff[9] - 1) > EE_END_LOCATION) ||   \                                               
            // also check if number of bytes != 1, 2, or 4
            (!((payload_buff[9] == 1) || (payload_buff[9] == 2) || (payload_buff[9] == 4))))       
         {                                                                                        
            send_nack(command_id_received, mgr_msgSEQ, NACK_BITFIELD_INVALID_EEPROM_SETTINGS);    
         }                                                                                           
         // valid location and number of bytes                     
         else                                   
         {                                                                                                
            if (payload_buff[9] == 1)
            {
               write_ee1(make16(payload_buff[8], payload_buff[7]),payload_buff[10]);
            }
            else if (payload_buff[9] == 2)               
            {                                          
               write_ee2(make16(payload_buff[8], payload_buff[7]),make16(payload_buff[11],payload_buff[10]));
            }
            else if (payload_buff[9] == 4)                                           
            {
               write_ee4(make16(payload_buff[8], payload_buff[7]),make32(payload_buff[13],payload_buff[12],payload_buff[11],payload_buff[10]));
            }
            send_ack(command_id_received, mgr_msgSEQ);
         }
         break; 
      
      case MSG_MGR_READ_EEPROM:                                                                             
         // if eeprom location is outside of the valid range                                     
         //    (not in between start and end location (taking into account the number of bytes))   
         if ((make16(payload_buff[8], payload_buff[7]) < EE_START_LOCATION) ||                        \                     
            ((make16(payload_buff[8], payload_buff[7]) + payload_buff[9] - 1) > EE_END_LOCATION) ||   \                                               
            // also check if number of bytes != 1, 2, or 4
            (!((payload_buff[9] == 1) || (payload_buff[9] == 2) || (payload_buff[9] == 4))))       
         {                                                                                        
            send_nack(command_id_received, mgr_msgSEQ, NACK_BITFIELD_INVALID_EEPROM_SETTINGS);    
         }                                                                                           
         // valid location and number of bytes                     
         else                                   
         {                                                                                                
            if (payload_buff[9] == 1)
            {                                
               temp_uint32 = read_ee1(make16(payload_buff[8], payload_buff[7]));
            }
            else if (payload_buff[9] == 2)               
            {                                          
               temp_uint32 = read_ee2(make16(payload_buff[8], payload_buff[7]));       
            }
            else if (payload_buff[9] == 4)                                           
            {
               temp_uint32 = read_ee4(make16(payload_buff[8], payload_buff[7]));
            }
            send_read_eeprom_response(command_id_received, mgr_msgSEQ, make16(payload_buff[8], payload_buff[7]), temp_uint32);  
         }
         break;
      
      case MSG_MGR_READ_XDCR:                                                    
         // if the type1 (fancy expensive all-sensors xdcrs) are connected   
         if (global_xcdr_type1_enabled)                                     
         {                             
            // ack it (yes, because it takes ~50ms for the xdcr to settle, it must be an
            //    unsolicited response that happens later)
            send_ack(command_id_received, mgr_msgSEQ);      
            // schedule calibrate valve                              
            PUSH_PRIORITY_QUEUE_MACRO(TAKE_XCDR_MESUREMENT_1);  
         }                                      
         // if a xdcr isn't connected, then nack it!
         else                               
         {  
            temp_uint32 |= NACK_BITFIELD_INVALID_HARDWARE;
            send_nack(command_id_received, mgr_msgSEQ, temp_uint32);  
         }       
         break;                                         
                                                                    
         
//////// Run/Test/Demo Commands////////        
      case MSG_MGR_RUN_CMD_NO_MOD:
            
         if (global_valve_position == VLV_POSITION_UNKNOWN)
         {
            temp_uint32 |= NACK_BITFIELD_VLV_POSITION_UNKNOWN;
         }
         if (!is_valve_position_valid(make16(payload_buff[16], payload_buff[15])))
         {
            temp_uint32 |= NACK_BITFIELD_INVALID_VLV_SETTING;
         }
         if (!is_rpm_setting_valid(make16(payload_buff[18], payload_buff[17])))
         {
            temp_uint32 |= NACK_BITFIELD_INVALID_RPM_SETTING;
         }
 
         // if the NACK bitfield contains anything, send the nack instead of proceeding
         if (temp_uint32 != 0)
         {
            send_nack(command_id_received, mgr_msgSEQ, temp_uint32); 
         }     
         // add non-modulated run command to command queue 
         else
         {
            push_sprinkler_queue(make32(payload_buff[10],payload_buff[9],payload_buff[8],payload_buff[7]), \
               make32(payload_buff[14],payload_buff[13],payload_buff[12],payload_buff[11]), \
               make16(payload_buff[16], payload_buff[15]), make16(payload_buff[18], payload_buff[17]), \
               0, BRAKE_AND_CHARGE_START);
            
            // have the micro check the battery to determine the proper charging
            PUSH_PRIORITY_QUEUE_MACRO(CHECK_BATTERY_STATE); 
            send_sprinkler_cmd_response(command_id_received, mgr_msgSEQ);
         }
         break;
         
      case MSG_MGR_RUN_CMD_MOD:
         // since this isn't implemented, nack it
         temp_uint32 |= NACK_BITFIELD_INCOMPATIBLE_STATE;  
         send_nack(command_id_received, mgr_msgSEQ, temp_uint32);
                             
         // add modulated run command to command queue
         // send_sprinkler_cmd_response(command_id_received, mgr_msgSEQ);
         break;
                                                  
      case MSG_MGR_TEST_NO_RPM_CTRL:   
            
         // this test added by JG
         if (!is_valve_value_full_open_or_close(make16(payload_buff[8], payload_buff[7])))
         {
            if (global_valve_position == VLV_POSITION_UNKNOWN)  
            {                                                    
               temp_uint32 |= NACK_BITFIELD_VLV_POSITION_UNKNOWN; 
            }                            
         }             
         if (!is_valve_position_valid(make16(payload_buff[8], payload_buff[7])))
         {
            temp_uint32 |= NACK_BITFIELD_INVALID_VLV_SETTING;
         } 
         if (!is_brake_duty_valid(make16(payload_buff[10], payload_buff[9])))
         {
            temp_uint32 |= NACK_BITFIELD_INVALID_BRAKE_SETTING;
         }
         if (!is_charge_duty_valid(make16(payload_buff[12], payload_buff[11])))
         {
            temp_uint32 |= NACK_BITFIELD_INVALID_CHARGE_SETTING;
         }
         // if in mppc non-dynamic mode, check mppc
         if ((payload_buff[14] == FALSE) && (!is_mppc_valid(payload_buff[13])))
         {
            temp_uint32 |= NACK_BITFIELD_INVALID_MPPC_SETTING;
         }
         
         // if the NACK bitfield contains anything, send the nack instead of proceeding
         if (temp_uint32 != 0)
         {
            send_nack(command_id_received, mgr_msgSEQ, temp_uint32);
         }
         // add non-modulated run command to command queue 
         else
         {
                                                
            // deal with JG-added special cases of commands to fully open or
            //  or fully close the valve in cases whete the valve has not
            //  yet been calibrated
            if ((make16(payload_buff[8], payload_buff[7]) == VLV_POSITION_OPENED) \
               && (global_valve_position == VLV_POSITION_UNKNOWN))
            {
               PUSH_PRIORITY_QUEUE_MACRO(OPEN_VALVE_UNKNOWN_STATE);
            }
            
            else if ((make16(payload_buff[8], payload_buff[7]) == VLV_POSITION_CLOSED) \
               && (global_valve_position == VLV_POSITION_UNKNOWN))
            {
               PUSH_PRIORITY_QUEUE_MACRO(CLOSE_VALVE_UNKNOWN_STATE);
            }
             
            else
            {
               PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);
            }                                       
                       
            global_brake_duty_set_value = (make16(payload_buff[10], payload_buff[9]));
            global_charge_duty_set_value = (make16(payload_buff[12], payload_buff[11]));
                               
            // clear queue
            clear_sprinkler_queue(); 
            
            if (payload_buff[14] == FALSE)                                              
            {
               // set test parameters with no rpm control and preset mppc value   
               push_sprinkler_queue(0, 0-1,make16(payload_buff[8], payload_buff[7]), 0, \
               0, NO_RPM_CONTROL);     
               global_mppc_set_value = payload_buff[13];
               global_control_loop_mechanism = NO_RPM_CONTROL;
            }
            else if (payload_buff[14] == TRUE)        
            {
               // set test parameters with no rpm control and dynamic mppc
               push_sprinkler_queue(0, 0-1,make16(payload_buff[8], payload_buff[7]), 0, \
               0, NO_RPM_CONTROL_DYN_MPPC);
               global_control_loop_mechanism = NO_RPM_CONTROL_DYN_MPPC;
            }
            
            global_valve_position_set_value = make16(payload_buff[8], payload_buff[7]);
            global_rpm_set_value = 0;

            send_full_report(command_id_received, mgr_msgSEQ);
         }
         break;
         
      case MSG_MGR_TEST_RPM_CTRL:
      
         // this test added by JG
         if (!is_valve_value_full_open_or_close(make16(payload_buff[8], payload_buff[7])))
         {
            if (global_valve_position == VLV_POSITION_UNKNOWN)
            {
               temp_uint32 |= NACK_BITFIELD_VLV_POSITION_UNKNOWN;
            }
         }
         if (!is_valve_position_valid(make16(payload_buff[8], payload_buff[7])))
         {
            temp_uint32 |= NACK_BITFIELD_INVALID_VLV_SETTING;
         }
         if (!is_rpm_setting_valid(make16(payload_buff[10], payload_buff[9])))
         {                               
            temp_uint32 |= NACK_BITFIELD_INVALID_RPM_SETTING;
         }
         
         // if the NACK bitfield contains anything, send the nack instead of proceeding
         if (temp_uint32 != 0)
         {
            send_nack(command_id_received, mgr_msgSEQ, temp_uint32);
         }
         // add non-modulated run command to command queue 
         else
         { 
            // clear queue 
            clear_sprinkler_queue();
            
            // set test parameters with rpm control
            // JG: Add test of extra byte to address whether or not to
            //  be have charging as part of the control loop
            if (payload_buff[11] == 0xFF)
            {
               push_sprinkler_queue(0, 0-1,make16(payload_buff[8], payload_buff[7]), \
                  make16(payload_buff[10], payload_buff[9]), 0, BRAKE_AND_CHARGE_START);
                  
               global_control_loop_mechanism = BRAKE_AND_CHARGE_START;
            }
            else
            {
               push_sprinkler_queue(0, 0-1,make16(payload_buff[8], payload_buff[7]), \
                  make16(payload_buff[10], payload_buff[9]), 0, BRAKE_ONLY);
               
               global_control_loop_mechanism = BRAKE_ONLY;
            }
               
            // deal with JG-added special cases of commands to fully open or
            //  or fully close the valve in cases where the valve has not
            //  yet been calibrated
            if ((make16(payload_buff[8], payload_buff[7]) == VLV_POSITION_OPENED) \
               && (global_valve_position == VLV_POSITION_UNKNOWN))
            {
               PUSH_PRIORITY_QUEUE_MACRO(OPEN_VALVE_UNKNOWN_STATE);
            }
            
            else if ((make16(payload_buff[8], payload_buff[7]) == VLV_POSITION_CLOSED) \
               && (global_valve_position == VLV_POSITION_UNKNOWN))
            {
               PUSH_PRIORITY_QUEUE_MACRO(CLOSE_VALVE_UNKNOWN_STATE);
            }                                     
            
            else
            {
               PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);  
            }
            
            global_valve_position_set_value = make16(payload_buff[8], payload_buff[7]);
            global_rpm_set_value = make16(payload_buff[10], payload_buff[9]);
            send_full_report(command_id_received, mgr_msgSEQ);   
         }
         break; 
         
      case MSG_MGR_DEMO:
         if (global_valve_position == VLV_POSITION_UNKNOWN)
         {
            temp_uint32 |= NACK_BITFIELD_VLV_POSITION_UNKNOWN;
         }     
         // if the NACK bitfield contains anything, send the nack instead of proceeding
         if (temp_uint32 != 0)
         {
            send_nack(command_id_received, mgr_msgSEQ, temp_uint32);
         }        
         else
         {
            clear_sprinkler_queue();
            push_sprinkler_queue(global_utc_time, global_utc_time + 30, 0x4000, 100, 0, BRAKE_AND_CHARGE_START);
            push_sprinkler_queue(global_utc_time + 30, global_utc_time + 60, 0x4000, 50, 0, BRAKE_AND_CHARGE_START);
            push_sprinkler_queue(global_utc_time + 60, global_utc_time + 90, 0x4000, 10, 0, BRAKE_AND_CHARGE_START);
            push_sprinkler_queue(global_utc_time + 90, 0-1, VLV_POSITION_CLOSED, 0, 0, BRAKE_AND_CHARGE_START);
            send_ack(command_id_received, mgr_msgSEQ);
            // add demo mode stuff to commmand queue
         }                                
      case MSG_MGR_CLEAR_SPINKLER_CMD_QUEUE:
         clear_sprinkler_queue();
         send_full_report(command_id_received, mgr_msgSEQ);
         break; 
      
      // To force the valve to move and change nothing else 
      //    (retrieve current settings for brake, charge, etc.
      //    Will force the valve to go fully open/closed if uncalibrated (position unknown)   
      case MSG_MGR_FORCE_VALVE_MOVEMENT:              
         // if the valve position is unknown and we're trying to send it full open or full closed, 
         //    set the valve position to the precalibration (the middle) and proceed as the valve
         //    will recalibrate its position
         if ((global_valve_position == VLV_POSITION_UNKNOWN) &&                                       \
            (is_valve_value_full_open_or_close(make16(payload_buff[8], payload_buff[7]))))  
         {                           
            global_valve_position = VLV_PRECALIBRATION_POSITION; 
         }      
         
         // if the valve position is unknown and we're trying to send it to a specific position, NACK it   
         else if ((global_valve_position == VLV_POSITION_UNKNOWN) &&                                  \
            (!is_valve_value_full_open_or_close(make16(payload_buff[8], payload_buff[7])))) 
         {
            temp_uint32 |= NACK_BITFIELD_VLV_POSITION_UNKNOWN;
         }          
         
         // if the NACK bitfield contains anything, send the nack instead of proceeding
         if (temp_uint32 != 0)                                     
         {
            send_nack(command_id_received, mgr_msgSEQ, temp_uint32);
         }   
         // otherwise, set the valve position, send a move valve command to the queue, and send a report
         else 
         {    
            global_valve_position_set_value = make16(payload_buff[8], payload_buff[7]);
            PUSH_PRIORITY_QUEUE_MACRO(MOVE_VALVE);  
            send_valve_report(command_id_received, mgr_msgSEQ);
         }
         break;    
   }       
}  // deal_with_packet


void prepare_prefix_and_payload(uint8_t payload_buf_length)
{
      // Setup send_prefix
      send_prefix[0] = CMD_SEND;
      // set the size part of the prefix to payload length + 9 other bytes
      send_prefix[1] = payload_buf_length + 9;
      send_prefix[2] = FLAG_SEND_REQUEST;
      send_prefix[3] = DESTADDR_HI;
      send_prefix[4] = DESTADDR_LO;
      // setup the ????
      send_prefix[5] = svc_index_rcvd;
      send_prefix[6] = APPDOMAIN_MAINTENANCE;
      send_prefix[7] = PRIORITY_HI;
      send_prefix[8] = 0;
      send_prefix[9] = 0;
      send_prefix[10] = 0xFF;
      send_prefix[11] = payload_buf_length;
      payload_buff[0] = 0x00;
      payload_buff[1] = 0x00;
      payload_buff[2] = 0xFC;
      payload_buff[3] = 0x12; 
}
void prepare_prefix_and_payload_ISR(uint8_t payload_buf_length)
{
      // Setup send_prefix
      send_prefix[0] = CMD_SEND;
      // set the size part of the prefix to payload length + 9 other bytes
      send_prefix[1] = payload_buf_length + 9;
      send_prefix[2] = FLAG_SEND_REQUEST;
      send_prefix[3] = DESTADDR_HI;
      send_prefix[4] = DESTADDR_LO;
      // setup the ????
      send_prefix[5] = svc_index_rcvd;
      send_prefix[6] = APPDOMAIN_MAINTENANCE;
      send_prefix[7] = PRIORITY_HI;                                                                      
      send_prefix[8] = 0;
      send_prefix[9] = 0;
      send_prefix[10] = 0xFF;
      send_prefix[11] = payload_buf_length;
      payload_buff[0] = 0x00;
      payload_buff[1] = 0x00;
      payload_buff[2] = 0xFC;                          
      payload_buff[3] = 0x12; 
}
void generate_message_and_send()
{
   uint8_t n, res_byte;
   // generate the message
   dust_hdlc_pkgen(send_prefix, 12, payload_buff, send_prefix[11]);
   
   // try to send the message a 3 times
   for (n = 0; n < 3; n++)
   {
      // try to send the message
      res_byte = dust_hdlc_pksend();   
      // timeout of 50ms if pksend returns an error
      setup_T0_int(T0_50MS);
      // if the message doesn't return no error, try sending again
      while ((res_byte != NO_ERR) && !TMR0IF);       
      if (TMR0IF)
         continue;
      else
         break;
   }
}
void generate_message_and_send_ISR()
{
   uint8_t n, res_byte;
   // generate the message
   dust_hdlc_pkgen_ISR(send_prefix, 12, payload_buff, send_prefix[11]);
                                   
   // try to send the message a 3 times
   for (n = 0; n < 3; n++)
   {
      // try to send the message
      res_byte = dust_hdlc_pksend_ISR();   
      // timeout of 50ms if pksend returns an error
      setup_T4_int(T4_50MS); 
      // if the message doesn't return no error, try sending again
      while ((res_byte != NO_ERR) && !TMR0IF);       
      if (TMR4IF)
         continue;
      else
         break;
   }
}

void send_full_report_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq)
{
   uint16_t temp_u16;
   //flash led to help identifying motes
   LED2_ON;
   prepare_prefix_and_payload(58);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_FULL_REPORT;
   payload_buff[8] = make8(global_sprinkler_num, 0);
   payload_buff[9] = make8(global_sprinkler_num, 1);
   payload_buff[10] = global_hardware_enabled;
   payload_buff[11] = read_system_state();
   // vbatt(unchanged charge state)
//   temp_u16 = get_vbatt_ISR(0);
   temp_u16 = 0;
   payload_buff[12] = make8(temp_u16, 0);
   payload_buff[13] = make8(temp_u16, 1);
   // vbatt(no charging)       
//   temp_u16 = get_vbatt_ISR(1);
   temp_u16 = 0;
   payload_buff[14] = make8(temp_u16, 0);
   payload_buff[15] = make8(temp_u16, 1);
   // vgen(unchanged charge state)
//   temp_u16 = get_vgen_ISR(0);
   temp_u16 = 0;
   payload_buff[16] = make8(temp_u16, 0);
   payload_buff[17] = make8(temp_u16, 1);      
   /*
   disable_interrupts(INT_CCP5);             
   temp_u16 = convert_period_to_rpm(global_current_period);
   enable_interrupts(INT_CCP5);
   */
   temp_u16 = 0;
   payload_buff[18] = make8(temp_u16, 0);
   payload_buff[19] = make8(temp_u16, 1);               
   payload_buff[20] = make8(global_rpm_set_value,0);
   payload_buff[21] = make8(global_rpm_set_value,1);
   disable_interrupts(INT_CCP4);
   if (global_current_sprinkler_queue_location != 255)
   {
      payload_buff[22] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].spin_rate,0);
      payload_buff[23] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].spin_rate,1);
   }
   else
   {
      payload_buff[22] = 0;
      payload_buff[23] = 0;
   }
   payload_buff[24] = make8(global_brake_duty,0);
   payload_buff[25] = make8(global_brake_duty,1);
   payload_buff[26] = make8(global_brake_duty_set_value,0);
   payload_buff[27] = make8(global_brake_duty_set_value,1);
   payload_buff[28] = make8(global_charge_duty,0);
   payload_buff[29] = make8(global_charge_duty,1);
   payload_buff[30] = make8(global_charge_duty_set_value,0);
   payload_buff[31] = make8(global_charge_duty_set_value,1);
   payload_buff[32] = global_mppc_value;
   payload_buff[33] = global_control_loop_mechanism;
   payload_buff[34] = make8(global_calibrate_fsr_valve_position, 0);
   payload_buff[35] = make8(global_calibrate_fsr_valve_position, 1);
   temp_u16 = convert_period_to_rpm(global_calibrate_fsr_period);
   payload_buff[36] = make8(temp_u16, 0);
   payload_buff[37] = make8(temp_u16, 1);
   payload_buff[38] = LATE;
   payload_buff[39] = make8(global_valve_position,0);
   payload_buff[40] = make8(global_valve_position,1);
   payload_buff[41] = make8(global_valve_position_set_value,0);
   payload_buff[42] = make8(global_valve_position_set_value,1);
   if (global_current_sprinkler_queue_location != 255)
   {
      payload_buff[43] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].vlv_open_amount,0);
      payload_buff[44] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].vlv_open_amount,1);
      payload_buff[45] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].start_time,0);
      payload_buff[46] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].start_time,1);
      payload_buff[47] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].start_time,2);
      payload_buff[48] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].start_time,3);
   }
   else
   {
      payload_buff[43] = 0;
      payload_buff[44] = 0;
      payload_buff[45] = 0;
      payload_buff[46] = 0;
      payload_buff[47] = 0;
      payload_buff[48] = 0;
   }
   enable_interrupts(INT_CCP4);
   payload_buff[49] = make8(global_xdcr_output, 0);
   payload_buff[50] = make8(global_xdcr_output, 1);
      //  JG: changed from mote uptime to boot time
   payload_buff[51] = make8(global_first_utc_time,0);
   payload_buff[52] = make8(global_first_utc_time,1);
   payload_buff[53] = make8(global_first_utc_time,2);
   payload_buff[54] = make8(global_first_utc_time,3);
   /*
   payload_buff[51] = make8(global_rtc_time,0);
   payload_buff[52] = make8(global_rtc_time,1);
   payload_buff[53] = make8(global_rtc_time,2);
   payload_buff[54] = make8(global_rtc_time,3);
   */
   payload_buff[55] = global_previous_shutdown_cause;
   payload_buff[56] = make8(FIRMWARE_VERSION,0);
   payload_buff[57] = make8(FIRMWARE_VERSION,1);

   generate_message_and_send();
   LED2_OFF;
}

void send_full_report(uint8_t command_id_in_response_to, uint16_t msg_seq)
{
   uint16_t temp_u16;
   //flash led to help identifying motes 
   prepare_prefix_and_payload(58);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_FULL_REPORT;
   payload_buff[8] = make8(global_sprinkler_num, 0);
   payload_buff[9] = make8(global_sprinkler_num, 1);
   payload_buff[10] = global_hardware_enabled;
   payload_buff[11] = read_system_state();
   // vbatt(unchanged charge state)
   temp_u16 = get_vbatt(0);
   payload_buff[12] = make8(temp_u16, 0);
   payload_buff[13] = make8(temp_u16, 1);
   // vbatt(no charging)       
   temp_u16 = get_vbatt(1);
   payload_buff[14] = make8(temp_u16, 0);
   payload_buff[15] = make8(temp_u16, 1);
   // vgen(unchanged charge state)
   temp_u16 = get_vgen(0);
   payload_buff[16] = make8(temp_u16, 0);
   payload_buff[17] = make8(temp_u16, 1);      
   disable_interrupts(INT_CCP5);             
   temp_u16 = convert_period_to_rpm(global_current_period);
   enable_interrupts(INT_CCP5);
   payload_buff[18] = make8(temp_u16, 0);
   payload_buff[19] = make8(temp_u16, 1);               
   payload_buff[20] = make8(global_rpm_set_value,0);
   payload_buff[21] = make8(global_rpm_set_value,1);
   disable_interrupts(INT_CCP4);
   if (global_current_sprinkler_queue_location != 255)
   {
      payload_buff[22] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].spin_rate,0);
      payload_buff[23] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].spin_rate,1);
   }
   else
   {
      payload_buff[22] = 0;
      payload_buff[23] = 0;
   }
   payload_buff[24] = make8(global_brake_duty,0);
   payload_buff[25] = make8(global_brake_duty,1);
   payload_buff[26] = make8(global_brake_duty_set_value,0);
   payload_buff[27] = make8(global_brake_duty_set_value,1);
   payload_buff[28] = make8(global_charge_duty,0);
   payload_buff[29] = make8(global_charge_duty,1);
   payload_buff[30] = make8(global_charge_duty_set_value,0);
   payload_buff[31] = make8(global_charge_duty_set_value,1);
   payload_buff[32] = global_mppc_value;
   payload_buff[33] = global_control_loop_mechanism;
   payload_buff[34] = make8(global_calibrate_fsr_valve_position, 0);
   payload_buff[35] = make8(global_calibrate_fsr_valve_position, 1);
   temp_u16 = convert_period_to_rpm(global_calibrate_fsr_period);
   payload_buff[36] = make8(temp_u16, 0);
   payload_buff[37] = make8(temp_u16, 1);
   payload_buff[38] = LATE;
   payload_buff[39] = make8(global_valve_position,0);
   payload_buff[40] = make8(global_valve_position,1);
   payload_buff[41] = make8(global_valve_position_set_value,0);
   payload_buff[42] = make8(global_valve_position_set_value,1);
   if (global_current_sprinkler_queue_location != 255)
   {
      payload_buff[43] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].vlv_open_amount,0);
      payload_buff[44] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].vlv_open_amount,1);
      payload_buff[45] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].start_time,0);
      payload_buff[46] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].start_time,1);
      payload_buff[47] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].start_time,2);
      payload_buff[48] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].start_time,3);
   }
   else
   {
      payload_buff[43] = 0;
      payload_buff[44] = 0;
      payload_buff[45] = 0;
      payload_buff[46] = 0;
      payload_buff[47] = 0;
      payload_buff[48] = 0;
   }
   enable_interrupts(INT_CCP4);
   payload_buff[49] = make8(global_xdcr_output, 0);
   payload_buff[50] = make8(global_xdcr_output, 1);
      //  JG: changed from mote uptime to boot time
   payload_buff[51] = make8(global_first_utc_time,0);
   payload_buff[52] = make8(global_first_utc_time,1);
   payload_buff[53] = make8(global_first_utc_time,2);
   payload_buff[54] = make8(global_first_utc_time,3);
   /*
   payload_buff[51] = make8(global_rtc_time,0);
   payload_buff[52] = make8(global_rtc_time,1);
   payload_buff[53] = make8(global_rtc_time,2);
   payload_buff[54] = make8(global_rtc_time,3);
   */
   payload_buff[55] = global_previous_shutdown_cause;
   payload_buff[56] = make8(FIRMWARE_VERSION,0);
   payload_buff[57] = make8(FIRMWARE_VERSION,1);

   generate_message_and_send();
}
void send_health_report(uint8_t command_id_in_response_to, uint16_t msg_seq)
{  
   uint16_t temp_u16;
   
   prepare_prefix_and_payload(18);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_HEALTH_REPORT;
   payload_buff[8] = make8(global_sprinkler_num, 0);
   payload_buff[9] = make8(global_sprinkler_num, 1);
   payload_buff[10] = global_hardware_enabled;
   payload_buff[11] = read_system_state();
   // vbatt(unchanged charge state)
   temp_u16 = get_vbatt(0);
   payload_buff[12] = make8(temp_u16, 0);
   payload_buff[13] = make8(temp_u16, 1);
   // vbatt(no charging)
   temp_u16 = get_vbatt(1);
   payload_buff[14] = make8(temp_u16, 0);
   payload_buff[15] = make8(temp_u16, 1);
   payload_buff[16] = global_valve_movements_since_endstop;
   // grab the most recent temperature
   payload_buff[17] = global_mote_temperature;
   generate_message_and_send();
}
void send_valve_report(uint8_t command_id_in_response_to, uint16_t msg_seq)
{
   uint16_t temp_u16;
   
   prepare_prefix_and_payload(28);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;                                                        
   payload_buff[7] = MSG_MOTE_VALVE_REPORT; 
   payload_buff[8] = make8(global_sprinkler_num, 0);
   payload_buff[9] = make8(global_sprinkler_num, 1);
   payload_buff[10] = make8(global_valve_position, 0);
   payload_buff[11] = make8(global_valve_position, 1);
   payload_buff[12] = make8((global_valve_time_to_open_1024th << 4), 0);
   payload_buff[13] = make8((global_valve_time_to_open_1024th << 4), 1);
   payload_buff[14] = make8((global_valve_time_to_close_1024th << 4), 0);
   payload_buff[15] = make8((global_valve_time_to_close_1024th << 4), 1);
   payload_buff[16] = make8(global_valve_calibration_utc_time, 0);
   payload_buff[17] = make8(global_valve_calibration_utc_time, 1);
   payload_buff[18] = make8(global_valve_calibration_utc_time, 2);
   payload_buff[19] = make8(global_valve_calibration_utc_time, 3);
   payload_buff[20] = make8(global_calibrate_fsr_valve_position, 0);
   payload_buff[21] = make8(global_calibrate_fsr_valve_position, 1);
   temp_u16 = convert_period_to_rpm(global_calibrate_fsr_period);
   payload_buff[22] = make8(temp_u16, 0);
   payload_buff[23] = make8(temp_u16, 1);
   payload_buff[24] = make8(global_calibrate_fsr_utc_time, 0);
   payload_buff[25] = make8(global_calibrate_fsr_utc_time, 1);
   payload_buff[26] = make8(global_calibrate_fsr_utc_time, 2);
   payload_buff[27] = make8(global_calibrate_fsr_utc_time, 3);
   generate_message_and_send();
}
void send_sprinkler_cmd_response(uint8_t command_id_in_response_to, uint16_t msg_seq)
{
   uint16_t temp_u16;
   
   prepare_prefix_and_payload(25);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_SPINKLER_CMD_RESPONSE;
   payload_buff[8] = make8(global_sprinkler_num, 0);
   payload_buff[9] = make8(global_sprinkler_num, 1);
   payload_buff[10] = global_hardware_enabled;
   payload_buff[11] = make8(global_rpm_set_value, 0);
   payload_buff[12] = make8(global_rpm_set_value, 1);
   disable_interrupts(INT_CCP4);
   if (global_current_sprinkler_queue_location != 255)
   {
      payload_buff[13] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].spin_rate,0);
      payload_buff[14] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].spin_rate,1);
   }
   else
   {
      payload_buff[13] = 0;
      payload_buff[14] = 0;
   }
   payload_buff[15] = make8(global_valve_position_set_value,0);
   payload_buff[16] = make8(global_valve_position_set_value,1);
   if (global_current_sprinkler_queue_location != 255)
   {
      payload_buff[17] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].vlv_open_amount,0);
      payload_buff[18] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].vlv_open_amount,1);
      payload_buff[19] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].start_time,0);
      payload_buff[20] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].start_time,1);
      payload_buff[21] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].start_time,2);
      payload_buff[22] = make8(global_sprinkler_queue[global_current_sprinkler_queue_location].start_time,3);
   }
   else
   {
      payload_buff[17] = 0;
      payload_buff[18] = 0;
      payload_buff[19] = 0;
      payload_buff[20] = 0;
      payload_buff[21] = 0;
      payload_buff[22] = 0;
   }
   enable_interrupts(INT_CCP4);
   // vbatt(no charging)
   temp_u16 = get_vbatt(1);
   payload_buff[23] = make8(temp_u16, 0);
   payload_buff[24] = make8(temp_u16, 1);
   generate_message_and_send();
}
void send_update_sp_num_response(uint8_t command_id_in_response_to, uint16_t msg_seq)
{
   prepare_prefix_and_payload(10);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_UPDATE_SP_NUM_RESPONSE;
   payload_buff[8] = make8(global_sprinkler_num, 0);
   payload_buff[9] = make8(global_sprinkler_num, 1);
   generate_message_and_send();
}       
void send_read_eeprom_response(uint8_t command_id_in_response_to, uint16_t msg_seq, uint16_t eeprom_location, uint32_t eeprom_data) 
{
   prepare_prefix_and_payload(16);
   payload_buff[4] = make8(msg_seq,0);                                                   
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_EEPROM_READ_RESPONSE; 
   payload_buff[8] = make8(global_sprinkler_num, 0);
   payload_buff[9] = make8(global_sprinkler_num, 1);
   payload_buff[10] = make8(eeprom_location, 0);
   payload_buff[11] = make8(eeprom_location, 1);  
   payload_buff[12] = make8(eeprom_data, 0);                         
   payload_buff[13] = make8(eeprom_data, 1);  
   payload_buff[14] = make8(eeprom_data, 2);    
   payload_buff[15] = make8(eeprom_data, 3);  
   generate_message_and_send();   
}               
void send_pressure_xdcr_msg(uint8_t command_id_in_response_to, uint16_t msg_seq)   
{                   
   prepare_prefix_and_payload(12);
   payload_buff[4] = make8(msg_seq,0);                                                   
   payload_buff[5] = make8(msg_seq,1);      
   payload_buff[6] = command_id_in_response_to;                   
   payload_buff[7] = MSG_MOTE_XDCR_READ_MSG;          
   payload_buff[8] = make8(global_sprinkler_num, 0);   
   payload_buff[9] = make8(global_sprinkler_num, 1);
   payload_buff[10] = make8(global_xdcr_output, 0);                 
   payload_buff[11] = make8(global_xdcr_output, 1);   
   generate_message_and_send();                              
}       
void send_ack(uint8_t command_id_in_response_to, uint16_t msg_seq)
{
   prepare_prefix_and_payload(8);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_ACK;
   generate_message_and_send();
}
void send_nack(uint8_t command_id_in_response_to, uint16_t msg_seq, uint32_t nack_bitfield)
{
   prepare_prefix_and_payload(12);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_NACK;
   payload_buff[8] = make8(nack_bitfield, 0);
   payload_buff[9] = make8(nack_bitfield, 1);
   payload_buff[10] = make8(nack_bitfield, 2);
   payload_buff[11] = make8(nack_bitfield, 3);
   generate_message_and_send();
}                                       

// unsolicted responses (only sent within CCP4 ISR)
void send_health_report_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq)
{
   uint16_t temp_u16;
   
   prepare_prefix_and_payload_ISR(18);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_HEALTH_REPORT;
   payload_buff[8] = make8(global_sprinkler_num, 0);
   payload_buff[9] = make8(global_sprinkler_num, 1);
   payload_buff[10] = global_hardware_enabled;
   payload_buff[11] = global_system_state;
 // vbatt(unchanged charge state)
   temp_u16 = get_vbatt_ISR(0);
   payload_buff[12] = make8(temp_u16, 0);
   payload_buff[13] = make8(temp_u16, 1);
   // vbatt(no charging)
   temp_u16 = get_vbatt_ISR(1);
   payload_buff[14] = make8(temp_u16, 0);
   payload_buff[15] = make8(temp_u16, 1);
   payload_buff[16] = global_valve_movements_since_endstop;
   payload_buff[17] = global_mote_temperature;
   generate_message_and_send_ISR();
}
void send_valve_report_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq)
{
   uint16_t temp_u16;
   prepare_prefix_and_payload_ISR(28);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_VALVE_REPORT;
   payload_buff[8] = make8(global_sprinkler_num, 0);
   payload_buff[9] = make8(global_sprinkler_num, 1);
   payload_buff[10] = make8(global_valve_position, 0);
   payload_buff[11] = make8(global_valve_position, 1);
   payload_buff[12] = make8((global_valve_time_to_open_1024th << 4), 0);
   payload_buff[13] = make8((global_valve_time_to_open_1024th << 4), 1);
   payload_buff[14] = make8((global_valve_time_to_close_1024th << 4), 0);
   payload_buff[15] = make8((global_valve_time_to_close_1024th << 4), 1);
   payload_buff[16] = make8(global_valve_calibration_utc_time, 0);
   payload_buff[17] = make8(global_valve_calibration_utc_time, 1);
   payload_buff[18] = make8(global_valve_calibration_utc_time, 2);
   payload_buff[19] = make8(global_valve_calibration_utc_time, 3);
   payload_buff[20] = make8(global_calibrate_fsr_valve_position, 0);
   payload_buff[21] = make8(global_calibrate_fsr_valve_position, 1);
   temp_u16 = convert_period_to_rpm_ISR(global_calibrate_fsr_period);
   payload_buff[22] = make8(temp_u16, 0);
   payload_buff[23] = make8(temp_u16, 1);
   payload_buff[24] = make8(global_calibrate_fsr_utc_time, 0);
   payload_buff[25] = make8(global_calibrate_fsr_utc_time, 1);
   payload_buff[26] = make8(global_calibrate_fsr_utc_time, 2);
   payload_buff[27] = make8(global_calibrate_fsr_utc_time, 3);
   generate_message_and_send_ISR();
}
void send_join_msg_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq)
{
   prepare_prefix_and_payload_ISR(18);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_JOIN_MSG;
   payload_buff[8] = make8(global_sprinkler_num, 0);
   payload_buff[9] = make8(global_sprinkler_num, 1);
   payload_buff[10] = global_hardware_enabled;
   payload_buff[11] = make8(global_rtc_time,0);
   payload_buff[12] = make8(global_rtc_time,1);
   payload_buff[13] = make8(global_rtc_time,2);
   payload_buff[14] = make8(global_rtc_time,3);   
   payload_buff[15] = global_previous_shutdown_cause;
   payload_buff[16] = make8(FIRMWARE_VERSION,0);
   payload_buff[17] = make8(FIRMWARE_VERSION,1);  
   generate_message_and_send_ISR();
}
void send_gps_point_msg_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq)
{
   prepare_prefix_and_payload_ISR(23);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_GPS_POINT_MSG;
   payload_buff[8] = make8(global_sprinkler_num, 0);
   payload_buff[9] = make8(global_sprinkler_num, 1);
   generate_message_and_send_ISR();
}                 
void send_pressure_xdcr_msg_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq)   
{                         
   prepare_prefix_and_payload_ISR(12); 
   payload_buff[4] = make8(msg_seq,0);                                                   
   payload_buff[5] = make8(msg_seq,1);      
   payload_buff[6] = command_id_in_response_to;                   
   payload_buff[7] = MSG_MOTE_XDCR_READ_MSG;    
   payload_buff[8] = make8(global_sprinkler_num, 0);   
   payload_buff[9] = make8(global_sprinkler_num, 1);                       
   payload_buff[10] = make8(global_xdcr_output, 0);                 
   payload_buff[11] = make8(global_xdcr_output, 1);      
   generate_message_and_send_ISR();                              
} 
void send_error_msg_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq)
{
   prepare_prefix_and_payload_ISR(14);
   payload_buff[4] = make8(msg_seq,0);
   payload_buff[5] = make8(msg_seq,1);
   payload_buff[6] = command_id_in_response_to;
   payload_buff[7] = MSG_MOTE_ERROR_MSG;
   payload_buff[8] = make8(global_sprinkler_num, 0);
   payload_buff[9] = make8(global_sprinkler_num, 1);
   payload_buff[10] = make8(global_error_message_bitfield,0);
   payload_buff[11] = make8(global_error_message_bitfield,1);
   payload_buff[12] = make8(global_error_message_bitfield,2);
   payload_buff[13] = make8(global_error_message_bitfield,3);
   generate_message_and_send_ISR();
}
