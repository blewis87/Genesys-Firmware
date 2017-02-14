// This file contains all functions definitions for the LCD and XDCR
// The globals.h file is needed

////////////////////LCD Functions Start/////////////////////////////////////////

//**************************************************************
// These routines communicate to the 16x2 LCD display via I2C. *
// Here are the HEX codes for the 32 display positions:        *
// Line 1   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F    *
// Line 2   40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F    *
//**************************************************************

void LCD_init(void)   
{
   output_low(LCD_RESETn);                               
  
   delay_ms(1);
   
   //setup_T0_int(T0_1MS);
   //while (!TMR0IF);

   output_high(LCD_RESETn);
   //setup_T0_int(T0_100MS);
   //while (!TMR0IF);
   
   delay_ms(100);
   i2c_start();
   
   global_lcd_enabled = !i2c_write(LCD_ADDRESS);    // (0x7C)WRITE LCD address
   if (!global_lcd_enabled)
   {
      i2c_stop();
      return;
   }

   i2c_write(0x00);              // (0x00)Control byte
   i2c_write(0x38);              // (0x38)Function Set: 8 bits,
   i2c_stop();                   //  use Instruction Table 0
   delay_cycles(255);
   
   i2c_start();
   i2c_write(LCD_ADDRESS);     // (0x7C)WRITE LCD address
   i2c_write(0x00);     // (0x00)Control byte
   i2c_write(0x40);     // set CG address 0
   i2c_stop();
   delay_cycles(255);
  
   i2c_start();         // define special character 
   i2c_write(LCD_ADDRESS);     // WRITE LCD address
   i2c_write(0x40);     // Datasend
   
// special char 0 (LEV1)
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b11111);
// special char 1 (LEV2)
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b11111);
   i2c_write(0b11111);
// special char 2 (LEV4)
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b11111);
   i2c_write(0b00000);
   i2c_write(0b00000);
// special char 3 (LEV5)
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b11111);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
// special char 4 (LEV7)
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b11111);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
// special char 5 (LEV8)
   i2c_write(0b00000);
   i2c_write(0b11111);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
// special char 6 (LEV9)
   i2c_write(0b11111);
   i2c_write(0b11111);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   i2c_write(0b00000);
   // special char 7 (BIGBOX)
   i2c_write(0b11111);
   i2c_write(0b11111);
   i2c_write(0b11111);
   i2c_write(0b11111);
   i2c_write(0b11111);
   i2c_write(0b11111);
   i2c_write(0b11111);
   i2c_write(0b00000); 
 
   i2c_stop(); 

   i2c_start();
   i2c_write(0x00);              // (0x00)Control byte
   i2c_write(0x38);              // (0x38)Function Set: 8 bits,
   i2c_stop();                   //  use Instruction Table 0
   
   i2c_stop(); 
   delay_cycles(255);
   
   i2c_start();                            
   i2c_write(LCD_ADDRESS);     // (0x7C)WRITE LCD address
   i2c_write(0x00);     // (0x00)Control byte
   i2c_write(0x38);     // (0x38)Function Set: 8 bits, use Instruction Table 0
//   delay_ms(40);
   delay_cycles(255);
   i2c_write(0x39);     // (0x39)Function Set: 8 bits, use Instruction Table 1
//   delay_ms(40);
   delay_cycles(255);
   i2c_write(0x14);     // (0x14)Internal OSC frequency
   i2c_write(0x74);     // (0x78 recommended, JG prefers 0x74)Contrast set
   i2c_write(0x5E);     // (0x5E)ICON display ON, booster ON
   i2c_write(0x6D);     // (0x6D)Follower circuit ON
//   delay_ms(200);
   delay_cycles(255);
   i2c_write(0x0C);     // (0x0C)Entire display ON; 
                        // (0x0F)Entire display ON/blink at cursor
   i2c_write(0x01);     // (0x01)Clear display
   delay_ms(40);
   i2c_write(0x06);     // (0x06)Entry Mode Set
//   delay_ms(40);
   delay_cycles(255);
   i2c_write(0x38);
//   delay_ms(40);
   delay_cycles(255);
   i2c_stop();
   

   i2c_start();
   i2c_write(LCD_ADDRESS);     // (0x7C)WRITE LCD address
   i2c_write(0x00);     // (0x00)Control byte
   i2c_write(0x38);     // (0x38)Function Set: 8 bits,
                        //  use Instruction Table 0
   i2c_write(0x18);     // Clear any shift
   i2c_write(0x20);     // CGRAM = 0
   // JG Jan 30 2015
   i2c_write(0x38);
   
   i2c_stop();
   
   strcpy (global_temp_line_buff, "                ");
   LCD_line1(global_temp_line_buff);
   LCD_line2(global_temp_line_buff);

   return;
}

