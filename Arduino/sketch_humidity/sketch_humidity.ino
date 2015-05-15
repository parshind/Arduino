#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x01 };
char server[] = "www.kakoykurs.ru";
EthernetClient client;

int humidityPin = A0;
int humidityValue = 0;

int temperaturePin = A1;
int temperatureValue = 0;

int lightPin = A2;
int lightValue = 0;

int plantId = 1;

void setup()
{
  pinMode(humidityPin,INPUT);
  Serial.begin(9600);
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop()
{
  humidityValue = analogRead(humidityPin);
  Serial.print("Humidity value = ");
  Serial.println(humidityValue);
  SendDataToServer(plantId, humidityValue, temperatureValue, lightValue);
  delay(10000);
}

boolean SendDataToServer(int plantId, int humidity, int temperature, int light)
{

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) 
  {
    Serial.println("connected");
    // Make a HTTP request:
    String uri = (String)"/plants/plant_measure.php?plantId=" + (String)plantId + (String)"&humidity="+ (String)humidity + (String)"&temperature=" + (String)temperature + (String)"&light=" + (String)light;
    Serial.println("uri: " + uri);
    
    client.println("GET " + uri + " HTTP/1.1");
    client.println((String)"Host: " + (String)server);
    client.println("Connection: close");
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
  if(client.connected())
   client.stop();
  
  return true;
}

