

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"

#include <ESPAsyncWebServer.h>
#include "config.h"


TTGOClass *ttgo;
PCF8563_Class *rtc;

AsyncWebServer server(80);

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


static lv_obj_t * lbl_workstation;
static lv_obj_t * lbl_risk;
static lv_obj_t * lbl_calltime;
static lv_obj_t * lbl_description;

static lv_obj_t * lbl_actualcard;
static lv_obj_t * lbl_totalcard;

static lv_obj_t * btn_back;
static lv_obj_t * btn_next;

const char* ssid = "2.4G Netvirtua apto 305.";
const char* password = "http3333";

char bufatual [1];
char buftotal [1];
const char* PARAM_MESSAGE = "message";
typedef struct {
    lv_obj_t *hour;
    lv_obj_t *minute;
    lv_obj_t *second;
} str_datetime_t;

typedef struct  {
     const char *workstation;
    const char *risk;
    const char *calltime;
    const char *description;
}str_card_t ;
str_card_t novocard;
str_card_t createCard(const char *workstation, const char *risk,const char *calltime,const char *description){
    
    Serial.println("/////// CRIAR NOVO CARD/////");
    str_card_t card;
    card.workstation = workstation;
    card.risk = risk;
    card.calltime = calltime;
    card.description = description;
    return card;

}
 str_card_t calls[10];

char chamados[50][50];


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

    // CLOCK MINUTES
    g_data.minute = lv_label_create(view, nullptr);
    lv_obj_add_style(g_data.minute, LV_OBJ_PART_MAIN, &stl_clock);
    lv_label_set_text(g_data.minute, "00");
    lv_obj_align(g_data.minute, view, LV_ALIGN_IN_TOP_RIGHT, -10, 0);

    // CLOCK HOURS
    g_data.hour = lv_label_create(view, nullptr);
    lv_obj_add_style(g_data.hour, LV_OBJ_PART_MAIN, &stl_clock);
    lv_label_set_text(g_data.hour, "00");
    lv_obj_align(g_data.hour, g_data.minute, LV_ALIGN_IN_TOP_MID, -35, 0);

     //TOP HORIZONTAL LINE
    static lv_point_t line_points[] = { {10, 0}, {230, 0} };
    lv_obj_t *line1;
    line1 = lv_line_create(view, NULL);
    lv_line_set_points(line1, line_points, 2);     /*Set the points*/
    lv_obj_add_style(line1, LV_OBJ_PART_MAIN, &stl_topline);
    lv_obj_align(line1, NULL, LV_ALIGN_IN_TOP_MID, 0, 35);
    
    // CARD BACKGROUND
    lv_obj_t * bg_card;
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
<<<<<<< HEAD
    lv_label_set_text(lbl_risk, "RISCO");
=======
    lv_label_set_text(lbl_risk, "RISK");
>>>>>>> ce14ea13e6074bf73f13af529d83474aba35eef4
    lv_obj_align(lbl_risk, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 40);

    // CALL TIME LABEL
    
    lbl_calltime = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_calltime, "12: 00");
    lv_obj_align(lbl_calltime, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 40);

    // DESCRIPTION LABEL
    
    lbl_description = lv_label_create(bg_card, NULL);
    lv_label_set_text(lbl_description, "DESCRIPTION");
    lv_obj_align(lbl_description, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 70);


    // BUTTON 1
    lv_obj_t * btn1 = lv_btn_create(bg_card, NULL);
    lv_obj_set_pos(btn1, 10, 125);
    lv_obj_set_width(btn1,95);
    lv_obj_set_height(btn1,35);
    lv_obj_set_event_cb(btn1, btn1_handler);
    
    // BUTTON 1 LABEL
    lv_obj_t * lbl_btn1;
    lbl_btn1 = lv_label_create(btn1, NULL);
    lv_label_set_text(lbl_btn1, "Finalizar");
    lv_label_set_text(lbl_btn1, LV_SYMBOL_OK);
    lv_obj_add_style(btn1, LV_OBJ_PART_MAIN, &stl_btn1);


    // BUTTON 2
    lv_obj_t * btn2 = lv_btn_create(bg_card, NULL);
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


    lv_obj_t * lbl_count_separator;
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
    

    // lv_task_create([](lv_task_t *t) {

    //     RTC_Date curr_datetime = rtc->getDateTime();
        
    //     lv_label_set_text_fmt(g_data.minute, "%02u", curr_datetime.minute);
    //     lv_label_set_text_fmt(g_data.hour, "%02u", curr_datetime.hour);

    // }, 1000, LV_TASK_PRIO_MID, nullptr);

    
    // setCpuFrequencyMhz(20);

    
}