void LCD_line1(unsigned char *text)
{
   uint8_t  n;

   if (!global_lcd_enabled) return;  
   LCD_position(0x00);   // Move to start of first line
   i2c_start();
   i2c_write(LCD_ADDRESS);   // WRITE LCD address
   i2c_write(0x40);    // Datasend
   for(n=0;n<16;n++)
   {
      i2c_write(*text);
      ++text;
   }
   i2c_stop();
}

void LCD_line2(unsigned char *text)
{
   uint8_t  n;
   
   if (!global_lcd_enabled) return;
   LCD_position(0x40);   // Move to start of second line
   i2c_start();
   i2c_write(LCD_ADDRESS);   // WRITE LCD address
   i2c_write(0x40);    // Datasend
   for(n=0;n<16;n++)
   {
      i2c_write(*text);
      ++text;
   }
   i2c_stop();
}

void LCD_place_text(unsigned char *text, int1 row, uint8_t col, uint8_t length)
{
   //places text at a particular position (row (0 or 1), col (0-15)).
   //not much error checking (for speed) on row/col or length (don't cause seg fault)
   
   uint8_t n;
   
   if (!global_lcd_enabled) return;
   if (col < 16)
   {
      if (row == 0) LCD_position(col);   //set LCD position (row,col)
      else LCD_position(0x40 + col);
      
      i2c_start();
      i2c_write(LCD_ADDRESS);   // WRITE LCD address
      i2c_write(0x40);    // Datasend
      for(n = 0; ((n < length) && ((n + col) < 16)); n++)
      {
        i2c_write(*text);
        ++text;
      }
      i2c_stop();
   }
}

void LCD_place_char(char ascii, int1 row, uint8_t col)
{
   //places a char at a particular position (row (0 or 1), col (0-15)).
   //no error checking (for speed) on row/col (don't cause seg fault)
   
   if (!global_lcd_enabled) return;
   if (col < 16)
   {
      if (row == 0) LCD_position(col);   //set LCD position (row,col)
      else LCD_position(0x40 + col);
      
      LCD_char(ascii);
   }
}

void LCD_place_uint8(uint8_t num, int1 row, uint8_t col, uint8_t length)
{
   uint8_t n; 
   
   if (!global_lcd_enabled) return;  
   
   if (length <= 3)
   {
      for (n = 0; n < 3; n++)
      {
         *(global_temp_line_buff + (2 - n)) = ('0' + (num%10));
         num/=10;
      }
      global_extra_lcd_ptr = global_temp_line_buff;
      for (n = length; n < 3; n++)
      {
         ++global_extra_lcd_ptr;
      }
      
      LCD_place_text(global_extra_lcd_ptr,row, col, length);
   }   
}

void LCD_place_uint16(uint16_t num, int1 row, uint8_t col, uint8_t length)
{
   uint8_t n;  
   char *global_temp_line_buff[5];
   
   if (!global_lcd_enabled) return;  
   
   if (length <= 5)
   {
      for (n = 0; n < 5; n++)
      {
         *(global_temp_line_buff + (4 - n)) = ('0' + (num%10));
         num/=10;
      }
      global_extra_lcd_ptr = global_temp_line_buff;
      for (n = length; n < 5; n++)
      {
         ++global_extra_lcd_ptr;
      }
      
      LCD_place_text(global_extra_lcd_ptr,row, col, length);
   }   
}

void LCD_place_uint32(uint32_t num, int1 row, uint8_t col, uint8_t length)
{
   uint8_t n;  
   
   if (!global_lcd_enabled) return;  
   
   if (length <= 10)
   {
      for (n = 0; n < 10; n++)
      {
         *(global_temp_line_buff + (9 - n)) = ('0' + (num%10));
         num/=10;
      }
      global_extra_lcd_ptr = global_temp_line_buff;
      for (n = length; n < 10; n++)
      {
         ++global_extra_lcd_ptr;
      }
      
      LCD_place_text(global_extra_lcd_ptr,row, col, length);
   }   
}

