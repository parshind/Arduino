
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <dht11.h>

dht11 DHT;

LiquidCrystal_I2C lcd(0x27,16,2);

bool isHeatOn = false;
int heatPin = A0;

int selectTempPin = A2;


int tempPin = 4;
int temp_threshold = 40;
float temp_delta = 2.0;

void setup()
{
  pinMode(heatPin,OUTPUT);
  pinMode(selectTempPin,INPUT);
  Serial.begin(9600);
  
  lcd.init();                       // Инициализация lcd             
  lcd.backlight();                  // Включаем подсветку
  // Курсор находится в начале 1 строки
  //lcd.print("Hello, world!");       // Выводим текст
  //lcd.setCursor(0, 1);              // Устанавливаем курсор в начало 2 строки
  //lcd.print("zelectro.com.ua");  
}

void Heat()
{
  Serial.println("Heat:");
  if(isHeatOn)
  {
    Serial.println("   Heat on");
    digitalWrite(heatPin,HIGH);
  }
  else
  {
    Serial.println("   Heat off");
    digitalWrite(heatPin,LOW);
  }
}

float GetThresholdTemp(void)
{
//  return 29;
  int v = analogRead(selectTempPin);
  Serial.print("THRESHOLD VAL = ");
  Serial.print(v);
  Serial.println();

  float val = (float)analogRead(selectTempPin)/921*50;
  Serial.print("THRESHOLD TEMPRATURE = ");
  Serial.print(val);
  Serial.print("*C");
  Serial.println();
  return val;
  
}
float GetCurrentTemp(void)
{
    int chk;
  Serial.print("DHT11, \t");
  chk = DHT.read(tempPin);    // READ DATA
  switch (chk){
    case DHTLIB_OK:  
                Serial.print("OK,\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Time out error,\t"); 
                break;
    default: 
                Serial.print("Unknown error,\t"); 
                break;
  }
  
  
  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);

  return DHT.temperature;
  
}


void loop()
{
  int temp = (int)GetCurrentTemp();
  temp_threshold = (int)GetThresholdTemp();
  
  if(isHeatOn && temp > temp_threshold + temp_delta ) isHeatOn  = false;
  else if(!isHeatOn && temp <= temp_threshold ) isHeatOn  = true;
  
  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print((int)temp);
  lcd.print("*C");
  
  lcd.setCursor(0, 1);
  lcd.print("B: ");
  lcd.print((int)temp_threshold);
  lcd.print("*C");
  
  lcd.setCursor(10, 0);
  if(isHeatOn) lcd.print("H: +");
  else lcd.print("H: -");
  
  Heat();
  delay(5000);
  
}


