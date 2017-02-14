/*
 * $HeadURL: https://svn/appeng/trunk/dev/src/api/dust_dn2500.h $
 * $Id: dust_dn2500.h 261 2010-02-19 23:34:29Z kottaway $
 *
 * Copyright (c) 2006, Dust Networks, Inc
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Dust Networks, Inc nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY DUST NETWORKS, INC AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL DUST NETWORKS, INC AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * This documentation might include technical inaccuracies or other errors. 
 * Corrections and improvements might be incorporated in new versions of the 
 * documentation. Dust Networks does not assume any liability arising out of 
 * the application or use of any products or services and specifically disclaims 
 * any and all liability, including without limitation consequential or incidental damages.
 *
 * 
 *
 * Dust Networks reserves the right to make corrections, modifications, enhancements,
 * improvements, and other changes to its products or services at any time and to 
 * discontinue any product or service without notice. Customers should obtain the 
 * latest relevant information before usage and should verify that such information 
 * is current and complete.
 */
 
 // NOTE THAT THIS FILE WAS UPDATED TO INCLUDE THE SEARCH capability
 //  now included in the Eterna parts (JG June 2013)

/* Max user data size.  This limit is specific to the 
 * amount of data that can be sent via radio to/from the Manager.
 * This is not an HDLC limit which might include byte stuffing.
 */
#define MAX_USER_DATA_SIZE       94

// prior to encoding for transmission
// cmd(1), len_pkt(1), flags(1), destaddr(2), svc_index(1), appdomain(1),
// priority(1), reserved(2), sequence_num(1), len_data(1), MAX_USER_DATA_SIZE
#define MAX_SEND_REQUEST         MAX_USER_DATA_SIZE + 12

// include now the delimiters (2), the FCS (w/ byte stuffing could be 4),
//  of the 12 bytes added above, there may be an extra 2 due to byte stuffing
//  and the message can be MAX_USER_DATA_SIZE*2 worst case!
#define MAX_SEND_BUFFER          MAX_USER_DATA_SIZE*2 + 2 + 4 + 12 + 2
// This adds up to 208!

// after decoding the received message
// cmd(1), len_pkt(1), flags(1), sourceaddr(2), sequence_num(1), len_data(1), 
// MAX_USER_DATA_SIZE
#define MAX_DATA_RCVD            MAX_USER_DATA_SIZE + 7

// prior to decoding, raw rcv data
// include delimiters (2), the FCS (4 max), of the 7 bytes above there may be
//  an extra stuffed byte, and the message can be MAX_USER_DATA_SIZE*2!
#define MAX_RCV_BUFFER           MAX_USER_DATA_SIZE*2 + 2 + 4 + 7 + 1
// This adds up to 202
 
#define MAX_JOINKEY_SIZE         16

#define MAX_BURST_MESSAGE        3

#define DESTADDR_GATEWAY         0xF981
#define DESTADDR_SWAPPED         0x81F9
#define DESTADDR_HI              0xF9
#define DESTADDR_LO              0x81

// Microprocessor to Mote Commands
#define CMD_SET            0x01     // Sets param on the mote.
#define CMD_GET            0x02     // Gets param from mote
#define CMD_SETNV          0x03     // Stores param in the mote’s NV
#define CMD_GETNV          0x04     // Retrieves param from the mote’s NV
#define CMD_SEND           0x05     // Packet destined for the network.
#define CMD_JOIN           0x06     // Requests that mote attempt to join
#define CMD_DISCONNECT     0x07     // Requests that mote disconnect
#define CMD_RESET          0x08     // Resets mote.
#define CMD_SLEEP          0x09     // Shuts down periphs & puts mote to sleep
#define CMD_TESTRADIOTX    0x0B     // Initiate a series of packet transmissions.
#define CMD_TESTRADIORX    0x0C     // Test radio reception for a specified chan.
#define CMD_CLEARNV        0x10     // Resets the mote’s NV to factory default..
#define CMD_SEARCH         0x11     // new Search for networks capability
                                    // (Eterna parts)

// Mote to Microprocessor Commands
#define CMD_TIME           0x0D     // Time and mote state information.
#define CMD_SERVICE        0x0E     // Notifies micro of changes in service stat.
#define CMD_EVENT          0x0F     // Notifies micro of new has occurred.
#define CMD_ADVERT         0x12     // Notifies micro that an adv has been rcvd.
#define CMD_DATA           0x81     // Packet from the network to the micro.


// Result Codes
#define RC_OK              0x00     // Operation was successfully completed.
#define RC_EXE_ERR         0x01     // Reserved.
#define RC_PARAM_ERR       0x02     // Reserved.
#define RC_BUSY            0x03     // Operation on this service is in progress.
#define RC_INVALID_LEN     0x04     // Invalid packet length.
#define RC_INV_STATE       0x05     // Invalid mote state for command.
#define RC_UNSUPPORTED     0x06     // Command not supported for hardware.
#define RC_UNKNOWN_PARAM   0x07     // Unknown parameter value.
#define RC_UNKNOWN_CMD     0x08     // Unknown command.
#define RC_WRITE_FAIL      0x09     // Write failure.
#define RC_READ_FAIL       0x0A     // Read failure.
#define RC_LOW_VOLTAGE     0x0B     // Voltage check failed.
#define RC_NO_RESOURCES    0x0C     // No resources (max # services active)
#define RC_INCOMPLETE_JOIN 0x0D     // Incomplete join information.
#define RC_NOT_FOUND       0x0E     // Parameter not found.
#define RC_INVALID_VALUE   0x0F     // Invalid value.


// Parameter Types
#define PARAM_MACADDR      0x01     // setNV, getNV
#define PARAM_JOINKEY      0x02     // setNV
#define PARAM_NETWORKID    0x03     // setNV, getNV
#define PARAM_TXPOWER      0x04     // set, setNV, getNV
#define PARAM_POWERINFO    0x05     // setNV, getNV
#define PARAM_JOINDUTY     0x06     // set
                                    // Allows micro to control  ratio of listen
                                    //  to sleep time during network search
#define PARAM_BATTLIFE     0x07     // set
                                    // Allows micro to update the remaining 
                                    //  battery life that mote reports to mgr
#define PARAM_SERVICE      0x08     // set. get 
                                    // Allows device to initiate service request
                                    //  or update an existing svc
#define PARAM_SPECIAL1     0x08     // setnv for special join timing, 1st param
#define PARAM_SPECIAL2     0x09     // setnv for special join timing, 2nd param
#define PARAM_EVENTMASK    0x0B     // set
                                    // Allows micro to disable events that may 
                                    //  be sent in the mote's event notification
#define PARAM_MOTEINFO     0x0C     // get
                                    // Gets static info about mote hw and sw
#define PARAM_NETWORKINFO  0x0D     // get
                                    // Gets mote's current net-related params
#define PARAM_MOTESTATUS   0x0E     // get
                                    // Gets  mote's state and 
                                    //  frequently changing information.
#define PARAM_TIME         0x0F     // get
#define PARAM_CHARGE       0x10     // get
                                    // Gets  mote's charge consumption.
