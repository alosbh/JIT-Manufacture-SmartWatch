# 1 "C:\\Users\\100059~1\\AppData\\Local\\Temp\\tmplhpmbdeq"
#include <Arduino.h>
# 1 "C:/Users/100059527/Desktop/JIT-Manufacture-SmartWatch/src/my-ttgo-watch.ino"


#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include <HTTPClient.h>
#include <asyncHTTPrequest.h>
#include <ESPAsyncWebServer.h>
#include "esp_wpa2.h"
#include "config.h"





TTGOClass *ttgo;
PCF8563_Class *rtc;
AXP20X_Class *power;
bool irq = false;
bool BLisOn = false;

AsyncWebServer server(80);
HTTPClient http;

const char* serverName = "http://10.57.38.133:3000/reloginho";
char ip_address[15];
asyncHTTPrequest asyncrequest;


LV_FONT_DECLARE(fn1_32);
LV_FONT_DECLARE(liquidCrystal_nor_32);
static lv_style_t stl_view;
static lv_style_t stl_logo;
static lv_style_t stl_bg_card;
static lv_style_t stl_btn1;
static lv_style_t stl_btn2;
static lv_style_t stl_topline;
static lv_style_t stl_clock;
static lv_style_t stl_transp;

static lv_obj_t * bg_card;
static lv_obj_t * lbl_workstation;
static lv_obj_t * lbl_risk;
static lv_obj_t * lbl_calltime;
static lv_obj_t * lbl_status;
static lv_obj_t * lbl_description;
static lv_obj_t * lbl_user;

static lv_obj_t * btn1;
static lv_obj_t * btn2;
static lv_obj_t * lbl_actualcard;
static lv_obj_t * lbl_totalcard;
static lv_obj_t * lbl_count_separator;

static lv_obj_t * btn_back;
static lv_obj_t * btn_next;

static lv_obj_t * lbl_IP;
static lv_obj_t * lbl_RSSI;

const char* ssid = "2.4G Netvirtua apto 305.";
const char* password = "http3333";
# 76 "C:/Users/100059527/Desktop/JIT-Manufacture-SmartWatch/src/my-ttgo-watch.ino"
const char* auxName = "LUCAS";
const char* ipRelogio= "http://10.57.39.51";

char bufrssi [4];
char bufatual [2];
char buftotal [2];
char bufbat[8];
float batlevel;
const char* PARAM_MESSAGE = "message";
typedef struct {
    lv_obj_t *hour;
    lv_obj_t *minute;
    lv_obj_t *second;
} str_datetime_t;


char chamados[50][50];


uint8_t counter = 0;
uint8_t atual = 0;

