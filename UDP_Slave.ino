#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#include "helpers.h"
#include "global.h"

void setup() {
  Serial.begin(115200);
  delay(500);

  WiFiUDP Udp;
  unsigned int localUdpPort = 8899;  // local port to listen on
  char incomingPacket[255];  // buffer for incoming packets
  String configdata="";
  String comedata="";

  while(1){//配置wifi
    if(Serial.available()>0){
      comedata = Serial.readStringUntil('?');
      if(comedata=="ok"){//如果接收到了ok则说明configdata配置的值是正确的
        if(ConfigESP(configdata)){//存储wifi配置
          ConfigureWifi();//配置正确则连接wifi
          break;
        }//若没有配置正确则继续接收configdata
      }else{//一直配置configdata
        char maxRange[32][32];int maxRanges = split(maxRange,string2char(comedata),":");
        if(maxRanges==3){
          configdata = comedata;
          Serial.print(comedata);
        }
      }
    }  
  }

  if(WiFi.isConnected()){//wifi连接成功
    Udp.begin(localUdpPort);
    Serial.print("WTF");
  }

  while(WiFi.isConnected()){
    if(Serial.available()>0){
       comedata = Serial.readStringUntil('?');
        char maxRange[32][32];int maxRanges = split(maxRange,string2char(comedata),".");
        if(maxRanges==4){
          config.TargetIP[0] = atol(maxRange[0]);
          config.TargetIP[1] = atol(maxRange[1]);
          config.TargetIP[2] = atol(maxRange[2]);
          config.TargetIP[3] = atol(maxRange[3]);
          Serial.print("update target ip success!!");
        }else{
          if(config.TargetIP[0]==NULL){
              Serial.println("err");
          }else{
             IPAddress targetIP(config.TargetIP[0],config.TargetIP[1],config.TargetIP[2],config.TargetIP[3]);
             Udp.beginPacket(targetIP,8899);
             Udp.write(comedata.c_str());
             Udp.endPacket();
          }
        }
    }

    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      Serial.println(incomingPacket);
    }
    
  }

  Serial.print("MD");
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
