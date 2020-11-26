

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
#define USE_SERIAL Serial
// #define EAP_IDENTITY "100059527" 
// #define EAP_PASSWORD "Sdesenha2016*"
// const char* ssid = "JABOFF0003";

TTGOClass *ttgo;
PCF8563_Class *rtc;
AXP20X_Class *power;
bool irq = false;
bool BLisOn = false;

AsyncWebServer server(80);
HTTPClient http;


/************    APIS */

char* GetWatchById_host = "http://10.57.16.40/JITAPI/Smartwatch/GetByIP/";


/**************************** */
char GetWatchById_Url[50] = {0};
char ip_address[15];
asyncHTTPrequest asyncrequest;
asyncHTTPrequest getrequest;


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

// const char* ssid = "2.4G Netvirtua apto 305.";
// const char* password = "http3333";

// const char* ssid = "Moto G Play 7041";
// const char* password = "isabela1";

const char* ssid = "JAB_RASP0001";
const char* password = "g4keKDI2RkXQT";

uint8_t num_tickets = 10;
// const char* auxName = "ANDRE";
// const char* ipRelogio= "http://10.57.38.132";


// const char* auxName = "LUCAS";
// const char* ipRelogio= "http://10.57.39.51";

const char* auxName = "ANDRE";
const char* ipRelogio= "http://10.57.38.132";

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


char chamados[100][100];


uint8_t counter = 0;
uint8_t atual = 0;