static str_datetime_t g_data;
void notFound(AsyncWebServerRequest *request);
void setStyles();
void SetupUI();
static void toggle_Cards_On();
static void toggle_Cards_Off();
static void btn1_handler(lv_obj_t *obj, lv_event_t event);
static void btn2_handler(lv_obj_t *obj, lv_event_t event);
void sendRequest(lv_obj_t *obj, lv_event_t event);
void requestCB(void* optParm, asyncHTTPrequest* asyncrequest, int readyState);
static void removefromArray(lv_obj_t *obj, lv_event_t event);
void printCard(uint8_t posic);
String urlencode(const String &s);
void setup();
void loop();
#line 100 "C:/Users/100059527/Desktop/JIT-Manufacture-SmartWatch/src/my-ttgo-watch.ino"
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
void setStyles(){



    lv_style_init(&stl_view);
    lv_style_set_radius(&stl_view, LV_OBJ_PART_MAIN, 12);
    lv_style_set_bg_color(&stl_view, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_bg_opa(&stl_view, LV_OBJ_PART_MAIN, LV_OPA_COVER);
    lv_style_set_border_width(&stl_view, LV_OBJ_PART_MAIN, 0);


    lv_style_init(&stl_logo);
    lv_style_set_text_color(&stl_logo, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_style_set_text_font(&stl_logo, LV_STATE_DEFAULT, &fn1_32);


    lv_style_init(&stl_clock);
    lv_style_set_text_color(&stl_clock, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_text_font(&stl_clock, LV_STATE_DEFAULT, &liquidCrystal_nor_32);


    lv_style_init(&stl_topline);
    lv_style_set_line_color(&stl_topline, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_line_width(&stl_topline, LV_STATE_DEFAULT, 2);
    lv_style_set_line_rounded(&stl_topline, LV_STATE_DEFAULT, 1);





    lv_style_init(&stl_bg_card);
    lv_style_set_bg_color(&stl_bg_card, LV_OBJ_PART_MAIN, LV_COLOR_YELLOW);



    lv_style_init(&stl_btn1);
    lv_style_set_bg_color(&stl_btn1, LV_STATE_DEFAULT, LV_COLOR_GREEN);
    lv_style_set_text_color(&stl_btn1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_color(&stl_btn1, LV_STATE_DEFAULT, LV_COLOR_GREEN);



    lv_style_init(&stl_btn2);
    lv_style_set_bg_color(&stl_btn2, LV_STATE_DEFAULT, lv_color_make(0xf0, 0x1f, 0x1f));
    lv_style_set_text_color(&stl_btn2, LV_STATE_DEFAULT, LV_COLOR_WHITE);


    lv_style_init(&stl_transp);

    lv_style_set_bg_opa(&stl_transp,LV_STATE_DEFAULT,LV_OPA_TRANSP);
    lv_style_set_border_opa(&stl_transp,LV_STATE_DEFAULT,LV_OPA_TRANSP);
    lv_style_set_border_opa(&stl_transp,LV_BTN_STATE_RELEASED,LV_OPA_TRANSP);
    lv_style_set_border_opa(&stl_transp,LV_BTN_STATE_PRESSED,LV_OPA_TRANSP);
    lv_style_set_border_opa(&stl_transp,LV_STATE_FOCUSED,LV_OPA_TRANSP);
    lv_style_set_border_opa(&stl_transp,LV_STATE_PRESSED,LV_OPA_TRANSP);
    lv_style_set_border_opa(&stl_transp,LV_BTN_STATE_ACTIVE,LV_OPA_TRANSP);

    lv_style_set_border_width(&stl_transp,LV_BTN_STATE_ACTIVE,0);
    lv_style_set_border_width(&stl_transp,LV_BTN_STATE_PRESSED,0);
    lv_style_set_border_width(&stl_transp,LV_BTN_STATE_ACTIVE,0);
    lv_style_set_border_width(&stl_transp,LV_BTN_STATE_CHECKED_RELEASED,0);
     lv_style_set_border_width(&stl_transp,LV_BTN_STATE_DISABLED,0);
     lv_style_set_border_width(&stl_transp,LV_BTN_STATE_CHECKED_PRESSED,0);
      lv_style_set_border_width(&stl_transp,LV_BTN_STATE_RELEASED,0);
      lv_style_set_border_width(&stl_transp,_LV_BTN_STATE_LAST,0);





}

void SetupUI()
{

    setStyles();


    lv_obj_t *view = lv_cont_create(lv_scr_act(), nullptr);
    lv_obj_set_size(view, 240, 240);
    lv_obj_add_style(view, LV_OBJ_PART_MAIN, &stl_view);


    lv_obj_t *jabil_logo = lv_label_create(view, nullptr);
    lv_obj_add_style(jabil_logo, LV_OBJ_PART_MAIN, &stl_logo);
    lv_label_set_text(jabil_logo, "JABIL");
    lv_obj_align(jabil_logo, view, LV_ALIGN_IN_TOP_LEFT, 10, 5);

    lv_obj_t * lbl_percentsignal;
    lbl_percentsignal = lv_label_create(view, NULL);

    lv_label_set_text(lbl_percentsignal, "%");
    lv_obj_align(lbl_percentsignal, view, LV_ALIGN_IN_TOP_RIGHT, -5, 0);


    g_data.minute = lv_label_create(view, nullptr);
    lv_obj_add_style(g_data.minute, LV_OBJ_PART_MAIN, &stl_clock);
    batlevel = (power->getBattVoltage()/100);
    Serial.println("************** NIVEL DA BATERIA ************");
    Serial.println(batlevel);
    sprintf (bufbat, "%.0f",batlevel);
    lv_label_set_text(g_data.minute,bufbat );
    lv_obj_align(g_data.minute, lbl_percentsignal, LV_ALIGN_IN_TOP_LEFT, -33, 0);
# 214 "C:/Users/100059527/Desktop/JIT-Manufacture-SmartWatch/src/my-ttgo-watch.ino"
    static lv_point_t line_points[] = { {10, 0}, {230, 0} };
    lv_obj_t *line1;
    line1 = lv_line_create(view, NULL);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_add_style(line1, LV_OBJ_PART_MAIN, &stl_topline);
    lv_obj_align(line1, NULL, LV_ALIGN_IN_TOP_MID, 0, 35);




    lv_obj_t * lbl_nocard;
    lbl_nocard = lv_label_create(view, NULL);
    lv_label_set_text(lbl_nocard, "Sem chamados.");
    lv_obj_align(lbl_nocard, view, LV_ALIGN_CENTER, 0, 0);


    lbl_IP = lv_label_create(view, NULL);
    lv_label_set_text(lbl_IP, "0.0.0.0");
    lv_obj_align(lbl_IP, view, LV_ALIGN_CENTER, 0, 20);


    lbl_RSSI = lv_label_create(view, NULL);
    lv_label_set_text(lbl_RSSI, "-0");
    lv_obj_align(lbl_RSSI, view, LV_ALIGN_CENTER, 0, 40);

    bg_card = lv_obj_create(view, NULL);
    lv_obj_set_pos(bg_card, 10, 40);
    lv_obj_set_width(bg_card,220);
    lv_obj_set_height(bg_card,166);
     lv_obj_add_style(bg_card, LV_OBJ_PART_MAIN, &stl_bg_card);






    lbl_workstation = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_workstation, "WORKSTATION");
    lv_obj_align(lbl_workstation, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);



    lbl_risk = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_risk, "RISCO");
    lv_obj_align(lbl_risk, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 40);




    lbl_status = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_status, "Status");
    lv_obj_align(lbl_status, NULL, LV_ALIGN_IN_TOP_RIGHT, -40, 100);


    lbl_calltime = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_calltime, "12: 00");
    lv_obj_align(lbl_calltime, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 40);



    lbl_description = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_description, "DESCRIPTION");
    lv_obj_align(lbl_description, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 70);

    lbl_user = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_user, "");
    lv_obj_align(lbl_user, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 100);



    btn1 = lv_btn_create(bg_card, NULL);
    lv_obj_set_pos(btn1, 10, 125);
    lv_obj_set_width(btn1,95);
    lv_obj_set_height(btn1,35);
    lv_obj_set_event_cb(btn1, sendRequest);


    lv_obj_t * lbl_btn1;
    lbl_btn1 = lv_label_create(btn1, NULL);
    lv_label_set_text(lbl_btn1, "Confirmar");
    lv_label_set_text(lbl_btn1, LV_SYMBOL_OK);
    lv_obj_add_style(btn1, LV_OBJ_PART_MAIN, &stl_btn1);



    btn2 = lv_btn_create(bg_card, NULL);
    lv_obj_set_pos(btn2, 120, 125);
    lv_obj_set_width(btn2,95);
    lv_obj_set_height(btn2,35);
    lv_obj_set_event_cb(btn2, removefromArray);


    lv_obj_t * lbl_btn2;
    lbl_btn2 = lv_label_create(btn2, NULL);
    lv_label_set_text(lbl_btn2, "Recusar");
    lv_label_set_text(lbl_btn2, LV_SYMBOL_CLOSE);
    lv_obj_add_style(btn2, LV_OBJ_PART_MAIN, &stl_btn2);



    lbl_count_separator = lv_label_create(view, NULL);
    lv_label_set_text(lbl_count_separator, "/");
    lv_obj_align(lbl_count_separator, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -5);


    lbl_actualcard = lv_label_create(view, NULL);
    lv_label_set_text(lbl_actualcard, "0");
    lv_obj_align(lbl_actualcard, lbl_count_separator, LV_ALIGN_IN_TOP_MID, -10, 0);


    lbl_totalcard = lv_label_create(view, NULL);
    lv_label_set_text(lbl_totalcard, "0");
    lv_obj_align(lbl_totalcard, lbl_count_separator, LV_ALIGN_IN_TOP_MID, 10, 0);

    btn_back= lv_btn_create(view, NULL);
    lv_obj_set_width(btn_back,80);
    lv_obj_set_height(btn_back,35);
    lv_obj_align(btn_back, view, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_obj_add_style(btn_back, LV_OBJ_PART_MAIN, &stl_transp);
    lv_obj_t * lbl_btn_back;
    lbl_btn_back = lv_label_create(btn_back, NULL);
    lv_label_set_text(lbl_btn_back, LV_SYMBOL_LEFT);

    lv_obj_set_event_cb(btn_back, btn1_handler);

    btn_next= lv_btn_create(view, NULL);
    lv_obj_set_width(btn_next,80);
    lv_obj_set_height(btn_next,35);
    lv_obj_align(btn_next, view, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
    lv_obj_add_style(btn_next, LV_OBJ_PART_MAIN, &stl_transp);
    lv_obj_t * lbl_btn_next;
    lbl_btn_next = lv_label_create(btn_next, NULL);
    lv_label_set_text(lbl_btn_next, LV_SYMBOL_RIGHT);
    lv_obj_set_event_cb(btn_next, btn2_handler);

    lv_obj_set_hidden(bg_card, true);


}
static void toggle_Cards_On(){

        lv_obj_set_hidden(bg_card, false);
}
static void toggle_Cards_Off(){

        lv_obj_set_hidden(bg_card, true);
}

static void btn1_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && atual >1) {
        Serial.println("Clicked\n");

    atual = atual-1;
   Serial.println("back");
  Serial.println();
  Serial.print(atual);
  Serial.print("/");
  Serial.print(counter);

    printCard(atual-1);
    }

}
static void btn2_handler(lv_obj_t *obj, lv_event_t event)
{
     if (event == LV_EVENT_CLICKED && atual < counter) {
        Serial.println("Clicked\n");

    atual = atual+1;
    Serial.println("next");
  Serial.println();
  Serial.print(atual);
  Serial.print("/");
  Serial.print(counter);

    printCard(atual-1);
    }


}

