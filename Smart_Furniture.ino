#include <WiFi.h>
#include "time.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>

uint8_t time_day_start = 6;
uint8_t time_day_end = 23;
uint8_t display_update = 10;  // in sec


#define line_1_start_y 1 //1 raw
#define line_2_offset_y 6

bool day_activated = true;
unsigned long previousMillis = 0;


void setup() {
  Serial.begin(9600);  //start UART
  ini_aht();
  ini_sgp();
  ini_wifi();
  ini_ws2812b();
  ini_time();
  ini_buttons();
  Serial.println("init finish");
}
void update_day() {
  draw_temperature_aht(get_temperature_aht(), 2, line_1_start_y);
  draw_point(13, line_1_start_y);
  draw_humidity_aht(get_humidity_aht(), 2, line_1_start_y + line_2_offset_y);
  draw_char('%', 10, line_1_start_y + line_2_offset_y, 255, 255, 90);
  draw_uint_sgp30(get_co2_sgp(), 1, line_1_start_y + line_2_offset_y * 2);
  draw_hour_esp(get_hour(), 1, line_1_start_y + line_2_offset_y * 3);
  draw_min_esp(get_min(), 8, line_1_start_y + line_2_offset_y * 3);
  draw_mday_esp(get_mday(), 1, line_1_start_y + line_2_offset_y * 4);
  draw_point(8, line_1_start_y + line_2_offset_y * 4 + 4);
  draw_mon_esp(get_mon(), 9, line_1_start_y + line_2_offset_y * 4);
  draw_temperature_esp(get_temperature_esp(), 2, line_1_start_y + line_2_offset_y * 5);
  draw_point(13, line_1_start_y + line_2_offset_y * 5);
  draw_humidity_esp(get_humidity_esp(), 2, line_1_start_y + line_2_offset_y * 6);
  draw_char('%', 10, line_1_start_y + line_2_offset_y * 6, 255, 255, 90);
  read_brightness();
}
void update_night() {
  clean_line(line_1_start_y);
  clean_line(line_1_start_y + line_2_offset_y);
  draw_uint_sgp30(get_co2_sgp(), 1, line_1_start_y + line_2_offset_y * 2);
  draw_hour_esp(get_hour(), 1, line_1_start_y + line_2_offset_y * 3);
  draw_min_esp(get_min(), 8, line_1_start_y + line_2_offset_y * 3);
  clean_line(line_1_start_y + line_2_offset_y * 4);
  clean_line(line_1_start_y + line_2_offset_y * 5);
  clean_line(line_1_start_y + line_2_offset_y * 6);
  set_brightness_1();
}

void refresh_all_data() {  //1 sec
  read_aht();
  read_sgp();
  check_wifi();
  read_time();
  read_wetter_data();
  print_time_colon(7, line_1_start_y + line_2_offset_y * 3);
  static uint16_t cnt = 0;
  cnt++;
  if (cnt > display_update) {
    cnt = 0;
    if (get_hour() > time_day_start && get_hour() < time_day_end) {
      update_day();
      day_activated = true;
    } else {
      if (key_handler() == false) {
        update_night();
        day_activated = false;
      } else if(key_handler() == true){
        update_day();
        day_activated = true;
      }
    }
  }
}
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    //debug_uart();
    refresh_all_data();
    set_time_offset();
  }
}
void debug_uart() {
debug_uart_aht();
debug_btn();
debug_uart_sgp();
debug_temt6000();
printLocalTime();
debug_uart_esp();
}

/*
  void ini_aht(void);
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
  bool read_button_1(void);
*/
