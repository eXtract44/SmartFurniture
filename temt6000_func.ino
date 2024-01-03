
void read_brightness() {
  static uint16_t cnt = 0;
  cnt++;
  if(cnt > 60){
  cnt= 0; 
  uint16_t light = analogRead(A0);
  light = map(light, 0, 4095, 1, 40); 
  strip.setBrightness(light);
  strip.show();
  }
}
void set_brightness_1() {
  strip.setBrightness(1);
}