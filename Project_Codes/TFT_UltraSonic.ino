#include <TouchScreen.h> //touch library
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library
#include <stdlib.h>
#include <stdio.h>

LCDWIKI_KBV my_lcd(ILI9341,A3,A2,A1,A0,A6); //model,cs,cd,wr,rd,reset

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

//param calibration from kbv
#define TS_MINX 911
#define TS_MAXX 117

#define TS_MINY 87 
#define TS_MAXY 906

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define BLACK        0x0000  /*   0,   0,   0 */
#define BLUE         0x001F  /*   0,   0, 255 */
#define RED          0xF800  /* 255,   0,   0 */
#define GREEN        0x07E0  /*   0, 255,   0 */
#define CYAN         0x07FF  /*   0, 255, 255 */
#define MAGENTA      0xF81F  /* 255,   0, 255 */
#define YELLOW       0xFFE0  /* 255, 255,   0 */
#define WHITE        0xFFFF  /* 255, 255, 255 */
#define NAVY         0x000F  /*   0,   0, 128 */
#define DARKGREEN    0x03E0  /*   0, 128,   0 */
#define DARKCYAN     0x03EF  /*   0, 128, 128 */
#define MAROON       0x7800  /* 128,   0,   0 */
#define PURPLE       0x780F  /* 128,   0, 128 */
#define OLIVE        0x7BE0  /* 128, 128,   0 */
#define LIGHTGREY    0xC618  /* 192, 192, 192 */
#define DARKGREY     0x7BEF  /* 128, 128, 128 */
#define ORANGE       0xFD20  /* 255, 165,   0 */
#define GREENYELLOW  0xAFE5  /* 173, 255,  47 */
#define PINK         0xF81F  /* 255,   0, 255 */

uint16_t color_mask[] = {0xF800,0xFFE0,0x07E0,0x07FF,0x001F,0xF81F}; //color select

int y_rotate;
int x_rotate;

const int sensorPin_1 = A4;
float sensorValue_1;
float voltageOut_1;
float temperatureK_1;
float temperatureC_1;

const int sensorPin_2 = A5;
float sensorValue_2;
float voltageOut_2;
float temperatureK_2;
float temperatureC_2;

int threshold;
boolean T1 = false;
boolean T2 = false;
boolean Timer_Hold = false;
int relay_pin_1 = 10; 
int relay_pin_2 = 11;
int relay_pin_3 = 12;

String CHminute;
String CHsecond;
String minute_10;
String second_10;
int minute;
int second;
unsigned long First_digit = 0;
unsigned long Second_digit = 0;
unsigned long Third_digit = 0;
unsigned long Fourth_digit = 0;
unsigned long Fifth_digit = 0;
int current_digit = 0;

void show_string(uint8_t *str,int16_t x,int16_t y,uint8_t csize,uint16_t fc, uint16_t bc,boolean mode)
{
    my_lcd.Set_Text_Mode(mode);
    my_lcd.Set_Text_Size(csize);
    my_lcd.Set_Text_colour(fc);
    my_lcd.Set_Text_Back_colour(bc);
    my_lcd.Print_String(str,x,y);
}