#define PARAM_RADIOSTATSRX 0x11     // get
                                    // Gets results of the mote radio rx test
#define PARAM_TTL          0x13     // get
                                    // Gets "time to live"
#define PARAM_OTAPLOCK     0x15     // setNV, getNV
                                    // Over-the-air-programming lockout control


// re get service: Service State - note Bit7=0 for no service pending
//                                          Bit7=1 for service pending
#define SVCSTATE_INACTIVE        0x00
#define SVCSTATE_ACTIVE          0x01
#define SVCSTATE_REQUESTED       0x02

// re get/set service: Service Request Flags Bitmask Values
#define SVCFLAGS_SOURCE          0x01     // Mote is source of data generated.
#define SVCFLAGS_SINK            0x02     // Mote is receiver of data.
#define SVCFLAGS_INTERMITTENT    0x04     // Intermittent traffic 
                                          //  (as opposed to regular reporting).

// Application domain values
#define APPDOMAIN_PUBLISH        0x00
#define APPDOMAIN_EVENT          0x01 
#define APPDOMAIN_MAINTENANCE    0x02 
#define APPDOMAIN_BLOCKXFER      0x03

// Mote Status values
#define MOTESTATE_INIT           0x00     // Mote is  booting.
#define MOTESTATE_IDLE           0x01     // Mote is accepting config cmds.  
                                          //  Upon receiving a join cmd, mote 
                                          //  moves into the Searching state.
                                          //  Idle state is low power state.
#define MOTESTATE_SEARCHING      0x02     // Mote’s rcvr is on with a 
                                          //  configurable duty cycle while
                                          //  mote is actively searching for net. 
#define MOTESTATE_NEGOTIATING    0x03     // Mote has detected a network 
                                          //  and is attempting to connect.
#define MOTESTATE_CONNECTED      0x04     // Mote has joined the net
#define MOTESTATE_OPERATIONAL    0x05     // Mote has links to both net mgr and
                                          //  gateway; ready to send data.
#define MOTESTATE_DISCONNECTED   0x06     // Mote is disconnected.
#define MOTESTATE_RADIOTEST      0x07
#define MOTESTATE_PROMISCUOUS    0x08
#define MOTESTATE_UNKNOWN        0xFF

// mote alarms bit masks
#define ALARMS_NVERROR           0x01        // Non-volatile error.
#define ALARMS_LOW_VOLTAGE       0x02        // Mote voltage too low.
#define ALARMS_OTP               0x04        // One time programmable error


// re get POWERINFO: power source values
#define PWRSOURCE_LINE           0x00
#define PWRSOURCE_BATTERY        0x01
#define PWRSOURCE_SCAVENGE       0x02

// re set/get txpower
#define TXPOWER_MIN              -2       // minimum power; PA off*/
#define TXPOWER_MAX              +8       // maximum power; PA on

// serviceIndication event codes
#define SVC_CREATED_UPDATED      0x00
#define SVC_DELETED              0x01
#define SVC_REJECTED             0x02
#define SVC_DELAYED_RESP         0x03

// serviceIndication Network Manager Codes
#define MGRCODE_SUCCESS             0     // no errors
#define MGRCODE_PARAM_TOOSMALL      4     // ERR: passed param too small
#define MGRCODE_BYTES_TOOFEW        5     // ERR: too few data bytes rcvd
#define MGRCODE_DEVICE_ERR          6     // ERR: device-specific cmd error
#define MGRCODE_SETNEAREST          8     // WARN: set to nearest possible val
#define MGRCODE_RESTRICTED          16    // ERR: access restricted
#define MGRCODE_BUSY                32    // ERR: network manager is busy 
#define MGRCODE_DELAYED_ATTEMPT     33    // ERR: delayed response initiated
                                          //      mgr will attempt to add links
#define MGRCODE_DENIED              65    // ERR: service request denied
#define MGRCODE_UNKNOWN_SVC         66    // ERR: unknown service flag
#define MGRCODE_UNKNOWN_APPDOMAIN   67    // ERR: unknown application domain
#define MGRCODE_UNKNOWN_NICKNAME    68    // ERR: unknown nickname

// re set batteryLife
#define PWRSTAT_NOMINAL             0x00
#define PWRSTAT_LOW                 0x01
#define PWRSTAT_CRIT_LOW            0x02
#define PWRSTAT_RECHARG_LOW         0x03
#define PWRSTAT_RECHARG_HIGH        0x04

#define BOOT_EVENT               0x01010101  // & with new_events_rcvd to
                                             //  determine if boot event
                                             //  occurred
#define ALARMS_EVENT             0x02020202
#define TIME_EVENT               0x04040404  // & with new_events_rcvd to
                                             //  determine if UTC time ref
                                             //  from mote has changed
#define JOIN_FAIL_EVENT          0x08080808
#define DISCONNECTED_EVENT       0x10101010
#define OPERATIONAL_EVENT        0x20202020  // mote has gateway to send data
#define CONFIG_EVENT             0x40404040


// EVENTMASK bit definitions
#define EVENTMASK_BOOT           0x01        // Mote booted up.
#define EVENTMASK_ALARMS         0x02        // Value of alarms field changed.
#define EVENTMASK_TIMECHANGE     0x04        // UTC time reference changed.    
#define EVENTMASK_JOINFAIL       0x08        // Join operation failed.
#define EVENTMASK_DISCONNECT     0x10        // Mote has disconnected.
#define EVENTMASK_OPERATIONAL    0x20        // Mote has gateway to send data.
#define EVENTMASK_CONFIG         0x40        // Write has changed mote config.

// 
// #define WRITEPROTECT_DISABLED    0x0         // bit 0 only
// #define WRITEPROTECT_ENABLED     0x1         // bit 0 only
// #define WRITEPROTECT_MASK        0x1

// FLAG bit masks & defines
//#define FLAG_TYPE_MASK          0x01
#define FLAG_TYPE_REQUEST           0x04     // from micro to mote re get/set
                                             //  and re join, reset, etc
#define FLAG_TYPE_REQUEST2          0x05
#define FLAG_SEND_REQUEST           0x04     // from micro to mote with send
                                             //  request only
#define FLAG_TYPE_TEST              0x00     // from micro to mote re
                                             //  testradiorx and testradiotx
#define FLAG_MOTE_INDICATION        0x00     // from mote to micro re
                                             //  indications: event, service,
                                             //  time
#define FLAG_TYPE_RESPONSE          0x01
#define FLAG_MOTECMD_RESPONSE       0x05     // from mote to micro after
                                             //  micro-initiated command
                                             //  e.g. join, reset, send
#define FLAG_MICROIND_RESPONSE      0x05     // from micro to mote after
                                             //  mote indication to micro
                                             //  time/service/event/data
#define FLAG_SET_RESPONSE           0x05     // from mote to micro after set
#define FLAG_GET_RESPONSE           0x05     // from mote to micro after get
                                             //  TEST THIS

#define FLAG_ID                     0x02
#define FLAG_ID_IGNORE              0x04
#define FLAG_END2END_RESPONSE       0x80
#define FLAG_TRANSPORT_TYPE_MASK    0x40


