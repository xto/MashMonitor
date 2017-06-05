// This #include statement was automatically added by the Particle IDE.
#include <OneWire.h>
#include <DS18.h>
#include <math.h>

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
  bool success = sensor.read();
  if(success){
    String tempC = String(sensor.celsius(), 2);
    if(Serial.available() > 2){
      Serial.print(tempC);
      Serial.read();
    }
    boolean currentLineIsBlank = true;
    if (client.connected()) {
      while (client.available()) {
        char c = client.read();
        Serial.println(c);
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/plain");
          client.println("Connection: close");
          client.println();
          client.print(tempC);
          delay(500);
          client.flush();
          client.stop();
        }

      }
    } else {
      client = server.available();
    }
  }
}