void sendRequest(lv_obj_t *obj, lv_event_t event){

  if (event == LV_EVENT_CLICKED) {


    Serial.println(chamados[((atual-1)*7)+0]);
    Serial.println(chamados[((atual-1)*7)+1]);
    Serial.println(chamados[((atual-1)*7)+2]);
    Serial.println(chamados[((atual-1)*7)+3]);
    Serial.println(chamados[((atual-1)*7)+4]);
    Serial.println(chamados[((atual-1)*7)+5]);


    StaticJsonDocument<200> doc2;

      doc2["Id"] = chamados[((atual-1)*7)+4];
      doc2["UserName"] = auxName;
      doc2["Ip"] = ipRelogio;

      String requestBody;
      serializeJson(doc2, requestBody);
    if(asyncrequest.readyState() == 0 || asyncrequest.readyState() == 4){

        asyncrequest.open("POST", "http://10.57.16.40/AioWatch/Confirm");


        asyncrequest.setReqHeader("Content-Type", "application/json");

        asyncrequest.send(requestBody);
    }
  }
}

void requestCB(void* optParm, asyncHTTPrequest* asyncrequest, int readyState){
    if(readyState == 4){
        Serial.println("***********REQUEST CB ***********");
        Serial.println(asyncrequest->responseHTTPcode());
        Serial.println(asyncrequest->responseText());
        if(asyncrequest->responseHTTPcode()==200){
          Serial.println("Esse chamado deu certo sim");
        }

        Serial.println();

    }
}