#define FLAG_NV_ONLY                   ~0x80
#define FLAG_NV_RAM                    0x80


// #define WRITE_PROTECT_MODE_OFF     0x00 // write allowed
// #define WRITE_PROTECT_MODE_ON      0x01 // write not allowed

// packet priorities
#define PRIORITY_LOW    0x00
#define PRIORITY_MED    0x01
#define PRIORITY_HI     0x02

// Pack on one byte boundaries because we "type cast" the data structures
//  to a destination buffer (array of bytes).

/* 
//Structure Declarations
#define DUST_DN2500_HEADER  \
    uint8_t cmd;     \
    uint8_t len_pkt; \
    uint8_t flags;


// A structure defined like this represents the first 4 bytes of 
//  send, join, disconnect, reset, sleep, testradioTX/RX, clearNV 
//  (cmd types 5,6,7,8,9,B,C,10)
#define DUST_DN2500_HEADER_RESPONSE    \
    uint8_t cmd;     \
    uint8_t len_pkt; \
    uint8_t flags;   \
    uint8_t result;
    
// A structure defined like this represents the first 4 bytes of all 
//  set, get, setNV, getNV responses (cmd types 1,2,3,4)
#define DUST_DN2500_PARAM_RESPONSE  \
    uint8_t cmd;     \
    uint8_t len_pkt; \
    uint8_t flags;   \
    uint8_t result;  \
    uint8_t parameter_type;        /* DUST_DN2500_CMD_[SET|GET]_PARAM_* 
*/

typedef union _mote_to_micro
// cmd types 1,2,3,4;  5,6,7,8,9,B,C,10;  D,E,F,81
// get/set; send/join/discon/reset/sleep/testradio/clrNV;  time/svc/event/data 
{
   char      dust_rcvd[MAX_DATA_RCVD];    // THIS is the buffer for unstuffed
                                          //  stripped received data
   struct _set_param_response
   // for set txPower, joinDutyCycle, batteryLife, eventMask
   // for setNV macAddress, networkID, txPower, joinKey, powerInfo, OTAPlockout
   //  and special1 and special2
   // Mote response to micro after most set and setNV commands
   {
      uint8_t     cmd;                 // CMD_SET or CMD_SETNV
      uint8_t     len_pkt;             // always 0x01  
      uint8_t     flags;               // expect FLAG_SET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_TXPOWER, PARAM_JOINTDUY, (set)
                                       // PARAM_BATTLIFE, PARAM_EVENTMASK (set)
                                       // PARAM_MACADDR, PARAM_NETWORKID, (setNV)
                                       // PARAM_TXPOWER, PARAM_JOINKEY, (setNV) 
                                       // PARAM_POWERINFO, PARAM_OTAPLOCK (setNV)
                                       // PARAM_SPECIAL1, PARAM_SPECIAL2
   }  set_param_response;
   
   struct _set_service_response
   // Mote respnse to micro after set service ONLY
   {
      uint8_t     cmd;                 // CMD_SET
      uint8_t     len_pkt;             // always 0x02
      uint8_t     flags;               // expect FLAG_SET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_SERVICE
      uint8_t     rem_svc_entries;
   }  set_service_response;
   
    struct _get_service_response 
   {
      uint8_t     cmd;                 // CMD_GET
      uint8_t     len_pkt;             // always 0x0B
      uint8_t     flags;               // expect FLAG_GET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_SERVICE
      uint8_t     svc_index;
      uint8_t     svc_state;           // SVCSTATE_*
      uint8_t     svc_flags;           // SVCRQST_*
      uint8_t     app_domain;          // APPDOMAIN_*
      uint16_t    destaddr;            // always 0xF981
      uint32_t    time;                // Period/latency: intermittent flag set
   } get_service_response;
   
   struct _get_moteinfo_response 
   {
      uint8_t     cmd;                 // CMD_GET
      uint8_t     len_pkt;             // always 0x11
      uint8_t     flags;               // expect FLAG_GET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_MOTEINFO
      uint8_t     api_version;
      uint8_t     serial_number[8];
      uint8_t     hw_model;
      uint8_t     hw_version;
      uint8_t     sw_major_ver;
      uint8_t     sw_minor_ver;
      uint8_t     sw_patch;
      uint16_t    sw_build;
   }  get_moteinfo_response;

   struct _get_networkinfo_response
   {
      uint8_t     cmd;                 // CMD_GET
      uint8_t     len_pkt;             // always 0x0D
      uint8_t     flags;               // expect FLAG_GET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_NETWORKINFO
      uint8_t     macaddr[8];
      uint16_t    mote_id;
      uint16_t    network_id;
   }  get_networkinfo_response;

   struct _get_motestatus_response
   {
      uint8_t     cmd;                 // CMD_GET
      uint8_t     len_pkt;             // always 0x0B
      uint8_t     flags;               // expect FLAG_GET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_MOTESTATUS
      uint8_t     mote_state;          // MOTESTATE_*
      uint8_t     mote_state_reason;   // Reserved -- ignore
      uint16_t    change_counter;
      uint8_t     number_of_parents;
      uint32_t    mote_alarms;         // ALARMS_*
      uint8_t     reserved;
   }  get_motestatus_response;

   struct _get_time_response
   {
      uint8_t     cmd;                 // CMD_GET
      uint8_t     len_pkt;             // always 0x10
      uint8_t     flags;               // expect FLAG_GET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_TIME
      uint32_t    utc_sec;
      uint32_t    utc_usec;
      uint8_t     asn[5];              // Absolute Slot No. (10msec slots since
                                       //  boot).
      uint16_t    asn_offset;          // usec into current slot number 
   }  get_time_response;
   
   struct _get_charge_response
   {
      uint8_t     cmd;                 // CMD_GET
      uint8_t     len_pkt;             // always 0x0B
      uint8_t     flags;               // expect FLAG_GET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_CHARGE
      uint32_t    charge_mC;           // charge since last reset
      uint32_t    uptime_sec;          // uptime since last reset
      int8_t      temp_celsius;
      uint8_t     temp_frac_celsius;   // Temperature in 1/255 of Celsius
   }  get_charge_response;
   
   struct _get_testrx_response
   {
      uint8_t     cmd;                 // CMD_GET
      uint8_t     len_pkt;             // always 0x05
      uint8_t     flags;               // expect FLAG_GET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_RADIOSTATSRX
      uint16_t    num_of_received_packets;
      uint16_t    num_of_recep_failures;
   }  get_testrx_response;
 
   struct _getNV_macaddr_response
   {
      uint8_t     cmd;                 // CMD_GETNV
      uint8_t     len_pkt;             // always 0x09
      uint8_t     flags;               // expect FLAG_GET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_MACADDR
      uint8_t     macaddr[8];
   }  getNV_macaddr_response;
   
   struct _getNV_networkid_response
   {
      uint8_t     cmd;                 // CMD_GETNV
      uint8_t     len_pkt;             // always 0x03
      uint8_t     flags;               // expect FLAG_GET_RESPONSE
      uint8_t     result;              // RC_NETWORKID
      uint8_t     param_type;          // PARAM_NETWORKID
      uint16_t    network_id;
   }  getNV_networkid_response;
   
   struct _getNV_txpower_response
   {
      uint8_t     cmd;                 // CMD_GETNV
      uint8_t     len_pkt;             // always 0x02
      uint8_t     flags;               // expect FLAG_GET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_TXPOWER
      uint8_t     txpower;
   }  getNV_txpower_response;
   
   struct _getNV_powerinfo_response
   {
      uint8_t     cmd;                 // CMD_GETNV
      uint8_t     len_pkt;             // always 0x0C
      uint8_t     flags;               // expect FLAG_GET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_POWERINFO
      uint8_t     power_source;        // PWRSOURCE_*
      uint16_t    discharge_current_uA;
      uint32_t    discharge_time_sec;
      uint32_t    recharge_time_sec;    
   }  getNV_powerinfo_response;

   struct _getnv_otaplock_response
   {
      uint8_t     cmd;                 // CMD_GETNV
      uint8_t     len_pkt;             // always 0x02
      uint8_t     flags;               // expect FLAG_GET_RESPONSE
      uint8_t     result;              // RC_*
      uint8_t     param_type;          // PARAM_OTAPLOCK
      uint8_t     otaplock;
   }  getnv_otaplock_response;
   
   struct _cmd_response
   {
      uint8_t     cmd;                 // CMD_SEND, CMD_JOIN,
                                       // CMD_DISCONNECT, CMD_RESET
                                       // CMD_SLEEP, CMD_TESTRADIOTX,
                                       // CMD_TESTRADIORX, CMD_CLEARNV
      uint8_t     len_pkt;             // always 0x00
      uint8_t     flags;               // always FLAG_MOTECMD_RESPONSE
      uint8_t     result;              // RC_*
   }  cmd_response;
   
   struct _time_indication
   // this is what you get from mote when you toggle _DUST_TIME
   {
      uint8_t     cmd;                 // CMD_TIME
      uint8_t     len_pkt;             // always 0x0F
      uint8_t     flags;               // See tables 98 and 1*****************
      uint32_t    utc_sec;
      uint32_t    utc_usec;
      uint8_t     asn[5];              // Absolute Slot No. (10msec slots since
                                       //  boot.
      uint16_t    asn_offset;          // usec into current slot number
   }  time_indication;
   
   struct _service_indication
   {
      uint8_t     cmd;                 // CMD_SERVICE
      uint8_t     len_pkt;             // always 0x0C
      uint8_t     flags;               // See tables 98 and 1*****************
      uint8_t     event_code;          // SVC_*
      uint8_t     mgr_code;            // MGRCODE_*
      uint8_t     svc_index;
      uint8_t     svc_state;           // SVCSTATE_*
      uint8_t     svc_flags;           // SVCRQST_*
      uint8_t     app_domain;          // APPDOMAIN_*
      uint16_t    destaddr;            // always 0xF981
      uint32_t    time;                // Period/latency: intermittent flag set
   }  service_indication;

   struct _events_indication
   {
      uint8_t     cmd;                 // CMD_EVENT
      uint8_t     len_pkt;             // always 0x09
      uint8_t     flags;               // See tables 98 and 1*****************
      uint32_t    new_events;          // EVENTMASK_* (?)
      uint8_t     mote_state;          // MOTESTATE_*
      uint32_t    mote_alarms;         // ALARMS_*
   }  events_indication;
   
   struct _advert_indication
   {
      uint8_t     cmd;                 // CMD_EVENT
      uint8_t     len_pkt;             // always 0x09
      uint8_t     flags;
      uint16_t    network_id;
      uint16_t    mote_id;
      uint8_t     rssi;                // rcvd signal strength
      uint8_t     hop_depth;           // (join priority)
   }  advert_indication;
   
   struct _data_from_net
   {
      uint8_t     cmd;                 // CMD_DATA
      uint8_t     len_pkt;             // always len_data + 4
      uint8_t     flags;               // See tables 98 and 1*****************
      uint16_t    sourceaddr;          // always 0xF981
      uint8_t     sequence_num;        // sequence number
      uint8_t     len_data;            // packet data length (<=MAX_USER_DATA_SIZE)
      uint8_t     data[MAX_USER_DATA_SIZE];
                                       // actual received data
   }  data_from_net;
}  mote_to_micro; 

