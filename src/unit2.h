#include <WiFi.h>
#include <lvgl.h>
#include "ui.h"
#include <Firebase_ESP_Client.h>

bool signupOK = true;
FirebaseAuth auth;
FirebaseConfig config;

// Define Firebase Data object
//sensor
FirebaseData fbdoph;
FirebaseData fbdotss;
FirebaseData fbdotl;

//relay
FirebaseData fbdoswpin;
FirebaseData fbdoswpout;
FirebaseData fbdoswec;
FirebaseData fbdoswmd;

String swpins;
String swpouts;
String swecs;
String swmodemo;

 int lvv;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "WIFI_AP"
#define WIFI_PASSWORD "WIFI_PASSWORD"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyDJrOElB47VcIWRTFgG6u3tCtBExkVV2JU"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://esp32server-c1cd2-default-rtdb.asia-southeast1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "fyudhaprawira@gmail.com"
#define USER_PASSWORD "11111111"

/*============================
       Switch update state
==============================*/
// Switch event handler
static void swmode(lv_event_t * e)
{
       lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
      bool state = lv_obj_has_state(obj,LV_STATE_CHECKED);

       if (state) {
            // Switch is ON
             
            
        } else {
            // Switch is OFF
        
            
        }
        

    }
}

static void swpin(lv_event_t * e)
{
       lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
      bool state = lv_obj_has_state(obj,LV_STATE_CHECKED);

       if (state) {
            // Switch is ON
            if (Firebase.RTDB.getString(&fbdoswpin, "/control/SWPIN")) {
            if (fbdoswpin.dataTypeEnum() == firebase_rtdb_data_type_string) {
              swpins = fbdoswpin.stringData();
              lv_label_set_text(ui_PINS,swpins.c_str());
            }
             
        } else {
            // Switch is OFF
             Serial.print("pin OFF\n");
        }
        

            }   
        }
}



static void swpout(lv_event_t * e)
{
       lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
      bool state = lv_obj_has_state(obj,LV_STATE_CHECKED);

       if (state) {
            // Switch is ON
             Serial.print("pout ON\n");
        } else {
            // Switch is OFF
             Serial.print("pout OFF\n");
        }
        

    }
}

static void swpec(lv_event_t * e)
{
       lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
      bool state = lv_obj_has_state(obj,LV_STATE_CHECKED);

       if (state) {
            // Switch is ON
             Serial.print("ec ON\n");
        } else {
            // Switch is OFF
             Serial.print("ec OFF\n");
        }
        
    }
}

/*============================
       Switch update call
==============================*/
void updatestatesw(void *pvParameters){
    while ((1))
    {
        /* code */

        lv_obj_add_event_cb(ui_SWM,swmode,LV_EVENT_ALL,NULL); 
          lv_obj_add_event_cb(ui_SWPIN,swpin,LV_EVENT_ALL,NULL); 
          lv_obj_add_event_cb(ui_SWPOUT,swpout,LV_EVENT_ALL,NULL);
          lv_obj_add_event_cb(ui_SWPOUT1,swpec,LV_EVENT_ALL,NULL);   
          lv_bar_set_value(ui_Bar1,lvv,LV_ANIM_ON);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
        }

void taskfb(void *pvParameters){
    while (1)
    {
       // Firebase.ready() should be called repeatedly to handle authentication tasks.
        if (Firebase.RTDB.getInt(&fbdoph, "/sensorread/pH")) {
            if (fbdoph.dataTypeEnum() == firebase_rtdb_data_type_integer) {
            int32_t phv =fbdoph.intData() ;
            char phv_str[10];
            snprintf(phv_str,sizeof(phv_str),"%d",phv);
            lv_label_set_text(ui_phvllb,phv_str);
            }
        }
        if (Firebase.RTDB.getInt(&fbdotss, "/sensorread/tss")) {
            if (fbdotss.dataTypeEnum() == firebase_rtdb_data_type_integer) {
            int32_t tssv =fbdotss.intData() ;
            char tssv_str[10];
            snprintf(tssv_str,sizeof(tssv_str),"%d",tssv);
            lv_label_set_text(ui_tssvllb,tssv_str);
            }
             }
             if (Firebase.RTDB.getInt(&fbdotl, "/sensorread/level")) {
            if (fbdotl.dataTypeEnum() == firebase_rtdb_data_type_integer) {
            lvv =fbdotl.intData() ;
            char lvvs_str[10];
            snprintf(lvvs_str,sizeof(lvvs_str),"%d",lvv);
            
            }
             }

             if (Firebase.RTDB.getString(&fbdoswpin, "/control/SWPIN")) {
            if (fbdoswpin.dataTypeEnum() == firebase_rtdb_data_type_string) {
              swpins = fbdoswpin.stringData();
              lv_label_set_text(ui_PINS,swpins.c_str());
            }
             }
                  if (Firebase.RTDB.getString(&fbdoswpout, "/control/SWPOUT")) {
            if (fbdoswpout.dataTypeEnum() == firebase_rtdb_data_type_string) {
              swpouts = fbdoswpout.stringData();
              lv_label_set_text(ui_POTS,swpouts.c_str());
            }
             }
                 if (Firebase.RTDB.getString(&fbdoswec, "/control/SWEC")) {
            if (fbdoswec.dataTypeEnum() == firebase_rtdb_data_type_string) {
              swecs = fbdoswec.stringData();
              lv_label_set_text(ui_ECS,swecs.c_str());
            }
             }
              if (Firebase.RTDB.getString(&fbdoswmd, "/control/SWMO")) {
            if (fbdoswmd.dataTypeEnum() == firebase_rtdb_data_type_string) {
              swmodemo = fbdoswmd.stringData();
              lv_label_set_text(ui_STATEMODE,swmodemo.c_str());
            }
             }


                 vTaskDelay(pdMS_TO_TICKS(2000));
                }
}

static void main_createtask (void){
      //xTaskCreatePinnedToCore(updateWifiInfoTask, "UpdateWifiInfoTask", 10000, NULL, 4, NULL, 1);
      xTaskCreatePinnedToCore(updatestatesw, "updatestatesw", 10000, NULL, 4, NULL, 1);
     xTaskCreatePinnedToCore(taskfb, "taskvb", 10000, NULL, 4, NULL, 1);
       
}