void LCD_clear(void)
{
   if (!global_lcd_enabled) return;
   
   strcpy (global_temp_line_buff, "                ");
   LCD_line1(global_temp_line_buff);
   LCD_line2(global_temp_line_buff);
}

void LCD_clear_line1(void)
{
   if (!global_lcd_enabled) return;
   
   strcpy (global_temp_line_buff, "                ");
   LCD_line1(global_temp_line_buff);
}

void LCD_clear_line2(void)
{
   if (!global_lcd_enabled) return;
   
   strcpy (global_temp_line_buff, "                ");
   LCD_line2(global_temp_line_buff);
}

void LCD_position(int position)
{
   if (!global_lcd_enabled) return;
   i2c_start();                            
   i2c_write(LCD_ADDRESS);     // (0x7C)WRITE LCD address
   i2c_write(0x00);     // (0x00)Control byte
   i2c_write(0x80+position); // 
   i2c_stop();
}

void LCD_char(char ascii)   
{
   if (!global_lcd_enabled) return;
   i2c_start();                            
   i2c_write(LCD_ADDRESS);   // WRITE LCD address
   i2c_write(0x40);    // Datasend
   i2c_write(ascii);    // 
   i2c_stop();
}

void LCD_shutdown(void)   
{       

   if (!global_lcd_enabled) return;  
      
   output_low(LCD_RESETn);
   delay_ms(5);
   output_high(LCD_RESETn);
   
   i2c_start();                            
   i2c_write(LCD_ADDRESS);     // WRITE LCD address
   i2c_write(0x00);     // Control byte
   i2c_write(0x39);     // Function Set: 8 bits, use Instruction Table 1
   i2c_write(0x52);     // ICON display OFF, booster OFF
   i2c_write(0x60);     // Follower circuit OFF
   i2c_write(0x08);     // Entire display OFF
   i2c_stop();
   
   output_low(LCD_RESETn);
   delay_ms(5);
   output_high(LCD_RESETn);
   global_lcd_enabled = 0;
}

void LCD_startup_splash(void)
{                              
   uint8_t n;
   
   if (!global_lcd_enabled) return;
   
   strcpy(global_temp_line_buff, "Jain Genesys    ");
   for (n = 0; n < 16; n++)
      {
      LCD_place_text(global_temp_line_buff,0,(15-n),16);
      setup_T0_int(T0_100MS);
      while (!TMR0IF);
      }
   strcpy(global_temp_line_buff, "Version         ");   
   for (n = 0; n < 16; n++)
   {
      LCD_place_text(global_temp_line_buff,1,(15-n),16);
      setup_T0_int(T0_100MS);
      while (!TMR0IF);
   }   
   strcpy(global_temp_line_buff, global_firmware_version_string);
   LCD_place_text(global_temp_line_buff,1,8,4);
   setup_T0_int(T0_2S);
   while (!TMR0IF);
   for (n = 0; n < 16; n++)
   {
      LCD_place_char(' ',0,(15-n));
      LCD_place_char(' ',1,(15-n));
      setup_T0_int(T0_100MS);
      while (!TMR0IF); 
   }   
}
/*
void LCD_display_battery_voltage(int1 lcd_line)
{
   //Grabs and displays battery voltage on LCD
   //Takes the LCD line that it should be displayed
   //on as input.

   
   uint8_t temp_bat_int_part = 0;
   uint16_t temp_bat_fract_part = 0;
   uint32_t temp_bat_both_part = 0;
   char * digit_result;
   
   if (!global_lcd_enabled) return;
   //set line buffer to all spaces
   char lcd_line_buff[17] = {' '};
   
   strcpy (lcd_line_buff, "Battery = ?.???V");

   // get the battery count (not charging)
   // and convert it to a voltage
   temp_bat_both_part = calc_vbatt(get_vbatt(1));
   temp_bat_fract_part = (uint16_t) (temp_bat_both_part);
   temp_bat_int_part = (uint8_t) (temp_bat_both_part >> 16);
   digit_result = word_to_4dig_new(temp_bat_fract_part);
   
   
   if (digit_result != 0)
   {
      lcd_line_buff[10] = '0' + temp_bat_int_part;
      lcd_line_buff[12] = digit_result[3];
      lcd_line_buff[13] = digit_result[2];
      lcd_line_buff[14] = digit_result[1];
   }
   else
   {
      strcpy (lcd_line_buff, "Battery ERROR!  ");
   }
   
   if (lcd_line == 0)
   {
      LCD_line1(lcd_line_buff);
   }
   else if (lcd_line == 1)
   {
      LCD_line2(lcd_line_buff);
   }
}

uint32_t LCD_battery_screen(uint16_t vRef)
{
//Puts the battery voltage on the LCD across both screens
//
}


void LCD_display_revision(int1 lcd_line)
{
   if (!global_lcd_enabled) return;
   strcpy (line2_buff, "Rev.            ");
   tempo_u8 = make8(FIRMWARE_REV,1);
   line2_buff[4] = hi2asc(tempo_u8);
   
#ifdef  WIP
   line2_buff[4] += 0x20;     // lowercase if work in progress
#endif
   line2_buff[5] = lo2asc(tempo_u8);
   tempo_u8 = make8(FIRMWARE_REV,0);
   line2_buff[6] = hi2asc(tempo_u8);
   line2_buff[7] = lo2asc(tempo_u8);
 
}
*/