/*
#define dust_dn2500_response_t                              dust_dn2500_param_response_t
#define dust_dn2500_set_param_tx_power_response_t           dust_dn2500_param_response_t
#define dust_dn2500_set_param_join_duty_cycle_response_t    dust_dn2500_param_response_t
#define dust_dn2500_set_param_battery_life_response_t       dust_dn2500_param_response_t
#define dust_dn2500_set_param_event_mask_response_t         dust_dn2500_param_response_t
#define dust_dn2500_set_param_write_protect_response_t      dust_dn2500_param_response_t
#define dust_dn2500_setnv_mac_address_response_t            dust_dn2500_param_response_t
#define dust_dn2500_setnv_network_id_response_t             dust_dn2500_param_response_t
#define dust_dn2500_setnv_tx_power_response_t               dust_dn2500_param_response_t
#define dust_dn2500_setnv_joinkey_response_t                dust_dn2500_param_response_t
#define dust_dn2500_setnv_otap_lockout_response_t           dust_dn2500_param_response_t
#define dust_dn2500_setnv_powerinfo_response_t              dust_dn2500_param_response_t
#define dust_dn2500_setnv_ttl_response_t                    dust_dn2500_param_response_t
#define dust_dn2500_send_response_t                         dust_dn2500_cmd_response_t
#define dust_dn2500_join_response_t                         dust_dn2500_cmd_response_t
#define dust_dn2500_search_response_t                       dust_dn2500_cmd_response_t
#define dust_dn2500_disconnect_response_t                   dust_dn2500_cmd_response_t
#define dust_dn2500_reset_response_t                        dust_dn2500_cmd_response_t
#define dust_dn2500_lowPowerSleep_response_t                dust_dn2500_cmd_response_t
#define dust_dn2500_testRadioTx_response_t                  dust_dn2500_cmd_response_t
#define dust_dn2500_testRadioRx_response_t                  dust_dn2500_cmd_response_t
#define dust_dn2500_clearnv_response_t                      dust_dn2500_cmd_response_t
#define dust_dn2500_set_param_hart_device_status_response_t dust_dn2500_param_response_t
#define dust_dn2500_set_param_hart_device_info_response_t   dust_dn2500_param_response_t
#define dust_dn2500_setnv_hart_antenna_gain_response_t      dust_dn2500_param_response_t

// MICRO TO MOTE RESPONSES AFTER TIME/SERVICE/EVENTS INDICATION OR DATA_FROM_NET
#define dust_dn2500_time_response_t    dust_dn2500_cmd_response_t
#define dust_dn2500_service_response_t dust_dn2500_cmd_response_t
#define dust_dn2500_events_response_t  dust_dn2500_cmd_response_t
#define dust_dn2500_data_response_t    dust_dn2500_cmd_response_t


struct dust_dn2500_cmd_response_t {
    DUST_DN2500_HEADER_RESPONSE
};
struct dust_dn2500_header_t {
    DUST_DN2500_HEADER
};

// WHAT UP WITH THIS....  hw/sw incompatibilities insurance?? -- ignore for now
// points to any extra bytes received that are not identified as a "dust" packet 
struct dust_dn2500_unknown_data_t {
   uint8_t*    data;
   uint8_t     len;
};
*/

