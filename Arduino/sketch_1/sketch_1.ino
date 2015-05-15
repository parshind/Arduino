
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

bool isHeatOn = false;
int heatPin = A0;
int tempPin = A1;
int selectTempPin = A2;

int ledPin=A3; //13

int temp_threshold = 40;
float temp_delta = 2.0;

void setup()
{
  pinMode(heatPin,OUTPUT);
  pinMode(tempPin,INPUT);
  
  
  pinMode(ledPin,OUTPUT);
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
    digitalWrite(ledPin,HIGH);
    digitalWrite(heatPin,HIGH);
  }
  else
  {
    Serial.println("   Heat off");
    digitalWrite(ledPin,LOW);
    digitalWrite(heatPin,LOW);
  }
}

float GetThresholdTemp(void)
{
  //return 20;
  float val = (float)analogRead(selectTempPin)/1024*30 + 10;
  Serial.print("THRESHOLD TEMPRATURE = ");
  Serial.print(val);
  Serial.print("*C");
  Serial.println();
  return val;
  
}
float GetCurrentTemp(void)
{
  
   float temp;
  float sample_avg=0;
  for(int i = 1; i <= 4096;i++)
  {
    temp = analogRead(tempPin);
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