void LCD_display_priority_time_stacks(void)
{                                           
      if (!global_lcd_enabled) return;    
      
   disable_interrupts(GLOBAL);
   LCD_place_uint8(global_current_priority_queue_location, 0, 0, 3);     
      
      if (global_current_priority_queue_location != 255)
      {     
         LCD_place_uint8(global_priority_queue[global_current_priority_queue_location], 0, 4, 3);
      }
      else
      {      
         strcpy (global_temp_line_buff, "   ");
         LCD_place_text(global_temp_line_buff, 0, 4, 3);
      }
      
      LCD_place_uint8(global_current_time_queue_location, 0, 8, 3);
      
      if (global_current_time_queue_location != 255)
      {
         LCD_place_uint8(global_priority_queue[global_current_time_queue_location], 0, 12, 3);
      }
      else
      {      
         strcpy (global_temp_line_buff, "   ");
         LCD_place_text(global_temp_line_buff, 0, 12, 3);
      }
      enable_interrupts(GLOBAL);
} 

void LCD_display_priority_time_stacks_2(void)
{
   uint8_t n;
   
   disable_interrupts(INT_CCP4);
   LCD_clear();
   for (n = 0; n < MAX_TIME_QUEUE_ITEMS; n++)
   {
      LCD_place_uint8(n,0,0,1);
      LCD_place_uint8(global_time_queue[n].pq_priority,0,2,3);                      
      LCD_place_uint32(global_time_queue[n].time_to_execute,0,6,5);
      LCD_place_uint8(global_current_time_queue_location, 0, 12, 3);       
      setup_T0_int(T0_250MS);                                                      
      while (!TMR0IF);    
   }                              
   enable_interrupts(INT_CCP4);                                 
}

void LCD_display_priority_queue(void)
{
   uint8_t n;
   
   disable_interrupts(GLOBAL);
   LCD_clear();
   for (n = 0; n < MAX_PRIORITY_QUEUE_ITEMS; n++)
   {
      LCD_place_uint8(n,0,0,2);
      LCD_place_uint8(global_priority_queue[n],0,3,3);
      LCD_place_uint8(global_current_priority_queue_location, 0, 7, 3);
      if (global_priority_queue[n] != EMPTY_PRIORITY_QUEUE)
         setup_T0_int(T0_1S);
      while (!TMR0IF);
   }
   enable_interrupts(GLOBAL);
}
                
void LCD_display_message_queue(void) 
{
   uint8_t n;
   disable_interrupts(INT_CCP4);
   
   for (n = global_current_message_queue_location; n != 255; n--)
   {                                                                                                                         
      LCD_clear();
      LCD_place_uint8(n, 0, 0, 3);                                
      LCD_place_uint32(global_message_queue[n].time_to_send, 0, 4, 10);
      LCD_place_uint8(global_message_queue[n].message_type, 1, 0, 3);
      LCD_place_uint16(global_message_queue[n].msg_seq, 1, 4, 5); 
      LCD_place_uint8(global_message_queue[n].attempt_num, 1, 10, 3); 
      while(input(SW1n));
   }                         
   LCD_clear();
   enable_interrupts(INT_CCP4);  
}                                                                       
void LCD_display_battery_voltage(int1 lcd_line)
{                                           
   if (!global_lcd_enabled) return;    
      
   uint32_t battery_voltage = calc_vbatt(get_vbatt(1));
   strcpy (global_temp_line_buff, "Vbatt =      V  ");
   if (lcd_line == 0) LCD_line1(global_temp_line_buff);
   else LCD_line2(global_temp_line_buff);
   LCD_display_batt_voltage(lcd_line, 8, 2);
//   LCD_place_uint8((uint8_t)(battery_voltage>>16),lcd_line,8,1);
//   LCD_place_uint16((uint16_t)battery_voltage,lcd_line,10,4);
}