void show_main_menu(void)
{
   my_lcd.Set_Draw_color(RED);
   my_lcd.Fill_Round_Rectangle(15, 5, 75, 34, 5);
   my_lcd.Fill_Round_Rectangle(95, 5, 150, 34, 5);
   my_lcd.Fill_Round_Rectangle(170, 5, 225, 34, 5);
   my_lcd.Fill_Round_Rectangle(245, 5, 305, 34, 5);
   my_lcd.Set_Draw_color(MAROON);
   my_lcd.Fill_Round_Rectangle(38, 50, 75, 79, 5);
   my_lcd.Fill_Round_Rectangle(95, 50, 130, 79, 5);
   my_lcd.Set_Draw_color(GREEN);
   my_lcd.Fill_Round_Rectangle(10, 180, 75, 230, 5);
   my_lcd.Set_Draw_color(BLUE);
   my_lcd.Fill_Round_Rectangle(95, 180, 160, 230, 5);
   show_string("40",28,10,3,WHITE, BLACK,1);
   show_string("50",107,10,3,WHITE, BLACK,1);
   show_string("60",181,10,3,WHITE, BLACK,1);
   show_string("70",260,10,3,WHITE, BLACK,1);
   show_string("+",50,55,3,WHITE, BLACK,1);
   show_string("-",106,55,3,WHITE, BLACK,1);
   show_string("000:00",32,90,3,GREEN, BLACK,1);
   show_string("ON",25,195,3,WHITE, BLACK,1);
   show_string("OFF",104,195,3,WHITE, BLACK,1);
   show_string("Select a digit to change",5,125,1,YELLOW, BLACK,1);
   show_string("T1 --.--",155,80,3,WHITE, BLACK,1);
   //show_string("o",255,52,1,WHITE, BLACK,1);
   //show_string("C",264,55,2,WHITE, BLACK,1);
   show_string("T2 --.--",155,120,3,WHITE, BLACK,1);
   //show_string("o",255,92,1,WHITE, BLACK,1);
   //show_string("C",264,95,2,WHITE, BLACK,1);
   my_lcd.Set_Draw_color(WHITE);
   my_lcd.Draw_Round_Rectangle(95, 180, 160, 230, 5);
 }

void setup(void){
  Serial.begin(9600);
  my_lcd.Init_LCD();
  Serial.println(my_lcd.Read_ID(), HEX);
  my_lcd.Fill_Screen(BLACK);
  my_lcd.Set_Rotation(1);
  show_main_menu();
  pinMode(13, OUTPUT);

  my_lcd.Set_Draw_color(WHITE);
  my_lcd.Draw_Round_Rectangle(15, 5, 75, 34, 5);
  threshold = 40;
        
  pinMode(sensorPin_1, INPUT);
  pinMode(sensorPin_2, INPUT);
  pinMode(relay_pin_1, OUTPUT);
  pinMode(relay_pin_2, OUTPUT);
  pinMode(relay_pin_3, OUTPUT);

  //cli();
  /* Ayarlamaların yapılabilmesi için öncelikle kesmeler durduruldu */

  /* Timer1 kesmesi saniyede bir çalışacak şekilde ayarlanacaktır (1 Hz)*/
  //TCCR1A = 0;
  //TCCR1B = 0;
  //TCNT1  = 0;
  //OCR1A = 15624;
  /* Bir saniye aralıklar için zaman sayıcısı ayarlandı */
  //TCCR1B |= (1 << WGM12);
  /* Adımlar arasında geçen süre kristal hızının 1024'e bölümü olarak ayarlandı */
  //TCCR1B |= (1 << CS12) | (1 << CS10);
  //TIMSK1 |= (1 << OCIE1A);
  /* Timer1 kesmesi aktif hale getirildi */
  //sei();
}

