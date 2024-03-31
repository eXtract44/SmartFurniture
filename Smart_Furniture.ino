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
  set_time_offset();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    //debug_uart();
    refresh_all_data();
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