//pic.h
void fosc_pri(void);
void fosc_pri_ISR(void);
void fosc_4m(void);
void fosc_1m(void);
void fosc_250k(void);
void fosc_250k_ISR(void);
void fosc_31250(void); 
void fosc_31250_ISR(void); 
void fosc_sec(void);
void osc_init(void);
void setup_T0_int(uint16_t value);
void setup_T2_int(uint8_t value);
void setup_T4_int(uint8_t value);
void periph_init(void);
void vars_init(void);     
void deep_sleep();
void start_rtc(void);
void safeI2C_start(void);
void safeI2C_write(uint8_t value); 
void setup_rb4_int(void);                                  
void write_system_state(uint8_t system_state_to_change_to); 
uint8_t read_system_state(void); 
                                                             
//eeprom.h
uint8_t read_ee1(uint16_t source);
uint16_t read_ee2(uint16_t source);
uint32_t read_ee4(uint16_t source);
void write_ee1(uint16_t dest, uint8_t dat8);
void write_ee2(uint16_t dest, uint16_t dat16);
void write_ee4(uint16_t dest, uint32_t dat32);
void store_all_eeprom_values(void);  
void read_all_eeprom_values(void);                  
void store_vcal_eeprom_values(void);  
void read_all_eeprom_variables(void);                              

//periph.h (LCD + XDCR)
//LCD declerations
void LCD_init(void);
void LCD_line1(unsigned char *text);
void LCD_line2(unsigned char *text);
void LCD_place_text(unsigned char *text, int1 row, uint8_t col, uint8_t length);
void LCD_place_char(char ascii, int1 row, uint8_t col);
void LCD_place_uint8(uint8_t num, int1 row, uint8_t col, uint8_t length);
void LCD_place_uint16(uint16_t num, int1 row, uint8_t col, uint8_t length);
void LCD_place_uint32(uint32_t num, int1 row, uint8_t col, uint8_t length);
void LCD_clear(void);
void LCD_clear_line1(void);
void LCD_clear_line2(void);
void LCD_position(int position);
void LCD_char(char ascii);
void LCD_shutdown(void);
void LCD_startup_splash(void);
void LCD_display_priority_time_stacks(void);
void LCD_display_priority_time_stacks_2(void);
void LCD_display_priority_queue(void);   
void LCD_display_message_queue(void); 
void LCD_display_battery_voltage(int1 lcd_line);
void LCD_display_batt_voltage(int1 lcd_line, uint8_t col, uint8_t precision);
void LCD_display_vgen(int1 lcd_line, uint8_t col, uint8_t precision);
void LCD_update_diplay(void);     
void XDCR_init(void);  
void adc_XDCR_init(void);
uint16_t XDCR_measure(void); 
uint16_t adc_XDCR_measure(void);    
void fixture_init(void);

//dust.h
void mote_init(void);
void mote_reset(void);
uint8_t mote_sleep(void);
void search_for_strongest(void);
void process_advert(void);
uint8_t initiate_join(void);
void mote_state_check(void);
void mote_temp_check(void);
void mote_time_update(void);
void init_msgSEQ(void);
void deal_with_mote_ISR(void);
uint8_t get_dustmsg_ISR(void);
void  dust_hdlc_pkgen(uint8_t* prefix, uint8_t size, uint8_t* buf, uint8_t size_buf);
void  dust_hdlc_pkgen_ISR(uint8_t* prefix, uint8_t size, uint8_t* buf, uint8_t size_buf);
uint8_t dust_hdlc_pksend();
uint8_t dust_hdlc_pksend_ISR();
uint8_t dust_hdlc_pkdecode_ISR(uint8_t bytes_to_decode);
uint16_t fcs_fcs16(uint16_t fcs, uint8_t data);
uint16_t fcs_fcs16_ISR(uint16_t fcs, uint8_t data);
static uint16_t fcs_calc(uint8_t* p, uint16_t size);
static uint16_t fcs_calc_ISR(uint8_t* p, uint16_t size);
uint8_t deal_with_packet(void);
void prepare_prefix_and_payload(uint8_t payload_buf_length);
void prepare_prefix_and_payload_ISR(uint8_t payload_buf_length);
void generate_message_and_send();
void generate_message_and_send_ISR();
void send_full_report(uint8_t command_id_in_response_to, uint16_t msg_seq);
void send_health_report(uint8_t command_id_in_response_to, uint16_t msg_seq);
void send_valve_report(uint8_t command_id_in_response_to, uint16_t msg_seq);
void send_sprinkler_cmd_response(uint8_t command_id_in_response_to, uint16_t msg_seq);
void send_update_sp_num_response(uint8_t command_id_in_response_to, uint16_t msg_seq);
void send_read_eeprom_response(uint8_t command_id_in_response_to, uint16_t msg_seq, uint16_t eeprom_location, uint32_t eeprom_data); 
void send_pressure_xdcr_msg(uint8_t command_id_in_response_to, uint16_t msg_seq); 
void send_pressure_xdcr_msg_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq); 
void send_ack(uint8_t command_id_in_response_to, uint16_t msg_seq);
void send_nack(uint8_t command_id_in_response_to, uint16_t msg_seq, uint32_t nack_bitfield);
void send_health_report_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq);
void send_valve_report_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq);
void send_join_msg_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq);
void send_gps_point_msg_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq);
void send_error_msg_ISR(uint8_t command_id_in_response_to, uint16_t msg_seq);