static void btn1_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED && atual >1) {
        Serial.println("Clicked\n");
    
    atual = atual-1;
    sprintf (bufatual, "%d",atual);
  lv_label_set_text(lbl_actualcard,bufatual);
  Serial.println();
  Serial.print(atual);
  Serial.print("/");
  Serial.print(counter);

    printCard(atual-1);
    } else if (event == LV_EVENT_VALUE_CHANGED) {
        Serial.printf("Toggled\n");
    }
}
static void btn2_handler(lv_obj_t *obj, lv_event_t event)
{
     if (event == LV_EVENT_CLICKED && atual < counter) {
        Serial.println("Clicked\n");
    
    atual = atual+1;
    sprintf (bufatual, "%d",atual);
  lv_label_set_text(lbl_actualcard,bufatual);
  Serial.println();
  Serial.print(atual);
  Serial.print("/");
  Serial.print(counter);

    printCard(atual-1);
    } else if (event == LV_EVENT_VALUE_CHANGED) {
        Serial.printf("Toggled\n");
    }
}

static void removefromArray(lv_obj_t *obj, lv_event_t event){

  if (event == LV_EVENT_CLICKED) {

    uint8_t pos = atual-1;
    uint8_t tam = counter-1;
    uint8_t i;
    Serial.println();
    Serial.print("Posição inicial no array: ");
    Serial.print(pos);
    Serial.println();
    Serial.print("Posição maxima do array: ");
    Serial.print(tam);


    Serial.println();
    
    
    
    
    for(i=pos; i<tam; i++)
          {
              Serial.print((i*4)+0);
               Serial.print("   =>   ");
               Serial.print((i*4)+4);
               Serial.println();
              strcpy(chamados[(i*4)+0],chamados[(i*4)+4]);
              strcpy(chamados[(i*4)+1],chamados[(i*4)+5]);
              strcpy(chamados[(i*4)+2],chamados[(i*4)+6]);
              strcpy(chamados[(i*4)+3],chamados[(i*4)+7]);
          };


    counter--;
    Serial.println();
    Serial.print("Quantidade de objetos agora é ");
    Serial.print(counter);

    if(atual==counter+1){
      Serial.println("Ultimo da lista!");
      atual = counter;
    }
    sprintf (buftotal, "%d",counter);
  lv_label_set_text(lbl_totalcard,buftotal);
  sprintf (bufatual, "%d",atual);
  lv_label_set_text(lbl_actualcard,bufatual);
  Serial.println();
    Serial.print("Objeto a ser impresso: ");
    Serial.print(atual);
    printCard(atual-1);
  }
  
};
  



void printCard(uint8_t posic){


Serial.println("\n*************** imprimir card **************\n");

    lv_label_set_text(lbl_workstation,calls[counter].workstation);
  lv_label_set_text(lbl_risk,calls[counter].risk);
  lv_label_set_text(lbl_calltime,calls[counter].calltime);
  lv_label_set_text(lbl_description,calls[counter].description);
Serial.println("///////   Posição no array://////////");
Serial.println(posic);
// Serial.println("///////   Endereço objeto://////////");
// Serial.println((long)&calls[counter]);
  Serial.println("/////   workstation  ///////");
  Serial.println(calls[posic].workstation);
  Serial.println((long)&calls[posic].workstation);
  // Serial.println(chamados[(posic*4)+0]);
  Serial.println("/////   risk  ///////");
  Serial.println(calls[posic].risk);
  Serial.println((long)&calls[posic].risk);
  // Serial.println(chamados[(posic*4)+1]);
  Serial.println("/////   calltime  ///////");
  Serial.println(calls[posic].calltime);
  Serial.println((long)&calls[posic].calltime);
  // Serial.println(chamados[(posic*4)+2]);
  Serial.println("/////   description  ///////");
  Serial.println(calls[posic].description);
  Serial.println((long)&calls[posic].description);
  // Serial.println(chamados[(posic*4)+3]);
}

