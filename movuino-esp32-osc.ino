#include "WiFi.h"
#include <OSCMessage.h>
 
WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,43,157); // IP of the receiver

// Network settings
char ssid[] = "hssnadr"; // your network SSID (name)
char pass[] = "adrien666";  // your network password
unsigned int localPort = 7777; // local port to listen for OSC packets

void setup() {
  Serial.begin(115200);

  Serial.println("Wait for WiFi... ");
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  Udp.begin(localPort);
}
 
void sendOSC() {
  OSCMessage msg("/movuino");
 
  float val1_ = random(100)*.01;
  float val2_ = random(100)*.01;
  float val3_ = random(100)*.01;
  msg.add( val1_);
  msg.add( val2_);
  msg.add( val3_);
   
  Udp.beginPacket(outIp, 7777);
  msg.send(Udp); // Send the bytes to the SLIP stream
  Udp.endPacket();  // Mark the end of the OSC Packet
  msg.empty();   // Free space occupied by message
  delay(10);
} 
 
void receiveMessage() {
  // Need to be called in the main loop()
  OSCMessage msg_;
  int size = Udp.parsePacket();
 
  if (size > 0) {
    while (size--) {
      msg_.fill(Udp.read());
    }
    if (!msg_.hasError()) {
     msg_.dispatch("/printInt", callbackOSC);
    }
  }
}

void callbackOSC(OSCMessage &msg) {
  Serial.print("Receive message: ");
  Serial.println(msg.getInt(0));
}
 
void loop() {
  sendOSC();
  delay(200);
  receiveMessage();
}
