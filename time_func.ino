#define COLOR_TIME_COLON 255, 255, 0
#define COLOR_TIME_COLON_BLINK 255, 255, 255

const char* ntpServer = "pool.ntp.org";  // time
long gmtOffset_sec = 3600;               //3600 sommer / 0 winter   ,,,,,,,,,ab 26.03 bis 29.10 - 3600 time
const int daylightOffset_sec = 3600;     //time

struct tm timeinfo;

String receivedMessage;

void read_uart() {
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == '\n') {
      Serial.println(receivedMessage);  // Print the received message in the Serial monitor
      //receivedMessage = "";  // Reset the received message
    } else {
      receivedMessage += receivedChar;  // Append characters to the received message
    }
  }
}
void set_time_offset() {
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == '?') {  //start dialog menu
      Serial.println("to set winter or sommer time, send:");
      Serial.println("w - winter_0_sec, s - sommer_3600_sec");
      Serial.print("Offset in sec now: ");
      Serial.println(gmtOffset_sec);
    } else if (receivedChar == 's') {
      gmtOffset_sec = 3600;
      Serial.println("now is sommer time !");
      Serial.print("Offset in sec now: ");
      Serial.println(gmtOffset_sec);
      ini_time();
      get_hour();
    } else if (receivedChar == 'w') {
      gmtOffset_sec = 0;
      Serial.println("now is winter time !");
      Serial.print("Offset in sec now: ");
      Serial.println(gmtOffset_sec);
      ini_time();
      get_hour();
    }
  }
}
void ini_time() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}
void read_time() {
  getLocalTime(&timeinfo);
}
uint8_t get_hour() {
  return timeinfo.tm_hour;
}
uint8_t get_min() {
  return timeinfo.tm_min;
}
uint8_t get_mday() {
  return timeinfo.tm_mday;
}
uint8_t get_mon() {
  return timeinfo.tm_mon + 1;
}
uint8_t get_sec() {
  return timeinfo.tm_sec;
}


void printLocalTime() {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
void print_time_colon(const uint8_t x, const uint8_t y) {
  static uint8_t cnt = 0;
  cnt++;
  if (cnt == 1) {
    draw_pixel_now(x, y + 1, COLOR_TIME_COLON);
    draw_pixel_now(x, y + 3, COLOR_TIME_COLON);
  }
  if (cnt == 2) {
    draw_pixel_now(x, y + 1, COLOR_TIME_COLON_BLINK);  //1, 19
    draw_pixel_now(x, y + 3, COLOR_TIME_COLON_BLINK);
    cnt = 0;
  }
}
