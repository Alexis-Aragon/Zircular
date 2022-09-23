#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  40        /* Time ESP32 will go to sleep (in seconds) */


void deepSleep(){
  // delay(20000);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR); // ESP32 wakes up every 5 seconds

  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON); // all RTC Peripherals are powered
  
  Serial.println("Going to deep-sleep now");
  Serial.flush(); 
  esp_deep_sleep_start();
  
}
