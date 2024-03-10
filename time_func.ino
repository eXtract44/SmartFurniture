#define COLOR_TIME_COLON 255, 255, 0
#define COLOR_TIME_COLON_BLINK 255, 255, 255

const char* ntpServer = "pool.ntp.org"; // time
long  gmtOffset_sec = 3600;//3600 sommer / 0 winter   ,,,,,,,,,ab 26.03 bis 29.10 - 3600 time
const int   daylightOffset_sec = 3600;//time

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
  static uint8_t cnt = 0;
  cnt++;
  if (cnt > 10) {
    cnt = 0;
    if (receivedMessage == "w") {
      receivedMessage = "";  // Reset the received message
      gmtOffset_sec = 3600;
      ini_time();
      get_hour();
    } else if (receivedMessage == "s") {
      receivedMessage = "";  // Reset the received message
      gmtOffset_sec = 0;
      ini_time();
      get_hour();
    }
    Serial.println("w - winter, s - sommer");
    if (gmtOffset_sec == 3600) { //winter
      Serial.println("now is winter time set");
    } else if (gmtOffset_sec = 0) { // sommer
      Serial.println("now is sommer time set");
    }
    Serial.print("Offset in sec: "); Serial.print(gmtOffset_sec);
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
    draw_pixel_now(x, y + 1, COLOR_TIME_COLON_BLINK); //1, 19
    draw_pixel_now(x, y + 3, COLOR_TIME_COLON_BLINK);
    cnt = 0;
  }
}