void LCD_display_batt_voltage(int1 lcd_line, uint8_t col, uint8_t precision)
{
   // precision is the number of digits to the right of the decimal pt
   // Valid values are 1, 2, 3
   // The assumption is that there is a SINGLE digit preceeding the dp
   
   if (!global_lcd_enabled) return;

   if (col>15)
      return;
   if (precision > 3)
      precision = 3;  
   
   if ((precision == 3) && (col > 11))
      precision--;
   if ((precision == 2) && (col > 12))
      precision--;
   if ((precision == 1) && (col > 13))
      precision--;
      
   uint16_t battery_voltage_BCD = calc_vbatt_BCD(get_vbatt(1));

   uint8_t volts = '0' + (uint8_t)(battery_voltage_BCD >> 12);
   uint8_t tenths = '0' + (uint8_t)((battery_voltage_BCD & 0x0F00) >> 8);
   uint8_t hundredths = '0' + (uint8_t)((battery_voltage_BCD & 0x00F0) >> 4);
   uint8_t thousandths = '0' + (uint8_t)(battery_voltage_BCD & 0x000F);
  
   switch (precision)
   {
      case 0:
         if (tenths > '5')
            volts++;
         break;
      
      case 1:
         if (hundredths > '5')
            tenths++;
         if (tenths > '9' )
         {
            volts++;
            tenths = '0';
         }
         break;

      case 2:
         if (thousandths > '5')
            hundredths++;
         if (hundredths > '9')
         {
            tenths++;
            hundredths = '0';
         }
         if (tenths > '9')
         {
            volts++;
            tenths = '0';
         }         
         break;
   }
  
   LCD_place_char (volts, lcd_line, col++);
   
   if (precision)
   {
      LCD_place_char ('.', lcd_line, col++);
      LCD_place_char (tenths, lcd_line, col++);
       
      if (precision > 1)
         LCD_place_char (hundredths, lcd_line, col++);
      if (precision > 2)
         LCD_place_char (thousandths, lcd_line, col);
   }
}    
 
void LCD_display_vgen(int1 lcd_line, uint8_t col, uint8_t precision)
{
   // precision is the number of digits to the right of the decimal pt
   // Valid values are 1, 2, 3
   // The assumption is that there are 2 digits preceding the decimpal point
   
   if (!global_lcd_enabled) return;

   if (col>15)
      return;
   if (precision > 3)
      precision = 3;  
   
   if ((precision == 3) && (col > 10))
      precision--;
   if ((precision == 2) && (col > 11))
      precision--;
   if ((precision == 1) && (col > 12))
      precision--;
      
   uint16_t vgen_BCD = calc_vgen_BCD(get_vgen(0));

   uint8_t tens = '0' + (uint8_t)(vgen_BCD >> 12)/10;
   uint8_t volts = '0' + (uint8_t)(vgen_BCD >> 12)%10;
   uint8_t tenths = '0' + (uint8_t)((vgen_BCD & 0x0F00) >> 8);
   uint8_t hundredths = '0' + (uint8_t)((vgen_BCD & 0x00F0) >> 4);
   uint8_t thousandths = '0' + (uint8_t)(vgen_BCD & 0x000F);
  
   switch (precision)
   {
      case 0:
         if (tenths > '5')
            volts++;
         break;
      
      case 1:
         if (hundredths > '5')
            tenths++;
         if (tenths > '9' )
         {
            volts++;
            tenths = '0';
         }
         break;

      case 2:
         if (thousandths > '5')
            hundredths++;
         if (hundredths > '9')
         {
            tenths++;
            hundredths = '0';
         }
         if (tenths > '9')
         {
            volts++;
            tenths = '0';
         }         
         break;
   }
  
   LCD_place_char (tens, lcd_line, col++);
   LCD_place_char (volts, lcd_line, col++);
   
   if (precision)
   {
      LCD_place_char ('.', lcd_line, col++);
      LCD_place_char (tenths, lcd_line, col++);
       
      if (precision > 1)
         LCD_place_char (hundredths, lcd_line, col++);
      if (precision > 2)
         LCD_place_char (thousandths, lcd_line, col);
   }
}    

