
#include <WiFi.h>
#include "time.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>


#define TIME_DAY_START 6
#define TIME_DAY_END 23
#define UPDATE_TIME_DISPLAY 10  //in sec
#define line_1_start_y 1
#define line_2_offser_y 6

bool day_activated = true;
unsigned long previousMillis = 0;

/*void ini_aht(void);
void ini_sgp(void);
void ini_wifi(void);
void ini_ws2812b(void);
void ini_time(void);
void ini_buttons(void);
void clean_line(const uint8_t y);
void draw_temperature_aht(float number, const uint8_t x, const uint8_t y);
void draw_humidity_aht(float number, const uint8_t x, const uint8_t y);
float get_temperature_aht(void);
uint8_t get_humidity_aht(void);
void draw_point(const uint8_t x, const uint8_t y);
void draw_uint_string(uint16_t number, const uint8_t x, const uint8_t y);
uint16_t get_co2_sgp(void);
void draw_hour_esp(uint8_t number, const uint8_t x, const uint8_t y);
void draw_min_esp(uint8_t number, const uint8_t x, const uint8_t y);
void draw_mday_esp(uint8_t number, const uint8_t x, const uint8_t y);
void draw_mon_esp(uint8_t number, const uint8_t x, const uint8_t y);
uint8_t get_hour(void);
uint8_t get_min(void);
uint8_t get_mday(void);
uint8_t get_mon(void);
void draw_temperature_esp(float number, const uint8_t x, const uint8_t y);
void draw_humidity_esp(uint8_t number, const uint8_t x, const uint8_t y);
float get_temperature_esp(void);
uint8_t get_humidity_esp(void);
void draw_char(const char text, const uint8_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b);
void read_brightness(void);
void set_brightness_1(void);
void read_aht(void);
void read_sgp(void);
void check_wifi(void);
void read_time(void);
void read_wetter_data(void);
void print_time_colon(const uint8_t x, const uint8_t y);
bool read_button_1(void);*/

void setup() {
  Serial.begin(9600);  //start UART
  ini_aht();
  ini_sgp();
  ini_wifi();
  ini_ws2812b();
  ini_time();
  ini_buttons();
  //update_day();
}
void update_day() {
  draw_temperature_aht(get_temperature_aht(), 2, line_1_start_y);
  draw_point(13, line_1_start_y);
  draw_humidity_aht(get_humidity_aht(), 2, line_1_start_y + line_2_offser_y);
  draw_char('%', 10, line_1_start_y + line_2_offser_y, 255, 255, 90);
  draw_uint_string(get_co2_sgp(), 1, line_1_start_y + line_2_offser_y * 2);
  draw_hour_esp(get_hour(), 1, line_1_start_y + line_2_offser_y * 3);
  draw_min_esp(get_min(), 8, line_1_start_y + line_2_offser_y * 3);
  draw_mday_esp(get_mday(), 1, line_1_start_y + line_2_offser_y * 4);
  draw_point(8, line_1_start_y + line_2_offser_y * 4 + 4);
  draw_mon_esp(get_mon(), 9, line_1_start_y + line_2_offser_y * 4);
  draw_temperature_esp(get_temperature_esp(), 2, line_1_start_y + line_2_offser_y * 5);
  draw_point(13, line_1_start_y + line_2_offser_y * 5);
  draw_humidity_esp(get_humidity_esp(), 2, line_1_start_y + line_2_offser_y * 6);
  draw_char('%', 10, line_1_start_y + line_2_offser_y * 6, 255, 255, 90);
  read_brightness();
}
void update_night() {
  clean_line(line_1_start_y);
  clean_line(line_1_start_y + line_2_offser_y);
  draw_uint_string(get_co2_sgp(), 1, line_1_start_y + line_2_offser_y * 2);
  draw_hour_esp(get_hour(), 1, line_1_start_y + line_2_offser_y * 3);
  draw_min_esp(get_min(), 8, line_1_start_y + line_2_offser_y * 3);
  clean_line(line_1_start_y + line_2_offser_y * 4);
  clean_line(line_1_start_y + line_2_offser_y * 5);
  clean_line(line_1_start_y + line_2_offser_y * 6);
  set_brightness_1();
}
void buttons_handler() {
  static uint16_t cnt_btn_1 = 0;
  if (read_button_1()) {
    cnt_btn_1++;
    if (cnt_btn_1 > 4) {
       day_activated = false;
       update_day();
       delay(20000);
      cnt_btn_1 = 0;
    }
  }
}
void refresh_all_data() {  //1 sec
  read_aht();
  read_sgp();
  check_wifi();
  read_time();
  read_wetter_data();
  print_time_colon(7, line_1_start_y + line_2_offser_y * 3);
  static uint16_t cnt = 0;
  cnt++;
  if (cnt > UPDATE_TIME_DISPLAY) {
    cnt = 0;
    if (get_hour() > TIME_DAY_START && get_hour() < TIME_DAY_END) {
      update_day();
      day_activated = true;
    } else {     
      update_night();
      day_activated = false;
    }
  }
}
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    refresh_all_data();
    buttons_handler();
  }
}
void debug() {
  // read_temperature(2, 1);
  // read_humidity(2, 7);
  // Serial.print(temperature.temperature);
  //Serial.println("indoor *c");
  //Serial.print(humidity.relative_humidity);
  //Serial.println("indoor H%");
  //  mySensor.measureAirQuality();
  //  Serial.print(mySensor.CO2);
  //  Serial.println("CO2 ppm");
  //  Serial.print(mySensor.TVOC);
  //  Serial.println("TVOC ppb");
  //  printLocalTime();
  //  Serial.print(mySensor.TVOC);
  //  Serial.println(" ppb");
  //  Serial.print(analogRead(A0));
  //  Serial.println("LUX");
}
