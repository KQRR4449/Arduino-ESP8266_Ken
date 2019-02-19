/*
 * ESP8266 simple Ping<->Pong
 * Ewald Kendziorra sr http://kendziorra.nl
 * URL: http://kendziorra.nl/arduino/93-esp8266-simple-pingpong
 * License: Free
 * Publishing: Only with this header
 * Editing: Free
 * Guarantee: No
 * Using: At your own risk
 * Support: No
 */
/***** used structures (ESP8266 SDK library file 'ping.h') *****  
    struct ping_option{
      uint32 count;
      uint32 ip;
      uint32 coarse_time;
      ping_recv_function recv_function;
      ping_sent_function sent_function;
      void* reverse;
    };
    struct ping_resp{
      uint32 total_count;
      uint32 resp_time;
      uint32 seqno;
      uint32 timeout_count;
      uint32 bytes;
      uint32 total_bytes;
      uint32 total_time;
      sint8  ping_err;
    };
 ***************************************************************/
// http://arduino.esp8266.com/versions/1.6.5-1160-gef26c5f/doc/reference.html
#include <ESP8266WiFi.h>       // https://github.com/esp8266/Arduino
extern "C" {
  #include <user_interface.h>  // https://github.com/esp8266/Arduino
  #include <ping.h>            // https://github.com/esp8266/Arduino
}
#include <Streaming.h>         // http://arduiniana.org/libraries/streaming
 
/******* EDIT *******/
const char* mySSID     = "-ssid-";
const char* myPASSWORD = "-password-";
const uint8_t pingCount = 3;     // number off Ping repetition
const uint8_t pingInterval = 1;  // Ping repetition every n sec 
   /* Local Ping example: 6 x IP on (W)LAN to test if the device is ONline or OFFline *
    * Of course, this can also with internet addresses                                */
const uint8_t ipCount = 6;       // number of IP addresses in pingIp[2][n] array
const char* pingIp[2][ipCount] = {{"192.168.178.34",   "192.168.178.26",   "192.168.178.38", "192.168.178.25", "192.168.178.30", "192.168.178.27"},
                                  {"ESP8266 WSserver", "ESP8266 WSclient", "A500 tablet",    "Samsung S4",     "Samsung TAB S2", "laptop Ada"}};
/***** END EDIT *****/
 
struct ping_option pingOpt, pOpt;
struct ping_resp pingResp;
const uint8_t port = 80;
uint8_t ipNext = 0;
 
void setup() {
  Serial.begin(115200);
  delay(100);
  WiFi.disconnect();
  WiFi.begin(mySSID, myPASSWORD);
  while (WiFi.status() != WL_CONNECTED) {delay(200); Serial << ".";}
  /***** no other WiFi function required *****/
  Serial << endl << "doPing() start" << endl << "============" << endl;
  Serial << "Ping IP: " << pingIp[0][ipNext] << ", " << pingIp[1][ipNext] << endl;
  doPing(pingIp[0][ipNext]);  // start Ping IP[0][0]
}
 
void loop(){}
 
void pingRecv(void *arg, void *pdata) {  // Pong callback function
//  struct ping_option *pingOpt = (struct ping_option *)arg;
  struct ping_resp *pingResp = (struct  ping_resp *)pdata;
  if (pingResp->ping_err == -1) Serial << "No Pong (device OFFline)" << endl;
  else Serial << "ping recv: bytes = " << pingResp->bytes << ", time = " << pingResp->resp_time << "ms" << endl;
}
 
void pingSent(void *arg, void *pdata) {  // Ping finished callback
//  struct ping_option *pingOpt = (struct ping_option *)arg;
//  struct ping_resp *pingResp = (struct  ping_resp *)pdata;
  Serial << "ping finished" << endl;
  if (++ipNext < ipCount)   {  // next Ping?
    Serial << "Ping IP: " << pingIp[0][ipNext] << ", " << pingIp[1][ipNext] << endl;
    doPing(pingIp[0][ipNext]);  // init and start Ping IP[0][n]
  }
}
 
void doPing(const char *targetIpAddress) {   // init and start Ping
  struct ping_option *pingOpt = &pOpt;
  pingOpt->count = pingCount;
  pingOpt->coarse_time = pingInterval;
  pingOpt->ip = ipaddr_addr(targetIpAddress);
  ping_regist_recv(pingOpt, pingRecv);  // Pong callback function 'pingRecv'
  ping_regist_sent(pingOpt, pingSent);  // Ping finished callback function 'pingSent'
  ping_start(pingOpt);  // start Ping
}