void setup() {

    
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->motor_begin();
    ttgo->lvgl_begin();
    SetupUI();
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.onNotFound(notFound);

    AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/rest/endpoint", [](AsyncWebServerRequest *request, JsonVariant &json) {
  JsonObject jsonObj = json.as<JsonObject>();

   ttgo->motor->onec();
  lv_label_set_text(lbl_workstation,jsonObj["workstation"]);
  lv_label_set_text(lbl_risk,jsonObj["risk"]);
  lv_label_set_text(lbl_calltime,jsonObj["calltime"]);
  lv_label_set_text(lbl_description,jsonObj["description"]);

  Serial.println("*********  NOVO CARD  *********");
  

    Serial.println("Valor counter");
    Serial.println(counter);
    // novocard = createCard(jsonObj["workstation"],jsonObj["workstation"],jsonObj["workstation"],jsonObj["workstation"]);
  // Serial.println("Endereço newcard");
  // Serial.println((long)&novocard);

    // str_card_t newcard;
    // newcard.workstation = jsonObj["workstation"];
    // newcard.risk = jsonObj["risk"];
    // newcard.calltime = jsonObj["calltime"];
    // newcard.description = jsonObj["description"];
    // calls[counter] = novocard;
    
    
    Serial.println("*********  ENDEREÇO WORKSTATION  *********");
    
    calls[counter].workstation = jsonObj["workstation"];
    Serial.println((long)&calls[counter].workstation);
    Serial.println(calls[counter].workstation);
    // strcpy(chamados[(counter*4)+0],jsonObj["workstation"]);
    Serial.println("*********  ENDEREÇO RISK  *********");
<<<<<<< HEAD
=======
    // Serial.println((long)&calls[counter].risk);
    
>>>>>>> ce14ea13e6074bf73f13af529d83474aba35eef4
    calls[counter].risk = jsonObj["risk"];
    Serial.println((long)&calls[counter].risk);
    Serial.println(calls[counter].risk);
    // strcpy(chamados[(counter*4)+1],jsonObj["risk"]);
    Serial.println("*********  ENDEREÇO CALLTIME  *********");
<<<<<<< HEAD
=======
    // Serial.println((long)&calls[counter].calltime);
>>>>>>> ce14ea13e6074bf73f13af529d83474aba35eef4
    calls[counter].calltime = jsonObj["calltime"];
    Serial.println((long)&calls[counter].calltime);
    Serial.println(calls[counter].calltime);
    // strcpy(chamados[(counter*4)+2],jsonObj["calltime"]);
    Serial.println("*********  ENDEREÇO DESCRIPTION  *********");
<<<<<<< HEAD
=======
    // Serial.println((long)&calls[counter].description);
>>>>>>> ce14ea13e6074bf73f13af529d83474aba35eef4
    calls[counter].description = jsonObj["description"];
    Serial.println((long)&calls[counter].description);
    Serial.println(calls[counter].description);
    // strcpy(chamados[(counter*4)+3],jsonObj["description"]);
  printCard(counter);
  counter = counter +1;
  atual = counter;
<<<<<<< HEAD
  Serial.println();
  Serial.print(atual);
  Serial.print("/");
  Serial.print(counter);
=======
  // Serial.println();
  // Serial.print(atual);
  // Serial.print("/");
  // Serial.print(counter);
>>>>>>> ce14ea13e6074bf73f13af529d83474aba35eef4
  


sprintf (buftotal, "%d",counter);
  lv_label_set_text(lbl_totalcard,buftotal);
  lv_label_set_text(lbl_actualcard,buftotal);
  

  

  request->send(200);
});
server.addHandler(handler);

    server.begin();
    log_d("Total heap: %d", ESP.getHeapSize());
  log_d("Free heap: %d", ESP.getFreeHeap());
  log_d("Used heap: %d", ESP.getHeapSize() - ESP.getFreeHeap());
  log_d("Total PSRAM: %d", ESP.getPsramSize());
  log_d("Free PSRAM: %d", ESP.getFreePsram());
  log_d("Used PSRAM: %d", ESP.getPsramSize() - ESP.getFreePsram());
}

void loop() {

    lv_task_handler();
    delay(5);
}