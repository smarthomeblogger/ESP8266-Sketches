#include <ESP8266WiFi.h>

const char* SSID = "NETZWERKNAME";
const char* PSK = "PASSWORT";

WiFiServer server(80);

void setup() {
  pinMode(13, OUTPUT);

  initWiFi();
}
 
void loop() {
  WiFiClient client = server.available();
  if(!client){
    return;
  }

  if(WiFi.status() != WL_CONNECTED){
    initWiFi();
  }

  String request = client.readStringUntil('\r');
  client.flush();

  if(request==""){
    client.stop();
    return;
  }

  //GPIOS schalten
  if(request.indexOf("mode=1")>=0){
    digitalWrite(13, 1);
  }
  else if(request.indexOf("mode=0")>=0){
    digitalWrite(13, 0);
  }

  //Ausgabe erzeugen
  String output;
  output += "<h1>LED schalten</h1>";
  output += "<form action=\"\" method=\"GET\">";
  output += "<button name=\"mode\" value=\"1\">AN</button>";
  output += "<br><br>";
  output += "<button name=\"mode\" value=\"0\">AUS</button>";
  output += "</form>";

  client.print(output);
  
  client.stop();
}

void initWiFi(){
  WiFi.begin(SSID, PSK);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }

  server.begin();
}