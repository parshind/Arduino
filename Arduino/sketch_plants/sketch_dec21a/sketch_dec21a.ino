#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <dht11.h>


dht11 DHT;
//Pin 4 of Arduino to Data of DHT11
#define DHT11_PIN 4
#define relayPin 5

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  
   lcd.init();                      // initialize the lcd 
   lcd.backlight();
   // Print a message to the LCD.

}

void loop()
{
  int check;

 
  check = DHT.read(DHT11_PIN);
  Serial.print("DHT11 STATUS - \t");
  
  //check status
  switch (check)
  {
  case DHTLIB_OK:
    Serial.print("OK\n");
    break;
  case DHTLIB_ERROR_CHECKSUM:
    Serial.print("Checksum error\n");
    break;
  case DHTLIB_ERROR_TIMEOUT:
    Serial.print("Timeout error\n");
    break;
  default:
    Serial.print("Unknown error\n");
    break;
  }
  
  //humidity and temperature output
  Serial.print("Humidity is ");
  Serial.print(DHT.humidity,1);
  Serial.print("\n");
  Serial.print("Temperature is ");
  Serial.println(DHT.temperature,1);
  Serial.println("=========================================");
  
  lcd.setCursor(0, 0);
  lcd.print("Hum:");
  lcd.print(DHT.humidity,1);
  lcd.print(",Temp:");
  lcd.print(DHT.temperature,1);
 
  if(DHT.temperature > 30) 
  {
    digitalWrite(relayPin, HIGH);
    Serial.println("Cooler on");
  
  }
  else
  {
    digitalWrite(relayPin, LOW);
    Serial.println("Cooler off");
  }

  delay(1000);
}