/* Arduino otomatik olarak her saniye aşağıdaki fonksiyonu çalıştıracaktır */
/*ISR(TIMER1_COMPA_vect){
  my_lcd.Set_Text_Mode(1);
  my_lcd.Set_Text_Size(3);
  my_lcd.Set_Text_colour(GREEN);
  my_lcd.Set_Text_Back_colour(BLACK);
  First_digit--;
  my_lcd.Set_Draw_color(BLACK);
  my_lcd.Fill_Rectangle(32, 90, 46, 110);
  my_lcd.Print_Number_Int(First_digit, 32, 90, 0, ' ', 10);
}*/

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop(){
comme:
  digitalWrite(relay_pin_1, HIGH);
  digitalWrite(relay_pin_2, HIGH);
  digitalWrite(relay_pin_3, HIGH);
  
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) 
  {
      x_rotate = map(p.y, TS_MINY, TS_MAXY, 0, my_lcd.Get_Display_Width());
      y_rotate = map(p.x, TS_MINX, TS_MAXX, my_lcd.Get_Display_Height(), 0);

      if(((x_rotate > 10)&&(x_rotate < 75))&&((y_rotate > 180)&&(y_rotate < 230))){
        if((First_digit != 0) || (Second_digit != 0) || (Third_digit != 0) || (Fourth_digit != 0) || (Fifth_digit != 0)){
          my_lcd.Set_Draw_color(WHITE);
          my_lcd.Draw_Round_Rectangle(10, 180, 75, 230, 5);
          my_lcd.Set_Draw_color(BLUE);
          my_lcd.Draw_Round_Rectangle(95, 180, 160, 230, 5);
          my_lcd.Set_Draw_color(BLACK);
          my_lcd.Draw_Rectangle(49, 88, 65, 112);
          my_lcd.Draw_Rectangle(31, 88, 47, 112);
          my_lcd.Draw_Rectangle(67, 88, 83, 112);
          my_lcd.Draw_Rectangle(103, 88, 119, 112);
          my_lcd.Draw_Rectangle(121, 88, 137, 112);
          my_lcd.Set_Draw_color(BLACK);
          my_lcd.Fill_Rectangle(180,205, 320, 215);
          show_string("The Program is ON",180,205,1,YELLOW, BLACK,1);
          my_lcd.Set_Draw_color(BLACK);
          my_lcd.Fill_Rectangle( 5, 125, 154, 135);
          CHminute = String(First_digit) + String(Second_digit) + String(Third_digit);
          CHsecond = String(Fourth_digit) + String(Fifth_digit);
          minute = CHminute.toInt();
          second = CHsecond.toInt();
          digitalWrite(relay_pin_1, LOW);
          digitalWrite(relay_pin_2, LOW);
          digitalWrite(relay_pin_3, LOW);
          temperature();
        }
      }

      
      if(((x_rotate > 15)&&(x_rotate < 75))&&((y_rotate > 5)&&(y_rotate < 34))){
        my_lcd.Set_Draw_color(WHITE);
        my_lcd.Draw_Round_Rectangle(15, 5, 75, 34, 5);
        my_lcd.Set_Draw_color(RED);
        my_lcd.Draw_Round_Rectangle(95, 5, 150, 34, 5);
        my_lcd.Draw_Round_Rectangle(170, 5, 225, 34, 5);
        my_lcd.Draw_Round_Rectangle(245, 5, 305, 34, 5);
        threshold = 40;
      }
      else if(((x_rotate > 98)&&(x_rotate < 150))&&((y_rotate > 5)&&(y_rotate < 34))){
        my_lcd.Set_Draw_color(WHITE);
        my_lcd.Draw_Round_Rectangle(95, 5, 150, 34, 5);
        my_lcd.Set_Draw_color(RED);
        my_lcd.Draw_Round_Rectangle(15, 5, 75, 34, 5);
        my_lcd.Draw_Round_Rectangle(170, 5, 225, 34, 5);
        my_lcd.Draw_Round_Rectangle(245, 5, 305, 34, 5);
        threshold = 50;
      }
      else if(((x_rotate > 170)&&(x_rotate < 225))&&((y_rotate > 5)&&(y_rotate < 34))){
        my_lcd.Set_Draw_color(WHITE);
        my_lcd.Draw_Round_Rectangle(170, 5, 225, 34, 5);
        my_lcd.Set_Draw_color(RED);
        my_lcd.Draw_Round_Rectangle(95, 5, 150, 34, 5);
        my_lcd.Draw_Round_Rectangle(15, 5, 75, 34, 5);
        my_lcd.Draw_Round_Rectangle(245, 5, 305, 34, 5);
        threshold = 60;
      }
      else if(((x_rotate > 245)&&(x_rotate < 305))&&((y_rotate > 5)&&(y_rotate < 34))){
        my_lcd.Set_Draw_color(WHITE);
        my_lcd.Draw_Round_Rectangle(245, 5, 305, 34, 5);
        my_lcd.Set_Draw_color(RED);
        my_lcd.Draw_Round_Rectangle(95, 5, 150, 34, 5);
        my_lcd.Draw_Round_Rectangle(170, 5, 225, 34, 5);
        my_lcd.Draw_Round_Rectangle(15, 5, 75, 34, 5);
        threshold = 70;
      }

      
      if(((x_rotate > 32)&&(x_rotate < 46))&&((y_rotate > 90)&&(y_rotate < 110))){
        current_digit = 1;
        my_lcd.Set_Draw_color(WHITE);
        my_lcd.Draw_Rectangle(31, 88, 47, 112);
        my_lcd.Set_Draw_color(BLACK);
        my_lcd.Draw_Rectangle(49, 88, 65, 112);
        my_lcd.Draw_Rectangle(67, 88, 83, 112);
        my_lcd.Draw_Rectangle(103, 88, 119, 112);
        my_lcd.Draw_Rectangle(121, 88, 137, 112);
        /*//(32, 90, 46, 110, 5)
        for(int i = 0; i < 25; i++){
        //while(1){
          digitalWrite(13, HIGH);
          TSPoint p = ts.getPoint();
          digitalWrite(13, LOW);
          pinMode(XM, OUTPUT);
          pinMode(YP, OUTPUT);
          x_rotate = map(p.y, TS_MINY, TS_MAXY, 0, my_lcd.Get_Display_Width());
          y_rotate = map(p.x, TS_MINX, TS_MAXX, my_lcd.Get_Display_Height(), 0);
          if(((x_rotate > 38)&&(x_rotate < 75))&&((y_rotate > 50)&&(y_rotate < 79))&&(First_digit != 9)){
            delay(50);
            First_digit++;
            //my_lcd.Set_Draw_color(BLACK);
            //my_lcd.Fill_Rectangle(32, 90, 46, 110);
          }
          else if(((x_rotate > 95)&&(x_rotate < 130))&&((y_rotate > 50)&&(y_rotate < 79))&&(First_digit != 0)){
            delay(50);
            First_digit--;
            //my_lcd.Set_Draw_color(BLACK);
            //my_lcd.Fill_Rectangle(32, 90, 46, 110);
          }
          /*my_lcd.Set_Draw_color(BLACK);
          my_lcd.Fill_Rectangle(32, 90, 46, 110);
          delay(200);
          my_lcd.Print_Number_Int(First_digit, 32, 90, 0, ' ', 10);
          delay(200);
        }*/
      }
      else if(((x_rotate > 50)&&(x_rotate < 64))&&((y_rotate > 90)&&(y_rotate < 110))){
        current_digit = 2;
        my_lcd.Set_Draw_color(WHITE);
        my_lcd.Draw_Rectangle(49, 88, 65, 112);
        my_lcd.Set_Draw_color(BLACK);
        my_lcd.Draw_Rectangle(31, 88, 47, 112);
        my_lcd.Draw_Rectangle(67, 88, 83, 112);
        my_lcd.Draw_Rectangle(103, 88, 119, 112);
        my_lcd.Draw_Rectangle(121, 88, 137, 112);
      }
      else if(((x_rotate > 68)&&(x_rotate < 82))&&((y_rotate > 90)&&(y_rotate < 110))){
        current_digit = 3;
        my_lcd.Set_Draw_color(WHITE);
        my_lcd.Draw_Rectangle(67, 88, 83, 112);
        my_lcd.Set_Draw_color(BLACK);
        my_lcd.Draw_Rectangle(31, 88, 47, 112);
        my_lcd.Draw_Rectangle(49, 88, 65, 112);
        my_lcd.Draw_Rectangle(103, 88, 119, 112);
        my_lcd.Draw_Rectangle(121, 88, 137, 112);
      }
      else if(((x_rotate > 104)&&(x_rotate < 118))&&((y_rotate > 90)&&(y_rotate < 110))){
        current_digit = 4;
        my_lcd.Set_Draw_color(WHITE);
        my_lcd.Draw_Rectangle(103, 88, 119, 112);
        my_lcd.Set_Draw_color(BLACK);
        my_lcd.Draw_Rectangle(31, 88, 47, 112);
        my_lcd.Draw_Rectangle(49, 88, 65, 112);
        my_lcd.Draw_Rectangle(67, 88, 83, 112);
        my_lcd.Draw_Rectangle(121, 88, 137, 112);
      }
      else if(((x_rotate > 122)&&(x_rotate < 136))&&((y_rotate > 90)&&(y_rotate < 110))){
        current_digit = 5;
        my_lcd.Set_Draw_color(WHITE);
        my_lcd.Draw_Rectangle(121, 88, 137, 112);
        my_lcd.Set_Draw_color(BLACK);
        my_lcd.Draw_Rectangle(31, 88, 47, 112);
        my_lcd.Draw_Rectangle(49, 88, 65, 112);
        my_lcd.Draw_Rectangle(67, 88, 83, 112);
        my_lcd.Draw_Rectangle(103, 88, 119, 112);
      }


      if(((x_rotate > 38)&&(x_rotate < 75))&&((y_rotate > 50)&&(y_rotate < 79))){
        my_lcd.Set_Text_Mode(1);
        my_lcd.Set_Text_Size(3);
        my_lcd.Set_Text_colour(GREEN);
        my_lcd.Set_Text_Back_colour(BLACK);
        switch(current_digit){
          case 1:{
            if(First_digit != 9){
              delay(200);
              First_digit++;
              my_lcd.Set_Draw_color(BLACK);
              my_lcd.Fill_Rectangle(32, 90, 46, 110);
              my_lcd.Print_Number_Int(First_digit, 32, 90, 0, ' ', 10);
            }
            break;
          }
          case 2:{
            if(Second_digit != 9){
              delay(200);
              Second_digit++;
              my_lcd.Set_Draw_color(BLACK);
              my_lcd.Fill_Rectangle(50, 90, 64, 110);
              my_lcd.Print_Number_Int(Second_digit, 50, 90, 0, ' ', 10);
            }
            break;
          }
          case 3:{
            if(Third_digit != 9){
              delay(200);
              Third_digit++;
              my_lcd.Set_Draw_color(BLACK);
              my_lcd.Fill_Rectangle(68, 90, 82, 110);
              my_lcd.Print_Number_Int(Third_digit, 68, 90, 0, ' ', 10);
            }
            break;
          }
          case 4:{
            if(Fourth_digit != 9){
              delay(200);
              Fourth_digit++;
              my_lcd.Set_Draw_color(BLACK);
              my_lcd.Fill_Rectangle(104, 90, 118, 110);
              my_lcd.Print_Number_Int(Fourth_digit, 104, 90, 0, ' ', 10);
            }
            break;
          }
          case 5:{
            if(Fifth_digit != 9){
              delay(200);
              Fifth_digit++;
              my_lcd.Set_Draw_color(BLACK);
              my_lcd.Fill_Rectangle(122, 90, 136, 110);
              my_lcd.Print_Number_Int(Fifth_digit, 122, 90, 0, ' ', 10);
            }
            break;
          }
        }
      }
      else if(((x_rotate > 95)&&(x_rotate < 130))&&((y_rotate > 50)&&(y_rotate < 79))){
        my_lcd.Set_Text_Mode(1);
        my_lcd.Set_Text_Size(3);
        my_lcd.Set_Text_colour(GREEN);
        my_lcd.Set_Text_Back_colour(BLACK);
        switch(current_digit){
          case 1:{
            if(First_digit != 0){
              delay(200);
              First_digit--;
              my_lcd.Set_Draw_color(BLACK);
              my_lcd.Fill_Rectangle(32, 90, 46, 110);
              my_lcd.Print_Number_Int(First_digit, 32, 90, 0, ' ', 10);
            }
            break;
          }
          case 2:{
            if(Second_digit != 0){
              delay(200);
              Second_digit--;
              my_lcd.Set_Draw_color(BLACK);
              my_lcd.Fill_Rectangle(50, 90, 64, 110);
              my_lcd.Print_Number_Int(Second_digit, 50, 90, 0, ' ', 10);
            }
            break;
          }
          case 3:{
            if(Third_digit != 0){
              delay(200);
              Third_digit--;
              my_lcd.Set_Draw_color(BLACK);
              my_lcd.Fill_Rectangle(68, 90, 82, 110);
              my_lcd.Print_Number_Int(Third_digit, 68, 90, 0, ' ', 10);
            }
            break;
          }
          case 4:{
            if(Fourth_digit != 0){
              delay(200);
              Fourth_digit--;
              my_lcd.Set_Draw_color(BLACK);
              my_lcd.Fill_Rectangle(104, 90, 118, 110);
              my_lcd.Print_Number_Int(Fourth_digit, 104, 90, 0, ' ', 10);
            }
            break;
          }
          case 5:{
            if(Fifth_digit != 0){
              delay(200);
              Fifth_digit--;
              my_lcd.Set_Draw_color(BLACK);
              my_lcd.Fill_Rectangle(122, 90, 136, 110);
              my_lcd.Print_Number_Int(Fifth_digit, 122, 90, 0, ' ', 10);
            }
            break;
          }
        }
      }

      
  }
}


