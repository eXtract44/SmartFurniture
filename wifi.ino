

const char* ssid       = "WiFi";
const char* password   = "Lokomotive132";
#define CONNECT_IN 720 //1 tick 1sec

void ini_wifi(){
   WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      Serial.printf("Connecting to %s ", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    Serial.println();
    Serial.print("Verbunden! IP-Adresse: ");
    Serial.println(WiFi.localIP());
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

void init_webserver() {
  // Webserver-Routen einrichten
  server.on("/", HTTP_GET, handleRoot);
  server.on("/update", HTTP_POST, handleUpdate);

  // Webserver starten
  server.begin();
  Serial.println("Webserver gestartet");
}
