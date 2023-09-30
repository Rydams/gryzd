
#include <GyverPortal.h>
#include <EEPROM.h>

struct LoginPass {
  char ssid[20];
  char pass[20];
};


LoginPass lp;


void build() {
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);

  GP.FORM_BEGIN("/login");
  GP.TEXT("lg", "Login", lp.ssid);
  GP.BREAK();
  GP.TEXT("ps", "Password", lp.pass);
  GP.SUBMIT("Submit");
  GP.FORM_END();

  GP.BUILD_END();
}


void setup() {
 delay(2000);
  Serial.begin(115200);
  Serial.println();

  // читаем логин пароль из памяти
  EEPROM.begin(100);
  EEPROM.get(0, lp);
loginPortal();
}


void loginPortal() {
  Serial.println("Portal start");

  // запускаем точку доступа
  WiFi.mode(WIFI_AP);
  WiFi.softAP("WiFi Config");

  // запускаем портал
  GyverPortal ui;
  ui.attachBuild(build);
  ui.start();
  ui.attach(action);

  // работа портала
  while (ui.tick());
}

void action(GyverPortal& p) {
  if (p.form("/login")) {      // кнопка нажата
    p.copyStr("lg", lp.ssid);  // копируем себе
    p.copyStr("ps", lp.pass);
    EEPROM.put(0, lp);              // сохраняем
    EEPROM.commit();                // записываем
    WiFi.softAPdisconnect();        // отключаем AP
  }
}

void loop() {
 

}
