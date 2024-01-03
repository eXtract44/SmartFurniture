


#define TOUCH_PIN   5

void ini_buttons(){
pinMode(TOUCH_PIN, INPUT);
}
bool read_button_1(){
     if (digitalRead(TOUCH_PIN) == true) return true;
     else return false;  
}