//util.h
void flash_leds(uint8_t num_of_flashes);
void flash_leds_ISR(uint8_t num_of_flashes);
void happy_lites(void);
char *word_to_4dig_new (uint16_t xword);
uint16_t convert_rpm_to_period(uint16_t rpm);
uint16_t convert_period_to_rpm(uint16_t period);
uint16_t convert_period_to_rpm_ISR(uint16_t period);
uint8_t  hi2asc(uint8_t xbyte);
uint8_t  lo2asc(uint8_t xbyte);

//stacks_queues.h
void clear_priority_queue();
uint8_t pop_priority_queue();
int1 push_priority_queue(uint8_t new_priority_queue_item);
void push_priority_queue_ISR(uint8_t new_priority_queue_item);
void push_priority_queue_ISR2(uint8_t new_priority_queue_item);
void clear_time_queue();
int1 pop_time_queue(); 
void pop_time_queue_ISR();
int1 push_time_queue(uint32_t time_to_execute, uint8_t priority);
void push_time_queue_ISR(uint32_t time_to_execute, uint8_t priority);
void allocate_command_queue(void);
void clear_command_queue(void);
int1 push_command_queue(struct CommandQueueItem new_command_queue_item);
void clear_message_queue(void);
void clear_sprinkler_queue(void);
void push_sprinkler_queue(uint32_t start_time,uint32_t end_time,uint16_t vlv_open_amount,uint16_t spin_rate,uint8_t zone_num, uint8_t control_loop_mechanism);
void pop_sprinkler_queue_ISR(void);
void pop_message_queue_and_send_ISR(void);
void push_message_queue(uint8_t message_type);
void push_message_queue_ISR(uint8_t message_type);
void remove_message_queue_item(uint16_t msg_seq);
void remove_message_queue_item_ISR(uint16_t msg_seq);
                                            
//control.h
void setup_gen_rpm(void);
uint16_t calc_gen_rpm(void);                          
int1  is_brake_duty_valid(uint16_t value);
int1  is_charge_duty_valid(uint16_t value);
int1  is_mppc_valid(uint8_t value);
int1  is_rpm_setting_valid(uint16_t value);
void set_brake_duty(uint16_t value);
void set_charge_duty(uint16_t value);                                 
void set_mppc(uint8_t value);
void set_fixture_dac(uint16_t dac_value);
void adjust_brake_for_rpm(uint16_t rpm_set_value);
void adjust_brake_for_vgen(uint16_t vgen_set_value);
void adjust_fixture_for_rpm(uint16_t fixture_set_value);
void setup_control_update_speed_vgen();
void adjust_mppc(void);       
void update_control_loop(void);
void stop_current_spinkler_setting(void);
void stop_current_spinkler_setting_ISR(void);

//battery.h
uint16_t get_vgen(int1 charge_state);
uint16_t get_vbatt(int1 charge_state);
uint16_t get_vbatt_ISR(int1 charge_state);
uint32_t calc_vbatt(uint16_t vref_count);
uint16_t calc_vbatt_BCD(uint16_t vref_count);
uint16_t calc_vgen_BCD(uint16_t vref_count);
void check_and_deal_with_battery(void);

//valve.h
void  cl_valve(void);
void  op_valve(void);
void  cl_valve_timed(uint32_t num_ints);
void  op_valve_timed(uint32_t num_ints);
void  comparator_setup(void);
uint16_t calc_valve_movement_time(uint16_t current_location, uint16_t destination);
void  set_comp_dac_level(void);
void  set_comp_dac_level_ISR(void);
int1  is_valve_position_valid(uint16_t value);
int1  is_valve_value_full_open_or_close(uint16_t value);