static str_datetime_t g_data;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
void setStyles(){


  //MAIN VIEW STYLE
    lv_style_init(&stl_view);
    lv_style_set_radius(&stl_view, LV_OBJ_PART_MAIN, 12);
    lv_style_set_bg_color(&stl_view, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_bg_opa(&stl_view, LV_OBJ_PART_MAIN, LV_OPA_COVER);
    lv_style_set_border_width(&stl_view, LV_OBJ_PART_MAIN, 0);

  //LOGO STYLE
    lv_style_init(&stl_logo);
    lv_style_set_text_color(&stl_logo, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_style_set_text_font(&stl_logo, LV_STATE_DEFAULT, &fn1_32);

  // CLOCK STYLE
    lv_style_init(&stl_clock);
    lv_style_set_text_color(&stl_clock, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_text_font(&stl_clock, LV_STATE_DEFAULT, &liquidCrystal_nor_32);

  // TOP LINE STYLE
    lv_style_init(&stl_topline);
    lv_style_set_line_color(&stl_topline, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_line_width(&stl_topline, LV_STATE_DEFAULT, 2);
    lv_style_set_line_rounded(&stl_topline, LV_STATE_DEFAULT, 1);




  // CARD BACKGROUND STYLE
    lv_style_init(&stl_bg_card);
    lv_style_set_bg_color(&stl_bg_card, LV_OBJ_PART_MAIN, LV_COLOR_YELLOW);


  // BUTTON 1 STYLE
    lv_style_init(&stl_btn1);
    lv_style_set_bg_color(&stl_btn1, LV_STATE_DEFAULT, LV_COLOR_GREEN);
    lv_style_set_text_color(&stl_btn1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_color(&stl_btn1, LV_STATE_DEFAULT, LV_COLOR_GREEN);
 

  // BUTTON 2 STYLE
    lv_style_init(&stl_btn2);
    lv_style_set_bg_color(&stl_btn2, LV_STATE_DEFAULT, lv_color_make(0xf0, 0x1f, 0x1f));
    lv_style_set_text_color(&stl_btn2, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    
    // TRANSP STYLE
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
    
    // _lv_style_set_opa(&stl_transp,LV_STATE_DEFAULT,LV_OPA_TRANSP);
    


}

void SetupUI()
{
    
    setStyles();
    
    //MAIN VIEW
    lv_obj_t *view = lv_cont_create(lv_scr_act(), nullptr);
    lv_obj_set_size(view, 240, 240);
    lv_obj_add_style(view, LV_OBJ_PART_MAIN, &stl_view);

    //LOGO
    lv_obj_t *jabil_logo = lv_label_create(view, nullptr);
    lv_obj_add_style(jabil_logo, LV_OBJ_PART_MAIN, &stl_logo);
    lv_label_set_text(jabil_logo, "JABIL");
    lv_obj_align(jabil_logo, view, LV_ALIGN_IN_TOP_LEFT, 10, 5);

    lv_obj_t * lbl_percentsignal;
    lbl_percentsignal = lv_label_create(view, NULL);
    // lv_obj_add_style(lbl_percentsignal, LV_OBJ_PART_MAIN, &stl_clock);
    lv_label_set_text(lbl_percentsignal, "%");
    lv_obj_align(lbl_percentsignal, view, LV_ALIGN_IN_TOP_RIGHT, -5, 0);

    // CLOCK MINUTES
    g_data.minute = lv_label_create(view, nullptr);
    lv_obj_add_style(g_data.minute, LV_OBJ_PART_MAIN, &stl_clock);
    batlevel = (power->getBattVoltage()/100);
    // Serial.println("************** NIVEL DA BATERIA ************");
    // Serial.println(batlevel);
    sprintf (bufbat, "%.0f",batlevel);
    lv_label_set_text(g_data.minute,bufbat );
    lv_obj_align(g_data.minute, lbl_percentsignal, LV_ALIGN_IN_TOP_LEFT, -33, 0);

    // CLOCK HOURS
    // g_data.hour = lv_label_create(view, nullptr);
    // lv_obj_add_style(g_data.hour, LV_OBJ_PART_MAIN, &stl_clock);
    // lv_label_set_text(g_data.hour, "00");
    // lv_obj_align(g_data.hour, g_data.minute, LV_ALIGN_IN_TOP_MID, -35, 0);

     //TOP HORIZONTAL LINE
    static lv_point_t line_points[] = { {10, 0}, {230, 0} };
    lv_obj_t *line1;
    line1 = lv_line_create(view, NULL);
    lv_line_set_points(line1, line_points, 2);     /*Set the points*/
    lv_obj_add_style(line1, LV_OBJ_PART_MAIN, &stl_topline);
    lv_obj_align(line1, NULL, LV_ALIGN_IN_TOP_MID, 0, 35);
    
    // CARD BACKGROUND

    // LABEL NO CARD
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


    

    // WORKSTATION LABEL
     
    lbl_workstation = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_workstation, "WORKSTATION");
    lv_obj_align(lbl_workstation, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);

   // RISK LABEL
    
    lbl_risk = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_risk, "RISCO");
    lv_obj_align(lbl_risk, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 40);

    
    // CALL TIME LABEL
    
    lbl_status = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_status, "Status");
    lv_obj_align(lbl_status, NULL, LV_ALIGN_IN_TOP_RIGHT, -40, 100);
    // CALL TIME LABEL
    
    lbl_calltime = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_calltime, "12: 00");
    lv_obj_align(lbl_calltime, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 40);

    // DESCRIPTION LABEL
    
    lbl_description = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_description, "DESCRIPTION");
    lv_obj_align(lbl_description, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 70);

    lbl_user = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_user, "");
    lv_obj_align(lbl_user, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 100);


    // BUTTON 1
    btn1 = lv_btn_create(bg_card, NULL);
    lv_obj_set_pos(btn1, 10, 125);
    lv_obj_set_width(btn1,95);
    lv_obj_set_height(btn1,35);
    lv_obj_set_event_cb(btn1, sendRequest);
    
    // BUTTON 1 LABEL
    lv_obj_t * lbl_btn1;
    lbl_btn1 = lv_label_create(btn1, NULL);
    lv_label_set_text(lbl_btn1, "Confirmar");
    lv_label_set_text(lbl_btn1, LV_SYMBOL_OK);
    lv_obj_add_style(btn1, LV_OBJ_PART_MAIN, &stl_btn1);


    // BUTTON 2
    btn2 = lv_btn_create(bg_card, NULL);
    lv_obj_set_pos(btn2, 120, 125);
    lv_obj_set_width(btn2,95);
    lv_obj_set_height(btn2,35);
    lv_obj_set_event_cb(btn2, removefromArray);
    
    // BUTTON 2 LABEL
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
    // lv_obj_set_event_cb(btn_back, btn1_handler);
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
        // Serial.println("Clicked\n");
    
    atual = atual-1;
   // Serial.println("back");
  // Serial.println();
  Serial.print(atual);
  Serial.print("/");
  Serial.print(counter);

    printCard(atual-1);
    } 
    
}
static void btn2_handler(lv_obj_t *obj, lv_event_t event)
{
     if (event == LV_EVENT_CLICKED && atual < counter) {
        // Serial.println("Clicked\n");
    
    atual = atual+1;
    // Serial.println("next");
  // Serial.println();
  Serial.print(atual);
  Serial.print("/");
  Serial.print(counter);

    printCard(atual-1);
    }
   
    
}

