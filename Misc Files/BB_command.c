/* Ideas on how to implement BB sprinkler command scheme
 * 
 * Brian Lewis
 */

#define MINIMUM_VLV_DWELL_TIME 300  //minimum amount of time spend in one valve position in seconds
#define DWELL_PERIOD_INCREMENT 30	//number of seconds to increment overall period of zone time when optimizing, smaller = more divides
#define MAX_NUM_OF_ZONES 4	//number of zones we are going to allow 4 gies a command_stack size of 25 bytes

// struct of individual sprinkler's parameters (for a single zone)
struct Sprinkler_parameters {
	uint8_t  duty_cycle;
	uint8_t  vlv_open_percent;
	uint8_t  spin_rate;
	}
	
// struct of singular sprinkler command
struct Command_stack_item {
	uint32_t start_time;
	uint32_t end_time;
	uint8_t num_of_zones;
	struct Sprinkler_parameters[MAX_NUM_OF_ZONES];
	uint32_t total_period;
}	 

struct Sprinkler_stack_item{
	uint32_t start_time;
	uiint8_t vlv_open_percent;
	uint8_t spin_rate;
	uint8_t zone_number;
	int1 new_sprinkler_cmd;
}

struct Sprinkler_stack_item sprinkler_stack[2];

struct Command_stack_item command_stack[2];

 
uint32_t calculate_total_dwell_period(struct Command_stack_item sprinkler_cmd);
void push_to_command_stack();
void check_sprinkler_stack();
struct Sprinkler_stack_item get_next_sprinkler_stack_item(uint8_t current_zone, struct Command_stack_item *current_command_stack);

 
  uint32_t calculate_total_dwell_period(struct Command_stack_item sprinkler_cmd)
  {
	/* Calculating the period of the entire set of modes ))
	* |-10%-|------20%------|----45%----|--25%--|  (the entire time to get through the set of modes)
	*
	* Basically a simple optimization/minimization problem
	* minimize these two equations 
	* the "overlap" due to asyncronsy of the start/end time and the total_dwell_period of all modes
	* (end_time - start_time) % total_dwell_period
	* the smallest dwell time minus the minimum dwell time possible
	* total_dwell_period * smallest_duty_cycle - MINIMUM_VLV_DWELL_TIME
	*/
	
	uint32_t total_period_time; // total time for sprinkler operation in seconds
	uint32_t total_dwell_period; // total time for single completion of changing through the dwell positions 
	uint32_t total_dwell_period_remainder, previous_total_dwell_period_remainder; //used for the optimization problem
	uint32_t maximum_smallest_dwell_period; //
	uint8_t nn;
	uint8_t smallest_dwell_dc = 101; // used for finding smallest dc in set of dwell parameters
	
	total_period_time = sprinkler_cmd.end_time - sprinkler_cmd.start_time;
	
	/* Figure out the smallest duty cycle in the set that isn't 0
	 * Preset minimum dc at 102%
	 */
	smallest_dwell_dc = 102;
	for (nn = 0; nn < NUM_OF_DWELL_POSITIONS; nn++)
	{
		// if the dc isn't 0 and less than the stored min dc, replace the min dc
		if ((sprinkler_cmd.Sprinkler_parameters[nn].duty_cycle != 0) && (sprinkler_cmd.Sprinkler_parameters[nn].duty_cycle < smallest_dwell_dc))
		{
			smallest_dwell_dc = sprinkler_cmd.Sprinkler_parameters[nn].duty_cycle;
		}
	}
	
	/* Somewhat cumbersome operation for minimizing 	
	 * total_period_time % total_dwell_period
	 * total_dwell_period * smallest_duty_cycle - MINIMUM_VLV_DWELL_TIME
	 *
	 * keeps track of the modulus of total_period_time % total_dwell_period
	 * starts with total_dwell_period 
	 */
	maximum_smallest_dwell_period = ((total_period_time * smallest_dwell_dc) + DWELL_PERIOD_INCREMENT);
	previous_smallest_dwell_period_remainder = maximum_smallest_dwell_period;
	for (smallest_dwell_period = MINIMUM_VLV_DWELL_TIME; smallest_dwell_period <= maximum_smallest_dwell_period; smallest_dwell_period += DWELL_PERIOD_INCREMENT)
	{
		total_dwell_period_remainder = total_period_time % total_dwell_period;
		if (total_dwell_period_remainder > previous_total_dwell_period_remainder)
		{
			return (total_period_time - DWELL_PERIOD_INCREMENT);
		}
		else
		{
			previous_total_dwell_period_remainder = total_dwell_period_remainder;
		}
	return 0; //indicating an error that no optimization was found (did not converge on a solution)
  }
  
  
  void push_to_command_stack()
  {	
	//decode message from mote
	//calculate_total_dwell_period
	//add to command stack in sorted fashion
	//if stack is full or calculate_total_command_period fails, tell mote
	//call check_sprinkler_stack()
  }
  
  void check_sprinkler_stack()
  {  
	//if CS1->SS0, replace, clear SS1, set new_sprinkler_cmd bit
	  if sprinkler_stack
	//else SS1->SS0, replace, clear SS1
	//if newbit in SS0, pop CS0, clear new_sprinkler_cmd bit
	//if SS1 = empty replace with next_ss() (comes from current CS0, regardless if valid)
	//if CS1 -> SS1, replace, set new_sprinkler_cmd bit
  }
  
  struct Sprinkler_stack_item get_next_sprinkler_stack_item(uint8_t current_zone, struct Command_stack_item *current_command_stack)
  /*
   *
   */
  {
	 //increment the current mode
	 current_mode++;
	 if (current_mode == *current_command_stack.num_of_zones)
	 {
		//if at last zone, increment to zone 0 and increase the start time of the current command
		 current_zone = 0;
		 *current_command_stack.start_time+=current_command_stack.total_period();
	 }
		//	
		 
  }
  
  
  