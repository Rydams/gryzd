// почти все компоненты



#include <GyverPortal.h>
GyverPortal ui;

int valSpin1;
int valSpinn1;
int valSpin2; 
int valSpinn2;
int valSpin3; 
int valSpinn3;
char buton;
char buton1;



void build() {
  GP.BUILD_BEGIN();  
  GP.THEME(GP_DARK); // темны цвет 
  GP.FORM_BEGIN("/update");
  
  GP.UPDATE("sld1,sld2,sld3,spn1,spn2,spn3");
  GP.TITLE("Продогрев"); // загаловак 
  GP.HR();  // прочерк 
  GP.NAV_TABS_LINKS("/home,/sett", "Home,Settings"); // выбор страницы 

if (ui.uri("/home")) {
  
  GP.LED("");     // индекатор 
  GP.SWITCH("");  //переклбчатель 
 

   GP.SLIDER("sld1", valSpin1, -20, 20); GP.BREAK();
   GP.SPINNER("spn1", valSpinn1, -20, 20);
   
   GP.SLIDER("sld2", valSpin2, -20, 20 ); GP.BREAK(); 
   GP.SPINNER("spn2", valSpinn2, -20, 20 );
   
   GP.SLIDER("sld3", valSpin3, -20, 20); GP.BREAK(); 
   GP.SPINNER("spn3", valSpinn3, -20, 20); 

 
    GP.SUBMIT("Submit");
  
} else if (ui.uri("/sett")) {
  
  GP.LABEL("Text");     GP.TEXT("", "", "login"); 
  GP.LABEL("Password"); GP.PASS("", "", "Pass");   
  GP.BUTTON("btn", "Button");
   GP.SUBMIT("Submit1");
}

  GP.BUILD_END();

}

void setup() {
  Serial.begin(115200);
   Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("esp32", "12345678");
  

  // подключаем конструктор и запускаем
  ui.attachBuild(build);
 ui.attach(action);
  ui.start();
}

void action() {
  
if (ui.uri("/home")){
  Serial.println("apdete");
if (ui.update()) {
  
 ui.updateInt("sld1",  valSpin1);
ui.updateInt("sld2",  valSpin2);
ui.updateInt("sld3",  valSpin3);
ui.updateInt("spn1",  valSpinn1);
ui.updateInt("spn2",  valSpinn2);
ui.updateInt("spn3",  valSpinn3);

}

}
if (ui.form()) {
if (ui.form("/update")) {
 valSpin1 = ui.getInt("sld1");
 valSpin2 = ui.getInt("sld2");
valSpin3 = ui.getInt("sld3");
ui.copyInt("spn1", valSpinn1);
ui.copyInt("spn2", valSpinn2);
ui.copyInt("spn3", valSpinn3);
 
      

}
Serial.print("sld1: ");
       Serial.println(valSpin1);   
      Serial.print("sld2: ");
      Serial.println(valSpin2);
      Serial.print("sld3: ");
      Serial.println(valSpin3);
      Serial.print("spn1: ");
      Serial.println(valSpinn1);
      Serial.print("spn2: ");
      Serial.println(valSpinn2);
      Serial.print("spn3: ");
      Serial.println(valSpinn3);
}


}

void loop() {
 
  
  ui.tick();
}