void temperature(){
  while(1){
      digitalWrite(13, HIGH);
      TSPoint p = ts.getPoint();
      digitalWrite(13, LOW);
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      if(p.z > MINPRESSURE && p.z < MAXPRESSURE){
        x_rotate = map(p.y, TS_MINY, TS_MAXY, 0, my_lcd.Get_Display_Width());
        y_rotate = map(p.x, TS_MINX, TS_MAXX, my_lcd.Get_Display_Height(), 0);
        if(((x_rotate > 95)&&(x_rotate < 160))&&((y_rotate > 180)&&(y_rotate < 230))){
          Reset();
          show_string("The Program is OFF",180,205,1,YELLOW, BLACK,1);
          return;
        }
      }

      my_lcd.Set_Text_Mode(1);
      my_lcd.Set_Text_Size(3);
      my_lcd.Set_Text_colour(GREEN);
      my_lcd.Set_Text_Back_colour(BLACK);

      if(Timer_Hold == false){
        if(second == 0){
          if(minute == 0){
            Reset();
            show_string("The Program is Finished",180,205,1,YELLOW, BLACK,1);
            return;
          }
          else{
            minute--;
            my_lcd.Set_Draw_color(BLACK);
            my_lcd.Fill_Rectangle(32, 90, 82, 110);
            if((minute < 100) && (minute >= 10)){
              my_lcd.Print_Number_Int(minute, 50, 90, 0, ' ', 10);
            }
            else if(minute < 10){
              my_lcd.Print_Number_Int(minute, 68, 90, 0, ' ', 10);
            }
            else{
              my_lcd.Print_Number_Int(minute, 32, 90, 0, ' ', 10);
            }
          }
          second = 60;
          second--;
          my_lcd.Set_Draw_color(BLACK);
          my_lcd.Fill_Rectangle(104, 90, 136, 110);
          my_lcd.Print_Number_Int(second, 104, 90, 0, ' ', 10);
        }
        else{
          second--;
          my_lcd.Set_Draw_color(BLACK);
          my_lcd.Fill_Rectangle(104, 90, 136, 110);
          my_lcd.Print_Number_Int(second, 104, 90, 0, ' ', 10);
        }
      }
      delay(800);
      
      my_lcd.Set_Text_Mode(1);
      my_lcd.Set_Text_Size(3);
      my_lcd.Set_Text_colour(WHITE);
      my_lcd.Set_Text_Back_colour(BLACK);
      sensorValue_1 = analogRead(sensorPin_1);
      voltageOut_1 = (sensorValue_1 * 5000) / 1023;
      temperatureK_1 = voltageOut_1 / 10;
      temperatureC_1 = temperatureK_1 - 273;
      my_lcd.Set_Draw_color(BLACK);
      my_lcd.Fill_Rectangle(209, 80, 340, 100);
      my_lcd.Print_Number_Float(temperatureC_1, 2, 209, 80, '.', 0, ' ');
      if(temperatureC_1 >= threshold){
        digitalWrite(relay_pin_1, HIGH);
        show_string("T1 is Cooling Down",155,155,1,YELLOW, BLACK,1);
        T1 = true;
      }
      if(temperatureC_1 <= (threshold-10)){
        digitalWrite(relay_pin_1, LOW);
        my_lcd.Set_Draw_color(BLACK);
        my_lcd.Fill_Rectangle(155,155, 320, 165);
        T1 = false;
      }

      my_lcd.Set_Text_Mode(1);
      my_lcd.Set_Text_Size(3);
      my_lcd.Set_Text_colour(WHITE);
      my_lcd.Set_Text_Back_colour(BLACK);
      sensorValue_2 = analogRead(sensorPin_2);
      voltageOut_2 = (sensorValue_2 * 5000) / 1023;
      temperatureK_2 = voltageOut_2 / 10;
      temperatureC_2 = temperatureK_2 - 273;
      my_lcd.Set_Draw_color(BLACK);
      my_lcd.Fill_Rectangle(209, 120, 340, 140);
      my_lcd.Print_Number_Float(temperatureC_2, 2, 209, 120, '.', 0, ' ');
      if(temperatureC_2 >= threshold){
        digitalWrite(relay_pin_2, HIGH);
        show_string("T2 is Cooling Down",155,170,1,YELLOW, BLACK,1);
        T2 = true;
      }
      if(temperatureC_2 <= (threshold-10)){
        digitalWrite(relay_pin_2, LOW);
        my_lcd.Set_Draw_color(BLACK);
        my_lcd.Fill_Rectangle(155,170, 320, 179);
        T2 = false;
      }

      if((T1 == true) && (T2 == true)){
        Timer_Hold = true;
        digitalWrite(relay_pin_3, HIGH);
        my_lcd.Set_Draw_color(BLACK);
        my_lcd.Fill_Rectangle(180,205, 320, 215);
        show_string("The Program is Paused",180,205,1,YELLOW, BLACK,1);
      }
      else if((T1 == false) || (T2 == false)){
        Timer_Hold = false;
        digitalWrite(relay_pin_3, LOW);
        my_lcd.Set_Draw_color(BLACK);
        my_lcd.Fill_Rectangle(180,205, 320, 215);
        show_string("The Program is ON",180,205,1,YELLOW, BLACK,1); 
      }
      
      delay(200);
    }
}

