 #include <Arduino.h>
 #include <WiFi.h>
 #include <WiFiClientSecure.h>
 #include <UniversalTelegramBot.h>
 #include <Wire.h>
 #include <RTClib.h>

 RTC_DS1307 rtc;
 char days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
 const char* ssid = "Boteon MikroTik";
 const char* password = "Boteon12345";
 const char* BotToken = "971743817:AAGZWg4KVHTxM2WncDje0ja9YZg-AMKsEg8";

 WiFiClientSecure net_ssl;
 UniversalTelegramBot bot(BotToken, net_ssl);

 int alarmHour = 0;
 int alarmMin = 0;
 bool alarmIsSet = false;
 const String alarmCmd = "/alarm";
 const int buz = 23;
 
 bool FinAlarmCmd(String input)
 {
     bool res = false;
     char buf[7];
     input.toCharArray(buf, 7);
     Serial.print("Parse command:");
     Serial.println(buf);
     if(alarmCmd == buf) res = true;
     else res = false;

     char timeBuf[6];
     input.toCharArray(buf,6, 7);
     Serial.print("Parse time:");
     Serial.println(timeBuf);
     if(res)
     {
         String hour;
         if(timeBuf[0] != '0')
            hour += timeBuf[1];
        alarmHour = hour.toInt();

        String min;
         if(timeBuf[3] != '0')
            min += timeBuf[4];
        alarmHour = min.toInt();

     }
     
 }

 void setup() 
{
    pinMode(buz, OUTPUT);
 Serial.begin(115200);
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 while(WiFi.status() != WL_CONNECTED)
 {
     delay(1000);
     Serial.print('.');
 }
 Serial.println("Connected");
 if(!rtc.isrunning())
 {
     Serial.println("RTC not running");
     rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
 }
}

 void loop() 
{
  if(bot.getUpdates(bot.last_message_received + 1))
  {
      String chat_id = String(bot.messages[0].chat_id);
      String text = bot.messages[0].text;
      alarmIsSet = FinAlarmCmd("/alarm 04:45");
      if(alarmIsSet)
      {
          Serial.println("alarm is setted");
          bot.sendMessage(chat_id, "alarm setted");
      }
  }
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
    Serial.print(now.month(), DEC);
  Serial.print('/');
    Serial.print(now.day(), DEC);
  Serial.print('/');
    Serial.print(days[now.dayOfTheWeek()]);
  Serial.print('/');
    Serial.print(now.hour(), DEC);
  Serial.print(':');
    Serial.print(now.minute(), DEC);
  

  if(alarmIsSet)
  {
      if(now.hour() == alarmHour && now.minute() == alarmMin)
      {
          Serial.println("ALARM");
          digitalWrite(buz,HIGH);
      }
  }
}