// uint8_t     macaddr[8];
uint8_t     joinkey[MAX_JOINKEY_SIZE]={0x44,0x55,0x53,0x54,0x4E,0x45,0x54,0x57,0x4F,0x52,0x4B,0x53,0x52,0x4F,0x43,0x4B};
// need these declaration for pkgen_setnv_macaddr

/*
// The following structures exist in RAM as 'global'.  No need to pass
//  the structure to the packet generation routines defined below
struct _set_battlife_struct
// setup up battlife_days and pwrstat prior to pkgen_set_battlife() call
{
   uint16_t    battlife_days;
   uint8_t     pwrstat;                // PWRSTAT_*
}  set_battlife_struct;

struct _setnv_powerinfo_struct 
// setup pwrsource, discharge_current_uA, discharge_time_sec,
//  and recharge_time_sec prior to pkgen_setnv_powerinfo()
{
   uint8_t     pwrsource;              // PWRSOURCE_*
   uint16_t    discharge_current_uA;
   uint32_t    discharge_time_sec;
   uint32_t    recharge_time_sec;    
}  setnv_powerinfo_struct;

struct _set_svc_struct
// setup  svc_index; request_flags, app_domain and time 
//  pwrstat prior to pkgen_set_battlife() call.  No need to setup dest_address
{
   uint8_t     svc_index;
   uint8_t     request_flags;          // SVCRQST_*
   uint8_t     app_domain;             // APPDOMAIN_*
   uint16_t    destaddr;               // always 0xF981
                                       //  set properly in pkgen_set_service()
   uint32_t    time;                   // Period/latency: intermittent flag set
}  set_svc_struct;

struct testradiotx_struct
// setup channel and num_packets prior to pkgen_testradiotx() call
{
   uint8_t     channel;                // 0-15
   uint16_t    num_packets;
}  testradiotx_struct;

struct testradiorx_struct
// setup channel and time_sec prior to pkgen_testradiorx() call
{
   uint8_t     channel;                // 0-15 
   uint16_t    time_sec;               // rcv packets for this many secs
}  testradiorx_struct;
*/


/* ------------------------------------------------------------------------- */
#define __swap32(src) (((uint32_t)src<<24)|(((uint32_t)src&0xff00)<<8)| \
                           (((uint32_t)src>>8)&0xff00)|((uint32_t)src>>24))
#define __swap16(src) (((uint16_t)src<<8)|((uint16_t)src>>8))
/* ------------------------------------------------------------------------- */


/* ----------------------  Packet Generation Macros ------------------------ */

/* setParameter<txPower> Request */
#define pkgen_set_txpower(power) { \
    uint8_t prefix[] = {CMD_SET,0x02/*length*/,FLAG_TYPE_REQUEST,PARAM_TXPOWER}; \
    uint8_t pw       = power; \
    dust_hdlc_pkgen(prefix, 4, (uint8_t*)&pw, 1); \
}

/* setParameter<joinDutyCycle> Request */
#define pkgen_set_joinduty(cycle) { \
    uint8_t prefix[] = {CMD_SET,0x02/*length*/,FLAG_TYPE_REQUEST,PARAM_JOINDUTY}; \
    uint8_t cy       = cycle; \
    dust_hdlc_pkgen(prefix, 4, (uint8_t*)&cy, 1); \
}

/* setParameter<batteryLife> Request */
// structure is a declared global
#define pkgen_set_battlife() { \
    uint8_t  prefix[] = {CMD_SET,0x04/*length*/,FLAG_TYPE_REQUEST,PARAM_BATTLIFE}; \
    set_battlife_struct.battlife_days = __swap16(set_battlife_struct.battlife_days); \
    dust_hdlc_pkgen(prefix, 4, (uint8_t*)&set_battlife_struct, 3); \
}

/* setParameter<service> Request */
// structure is a declared global
#define pkgen_set_service() { \
    uint8_t  prefix[] = {CMD_SET,0x0A/*length*/,FLAG_TYPE_REQUEST,PARAM_SERVICE}; \
    set_svc_struct.destaddr = DESTADDR_SWAPPED; \
    set_svc_struct.time         = __swap32(set_svc_struct.time); \
    dust_hdlc_pkgen(prefix, 4, (uint8_t*)&set_svc_struct, 9); \
}

/* setParameter<eventMask> Request */
// eventmask is one of EVENTMASK_*
#define pkgen_set_eventmask(eventmask) { \
    uint8_t  prefix[] = {CMD_SET,0x05/*length*/,FLAG_TYPE_REQUEST,PARAM_EVENTMASK}; \
    uint32_t dstem    = __swap32(eventmask); \
    dust_hdlc_pkgen(prefix, 4, (uint8_t*)&dstem, 4); \
}

/* getParameter<moteInfo> Request */
#define pkgen_get_moteinfo() { \
    uint8_t cmd[] = {CMD_GET,0x01/*length*/,FLAG_TYPE_REQUEST,PARAM_MOTEINFO}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 4); \
}
 
/* getParameter<networkInfo> Request */
#define pkgen_get_networkinfo() { \
    uint8_t cmd[] = {CMD_GET,0x01/*length*/,FLAG_TYPE_REQUEST,PARAM_NETWORKINFO}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 4); \
}

/* getParameter<moteStatus> Request */
#define pkgen_get_motestatus() { \
    uint8_t cmd[] = {CMD_GET,0x01/*length*/,FLAG_TYPE_REQUEST,PARAM_MOTESTATUS}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 4); \
}

/* getParameter<time> Request */
#define pkgen_get_time() { \
    uint8_t cmd[] = {CMD_GET,0x01/*length*/,FLAG_TYPE_REQUEST,PARAM_TIME}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 4); \
}

/* getParameter<charge> Request */
#define pkgen_get_charge() { \
    uint8_t cmd[] = {CMD_GET,0x01/*length*/,FLAG_TYPE_REQUEST,PARAM_CHARGE}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 4); \
}

/* getParameter<testRadioRxStats> Request */
#define pkgen_get_radiostatsrx() { \
    uint8_t cmd[] = {CMD_GET,0x01/*length*/,FLAG_TYPE_REQUEST,PARAM_RADIOSTATSRX}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 4); \
}

/* getParameter<service> Request */
#define pkgen_get_service(service_idx) { \
    uint8_t prefix[] = {CMD_GET,0x02/*length*/,FLAG_TYPE_REQUEST,PARAM_SERVICE}; \
    uint8_t idx      = svc_index; \
    dust_hdlc_pkgen(prefix, 4, (uint8_t*)&idx, 1); \
}