void Reset(){
  my_lcd.Set_Draw_color(WHITE);
  my_lcd.Draw_Round_Rectangle(95, 180, 160, 230, 5);
  my_lcd.Set_Draw_color(GREEN);
  my_lcd.Draw_Round_Rectangle(10, 180, 75, 230, 5);
  my_lcd.Set_Draw_color(BLACK);
  my_lcd.Fill_Rectangle(209, 80, 340, 100);
  my_lcd.Fill_Rectangle(209, 120, 340, 140);
  show_string("--.--",209,80,3,WHITE, BLACK,1);
  show_string("--.--",209,120,3,WHITE, BLACK,1);
  my_lcd.Set_Draw_color(BLACK);
  my_lcd.Fill_Rectangle(32, 90, 82, 110);
  my_lcd.Fill_Rectangle(104, 90, 136, 110);
  show_string("000:00",32,90,3,GREEN, BLACK,1);
  my_lcd.Set_Draw_color(WHITE);
  my_lcd.Draw_Round_Rectangle(95, 180, 160, 230, 5);
  my_lcd.Set_Draw_color(BLACK);
  my_lcd.Fill_Rectangle(180,205, 320, 215);
  my_lcd.Fill_Rectangle(155,155, 320, 179);
  show_string("Select a digit to change",5,125,1,YELLOW, BLACK,1);

  T1 = false;
  T2 = false;
  Timer_Hold = false;
  
  First_digit = 0;
  Second_digit = 0;
  Third_digit = 0;
  Fourth_digit = 0;
  Fifth_digit = 0;
  current_digit = 0;
}
