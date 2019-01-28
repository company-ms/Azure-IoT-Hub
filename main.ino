#include <ESP8266WiFi.h>
#include <time.h>
#include <coredecls.h>
#include "iothub.h"
#include "DHTesp.h" // библиотека для температуры
#define DHTPIN 5 // пин датчика температуры
#include "TimeLib.h"
const char *wifiName = "***";
const char *wifiPassword = "*****";
unsigned long timing = 0; //Время ДЛЯ задержки (не счетчик)
unsigned long timingForVCC; // таймер для измерения vcc

void onTimeRetrieved();


DHTesp dht;//переименовываем тип датчика DHT для удобства
ADC_MODE(ADC_VCC);//для измерения нарпяжения 

void setup()
{   
    WiFi.setAutoReconnect(true); // отключаем/включаем автоРеконнект
    ESP.wdtEnable(1000);
    Serial.begin(74880);
    settimeofday_cb(onTimeRetrieved);
    configTime(0, 0, "pool.ntp.org");
    Serial.println("reason reboot: ");
    Serial.println(ESP.getResetReason()); 
    Serial.print("Size Cketch: ");
    Serial.println(ESP.getSketchSize());
    WiFi.begin(wifiName, wifiPassword);
    dht.setup(DHTPIN); //инициализируем DHT на 13 пине
    // other initialization here
}

void onTimeRetrieved()
{
  Serial.printf("Current timestamp: %d\n", time(NULL));
  initHubConnection();
}

void Cheking()
{
if (millis() - timingForVCC > 10000)//значение паузы 
      { 
      timingForVCC = millis(); 
      Serial.print ("VCC ");
      Serial.println (ESP.getVcc());
      Serial.print ("Size help: ");
      Serial.println (ESP.getFreeHeap());
      Serial.print ("Status Wifi: ");
      Serial.println (WiFi.waitForConnectResult());
      }
}

void telemetria()
{
  if (millis() - timing > 10000 && WiFi.status() == WL_CONNECTED)
  {
  timing = millis();
  float tem = dht.getHumidity();
  float hum = dht.getTemperature();
  String strMsg = "Temper ";
  strMsg += tem;
  strMsg += " Humi ";
  strMsg += hum;
  char msg[255];
  strMsg.toCharArray(msg, 255);
  Serial.println(msg);
  sendHubEvent(msg);
  }
}


void loop()
{
  
    runHubConnection();
    telemetria(); 
   
    Cheking();
    delay(20);
}