/* setNVParameter<macAddress> Request */
// macaddr array is declared
#define pkgen_setnv_macaddr() { \
    uint8_t prefix[]   = {CMD_SETNV,0x0D/*length*/,FLAG_TYPE_REQUEST, 0x00, 0x00, 0x00, 0x00, PARAM_MACADDR}; \
    dust_hdlc_pkgen(prefix, 8, macaddr, 8); \
}

/* setNVParameter<networkID> Request */
#define pkgen_setnv_networkid(networkid) { \
    uint8_t prefix[] = {CMD_SETNV,0x07/*length*/,FLAG_TYPE_REQUEST, 0x00, 0x00, 0x00, 0x00, PARAM_NETWORKID}; \
    uint16_t net_id  = networkid; \
    dust_hdlc_pkgen(prefix, 8, (uint8_t*)&net_id, 2); \
}

/* setNVParameter<txPower> Request */
#define pkgen_setnv_txpower(txpower) { \
    uint8_t prefix[] = {CMD_SETNV,0x06/*length*/,FLAG_TYPE_REQUEST, 0x00, 0x00, 0x00, 0x00, PARAM_TXPOWER}; \
    uint8_t dst_power  = txpower; \
    dust_hdlc_pkgen(prefix, 8, (uint8_t*)&dst_power, 1); \
}

/* setNVParameter<joinKey> Request */
// joinkey array is declared

#define pkgen_setnv_joinkey(joinkey) { \
    uint8_t prefix[]   = {CMD_SETNV,0x15/*length*/,FLAG_TYPE_REQUEST, 0x00, 0x00, 0x00, 0x00, PARAM_JOINKEY}; \
    dust_hdlc_pkgen(prefix, 8, joinkey, 16); \
}

//#define pkgen_setnv_joinkey(joinkey) { \
//      uint8_t prefix[]   = {CMD_SETNV,0x15/*length*/,FLAG_TYPE_REQUEST, 0x00, 0x00, 0x00, 0x00, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; \
//      dust_hdlc_pkgen(prefix, 8,,16); \
//}


/* setNVParameter<powerInfo> Request */
// structure is a declared global
#define pkgen_setnv_powerinfo() { \
    uint8_t prefix[] = {CMD_SETNV,0x10/*length*/,FLAG_TYPE_REQUEST, 0x00, 0x00, 0x00, 0x00, PARAM_POWERINFO}; \
    setnv_powerinfo_struct.discharge_current_uA     = __swap16(setnv_powerinfo_struct.discharge_current_uA); \
    setnv_powerinfo_struct.discharge_time_sec       = __swap32(setnv_powerinfo_struct.discharge_time_sec); \
    setnv_powerinfo_struct.recharge_time_sec        = __swap32(setnv_powerinfo_struct.recharge_time_sec); \
    dust_hdlc_pkgen(prefix, 8, (uint8_t*)&setnv_powerinfo_struct, 11); \
} 

/* setNVParameter<OTAPlockout> Request */
#define pkgen_setnv_otaplock(lockout) { \
   uint8_t prefix[] = {CMD_SETNV,0x06/*length*/,FLAG_TYPE_REQUEST, 0x00, 0x00, 0x00, 0x00, PARAM_OTAPLOCK}; \
   uint8_t otaplock = lockout; \
   dust_hdlc_pkgen(prefix, 8, (uint8_t*)&otaplock, 1); \
}

/* setNVParameter<SpecialJoinTimingParams> Request */
#define pkgen_setnv_special1(special1) { \
   uint8_t prefix[] = {CMD_SETNV,0x07/*length*/,0x02, 0x67, 0x12, 0x04, 0x04, PARAM_SPECIAL1}; \
   uint16_t spec_param1  = special1; \
   dust_hdlc_pkgen(prefix, 8, (uint8_t*)&spec_param1, 2); \
}

/* setNVParameter<SpecialJoinTimingParams> Request */
#define pkgen_setnv_special2(special2) { \
   uint8_t prefix[] = {CMD_SETNV,0x07/*length*/,0x00, 0x67, 0x12, 0x04, 0x04, PARAM_SPECIAL2}; \
   uint16_t spec_param2 = special2; \
   dust_hdlc_pkgen(prefix, 8, (uint8_t*)&spec_param2, 2); \
}

/* getNVParameter<macAddress> Request */
#define pkgen_getnv_macaddr() { \
    uint8_t cmd[] = {CMD_GETNV,0x05/*length*/,FLAG_TYPE_REQUEST, 0x00, 0x00, 0x00, 0x00, PARAM_MACADDR}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 8); \
}

/* getNVParameter<networkID> Request */
#define pkgen_getnv_networkid() { \
    uint8_t cmd[] = {CMD_GETNV,0x05/*length*/,FLAG_TYPE_REQUEST, 0x00, 0x00, 0x00, 0x00, PARAM_NETWORKID}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 8); \
}

/* getNVParameter<txPower> Request */
#define pkgen_getnv_txpower() { \
    uint8_t cmd[] = {CMD_GETNV,0x05/*length*/,FLAG_TYPE_REQUEST, 0x00, 0x00, 0x00, 0x00, PARAM_TXPOWER}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 8); \
}

/* getNVParameter<powerInfo> Request */
#define pkgen_getnv_powerinfo() { \
    uint8_t cmd[] = {CMD_GETNV,0x05/*length*/,FLAG_TYPE_REQUEST, 0x00, 0x00, 0x00, 0x00, PARAM_POWERINFO}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 8); \
}

/* getNVParameter<OTAPlockout> Request */
#define pkgen_getnv_otaplock() { \
    uint8_t cmd[]   = {CMD_GETNV,0x05/*length*/,FLAG_TYPE_REQUEST, 0x00, 0x00, 0x00, 0x00, PARAM_OTAPLOCK}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 8); \
}

// command to mote to test the radio transmitter
// structure is a declared global
#define pkgen_testradiotx() { \
    uint8_t  prefix[] = {CMD_TESTRADIOTX, 3 /*payload length*/, FLAG_TYPE_TEST}; \
    testradiotx_struct.num_packets = __swap16(testradiotx_struct.num_packets); \
    dust_hdlc_pkgen(prefix, 3, (uint8_t*)&testradiotx_struct, 3); \
}

#define pkgen_testradiorx() { \
    uint8_t  prefix[] = {CMD_TESTRADIORX, 3 /*payload length*/, FLAG_TYPE_TEST}; \
    testradiorx_struct.time_sec = __swap16(testradiorx_struct.time_sec); \
    dust_hdlc_pkgen(prefix, 3, (uint8_t*)&testradiorx_struct, 3); \
}

/* Join Request */
#define pkgen_join() { \
    uint8_t cmd[]   = {CMD_JOIN,0x00/*payload length*/,FLAG_TYPE_REQUEST}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 3); \
}

/* Disconnect Request */
#define pkgen_disconnect() { \
    uint8_t cmd[]   = {CMD_DISCONNECT,0x00/*payload length*/,FLAG_TYPE_REQUEST}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 3); \
}

/* Reset Request */
#define pkgen_reset() { \
    uint8_t cmd[]   = {CMD_RESET,0x00/*payload length*/,FLAG_TYPE_REQUEST}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 3); \
}