void sendRequest(lv_obj_t *obj, lv_event_t event){

  if (event == LV_EVENT_CLICKED) {

     
    StaticJsonDocument<200> doc2;

      doc2["ticketId"] = chamados[((atual-1)*num_tickets)+4];
      doc2["ip"] = ip_address;
      
      String requestBody;
      serializeJson(doc2, requestBody);

      Serial.println("Request que vou fazer:");
      Serial.println(requestBody);
    if(asyncrequest.readyState() == 0 || asyncrequest.readyState() == 4){
   
        lv_obj_set_hidden(btn1, true);
        asyncrequest.open("POST", "http://10.57.16.40/JITAPI/Ticket/Confirm");
        // asyncrequest.open("POST", "http://10.57.38.133:3000/reloginho");
  
        asyncrequest.setReqHeader("Content-Type", "application/json");
        
        asyncrequest.send(requestBody);
    }
  }
}

void getWatchUser(){

  


    
  

    strcat(GetWatchById_Url, GetWatchById_host);
    strcat(GetWatchById_Url, ip_address);
    Serial.print("############ENDEREÇO DE API:");
    Serial.println(GetWatchById_Url);

    HTTPClient http;

    StaticJsonDocument<200> result;
    StaticJsonDocument<200> userjson;
        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        http.begin(GetWatchById_Url); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);
                deserializeJson(result, payload);
                // const char* id = result["user"]["id"];

                Serial.println("##########");
                // Serial.println(id);
                // auto payload = "..."; // JSON content here
                // StaticJsonDocument<1024> result;
                deserializeJson(result, payload);
                Serial.println(payload);
                // Serial.println(result);
                auto user = result["user"].as<const char*>();
                Serial.println(user);
                StaticJsonDocument<256> userObj;
                deserializeJson(userObj, user);
                auto id = userObj[0]["id"].as<int>();
                auto text = userObj[0]["text"].as<const char*>();
                Serial.println(id);
                Serial.println(text);
                // StaticJsonDocument<256> userObj;
                // deserializeJson(userObj, user);
                // auto id = userObj[0]["id"].as<int>();
                // auto text = userObj[0]["text"].as<const char*>();
                // Serial.println("#################");
                // Serial.println(id);
                // Serial.println(text);
                // Serial.println("#################");
    
                
                
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    


}
}
void charToString(const char S[], String &D)
{
 
 String rc(S);
 D = rc;
 
}

void requestGetCB(void* optParm, asyncHTTPrequest* getrequest, int readyState){
    if(readyState == 4){

        Serial.println("RESPOSTA REQUEST");
        Serial.println(readyState);
        Serial.println(getrequest->responseText());
        Serial.println();
        getrequest->setDebug(false);
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
        
    }
}

static void removefromArray(lv_obj_t *obj, lv_event_t event){

  if (event == LV_EVENT_CLICKED) {

    Serial.println("***********BOTAO VERMELHO ***********");
    uint8_t pos = atual-1;
    uint8_t tam = counter-1;
    uint8_t i;

    Serial.print("Counter:");
    Serial.println(counter);
    Serial.print("Atual:");
    Serial.println(atual);
    Serial.print("Pos:");
    Serial.println(pos);
    Serial.print("Tam:");
    Serial.println(tam);


    for(i=pos; i<tam; i++)
          {

              strcpy(chamados[(i*num_tickets)+0],chamados[(i*num_tickets)+num_tickets+0]);
              strcpy(chamados[(i*num_tickets)+1],chamados[(i*num_tickets)+num_tickets+1]);
              strcpy(chamados[(i*num_tickets)+2],chamados[(i*num_tickets)+num_tickets+2]);
              strcpy(chamados[(i*num_tickets)+3],chamados[(i*num_tickets)+num_tickets+3]);
              strcpy(chamados[(i*num_tickets)+4],chamados[(i*num_tickets)+num_tickets+4]);
              strcpy(chamados[(i*num_tickets)+5],chamados[(i*num_tickets)+num_tickets+5]);
              strcpy(chamados[(i*num_tickets)+6],chamados[(i*num_tickets)+num_tickets+6]);
          };


    counter--;

    Serial.print("Counter:");
    Serial.println(counter);
    Serial.print("Atual:");
    Serial.println(atual);

    if(atual==counter+1){

      Serial.println("Atual = counter+1");
      atual = counter;
    }

    printCard(atual-1);
  }
  if(counter==0){
      Serial.println("Counter=0");
      toggle_Cards_Off();
  }
  
};
  


