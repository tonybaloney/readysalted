/*
 MQTT example for controlling the water fountain in the front yard
 relay for the pump is connected to PIN 2, HIGH state is on, LOW state is off
*/

#include <SPI.h>
#include <Ethernet.h>
#include "PubSubClient.h"

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xEE };

IPAddress server(168, 128, 36, 204);

int RELAY_PIN = 2;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message on topic - ");
  Serial.println(topic);
  if (topic == "fountainStateOn")
      digitalWrite(RELAY_PIN, HIGH);
  else if (topic == "fountainStateOff")
      digitalWrite(RELAY_PIN, LOW);
  else if (topic == "fountainStateRead")
   {
      char * buf;
      sprintf(buf, "State is %i", digitalRead(RELAY_PIN));
      publish("fountainState", buf);
   }
}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void connect(){
  Ethernet.begin(mac);
  if (client.connect("cabbage_fountain")) {
    client.publish("outTopic","fountation controller #1 connected");
    client.subscribe("fountainStateOn");
    client.subscribe("fountainStateOff");
    client.subscribe("fountainStateRead");
    Serial.println("Connected");
  } else {
    Serial.println("Could not connect");
    Serial.print(client.state());
    delay(5000);
    connect();
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting");
  Serial.print("MQTT Version ");
  Serial.print(MQTT_VERSION);
  
  connect();
  pinMode(RELAY_PIN, OUTPUT);
}

void publish(char * topic, char * payload){
  client.publish(topic, payload);
}

void loop()
{
  // TODO : Detect disconnection and reconnect
  client.loop();
}

