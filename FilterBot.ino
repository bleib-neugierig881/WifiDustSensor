/*
  EchoBot

  This example shows how to program a Telegram Bot
  that echoes your messages.

  For a step-by-step tutorial visit:
  https://create.arduino.cc/projecthub/Arduino_Genuino/telegram-bot-library-ced4d4

  In oreder to make the bot more reliable in the long run we suggest using a watchdog
  The Adafruit_SleepyDog is a handy library that will reset the board if something goes wrong

  Updated 29 May 2016
  by Tommaso Laterza
  Updated 13 February 2018
  by Tommaso Laterza

  This example code is in the public domain.

*/

#include "arduino_secrets.h"
#include <WiFi101.h>
#include <SPI.h>
#include <TelegramBot.h> 
#include <Adafruit_SleepyDog.h>
#define        COV_RATIO                       0.2            //ug/mmm / mv
#define        NO_DUST_VOLTAGE                 450            //mv
#define        SYS_VOLTAGE                     5000           

const int iled = 4;                                            //drive the led of sensor
const int vout = 0;                                            //analog input

float density, voltage;
int   adcvalue, count, trigger;

// Initialize Wifi connection to the router
char ssid[] = SECRET_SSID;             // your network SSID (name)
char pass[] = SECRET_PASS;           // your network key

// Initialize Telegram BOT
const char BotToken[] = SECRET_BOT_TOKEN;
const char MessageID[] = SECRET_MESSAGE_ID;

WiFiSSLClient client;
TelegramBot bot (BotToken, client);

int Filter(int m)
{
  static int flag_first = 0, _buff[10], sum;
  const int _buff_max = 10;
  int i;
  
  if(flag_first == 0)
  {
    flag_first = 1;

    for(i = 0, sum = 0; i < _buff_max; i++)
    {
      _buff[i] = m;
      sum += _buff[i];
    }
    return m;
  }
  else
  {
    sum -= _buff[0];
    for(i = 0; i < (_buff_max - 1); i++)
    {
      _buff[i] = _buff[i + 1];
    }
    _buff[9] = m;
    sum += _buff[9];
    
    i = sum / 10.0;
    return i;
  }
}


void setup() {

  pinMode(iled, OUTPUT);
  digitalWrite(iled, LOW);                                     //iled default closed
  pinMode(vout,INPUT);
  Serial.begin(115200);
  while (!Serial) {} // Wait for the Serial monitor to be opened
  delay(3000);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //bot.begin();
  //Watchdog.enable(10000); // Timer set to 10 sec;
  count = 0;
}
  
void loop() {

  //analogWrite(iled,230);
  Serial.println("On");
  adcvalue = analogRead(vout);
  Serial.println("Off");
  voltage = (SYS_VOLTAGE / 4096.0) * adcvalue * 11;
  if(voltage >= NO_DUST_VOLTAGE)
  {
    voltage -= NO_DUST_VOLTAGE;
    
    density = voltage * COV_RATIO;
  }
  else
    density = 0;
  /*Watchdog.reset(); //if function is not called in 10 sec, the board will reset itself
  message m = bot.getUpdates(); // Read new messages
  if (count == 200 && density > 120 && trigger == 0){
    bot.sendMessage(MessageID, "It's dusty! Turn on the filter.");
    count = 0;
    trigger = 1;
  }
  count ++;
  if (count == (200*60)){
    count = 0;
    trigger = 0;
  }*/
}