void LCD_display_shutdown_cause(int1 lcd_line)           
{                   
   if (!global_lcd_enabled) return;    
   
   strcpy (global_temp_line_buff, "Shtdwn Cause:   ");
   global_temp_line_buff[14] = hi2asc(global_previous_shutdown_cause);
   global_temp_line_buff[15] = lo2asc(global_previous_shutdown_cause);
   if (lcd_line == 0) LCD_line1(global_temp_line_buff);
   else LCD_line2(global_temp_line_buff);
}
                                    
void LCD_update_diplay(void)           
{                                      
   // char for displaying mote state   
   char state_char = ' ';                
      
   switch(global_lcd_page_number)      
   {    
      // display screen 1    
      case 1:                                   
         // This is where you should put the stuff that doesn't need to be updated all the time
         // last update was skipped, clear screen and fill screen        
         if (global_skip_lcd_update_count == 1)
         {
            LCD_clear();                                        
            global_skip_lcd_update_count = 0;  
            strcpy (global_temp_line_buff, "v      r    t   ");
            LCD_line1(global_temp_line_buff);           
            strcpy (global_temp_line_buff, "c      q    a   ");
            LCD_line2(global_temp_line_buff);   
         }                                                 
                               
         // This is where you put what you want on the screen                                       
         // v indicates Valve position
         // r indicates speed of rotation Rpm
         // t indicates message Type
         // c indicates real-time Clock in seconds
         // q indicates message Queue location
         // a indicates Attempt_num
         LCD_place_uint16(global_valve_position, 0, 1, 5);          
         LCD_place_uint16(global_current_rpm,0,8,3);           
         LCD_place_uint32(global_rtc_time, 1,1,5);                     
         //LCD_place_uint16(global_xdcr_output, 1, 8, 5);                
         //LCD_place_uint32(global_utc_time, 1,1,10);          
                                                                                     
         LCD_place_uint8(global_current_message_queue_location, 1, 8, 3);
         if (global_current_message_queue_location != 255)
         {                             
            LCD_place_uint8(global_message_queue[global_current_message_queue_location].message_type,0,13,3);
            LCD_place_uint8(global_message_queue[global_current_message_queue_location].attempt_num,1,13,3);
         }   
               
      break; 
      // display screen 2 
      case 2:              
         // This is where you should put the stuff that doesn't need to be updated all the time
         // last update was skipped, clear screen and fill screen        
         if (global_skip_lcd_update_count == 1)  
         {                                        
            LCD_clear();                                        
            global_skip_lcd_update_count = 0;
            strcpy (global_temp_line_buff, "m       n     * ");
            LCD_line1(global_temp_line_buff);           
            strcpy (global_temp_line_buff, "p       .       ");
            LCD_line2(global_temp_line_buff);   
         }                                                      
                   
         // This is the place where you put what you want onto the screen
         // m indicates Mote ID (hex)
         // n indicates Net ID (hex)
         // * indicates mote state (Booting, Idle, Searching,
         //                         Negotiating, Connected,
         //                         Operational, Disconnected,
         //                         Radio test, Promiscuous Listen)
         // p indicates sprinkler Position (2 digit pivot#, 3 digit location)
         // . is the decimal pt in the battery voltage (2 decimal precision)
         // final 4 places are for Firmware Rev 1st char A-F (e.g. A123)
         // last four characters of line 2 is the hex firmware
         //  revision, e.g. D308   
         
         // place the mac address on the LCD
         global_temp_line_buff[0] = hi2asc(macaddr_rcvd[5]);
         global_temp_line_buff[1] = lo2asc(macaddr_rcvd[5]);
         global_temp_line_buff[2] = hi2asc(macaddr_rcvd[6]);
         global_temp_line_buff[3] = lo2asc(macaddr_rcvd[6]);
         global_temp_line_buff[4] = hi2asc(macaddr_rcvd[7]);
         global_temp_line_buff[5] = lo2asc(macaddr_rcvd[7]);
         LCD_place_text(global_temp_line_buff,0,1,6); 
                               
         // place the network id on the LCD                       
         global_temp_line_buff[0] = hi2asc(make8(network_id_rcvd,1));
         global_temp_line_buff[1] = lo2asc(make8(network_id_rcvd,1));
         global_temp_line_buff[2] = hi2asc(make8(network_id_rcvd,0));
         global_temp_line_buff[3] = lo2asc(make8(network_id_rcvd,0));
         LCD_place_text(global_temp_line_buff,0,9,4);       
         
         // put the mote state on the LCD      
         switch (global_mote_state)
         {              
            case MOTESTATE_INIT:
               // booting
               state_char = 'B';
               break;  
            case MOTESTATE_IDLE:
               state_char = 'I';
               break; 
            case MOTESTATE_SEARCHING:
               state_char = 'S';
               break;     
            case MOTESTATE_NEGOTIATING:
               state_char = 'N';
               break;
            case MOTESTATE_CONNECTED:
               state_char = 'C';
               break; 
            case MOTESTATE_OPERATIONAL:
               state_char = 'O';
               break; 
            case MOTESTATE_DISCONNECTED:
               state_char = 'D';
               break;  
            case MOTESTATE_RADIOTEST:
               state_char = 'R';
               break;  
            case MOTESTATE_PROMISCUOUS:
               state_char = 'P';
               break; 
            default:
               state_char = '*';
               break;   
         }                     
         LCD_place_char(state_char, 0, 15);          
                 
         // put the sprinkler position??? on the LCD                             
         strcpy(global_temp_line_buff, "***");
         if (state_char == 'O')
            LCD_place_uint8((uint8_t)(network_id_rcvd - NETWORK_ID_DEF), 1, 1, 2);
         else
            LCD_place_text(global_temp_line_buff,1,1,2);
                                      
         // put the sprinkler number on the LCD
         if (global_sprinkler_num > 999)
            LCD_place_text(global_temp_line_buff,1,3,3);
         else
            LCD_place_uint16(global_sprinkler_num, 1, 3, 3);
         
         // put the battery voltage on the LCD
         LCD_display_batt_voltage(1, 7, 2);                
         
         // put the firmware version on the LCD
         strcpy(global_temp_line_buff, global_firmware_version_string);      
         LCD_place_text(global_temp_line_buff,1,12,4);  
                                                            
         break;        
      
      // another screen
      case 3:
       
         if (global_skip_lcd_update_count == 1)  
         {                                        
            LCD_clear();                                        
            global_skip_lcd_update_count = 0;
            strcpy (global_temp_line_buff, "br    ch    mp  ");
            LCD_line1(global_temp_line_buff);           
            strcpy (global_temp_line_buff, "r   /    vg  .  ");
            LCD_line2(global_temp_line_buff);   
         }                                                      
                   
         // This is the place where you put what you want onto the screen
         // br indicates BRaking duty cycle (0-400)
         // ch indicates CHarging duty cycle (0-400)
         // mp indicates MPpc setting (2-digit HEX)
         // r.../... indicates measured RPM / target RMP
         // vg indicates detected voltage from generator 00.00
         
         LCD_place_uint16(global_brake_duty,0,2,3); 
         LCD_place_uint16(global_charge_duty,0,8,3);
         LCD_place_char(hi2asc(global_mppc_value),0,14);
         LCD_place_char(lo2asc(global_mppc_value),0,15);
         
         LCD_place_uint16(global_current_rpm,1,1,3);
         LCD_place_uint16(global_rpm_set_value,1,5,3);
         
         // put the vgen voltage on the LCD

         LCD_display_vgen(1, 11, 2);

      break;
      
      // extra screen
      case 4:
         if (global_skip_lcd_update_count == 1)
         {                                        
            LCD_clear();                                        
            global_skip_lcd_update_count = 0;
            strcpy (global_temp_line_buff, "system state    ");
            LCD_line1(global_temp_line_buff);           
            strcpy (global_temp_line_buff, "                ");
            LCD_line2(global_temp_line_buff);   
         }    
            LCD_place_uint8(read_system_state(),0,13,3); 
            LCD_place_uint32(global_last_rpm_value_time,1,0,10); 
         break;   
                                                                      
      // if somehow global_lcd_page_number get incremented above the last page, clear the LCD   
      default:
         LCD_clear();               
      break;    
   }                                             
}      

                                                                                                  
void XDCR_init(void)
{
   i2c_start();

   global_xcdr_type1_enabled = !i2c_write(XDCR_ADDRESS);    // (0x48) WRITE LCD address?
   if (!global_xcdr_type1_enabled)
   {
      i2c_stop();                                    
      delay_cycles(255);
      output_low(XDCR_PWR);  
      return;                            
   }                             
   //i2c_write(0b00000000);                                   
   i2c_write(0x00); 
   i2c_stop();           
   // set up the ADC here
}    
              