static void removefromArray(lv_obj_t *obj, lv_event_t event){

  if (event == LV_EVENT_CLICKED) {

    uint8_t pos = atual-1;
    uint8_t tam = counter-1;
    uint8_t i;


    for(i=pos; i<tam; i++)
          {
              strcpy(chamados[(i*7)+0],chamados[(i*7)+7]);
              strcpy(chamados[(i*7)+1],chamados[(i*7)+8]);
              strcpy(chamados[(i*7)+2],chamados[(i*7)+9]);
              strcpy(chamados[(i*7)+3],chamados[(i*7)+10]);
              strcpy(chamados[(i*7)+4],chamados[(i*7)+11]);
              strcpy(chamados[(i*7)+5],chamados[(i*7)+12]);
              strcpy(chamados[(i*7)+6],chamados[(i*7)+13]);
          };


    counter--;

    if(atual==counter+1){

      atual = counter;
    }

    printCard(atual-1);
  }
  if(counter==0){
      toggle_Cards_Off();
  }

};



void printCard(uint8_t posic){


    lv_obj_set_hidden(bg_card, true);

    if(strcmp(chamados[(posic*7)+5],"Open")==0){
      Serial.println("mostrei o botao");
      lv_obj_set_hidden(btn1, false);

    }
    else{
      Serial.println("escondi o botao");
      lv_obj_set_hidden(btn1, true);

    }
    lv_obj_set_hidden(bg_card, false);
    lv_label_set_text(lbl_workstation,chamados[(posic*7)+0]);
    lv_label_set_text(lbl_risk,chamados[(posic*7)+1]);
    lv_label_set_text(lbl_calltime,chamados[(posic*7)+2]);
    lv_label_set_text(lbl_description,chamados[(posic*7)+3]);
    lv_label_set_text(lbl_status,chamados[(posic*7)+5]);
    lv_label_set_text(lbl_user,chamados[(posic*7)+6]);

    Serial.println("********8 MEU CARD *******");

    Serial.println(chamados[(posic*7)+0]);
    Serial.println(chamados[(posic*7)+1]);
    Serial.println(chamados[(posic*7)+2]);
    Serial.println(chamados[(posic*7)+3]);
    Serial.println(chamados[(posic*7)+4]);
    Serial.println(chamados[(posic*7)+5]);
    Serial.println(chamados[(posic*7)+6]);






Serial.println();
    Serial.print(atual);
    Serial.print("/");
    Serial.print(counter);
    Serial.println();
    sprintf (bufatual, "%d",atual);
  sprintf (buftotal, "%d",counter);
  lv_label_set_text(lbl_actualcard,bufatual);
  lv_label_set_text(lbl_totalcard,buftotal);


}


