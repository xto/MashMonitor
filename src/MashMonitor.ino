// This #include statement was automatically added by the Particle IDE.
#include <OneWire.h>
#include <DS18.h>

/*
* Project MashMonitor
* Description:
* Author:
* Date:
*/


// -----------------
// Read temperature
// -----------------

// Data wire is plugged into port 0 on the Arduino
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DS18 sensor(D0);

// Create a variable that will store the temperature value
/*double temperature = 0.0;*/

TCPServer server = TCPServer(4499);
TCPClient client;
char IPString[16];

void setup(){
  IPAddress localIP = WiFi.localIP();
  while (!WiFi.ready()){
    delay(500);
  }
  sprintf(IPString, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
  Particle.variable("LocalIP", IPString);

  server.begin();
  Serial.begin(9600);

}

void loop(){
  float tempC;
  bool success = sensor.read();
  if(success){
    tempC = sensor.celsius();
    String potato = String(tempC);
    char tempString[potato.length()+1];
    potato.toCharArray(tempString, potato.length());
    if(Serial.available() > 2){
      Serial.print(tempString);
      Serial.read();
    }

    if (client.connected()) {
      while (client.available()) {
        for(int i = 0; i < strlen(tempString); i++){
          server.write(tempString[i]);
        }
        client.stop();
      }
      } else {
        client = server.available();
      }
    }
  }