void adc_XDCR_init(void)
{   
   uint8_t n;
   uint16_t xdcr_output_sum = 0;
   
   // turn on pullup resisitor on xdcr line (so you can check if it's connected)                    
   port_b_pullups(PORTB_PULLUPS_WITH_XDCR);  
   
   // turn on XDCR and wait 100ms for the adc to settle (really only 25ms or so is needed)
   output_high(XDCR_PWR);      
   setup_T0_int(T0_100MS);                                    
   while(!TMR0IF);          
             
   // turn on ADC                  
   ADON = TRUE;               
                                  
   // set adc to V_EXTRA_IO_1 (pin 7 on header going "across river")
   set_adc_channel(ADC_XDCR);      

   // sum up 8 samples from the adc (.3 ms/sample @ 250Khz clock)                                                                                                                                                                           
   for (n = 0; n < 8; n++)
   {                                                                                                                                                                                                        
      xdcr_output_sum += read_adc();
   }                    
       
   // turn off XDCR power
   output_low(XDCR_PWR); 
                                                   
   // turn off adc                                                                                         
   ADON = FALSE;            
                                  
   if (xdcr_output_sum < XDCR_ADC_OFF_LIMIT)
   {
      global_xcdr_type1_enabled = TRUE;   
   }                                            
                        
   // return pullups to normal state (without testing if XDCR is there)                           
   port_b_pullups(PORTB_PULLUPS);        
}                                    