/* lowPowerSleep Request */
#define pkgen_sleep() { \
    uint8_t cmd[]   = {CMD_SLEEP,0x00/*payload length*/,FLAG_TYPE_REQUEST}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 3); \
}

/* ClearNV Request */
#define pkgen_clearnv() { \
    uint8_t cmd[]   = {CMD_CLEARNV,0x00/*payload length*/,FLAG_TYPE_REQUEST}; \
    ((char*)(NULL), 0, cmd, 3); \
}

/* Search Request */
#define pkgen_search() { \
    uint8_t cmd[]   = {CMD_SEARCH,0x00/*payload length*/,FLAG_TYPE_REQUEST}; \
    dust_hdlc_pkgen((char*)(NULL), 0, cmd, 3); \
}

// MOTE command responses.  The micro sends a response to mote 
//  after having received an indication or a data packet

#define pkgen_time_ind_response(result_fl) { \
    uint8_t prefix[] = {CMD_TIME,0x00};   \
    uint16_t r        = result_fl; \
    dust_hdlc_pkgen(prefix, 2, (uint8_t*)&r, 2); \
}

#define pkgen_service_ind_response(result_fl) { \
    uint8_t prefix[] = {CMD_SERVICE,0x00}; \
    uint16_t r       = result_fl; \
    dust_hdlc_pkgen(prefix, 2, (uint8_t*)&r, 2);  \
}

#define pkgen_event_ind_response(result_fl) { \
    uint8_t prefix[] = {CMD_EVENT,0x00};  \
    uint16_t r        = result_fl; \
    dust_hdlc_pkgen(prefix, 2, (uint8_t*)&r, 2); \
}

#define pkgen_advert_ind_response(result_fl) { \
    uint8_t prefix[] = {CMD_ADVERT,0x00}; \
    uint16_t r       = result_fl; \
    dust_hdlc_pkgen(prefix, 2, (uint8_t*)&r, 2); \
}

#define pkgen_data_rcvd_response(result_fl) { \
    uint8_t prefix[] = {CMD_DATA,0x00};  \
    uint16_t r       = result_fl; \
    dust_hdlc_pkgen(prefix, 2, (uint8_t*)&r, 2); \
}

// Added by brian
#define pkgen_mote_ack() { \
 uint8_t cmd[]   = {(flags_rcvd+1),response_type}; \
 dust_hdlc_pkgen((cmd_rcvd << 8), 2, cmd, 2); \
}
   
// 
/*
struct dust_dn2500_t {

    // SET Parameters Responses
    int8_t (*set_param_tx_power_response)(void* handle, struct dust_dn2500_set_param_tx_power_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*set_param_join_duty_cycle_response)(void* handle, struct dust_dn2500_set_param_join_duty_cycle_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*set_param_battery_life_response)(void* handle, struct dust_dn2500_set_param_battery_life_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*set_param_service_response)(void* handle, struct dust_dn2500_set_param_service_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*set_param_hart_device_status_response)(void* handle, struct dust_dn2500_set_param_hart_device_status_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*set_param_hart_device_info_response)(void* handle, struct dust_dn2500_set_param_hart_device_info_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*set_param_event_mask_response)(void* handle, struct dust_dn2500_set_param_event_mask_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*set_param_write_protect_response)(void* handle, struct dust_dn2500_set_param_write_protect_response_t* p, struct dust_dn2500_unknown_data_t* un);

    int8_t (*get_param_mote_info_response)(void* handle, struct dust_dn2500_get_param_mote_info_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*get_param_network_info_response)(void* handle, struct dust_dn2500_get_param_network_info_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*get_param_mote_status_response)(void* handle, struct dust_dn2500_get_param_mote_status_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*get_param_time_response)(void* handle, struct dust_dn2500_get_param_time_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*get_param_charge_response)(void* handle, struct dust_dn2500_get_param_charge_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*get_param_test_radio_rxstats_response)(void* handle, struct dust_dn2500_get_param_test_radio_rxstats_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*get_param_service_response)(void* handle, struct dust_dn2500_get_param_service_response_t* p, struct dust_dn2500_unknown_data_t* un);

    // Non-Volatile Parameter Responses
    int8_t (*setnv_mac_address_response)(void* handle, struct dust_dn2500_setnv_mac_address_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*setnv_network_id_response)(void* handle, struct dust_dn2500_setnv_network_id_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*setnv_tx_power_response)(void* handle, struct dust_dn2500_setnv_tx_power_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*setnv_joinkey_response)(void* handle, struct dust_dn2500_setnv_joinkey_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*setnv_powerinfo_response)(void* handle, struct dust_dn2500_setnv_powerinfo_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*setnv_ttl_response)(void* handle, struct dust_dn2500_setnv_ttl_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*setnv_hart_antenna_gain)(void* handle, struct dust_dn2500_setnv_hart_antenna_gain_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*setnv_otap_lockout)(void* handle, struct dust_dn2500_setnv_otap_lockout_response_t* p, struct dust_dn2500_unknown_data_t* un);

    int8_t (*getnv_mac_address_response)(void* handle, struct dust_dn2500_getnv_mac_address_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*getnv_network_id_response)(void* handle, struct dust_dn2500_getnv_network_id_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*getnv_tx_power_response)(void* handle, struct dust_dn2500_getnv_tx_power_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*getnv_powerinfo_response)(void* handle, struct dust_dn2500_getnv_powerinfo_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*getnv_ttl_response)(void* handle, struct dust_dn2500_getnv_ttl_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*getnv_hart_antenna_gain)(void* handle, struct dust_dn2500_getnv_hart_antenna_gain_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*getnv_otap_lockout)(void* handle, struct dust_dn2500_getnv_otap_lockout_response_t* p, struct dust_dn2500_unknown_data_t* un);

    // Command Responses
    int8_t (*send_response)(void* handle, struct dust_dn2500_send_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*join_response)(void* handle, struct dust_dn2500_join_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*disconnect_response)(void* handle, struct dust_dn2500_disconnect_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*search_response)(void* handle, struct dust_dn2500_search_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*reset_response)(void* handle, struct dust_dn2500_reset_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*lowPowerSleep_response)(void* handle, struct dust_dn2500_lowPowerSleep_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*hartPayload_response)(void* handle, struct dust_dn2500_hartPayload_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*testRadioTx_response)(void* handle, struct dust_dn2500_testRadioTx_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*testRadioRx_response)(void* handle, struct dust_dn2500_testRadioRx_response_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*clearnv_response)(void* handle, struct dust_dn2500_clearnv_response_t* p, struct dust_dn2500_unknown_data_t* un);

    // MOTE commands sent to sensor processor
    int8_t (*time_request)(void* handle, struct dust_dn2500_time_request_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*service_indication)(void* handle, struct dust_dn2500_service_indication_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*events_request)(void* handle, struct dust_dn2500_events_request_t* p, struct dust_dn2500_unknown_data_t* un);
    int8_t (*data_request)(void* handle, struct dust_dn2500_data_request_t* p, struct dust_dn2500_unknown_data_t* un);

    // Unknown command
    int8_t (*unknown_command)(void* handle, uint8_t* data, uint8_t len);

    // source buffer
    uint8_t     bf_src[MAX_ENCODED_PACKET_SIZE];
    uint8_t     len_src;

    // destination buffer
    uint8_t     bf_dest[MAX_DEST_BUF_SIZE];
    uint8_t     len_dest;

    uint8_t     idx;
    

    // Used in case SW version isn't in sync with HW firmware version
   struct dust_dn2500_unknown_data_t unknown;
};

// Utility commands to queue and clear packet data
int8_t dust_dn2500_q_clear(struct dust_dn2500_t* ia);
int8_t dust_dn2500_q_byte(struct dust_dn2500_t* ia, uint8_t byte);

// Routine to dispatch incoming packets to callbacks
int8_t dust_dn2500_dispatcher(struct pappy_t* md, struct dust_dn2500_t* ia, void* handle);
//user defined

*/

