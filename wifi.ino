

const char* ssid       = "WiFi";
const char* password   = "Lokomotive132";
#define CONNECT_IN 720 //1 tick 1sec

void ini_wifi(){
      WiFi.begin(ssid, password);
      Serial.printf("Connecting to %s ", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   Serial.println(" CONNECTED");
}

void check_wifi(){
  static uint16_t connect_cnt = 0;
  ++connect_cnt;
  if(connect_cnt > CONNECT_IN){   
    if(WiFi.status() != WL_CONNECTED)
    WiFi.begin(ssid, password);  
    connect_cnt=0;
  }
}