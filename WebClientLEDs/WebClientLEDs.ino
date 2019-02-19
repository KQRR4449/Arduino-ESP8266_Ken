// Column locations.
//3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456
//       1         2         3         4         5         6         7         8         9     9
//const unsigned long FAKE_LONG     =          10000UL; // Fake variable for column positions.

/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define DEBUG_ESP_HTTP_SERVER

const byte            VER_MAJOR     =                1; // Major version.
const byte            VER_MINOR     =                2; // Minor version.

MDNSResponder mdns;

// Forward references.
void        dashedLine(byte cnt, char c = '-');
void        handleNotFound();

// Replace with your network credentials
const char* ssid      = "KQRR_AC1900";
const char* password  = "0140,athens,record,SUIT,WRITTEN,promise,THIN,8604,FRED,2763";

ESP8266WebServer server(80);

String webPage = "";

int gpio0_pin = 0;
int gpio2_pin = 2;

void setup(void){
  byte                macAdr[6];                        // Mac address array.
  unsigned long       start;                            // Start time ms.
  
  webPage += "<h1>ESP8266 Web Server</h1><p>Socket #1 <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></p>";
  webPage += "<p>Socket #2 <a href=\"socket2On\"><button>ON</button></a>&nbsp;<a href=\"socket2Off\"><button>OFF</button></a></p>";
  
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, LOW);
  
  Serial.begin(115200);
  while(!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println();
  Serial.printf(F("Starting WebClientLEDs version %d.%d, Build date %s %s\n"),
                                                                            VER_MAJOR,
                                                                            VER_MINOR,
                                                                            __DATE__,
                                                                            __TIME__);
  dashedLine(80);

  start = millis();
  WiFi.begin(ssid, password);
  // Wait for connection
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf(F("\nConnected to  %s. Connect time %lums.\n"), ssid,
                                                                millis() - start);
  Serial.print("IP address:  ");
  Serial.println(WiFi.localIP());

  WiFi.macAddress(macAdr);
  Serial.printf(F("MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n"),
    macAdr[0], macAdr[1], macAdr[2], macAdr[3], macAdr[4], macAdr[5]);
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){
    #ifdef DEBUG_ESP_HTTP_SERVER
    dashedLine(80);
    #endif // DEBUG_ESP_HTTP_SERVER
    server.send(200, "text/html", webPage);
  });
  server.on("/socket1On", [](){
    #ifdef DEBUG_ESP_HTTP_SERVER
    dashedLine(80);
    #endif // DEBUG_ESP_HTTP_SERVER
    server.send(200, "text/html", webPage);
    digitalWrite(gpio0_pin, HIGH);
    delay(1000);
  });
  server.on("/socket1Off", [](){
    #ifdef DEBUG_ESP_HTTP_SERVER
    dashedLine(80);
    #endif // DEBUG_ESP_HTTP_SERVER
    server.send(200, "text/html", webPage);
    digitalWrite(gpio0_pin, LOW);
    delay(1000); 
  });
  server.on("/socket2On", [](){
    #ifdef DEBUG_ESP_HTTP_SERVER
    dashedLine(80);
    #endif // DEBUG_ESP_HTTP_SERVER
    server.send(200, "text/html", webPage);
    digitalWrite(gpio2_pin, HIGH);
    delay(1000);
  });
  server.on("/socket2Off", [](){
    #ifdef DEBUG_ESP_HTTP_SERVER
    dashedLine(80);
    #endif // DEBUG_ESP_HTTP_SERVER
    server.send(200, "text/html", webPage);
    digitalWrite(gpio2_pin, LOW);
    delay(1000); 
  });

  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
  dashedLine(80);
}
 
void loop(void){
  server.handleClient();
}

void dashedLine(byte cnt, char c) {
  while( cnt-- > 0) {
    Serial.write(c);
  }
  Serial.println();
}

void handleNotFound(){
  #ifdef DEBUG_ESP_HTTP_SERVER
  dashedLine(80, '=');
  Serial.println(F("Entering handleNotFound."));
  #endif // DEBUG_ESP_HTTP_SERVER
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