void printCard(uint8_t posic){


    lv_obj_set_hidden(bg_card, true); 

    if(strcmp(chamados[(posic*num_tickets)+5],"Open")==0){
      // Serial.println("mostrei o botao");
      lv_obj_set_hidden(btn1, false); 
      
    }
    else{
      // Serial.println("escondi o botao");
      lv_obj_set_hidden(btn1, true);
      
    }
    lv_obj_set_hidden(bg_card, false); 
    lv_label_set_text(lbl_workstation,chamados[(posic*num_tickets)+0]);
    lv_label_set_text(lbl_risk,chamados[(posic*num_tickets)+1]);
    lv_label_set_text(lbl_calltime,chamados[(posic*num_tickets)+2]);
    lv_label_set_text(lbl_description,chamados[(posic*num_tickets)+3]);
    lv_label_set_text(lbl_status,chamados[(posic*num_tickets)+5]);
    lv_label_set_text(lbl_user,chamados[(posic*num_tickets)+6]);

    Serial.print(atual);
    Serial.print("/");
    Serial.print(counter);
  
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

    //!Clear IRQ unprocessed  first
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
    
    

    Serial.print("Wifi IP: ");
    // Serial.println(WiFi.localIP());

    
  

    getrequest.setDebug(true);
    getrequest.onReadyStateChange(requestGetCB);

    asyncrequest.setDebug(true);
    asyncrequest.onReadyStateChange(requestCB);
    getWatchUser();

    server.onNotFound(notFound);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Andre Lara hehehe 8)");
    });

    AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/receberchamado", [](AsyncWebServerRequest *request, JsonVariant &json) {
  
        Serial.println("********************************** /RECEBERCHAMADO");
        JsonObject jsonObj = json.as<JsonObject>();

        String atualizarchamadopayload;
      serializeJson(jsonObj, atualizarchamadopayload);
      Serial.println("esse eh o chamado q estou receebendo");
      Serial.println(atualizarchamadopayload);
        

        if(!(counter==num_tickets)){

          if(BLisOn){
          
          }
          else{
            ttgo->openBL();
            BLisOn = true;
          }
        ttgo->motor->onec();
          strcpy(chamados[(counter*num_tickets)+0],jsonObj["workstation"]);
        

        strcpy(chamados[(counter*num_tickets)+1],jsonObj["risk"]);
        Serial.println("TESTE DO RISCO");
        

        Serial.print("Bool 1:");
        Serial.println(strcmp(chamados[(counter*num_tickets)+1],"1")==0);
        Serial.print("Bool 2:");
        Serial.println(strcmp(chamados[(counter*num_tickets)+1],"0")==0);
        Serial.print("Bool 3:");
        Serial.println(strcmp(chamados[(counter*num_tickets)+1],"1"));
        Serial.print("Bool 4:");
        Serial.println(strcmp(chamados[(counter*num_tickets)+1],"0"));
        if(strcmp(chamados[(counter*num_tickets)+1],"1")==0){
          sprintf(chamados[(counter*num_tickets)+1],"Rodando");
          Serial.println("Linha rodando");
        }
        else if(strcmp(chamados[(counter*num_tickets)+1],"0")==0){
          Serial.println("Linha parada");
          sprintf(chamados[(counter*num_tickets)+1],"Parada");
        }
        Serial.println(strcmp(chamados[(counter*num_tickets)+1],"1"));
        Serial.println(strcmp(chamados[(counter*num_tickets)+1],"0"));
        strcpy(chamados[(counter*num_tickets)+2],jsonObj["calltime"]);

        strcpy(chamados[(counter*num_tickets)+3],jsonObj["description"]);
       
        strcpy(chamados[(counter*num_tickets)+4],jsonObj["id"]);
        strcpy(chamados[(counter*num_tickets)+5],"Open");
        strcpy(chamados[(counter*num_tickets)+6],"");
        counter = counter +1;
        atual = counter;
        printCard(counter-1);
        
        toggle_Cards_On();
        
        request->send(200);
        ttgo->motor->onec();
        }
        

    });

    AsyncCallbackJsonWebHandler* handler2 = new AsyncCallbackJsonWebHandler("/atualizarchamado", [](AsyncWebServerRequest *request, JsonVariant &json) {
  

      Serial.println("********************************** /ATUALIZARCHAMADO");
        JsonObject jsonObj = json.as<JsonObject>();

        String atualizarchamadopayload;
      serializeJson(jsonObj, atualizarchamadopayload);
      Serial.println("esse eh o chamado q estou receebendo");
      Serial.println(atualizarchamadopayload);
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
         char nomepeq[10] = "a";
        strcpy(idbuscado,jsonObj["TicketId"]);
        Serial.print("ID RECEBIDA:");
        Serial.println(idbuscado);
        
        
        Serial.println("USUARIO RECEBIDO");
        
        // String name = jsonObj["UserName"];
        // name.toCharArray(userbuscado,20);
        strcpy(userbuscado,jsonObj["UserName"]);
        Serial.println(userbuscado);
        for(int z=0;z<10;z++){
          Serial.println(userbuscado[z]);
          
          if(isWhitespace(userbuscado[z])) {
            
            break;
          }
          else{
            nomepeq[z]=userbuscado[z];

          }
        }
         Serial.println("USUARIO trim");
         Serial.println(nomepeq);
        String stats = jsonObj["Status"];
        stats.toCharArray(statusbuscado,20);
        Serial.println("STATUS RECEBIDO");
        // strcpy(statusbuscado,jsonObj["Status"]);
        Serial.println(stats);
        Serial.println(statusbuscado);
        

        Serial.print("Counter: ");
        Serial.println(counter);
        for (int i = 4; i <= ((counter*num_tickets)-3); i=i+num_tickets) {
        Serial.print("i: ");
        Serial.println(i);

        Serial.print("Ids: ");
        Serial.println(chamados[i]);

           if(strcmp(chamados[i], idbuscado)==0){

            Serial.println("Achei meu chamado buscado");
            strcpy(chamados[i+1],statusbuscado);
            strcpy(chamados[i+2],nomepeq);
            Serial.println(chamados[i-4]);
            Serial.println(chamados[i-3]);
            Serial.println(chamados[i-2]);
            Serial.println(chamados[i-1]);
            Serial.println(chamados[i]);
            Serial.println(chamados[i+1]);
            Serial.println(chamados[i+2]);
            atual=((i-4)/num_tickets)+1;
            printCard(atual-1);
            if(strcmp(statusbuscado,"Done")==0){
              // Serial.println("===================ENTREI NO IF DO DONE");
              removefromArray(btn2,LV_EVENT_CLICKED);
            }
            
            break;
           }
        }
          // Serial.print("Comparei ");
          // Serial.print(chamados[i]);
          // Serial.print(" com ");
          // Serial.print(idbuscado);
        //   // Serial.println("Boolean:");
        //   // Serial.println(idbuscado==chamados[i]);

        //   // Serial.println("****ITERANDO***");
        //   if(strcmp(chamados[i], idbuscado)==0){

            
        //     Serial.println("Achei meu chamado buscado");
        //     strcpy(chamados[i+1],statusbuscado);
        //     strcpy(chamados[i+2],userbuscado);
            
        //     // Serial.println(chamados[i-4]);
        //     // Serial.println(chamados[i-3]);
        //     // Serial.println(chamados[i-2]);
        //     // Serial.println(chamados[i-1]);
        //     // Serial.println(chamados[i]);
        //     // Serial.println(chamados[i+1]);
        //     // Serial.println(chamados[i+2]);
        //     atual=((i-4)/num_tickets)+1;
        //     printCard(atual-1);

        //     // Serial.println("===================STATUS EH DONE OU NAO");
        //       // Serial.println(statusbuscado);
        //       // Serial.println(statusbuscado=="Done");
        //       // Serial.println(strcmp(statusbuscado,"Done"));
              
        //     if(strcmp(statusbuscado,"Done")==0){
        //       // Serial.println("===================ENTREI NO IF DO DONE");
        //       removefromArray(btn2,LV_EVENT_CLICKED);
        //     }
            
        //     break;

        //   }
    
    
        //}

        Serial.println("Vou rodar agora a resposta do request");
        request->send(200);
        ttgo->motor->onec();

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