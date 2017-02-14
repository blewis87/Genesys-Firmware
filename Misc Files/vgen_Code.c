/*
      if (!input(SW1n)) 
      {
         advance_mppc_flag++;
         if (advance_mppc_flag == 1) global_mppc_value = 0;
         while (!input(SW1n));
      }
      if (advance_mppc_flag && TMR0IF)
      {
         global_mppc_value++;
         set_mppc(global_mppc_value);
         setup_T0_int(T0_1S);
      }
      */

      /*
      // if user presses button to up MPPC or to change vgen set point
      if (!input(SW1n))
      {
         setup_T0_int(T0_10MS);
         while (!input(SW1n))
         {
            if (TMR0IF)
            {
               global_mppc_value++;
               set_mppc(global_mppc_value);
               setup_T0_int(T0_100MS);
            }
            get_vgen(0); 
            LCD_place_uint16(global_vgen, 0, 12, 4); 
            LCD_place_uint8(global_mppc_value, 1, 4, 3);
            LCD_place_uint16(calc_gen_rpm(), 1, 11, 5);
         }
      }
      */      
      
      // if user presses button to up MPPC or to change vgen set point
      if (!input(SW1n))
      {
         // wait for 3S to differentiate between long and short press
         // or until the button is unpressed
            setup_T0_int(T0_3S);
            while (!TMR0IF && !input(SW1n));
            
            // If the button got unpressed before the timer (else statement) 
            // increase the mppc value, otherwise, move onto the next vgen
            // set point and reset mppc to 0 and brake to 0
            if (TMR0IF)
            {               

            }
            else
            {  
               global_vgen_set_value+= 100;
               global_mppc_value = NO_MPPC;
               set_mppc(global_mppc_value);
               max_batt_charge_current = 0;
            }
         // wait for switch release
         while (!input(SW1n));
         LCD_clear();
      }  
      
      
      /*
      tempu16++;
      if (tempu16 > MAX_CHARGE) tempu16 = NO_CHARGE;
      set_charge_duty(tempu16);
      */

      
      /*
      for (tempu16 = NO_BRK; tempu16 <= MAX_BRK; tempu16+= (MAX_BRK - NO_BRK)/100)
      set_brake_duty(uint16_t value);
      set_charge_duty(uint16_t value);
      set_mppc(uint8_t value);
      set_fixture_dac(uint16_t dac_value);
      */
      
      /*
      if (TMR2IF) 
      {
         t2_loop_delay_cycles_left--;
         
         if (t2_loop_delay_cycles_left == 0)
         {

            //adjust_brake_for_vgen(global_vgen_set_value);
            vgen_difference = (abs(global_vgen_set_value - get_vgen(0)));
            
            // if max brake and still too slow, move on
            if (((global_vgen_set_value > get_vgen(0)) && (global_brake_duty == NO_BRK)) || ((get_vbattcharge(0) < 50) && (vgen_difference < 50)))
            {  
               charge_current_array[global_mppc_value] = 0;
               global_mppc_value++;
               set_mppc(global_mppc_value);
               delay_ms(1000);
               //setup_timer_0(T0_4S);
               //while(!TMR0IF);
            }
            
            
            //setup_T2_int(T2_64MS);
            //t2_loop_delay_cycles_left = 20;
            if (vgen_difference > 20) 
            {
               adjust_brake_for_vgen(global_vgen_set_value);
               stable_loop_counter = 0;
            }
            else
            {
               //stable vgen
               if (stable_loop_counter++ > 2)
               {
                  batt_charge_current = get_vbattcharge(0);
                  charge_current_array[global_mppc_value] = batt_charge_current;
                  LCD_place_uint16(batt_charge_current,1,13,3);
                  if (batt_charge_current > max_batt_charge_current)
                  {
                     max_batt_charge_current = batt_charge_current;
                     max_charge_mppc = global_mppc_value;
                  }
                  
                  if (global_mppc_value != MAX_MPPC)
                  {
                     global_mppc_value++;
                     set_mppc(global_mppc_value);
                  }
                  else
                  {
                     strcpy (global_temp_line_buff, "Ready to unload!");
                     LCD_line1(global_temp_line_buff);
                     while(input(SW1n));
                     for (n = 0; n != 255; n++)
                     {
                        LCD_clear();
                        LCD_place_uint8(n,0,0,3);
                        LCD_place_uint16(global_vgen_set_value, 0, 10, 4);
                        LCD_place_uint16(charge_current_array[global_mppc_value],1,0,4);
                        while(input(SW1n));
                     }
                     global_vgen_set_value += 250;
                     global_mppc_value = NO_MPPC;
                     set_mppc(global_mppc_value);
                  }
                  stable_loop_counter = 0;
               }
            }
               
               
               
            if (vgen_difference < 15)
            {
               setup_T2_int(T2_64MS);
               t2_loop_delay_cycles_left = 25;
            }
            else if (vgen_difference < 25) 
            {
               setup_T2_int(T2_64MS);
               t2_loop_delay_cycles_left = 20;
            }
            else if (vgen_difference < 50) 
            {  
               setup_T2_int(T2_64MS);
               t2_loop_delay_cycles_left = 15;
            }
            else if (vgen_difference < 100) 
            {
               setup_T2_int(T2_64MS);
               t2_loop_delay_cycles_left = 10;
            }
            else if (vgen_difference < 200) 
            {
               setup_T2_int(T2_64MS);
               t2_loop_delay_cycles_left = 5;
            }
            else 
            {
               setup_T2_int(T2_64MS);
               t2_loop_delay_cycles_left = 1;
            }
            
         }
         else
         {
               TMR2ON = FALSE;
               set_timer2(0);
               TMR2IF = FALSE;
               TMR2ON = TRUE;
         }
      }    
*/      