// HERE ARE ITEMS FROM DUST_HDLC.H
#define CONTROL_ESCAPE 0x7d
#define FLAG_SEQUENCE  0x7e
#define XOR_BYTE       0x20

/* Low level routine return values */
#define DUST_HDLC_RTN_SUCCESS                     0
#define DUST_HDLC_RTN_NOT_ENOUGH_BUFFER_SPACE    -1
#define DUST_HDLC_RTN_BAD_CRC                    -2
#define DUST_HDLC_RTN_INVALID_PK_DELIMITERS      -3


// HERE ARE ITEMS FROM DUST_HDLC.C
#define FCS_INITIAL_FCS16 ((uint16_t) 0xffff)
#define FCS_GOOD_FCS16    ((uint16_t) 0xf0b8)

// We will use the routine that takes LESS PROCESSING TIME at the expense
//  or greater memory usage.

// lookup table for calculation of checksum
UNSIGNED INT16 CONST fcstab[256] =  {
   0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
   0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
   0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
   0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
   0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
   0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
   0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
   0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
   0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
   0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
   0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
   0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
   0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
   0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
   0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
   0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
   0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
   0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
   0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
   0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
   0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
   0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
   0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
   0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
   0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
   0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
   0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
   0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
   0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
   0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
   0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
   0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

/*
// EVEN MORE STUFF NOT RELEVANT TO JACK IMPLEMENTATION
// ITEMS FROM DUST_DN2500_HANDLER.H
// I THINK THESE ARE FUNCTION PROTOTYPES!
// set_parameter response packets received by sensor processor
int8_t dust_dn2500_set_param_tx_power_response(void* handle, struct dust_dn2500_set_param_tx_power_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_set_param_join_duty_cycle_response(void* handle, struct dust_dn2500_set_param_join_duty_cycle_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_set_param_battery_life_response(void* handle, struct dust_dn2500_set_param_battery_life_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_set_param_service_response(void* handle, struct dust_dn2500_set_param_service_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_set_param_event_mask_response(void* handle, struct dust_dn2500_set_param_event_mask_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_set_param_hart_device_status_response(void* handle, struct dust_dn2500_set_param_hart_device_status_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_set_param_hart_device_info_response(void* handle, struct dust_dn2500_set_param_hart_device_info_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_set_param_write_protect_response(void* handle, struct dust_dn2500_set_param_write_protect_response_t* p, struct dust_dn2500_unknown_data_t* un);

// get_parameter response packets received by sensor processor
int8_t dust_dn2500_get_param_mote_info_response(void* handle, struct dust_dn2500_get_param_mote_info_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_get_param_network_info_response(void* handle, struct dust_dn2500_get_param_network_info_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_get_param_mote_status_response(void* handle, struct dust_dn2500_get_param_mote_status_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_get_param_service_response(void* handle, struct dust_dn2500_get_param_service_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_get_param_time_response(void* handle, struct dust_dn2500_get_param_time_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_get_param_charge_response(void* handle, struct dust_dn2500_get_param_charge_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_get_param_test_radio_rxstats_response(void* handle, struct dust_dn2500_get_param_test_radio_rxstats_response_t* p, struct dust_dn2500_unknown_data_t* un);

// get non-volatile response packets received by sensor processor
int8_t dust_dn2500_getnv_mac_address_response(void* handle, struct dust_dn2500_getnv_mac_address_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_getnv_network_id_response(void* handle, struct dust_dn2500_getnv_network_id_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_getnv_tx_power_response(void* handle, struct dust_dn2500_getnv_tx_power_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_getnv_powerinfo_response(void* handle, struct dust_dn2500_getnv_powerinfo_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_getnv_ttl_response(void* handle, struct dust_dn2500_getnv_ttl_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_getnv_hart_antenna_gain(void* handle, struct dust_dn2500_getnv_hart_antenna_gain_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_getnv_otap_lockout(void* handle, struct dust_dn2500_getnv_otap_lockout_response_t* p, struct dust_dn2500_unknown_data_t* un);

// set non-volatile response packets received by sensor processor
int8_t dust_dn2500_setnv_mac_address_response(void* handle, struct dust_dn2500_setnv_mac_address_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_setnv_network_id_response(void* handle, struct dust_dn2500_setnv_network_id_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_setnv_tx_power_response(void* handle, struct dust_dn2500_setnv_tx_power_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_setnv_joinkey_response(void* handle, struct dust_dn2500_setnv_joinkey_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_setnv_powerinfo_response(void* handle, struct dust_dn2500_setnv_powerinfo_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_setnv_ttl_response(void* handle, struct dust_dn2500_setnv_ttl_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_setnv_hart_antenna_gain(void* handle, struct dust_dn2500_setnv_hart_antenna_gain_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_setnv_otap_lockout(void* handle, struct dust_dn2500_setnv_otap_lockout_response_t* p, struct dust_dn2500_unknown_data_t* un);

// motes response packets received by sensor processor
int8_t dust_dn2500_send_response(void* handle, struct dust_dn2500_send_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_join_response(void* handle, struct dust_dn2500_join_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_disconnect_response(void* handle, struct dust_dn2500_disconnect_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_search_response(void* handle, struct dust_dn2500_search_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_reset_response(void* handle, struct dust_dn2500_reset_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_lowPowerSleep_response(void* handle, struct dust_dn2500_lowPowerSleep_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_testRadioTx_response(void* handle, struct dust_dn2500_testRadioTx_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_testRadioRx_response(void* handle, struct dust_dn2500_testRadioRx_response_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_clearnv_response(void* handle, struct dust_dn2500_clearnv_response_t* p, struct dust_dn2500_unknown_data_t* un);

// Mote request packets received by sensor processor
int8_t dust_dn2500_time_request(void* handle, struct dust_dn2500_time_request_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_service_indication(void* handle, struct dust_dn2500_service_indication_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_events_request(void* handle, struct dust_dn2500_events_request_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_data_request(void* handle, struct dust_dn2500_data_request_t* p, struct dust_dn2500_unknown_data_t* un);
int8_t dust_dn2500_unknown_command(void* handle, uint8_t* data, uint8_t len);

*/
