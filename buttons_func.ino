
#define TOUCH_PIN   5

void ini_buttons(){
pinMode(TOUCH_PIN, INPUT);
}
bool read_button_1(){
     return digitalRead(TOUCH_PIN);  
}
