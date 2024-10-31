
#define WEB_SERVER 0
#if WEB_SERVER

void init_webserver() {
  // Webserver-Routen einrichten
  server.on("/", HTTP_GET, handleRoot);
  server.on("/update", HTTP_POST, handleUpdate);

  // Webserver starten
  server.begin();
  Serial.println("Webserver gestartet");
}
void handleRoot() {
  String html = "<html><body>";
  html += "<h1>ESP8266 Werte Einstellen</h1>";
  html += "<form action=\"/update\" method=\"POST\">";
  html += "gmtOffset_sec: <input type=\"number\" name=\"gmtOffset_sec\" value=\"" + String(gmtOffset_sec) + "\"><br>";
  /*html += "user_day_start_hour: <input type=\"number\" name=\"user_day_start_hour\" value=\"" + String(espPacket.tm_user_day_start_hour) + "\"><br>";
  html += "user_day_end_hour: <input type=\"number\" name=\"user_day_end_hour\" value=\"" + String(espPacket.tm_user_day_end_hour) + "\"><br>";
  html += "user_current_menu: <input type=\"number\" name=\"user_current_menu\" value=\"" + String(espPacket.user_current_menu) + "\"><br>";
  html += "user_thema: <input type=\"number\" name=\"user_thema\" value=\"" + String(espPacket.user_thema) + "\"><br>";*/

  html += "<input type=\"submit\" value=\"Speichern\">";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleUpdate() {
  if (server.hasArg("gmtOffset_sec") /*&& server.hasArg("user_day_start_hour") && server.hasArg("user_day_end_hour") && server.hasArg("user_current_menu") && server.hasArg("user_thema")*/) {

    gmtOffset_sec = server.arg("gmtOffset_sec").toInt();
    /*espPacket.tm_user_day_start_hour = server.arg("user_day_start_hour").toInt();
    espPacket.tm_user_day_end_hour = server.arg("user_day_end_hour").toInt();
    espPacket.user_current_menu = server.arg("user_current_menu").toInt();
    espPacket.user_thema = server.arg("user_thema").toInt();*/

    // Werte im EEPROM speichern
    //EEPROM.write(0, gmtOffset_sec);
   /* EEPROM.write(1, espPacket.tm_user_day_start_hour);
    EEPROM.write(2, espPacket.tm_user_day_end_hour);
    EEPROM.write(3, espPacket.user_current_menu);
    EEPROM.write(4, espPacket.user_thema);*/
    //EEPROM.commit();

    server.send(200, "text/html", "<html><body><h1>Werte gespeichert!</h1><a href=\"/\">Zurück</a></body></html>");
    ini_time();
  } else {
    server.send(400, "text/html", "Fehler: Werte fehlen!");
  }
}
#endif