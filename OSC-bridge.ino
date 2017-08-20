#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

//#define LED_BUILTIN 15

char ssid[] = "God";                // your network SSID (name)
char pass[] = "activate";                   // your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress outIp(192, 168, 0, 255);        // remote IP (not needed for receive)
const unsigned int outPort = 8000;          // remote port (not needed for receive)
const unsigned int localPort = 8000;        // local port to listen for UDP packets (here's where we send the packets)


OSCErrorCode error;
unsigned int ledState = LOW;              // LOW means led is *on*

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, ledState);    // turn *on* led

  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());



}


void loop() {
handleWifi();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool beatDetected = false;
int beatValue = 0;
unsigned long timeSinceBeat = 0;

void handleWifi()
{
  int rd, wr;
  char addressName[40];
  OSCMessage msgIN;
  static bool packet = 0;
  static unsigned long  tr = 0, tw = 0;
  static char beatPacketBuf[12];

  if ((rd = Udp.parsePacket()) > 0)
  {
    // Parse the beatpacket
    if (rd == 12) {
      Udp.read(beatPacketBuf, 12);
      if (beatPacketBuf[0] == 'B' &&
          beatPacketBuf[1] == 'E' &&
          beatPacketBuf[2] == 'A' &&
          beatPacketBuf[3] == 'T') {
        beatDetected = true;
      }
      return;
    }

    if (tr - micros() > 10000)
    {
      while (rd--)
        msgIN.fill(Udp.read());
      if (!msgIN.hasError())
      {
        digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on

if (!msgIN.hasError())
{
      Udp.beginPacket(outIp, outPort);
      msgIN.send(Udp);
      Udp.endPacket();

        digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED off
      }
      msgIN.empty();
    } 
    else {
      while (rd--)
        Udp.read();
    }
    tr = micros();
  }
}
  WiFi.mode(WIFI_STA); 
  delay(10000);
}