String urlencode(const String &s) {
  static const char lookup[]= "0123456789abcdef";
  String result;
  size_t len = s.length();

  for(size_t i = 0; i < len; i++) {
    const char c = s[i];
    if(('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c=='-' || c=='_' || c=='.' || c=='~')) {
      result += c;
    } else {
      result += "%" + String(lookup[(c & 0xf0) >>4]) + String(lookup[c & 0x0f]);
    }
  }

  return result;
}

void setup() {


    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    BLisOn = true;
    power = ttgo->power;
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        irq = true;
    }, FALLING);


    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    ttgo->power->clearIRQ();
    ttgo->motor_begin();
    ttgo->lvgl_begin();
    SetupUI();

    ttgo->tft->drawString("Waiting for Wifi",80,120);


    WiFi.begin(ssid, password);



    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(WiFi.status());
    }

    String meuip = WiFi.localIP().toString();
    meuip.toCharArray(ip_address,15);
    lv_label_set_text(lbl_IP,ip_address);


    long rssi = WiFi.RSSI();
    if(rssi <= -85 ){
        Serial.print("rssi maior q 85 ");
        lv_label_set_text(lbl_RSSI,"85+");

      }
    if(rssi >-85 && rssi <= -80 ){
        Serial.print("rssi entre 85 e 80 ");
        lv_label_set_text(lbl_RSSI,"85~80");

      }
    if(rssi >-80 && rssi <= -75 ){
        Serial.print("rssi entre 80 e 75 ");
        lv_label_set_text(lbl_RSSI,"80~75");

      }
    if(rssi >-75 && rssi <= -70 ){
        Serial.print("rssi entre 75 e 70 ");
        lv_label_set_text(lbl_RSSI,"75~70");

      }
    if(rssi >-70 && rssi <= -65 ){
        Serial.print("rssi entre 70 e 65 ");
        lv_label_set_text(lbl_RSSI,"70~65");

      }
    if(rssi >-65 && rssi <= -60 ){
        Serial.print("rssi entre 65 e 60 ");
        lv_label_set_text(lbl_RSSI,"65~60");

      }
      if(rssi >-60 && rssi <= -55 ){
        Serial.print("rssi entre 60 e 55 ");

      }
      if(rssi >-55 && rssi <= -50 ){
        Serial.print("rssi entre 55 e 50 ");
        lv_label_set_text(lbl_RSSI,"55~50");

      }
      if(rssi >-50 && rssi <= -45 ){
        Serial.print("rssi entre 50 e 45 ");
        lv_label_set_text(lbl_RSSI,"50~45");

      }
      if(rssi >-45 && rssi <= -40 ){
        Serial.print("rssi entre 45 e 40 ");
        lv_label_set_text(lbl_RSSI,"45~40");

      }
      if(rssi >-40 && rssi <= -35 ){
        Serial.print("rssi entre 40 e 35 ");
        lv_label_set_text(lbl_RSSI,"40~35");

      }
      if(rssi > -35 ){
        Serial.print("rssi menor que 35 ");
        lv_label_set_text(lbl_RSSI,"35-");

      }
