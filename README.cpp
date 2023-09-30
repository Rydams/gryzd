// мосфет весит на  пине 
// датчики температуры на ,   пинах 
// экран весит на SDA – , SCL – 

// https://esp32io.com/tutorials/esp32-temperature-sensor // 18b20 без адреса

#include <Arduino.h>
#include "OneWire.h"
#include "DallasTemperature.h"
#include "GyverPID.h"
#include <GyverOLED.h>
#include <EEPROM.h>
#include <GyverButton.h>      // Либа кнопок


#define EEPROM_ADDR 90   // для епрома 
#define RELAY_IN 13       // пин для включение реле нагрева 

// _________________________________датчик температуры_________________________________________
#define ONE_WIRE_BUS1 15 // датчик темературы обявление, настройка: пин 15 
#define ONE_WIRE_BUS2 19  // датчик темературы обявление, настройка: пин 19

OneWire oneWire1(ONE_WIRE_BUS1);  // Настройка экземпляра библиотеки oneWire для связи с устройством с возможностью подключения одним проводом (OneWire)
OneWire oneWire2(ONE_WIRE_BUS2);

DallasTemperature sensors1(&oneWire1);  // Передача ссылки на нашу библиотеку oneWire библиотеке датчика температуры Dallas Temperature
DallasTemperature sensors2(&oneWire2);

DeviceAddress sensor1 = { 0x28, 0x1B, 0x1B, 0x80, 0xE3, 0xE1, 0x3C, 0xE7 };  //  адрес датчика на пине, 0 потомучто датчик один 
DeviceAddress sensor2 = { 0x28, 0xAE, 0xA6, 0x80, 0xE3, 0xE1, 0x3C, 0xA8 };

// ___________________________Обьект дисплея________________________
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled; 
#define OLED_SOFT_BUFFER_64     // Буфер на стороне МК

// ____________________________Кнопки______________________________
GButton up(26, HIGH_PULL);     
GButton down(27, HIGH_PULL);
GButton ok(14, HIGH_PULL);

//_________________________пременые для упровления на дисплее________________________________
#define ITEMS 4             // Общее кол во пунктов (больше 8 - нужно несколько страниц)
struct
{
  long fotl1 =0 ;
  long fotl2 =0 ;
  bool fotl3 =0 ;
  int fotl4 =0 ;

}settings;






const int buttonPin = 25;   // изначальный режим работы нагрев 
int16_t knopkarab = 0;  
bool raotaFlg = 0;
bool raota = 0;
int16_t pridel1 = constrain(pridel1, settings.fotl1 , settings.fotl2 ); // работает от до 

int16_t termos1;
int16_t termos2;
uint32_t myTimer1, myTimer2, myTimer3,myTimer4,myTimer5,myTimer6; // таймеры для millis


GyverPID regulator(0.05, 0.05, 0.5, 2);  // коэф. П, коэф. И, коэф. Д, период дискретизации dt (мс)
// или так:
// GyverPID regulator(0.1, 0.05, 0.01);  // можно П, И, Д, без dt, dt будет по умолч. 100 мс

// _____________Битмап с картинкой стрелочки (Если нужен)___________________________
const uint8_t ptr_bmp[] PROGMEM = {
  0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0xFF, 0xFF, 0x7E, 0x3C, 0x18,
};


void setup() {
  Serial.begin(9600); 
  EEPROM.begin(1000);   // для esp8266/esp32
  sensors1.begin();   // опрос датчика 
  sensors2.begin();

  pinMode(RELAY_IN, OUTPUT);
  pinMode(buttonPin, INPUT); 
 
 //_______________________проверка епром памчять на ашибку___________________________________________
  if(EEPROM.read(0) != 'W'){
      EEPROM.put(1, settings);
      EEPROM.write(0, 'w');
    }else{
        EEPROM.get(1,settings);
}

  EEPROM.commit();     // для esp8266/esp32
  EEPROM.get(EEPROM_ADDR, settings);     

  oled.init();           // Инциализация дисплея
  oled.setContrast(255); // Макс. яркость

//________________________________________________________________________________________________________

  regulator.setDirection(NORMAL); // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(0, 255);    // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
 
// в процессе работы можно менять коэффициенты
  regulator.Kp = 5.2;
  regulator.Ki += 0.5;
  regulator.Kd = 0;
}


 

void printPointer(uint8_t pointer) {// Символьный указатель
  // Символьный указатель - лучший выбор в многостраничном меню
  oled.setCursor(0, pointer);
  oled.print(">");
}

// меню авто от 
void func1(void) { 
      
  oled.clear();
  oled.update();
  while (1) {
    oled.home();
    oled.print(settings.fotl1);
    oled.update();
down.tick();
up.tick();
ok.tick();

if (up.isClick() ) {
  ++settings.fotl1 ; 

if (settings.fotl1 == 0 || settings.fotl1 == 9 || settings.fotl1 == -9 ) {
        oled.clear();
        oled.update();
      }

}
    if (down.isClick() ) {
      --settings.fotl1;

      if (settings.fotl1 == 0 || settings.fotl1 == 9 || settings.fotl1 == -9 ) {
        oled.clear();
        oled.update();
      }
    }

    if (ok.isClick() )
    { 
      return; 
    
    } // return возвращает нас в предыдущее меню
  }
}

