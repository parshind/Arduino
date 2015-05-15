#include <SPI.h>
#include <Ethernet.h>
#include <string.h>

byte mac[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x01 };
char server[] = "www.kakoykurs.ru";
String uri = "/plants/plant_measure.php";
//http://kakoykurs.ru/plants/plant_measure.php?plantId=2&humidity=10&temperature=10&light=10

int plantId0=1;
int plantId1=2;
int humidityPin0 = A0;
int humidityPin1 = A4;

int tempPin = A3;
int lightPin = A2;

float humidityValue0 = 0; //percent value
float humidityValue1 = 0; //percent value
float tempValue = 0; // temperature in C
float lightValue = 0; //light in percent

EthernetClient client;

void setup() 
{
  pinMode(humidityPin0, INPUT);  
  pinMode(humidityPin1, INPUT);  
  pinMode(tempPin, INPUT);  
  pinMode(lightPin, INPUT);  
  Serial.begin(9600);
  //analogReference(INTERNAL);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

}


void loop()
{  
  humidityValue0 = GetHumidityValue(humidityPin0);
  humidityValue1 = GetHumidityValue(humidityPin1);
  tempValue = GetCurrentTemp(tempPin);
  lightValue = GetCurrentLight(lightPin);  

  SendDataToServer(plantId0, humidityValue0, tempValue, lightValue);
  SendDataToServer(plantId1, humidityValue1, tempValue, lightValue);
  /*
   // Make a HTTP request:
        //plantId=2&humidity=10&temperature=10&light=10
          char humidityValueString[20];
          dtostrf(humidityValue,5,2,humidityValueString);
    
          char tempValueString[20];
          dtostrf(tempValue,5,2,tempValueString);
    
          char lightValueString[20];
          dtostrf(lightValue,5,2,lightValueString);
        
        String get = (String)"GET " +  (String)uri + (String)"?plantId=" + 
          (String)plantId + (String)"&humidity=" + 
          String(humidityValueString) + (String)"&temperature=" + 
          String(tempValueString) + (String)"&light=" + 
          String(lightValueString) +  (String)" HTTP/1.1"; 
        Serial.println(get);
        client.println(get);
        client.println("Host: " + (String)server);
        Serial.println("Host: " + (String)server);
        client.println("Connection: close");
        client.println();
        client.stop();
  */
  delay(10000);
}


boolean SendDataToServer(int plantId, float humidity, float temperature, float light)
{

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) 
  {
    Serial.println("connected");
    // Make a HTTP request:
    
    char humidityValueString[20];
    dtostrf(humidity,3,2,humidityValueString);

    char tempValueString[20];
    dtostrf(temperature,3,2,tempValueString);

    char lightValueString[20];
    dtostrf(light,3,2,lightValueString);
          
          
    String get_uri = (String)"GET " + (String)uri + (String)"?plantId=" + (String)plantId + (String)"&humidity="+ (String)humidityValueString + (String)"&temperature=" + (String)tempValueString + (String)"&light=" + (String)lightValueString + (String)" HTTP/1.1";
    Serial.println("uri: " + get_uri);
    
    client.println(get_uri);
    client.println((String)"Host: " + (String)server);
    client.println("Connection: close");
    client.println();
    client.stop();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }

  
  return true;
}


float GetHumidityValue(int pin)
{
  float humidity = analogRead(pin);
  Serial.print("read humidity value = ");
  Serial.println(humidity);
  humidity = (1024 - humidity)/1024*100;
  Serial.print("humidity = ");
  Serial.print(humidity);
  Serial.print("%");
  Serial.println();
  return humidity;
}

float GetCurrentLight(int pin)
{
  float light = analogRead(pin);
  Serial.print("read light resistor value = ");
  Serial.println(light);
  light = light/1024*100;
  Serial.print("light = ");
  Serial.print(light);
  Serial.print("%");
  Serial.println();
  return light;
}

float GetCurrentTemp(int pin)
{
  float temp;
  float sample_avg=0;
  for(int i = 1; i <= 4096;i++)
  {
    temp = analogRead(pin);
    temp=(5.0 * temp * 100.0) / 1024.0;
    sample_avg = sample_avg + (temp - sample_avg) / i;
  } 

  //temp = temp * 0.48828125;
  //temp=(5.0 * temp * 100.0) / 1024.0;
  
  Serial.print("TEMPRATURE = ");
  Serial.print(sample_avg);
  Serial.print("*C");
  Serial.println();
  return sample_avg;
}

