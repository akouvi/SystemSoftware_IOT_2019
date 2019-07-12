#include "Arduino.h" //Die Servobibliothek wird aufgerufen. Sie wird benötigt, damit die Ansteuerung des Servos vereinfacht wird.
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>
#include <PubSubClient.h>

// Initialize the client library
WiFiClient espClient;
//This library provides a client for doing simple publish/subscribe messaging with a server that supports MQTT.
PubSubClient client(espClient);
//This library allows an Arduino board to control RC (hobby) servo motors
Servo servoObject;

//This function will be called whenever a message arrive
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  payload[length]='\0'; //The end of a String
  int a = atoi((const char*)payload); //Asci to Integer
  servoObject.write(a);   
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("client-sfasdfasdf")) {
      Serial.println("connected");
      client.publish("/status/devices", "I'm here");
      
      client.subscribe("/servo/1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

/*
*Das Setup enthält die Information, dass das Servo an der Steuerleitung mit Pin 2 verbunden wird. 
*Hier ist natürlich auch ein anderer Pin möglich.
*/
void setup() {
  servoObject.attach(2);
  servoObject.write(0);
  delay(2000); // Das Programm stopt für 2 Sekunden

  Serial.begin(9600);
  Serial.println("Hallo Welt");
  //Setting WIFI mode to STA
  WiFi.mode(WIFI_STA);
  //Setting the host name
  WiFi.hostname("dpsasdf");
  /*
  Connecting to WIFI 
  username : hsb-labor
  password : 6MVfNSqdMr5SZo6d
  */
  WiFi.begin("hsb-labor", "6MVfNSqdMr5SZo6d");

  //While we're not connected , delay and try again
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Print the IP of the arduino
  Serial.println(WiFi.localIP());
  //Set the server and the callBack function
  // This IP-Address is IP of the Raspbery
  client.setServer("192.168.206.19", 1883);
  client.setCallback(callback);
}

//This function will keep try to connect to the Arduino
/*Im „loop“ wird über den write-Befehl „servoblau.write(Grad)“ das Servo angesteuert. 
Zwischen den einzelnen Positionen gibt es eine Pause, damit das Servo genug Zeit hat, 
die gewünschten Positionen zu erreichen.
*/
void loop() {
  if (!client.connected())
   reconnect();

  client.loop();
}