//меню авто до  

void func2(void) {
  oled.clear();
  oled.update();
  while (2) {
    oled.home();
    oled.print(settings.fotl2);
    oled.update();
down.tick();
up.tick();
ok.tick();

    if (up.isClick() ) {
      
      
      ++settings.fotl2; 
if (settings.fotl2 == 0 || settings.fotl2 == 9 || settings.fotl2 == -9 ) {
        oled.clear();
        oled.update();
      }
  }
    if (down.isClick() ) {
      --settings.fotl2;
if (settings.fotl2 == 0 || settings.fotl2 == 9 || settings.fotl2 == -9 ) {
        oled.clear();
        oled.update();
      }
    }

    if (ok.isClick())  return; // return возвращает нас в предыдущее меню // eeprom опдейт дабавить !!!
  }
}

//меню режим работы
void func3(void) {
  oled.clear();
  oled.update();
  while (2) {
    oled.home();

    if (settings.fotl3 == 1 ){
      oled.print("авто");
    }
    if (settings.fotl3 == 0 ){
      oled.print("ручной");
    }
oled.update();
down.tick();
up.tick();
ok.tick();

if (up.isStep()){ 
 if (settings.fotl3 == 0) {
    settings.fotl3 =1;
  } else{
    settings.fotl3 = 0;
  }
}

if (down.isStep()){ 
 if (settings.fotl3 == 1) {
    settings.fotl3 = 0;
  } else{
    settings.fotl3 = 1;
  }
}
    if (ok.isClick())  return; // return возвращает нас в предыдущее меню // eeprom опдейт дабавить !!!
  }
}

//меню нагрев на
void func4(void) {
  oled.clear();
  oled.update();
  while (2) {
    oled.home();
    oled.print(settings.fotl4);
    oled.update();
down.tick();
up.tick();
ok.tick();

    if (up.isClick() ){ 
      ++settings.fotl4; 

      if (settings.fotl4 == 0 || settings.fotl4 == 9 || settings.fotl4 == -9 ) {
        oled.clear();
        oled.update();
      }
    }

    if (down.isClick() ){ 
      --settings.fotl4;

      if (settings.fotl4 == 0 || settings.fotl4 == 9 || settings.fotl4 == -9 ) {
        oled.clear();
         oled.update();

      }
    }

    if (ok.isClick())  return; // return возвращает нас в предыдущее меню // eeprom опдейт дабавить !!!
  }
}

// запрос температуры 
void opros1(void) {  

  if (millis() - myTimer1 >= 1000) {
    myTimer1 = millis(); // сбросить таймер
    sensors2.requestTemperatures(); // Отправка команды на получение показаний температуры
    
  }


    sensors1.requestTemperatures(); // Отправка команды на получение показаний температуры


  // !!!!!!!!
    if (termos1 <= -40){  // хз робит или нет должно устронять ащибку проподания температуры при быстром  нагреве

        sensors1.requestTemperatures(); // Отправка команды на получение показаний температуры жидкасти 
        termos1 = sensors1.getTempCByIndex(0);  // тепмпература жидкасти
      }


      termos2 = sensors2.getTempCByIndex(0); // тепмпература окр среды 
      termos1 = sensors1.getTempCByIndex(0);  // тепмпература жидкасти


}

//главное меню 
void menu1(void){ 

/* Кнопки */
   if (millis() - myTimer3 >= 100) {
    myTimer3 = millis(); // сбросить таймер

  up.tick();                 // Опрос кнопок
  down.tick();
  ok.tick();
   }

   static int8_t pointer = 0; // Переменная указатель

if (up.isClick() or up.isHold()) {                // Если кнопку нажали или удерживают
    pointer = constrain(pointer - 1, 0, ITEMS - 1); // Двигаем указатель в пределах дисплея
  }

if (down.isClick() or down.isHold()) {
    pointer = constrain(pointer + 1, 0, ITEMS - 1);
  }

  if (ok.isClick()) {   // Нажатие на ОК - переход в пункт меню
    switch (pointer) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
      case 0: func1(); break;  // По нажатию на ОК при наведении на 0й пункт вызвать функцию
      case 1: func2(); break;
      case 2: func3(); break;
      case 3: func4(); break;

        // И все остальные
    }
  }
/* меню */
  oled.clear();           // Очищаем буфер
  oled.home();            // Курсор в левый верхний угол

  if (pointer < 4) {      // Первая страница
    oled.print            // Вывод всех пунктов
    (F(
       "  \r авто от 0:\n" // Не забываем про '\n' - символ переноса строки
       "  \r авто до  1:\n"
       "  \r режим работы 2:\n"
       "  \r нагрев на  3:\n"
       
     ));
  } 

  printPointer(pointer);  // Вывод указателя
  oled.update();          // Выводим кадр на дисплей
}

