
#include <WiFi.h>
#include "time.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>


#define TIME_DAY_START 6
#define TIME_DAY_END 23
#define UPDATE_TIME_DISPLAY 30  //in sec
#define line_1_start_y 1
#define line_2_offser_y 6

bool day_activated = false;
unsigned long previousMillis = 0;


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
void update_day_after_night(){
  draw_float_string(get_temperature_aht(), 2, line_1_start_y);
  draw_int_string(get_humidity_aht(), 2, line_1_start_y + line_2_offser_y);
  draw_int_string(get_mday(), 1, line_1_start_y + line_2_offser_y * 4);
  draw_int_string(get_mon(), 9, line_1_start_y + line_2_offser_y * 4);
  draw_float_string(get_temperature_esp(), 2, line_1_start_y + line_2_offser_y * 5);
  draw_int_string(get_humidity_esp(), 2, line_1_start_y + line_2_offser_y * 6);
}
void buttons_handler(){
  static uint16_t cnt_btn_1 = 0;
  if(read_button_1()){
    cnt_btn_1++;   
    if(cnt_btn_1>4){
      update_day_after_night();
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
  static uint16_t cnt = UPDATE_TIME_DISPLAY+1;
  cnt++;
  if (cnt > UPDATE_TIME_DISPLAY) {
    cnt = 0;
    if (get_hour() > TIME_DAY_START && get_hour() < TIME_DAY_END) {
      if(day_activated == false){
        update_day_after_night();
        day_activated = true;
      }
      update_day();
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