# 657 "C:/Users/100059527/Desktop/JIT-Manufacture-SmartWatch/src/my-ttgo-watch.ino"
    Serial.print("Wifi IP: ");
    Serial.println(WiFi.localIP());


  Serial.print("RSSI:");
Serial.println(rssi);

    asyncrequest.setDebug(true);
    asyncrequest.onReadyStateChange(requestCB);

    server.onNotFound(notFound);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Andre Lara hehehe 8)");
    });

    AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/receberchamado", [](AsyncWebServerRequest *request, JsonVariant &json) {

        JsonObject jsonObj = json.as<JsonObject>();

        if(BLisOn){

          }
          else{
            ttgo->openBL();
            BLisOn = true;
          }
        ttgo->motor->onec();
        strcpy(chamados[(counter*7)+0],jsonObj["workstation"]);

        strcpy(chamados[(counter*7)+1],jsonObj["risk"]);

        strcpy(chamados[(counter*7)+2],jsonObj["calltime"]);

        strcpy(chamados[(counter*7)+3],jsonObj["description"]);

        strcpy(chamados[(counter*7)+4],jsonObj["id"]);
        strcpy(chamados[(counter*7)+5],"Open");
        strcpy(chamados[(counter*7)+6],"");
        counter = counter +1;
        atual = counter;
        printCard(counter-1);

        toggle_Cards_On();

        request->send(200);

    });

    AsyncCallbackJsonWebHandler* handler2 = new AsyncCallbackJsonWebHandler("/atualizarchamado", [](AsyncWebServerRequest *request, JsonVariant &json) {

        JsonObject jsonObj = json.as<JsonObject>();

        if(BLisOn){

          }
          else{
            ttgo->openBL();
            BLisOn = true;
          }
        ttgo->motor->onec();

        char idbuscado [3];
         char userbuscado [20];
         char statusbuscado [20];
         Serial.println("ID RECEBIDA");
        strcpy(idbuscado,jsonObj["Id"]);
        Serial.println("USUARIO RECEBIDO");
        strcpy(userbuscado,jsonObj["UserName"]);
        Serial.println("STATUS RECEBIDO");
        strcpy(statusbuscado,jsonObj["Status"]);



        for (int i = 4; i <= ((counter*7)-3); i=i+7) {
          Serial.print("Comparei ");
          Serial.print(chamados[i]);
          Serial.print(" com ");
          Serial.print(idbuscado);
          Serial.println("Boolean:");
          Serial.println(idbuscado==chamados[i]);

          Serial.println("****ITERANDO***");
          if(strcmp(chamados[i], idbuscado)==0){


            Serial.println("Achei meu chamado buscado");
            strcpy(chamados[i+1],statusbuscado);
            strcpy(chamados[i+2],userbuscado);

            Serial.println(chamados[i-4]);
            Serial.println(chamados[i-3]);
            Serial.println(chamados[i-2]);
            Serial.println(chamados[i-1]);
            Serial.println(chamados[i]);
            Serial.println(chamados[i+1]);
            Serial.println(chamados[i+2]);
            atual=((i-4)/7)+1;
            printCard(atual-1);
            break;

          }


        }


        request->send(200);

    });


    server.addHandler(handler);
    server.addHandler(handler2);


    server.begin();




}

void loop() {

    if (irq) {
        irq = false;
        ttgo->power->readIRQ();
        if (ttgo->power->isPEKShortPressIRQ()) {

          if(BLisOn){
            ttgo->closeBL();
            BLisOn = false;
          }
          else{
            ttgo->openBL();
            BLisOn = true;
          }

        }
        ttgo->power->clearIRQ();
    }

    lv_task_handler();
    delay(5);
}