//регулятор температуры 
void regulator11(void){

if (termos1 <= -40){  // хз робит или нет должно устронять ащибку проподания температуры при быстром  нагреве
    sensors1.requestTemperatures(); // Отправка команды на получение показаний температуры жидкасти 
    termos1 = sensors1.getTempCByIndex(0);  // тепмпература жидкасти
  }

if (knopkarab == true){ // 
  digitalWrite(RELAY_IN, LOW);
}
// _________________________выполнять если авто режим_________________________________________________
if (knopkarab == false){

          if (termos1 <= -40){  // хз робит или нет должно устронять ащибку проподания температуры при быстром  нагреве
              sensors1.requestTemperatures(); // Отправка команды на получение показаний температуры жидкасти 
              termos1 = sensors1.getTempCByIndex(0);  // тепмпература жидкасти
            }
  if (settings.fotl3 == 1){  // режим работы если переключатель в авто режим
      digitalWrite(RELAY_IN, HIGH);
         regulator.input = termos1;   // сообщаем регулятору текущую температуру жидкасти 
  // getResultTimer возвращает значение для управляющего устройства
  // (после вызова можно получать это значение как regulator.output)
  // обновление происходит по встроенному таймеру на millis()
     analogWrite(12, regulator.getResultTimer());  // отправляем на мосфет прин 5 
         regulator.setpoint = termos2 + 5;        // сообщаем регулятору температуру, которую он должен поддерживать
}
  // _________________________выполнять если ручной режим_________________________________________________
  if (settings.fotl3 == 0){            // режим работы если переключатель ручной режим 

                if (termos1 <= -40){  // хз робит или нет должно устронять ащибку проподания температуры при быстром  нагреве
                  sensors1.requestTemperatures(); // Отправка команды на получение показаний температуры жидкасти 
                  termos1 = sensors1.getTempCByIndex(0);  // тепмпература жидкасти
                }

           // кнопка обогрева зерал нажата          
    if (termos2 >= pridel1){    // если температура окр сркды от  до  то выполняем это
        digitalWrite(RELAY_IN, HIGH);
// читаем с датчика температуру
          regulator.input = termos1; // сообщаем регулятору текущую температуру жидкасти                            
// обновление происходит по встроенному таймеру на millis()
        analogWrite(12, regulator.getResultTimer());   // отправляем на мосфет
          regulator.setpoint = termos2 + settings.fotl4; // сообщаем регулятору температуру, которую он должен поддерживать
       }
  }
          if (termos1 <= -40){  // хз робит или нет должно устронять ащибку проподания температуры при быстром  нагреве
              sensors1.requestTemperatures(); // Отправка команды на получение показаний температуры жидкасти 
              termos1 = sensors1.getTempCByIndex(0);  // тепмпература жидкасти
            }

  }

  if (millis() - myTimer4 >= 200) {
    myTimer4 = millis(); // сбросить таймер
    oled.clear();           // Очищаем буфер
    oled.home();            // Курсор в левый верхний угол
    oled.print (termos2);
    // курсор на начало 3 строки
    oled.setCursor(0, 3);
    oled.print (termos1);
    oled.update();          // Выводим кадр на дисплей
  }
}

//выбор режима раьоты или настройки
void vbregim(void){

// _____________________________выбор режима или настройка или работа ___________________________
  if (millis() - myTimer2 >= 500) {
    myTimer2 = millis(); // сбросить таймер
ok.tick();
  }
// сахронение при выходе из меню 
if (ok.isStep()){ 
 if (raota == 0) {
  raota = 1;
  } else{
raota = 0;
 EEPROM.put(EEPROM_ADDR, settings);
      EEPROM.commit();     // для esp8266/esp32
  }
}

}


void loop() {

// выбор режима
vbregim();

// для работы регулятоа
if (raota == 0) { 
 knopkarab = digitalRead(buttonPin);

          if (termos1 <= -40){  // хз робит или нет должно устронять ащибку проподания температуры при быстром  нагреве
              sensors1.requestTemperatures(); // Отправка команды на получение показаний температуры жидкасти 
              termos1 = sensors1.getTempCByIndex(0);  // тепмпература жидкасти
            }

opros1();
regulator11();

  if (raotaFlg == 0  && raota == 0){
    if (millis() - myTimer6 >= 1000) {

    myTimer6 = millis(); // сбросить таймер
raota != 1;
raotaFlg = 1;
    }
  }
}


// для работы только дисплея 
if (raota == 1 ) { 
  menu1();

    if (raotaFlg == 1  && raota == 1){
      if (millis() - myTimer6 >= 1000) {

      myTimer6 = millis(); // сбросить таймер
  raota != 0;
  raotaFlg = 0;
    }
    }
}
}