uint16_t XDCR_measure(void)                                         
{                                         
   uint32_t temp_u32;                                          
    
   i2c_start();
                                                    
   // (0x49) READ ADC address?                
   i2c_write(XDCR_ADDRESS + 1);   
                                    
   temp_u32 = make32(i2c_read(TRUE), i2c_read(TRUE), i2c_read(FALSE), 0);
//   LCD_clear();                    
//   LCD_place_uint32(temp_u32,0,0,10);  
//   global_skip_lcd_update_count = 5;              
                                     
   i2c_stop();                                                         
                                           
   if ((make8(temp_u32,3) & 0xC0) == 0xC0) 
   {                                
      return 0xFFFF;        // saturated top of scale
   }                                                                                    
   else if (!(make8(temp_u32,3) & 0xC0))                         
   {                                                                          
      return 0;             // saturated bottom of scale
   }            
   else                                    
   {                                                                                                                                                                         
      temp_u32 = temp_u32<<2;           // ignore "Bit23 Sig" and "Bit22 MSB"
//      LCD_clear();                                    
//      LCD_place_uint8(make8(temp_u32,1),1,0,3);
//     global_skip_lcd_update_count = 5;                                                                                  
      return make16(make8(temp_u32,3), make8(temp_u32,2));              
   }                                                              
}

uint16_t adc_XDCR_measure()    
{                          
   uint8_t n;
   uint16_t adc_value = 0;                  
   
   // if the XDCR isn't connected, return 0 (really, you should never see 0 with adc noise)                     
   if (!global_xcdr_type1_enabled) return 0;       
   
   // turn on ADC        
   ADON = TRUE;               
                                  
   // set adc to V_EXTRA_IO_1 (pin 7 on header going "across river"
   set_adc_channel(ADC_XDCR);      
                                    
   // sum up 8 samples from the adc (.3 ms/sample @ 250Khz clock)                                                                                                                                                                             
   for (n=0; n<8; n++)
   {                                                                                                                                                                                                        
      adc_value += read_adc();
   }                    
                     
   // turn off XDCR power
   output_low(XDCR_PWR); 
               
   // turn off adc                                                                                         
   ADON = FALSE;            
                                                 
   // store XDCR output globally (not shifted)                                              
   global_xdcr_output = adc_value;        
                   
   // return the voltage shifted by 3 bits to scale from (0-1023) * 8 to 0-65535 or close enough    
   return global_xdcr_output;                    
}


void  fixture_init()         
{
   uint8_t  resbyte;              
   resbyte = 0;                                                                                             
   
   safeI2C_start();        
   
   resbyte += i2c_write(FIXTURE_DAC_ADDRESS);                 // FIXTURE DAC ADDRESS

   resbyte += i2c_write (0x60);                // use internal reference
   resbyte += i2c_write (0x00);                // set DAC to 0
   resbyte += i2c_write (0x00);                // low nibble not used (12 bit DAC)
   
   if (resbyte != 0) global_fixture_enabled = TRUE;
   i2c_stop();
   
   set_fixture_dac(NO_FIXTURE);
}                             
