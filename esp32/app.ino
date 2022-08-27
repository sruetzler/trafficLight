//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial
#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include "src/Config.h"
#include "src/TrafficLight.h"
#include "src/Bluetooth.h"

// #include <WiFiManager.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

Config g_config;
TrafficLight* g_trafficLight;
Bluetooth* g_blueTooth;
// WiFiManager manager;

void setup() {
  Serial.begin(115200);
  g_config.init();
    // bool success = manager.autoConnect("Fussgaengerampel");
 
    // if(!success) {
    //     Serial.println("Failed to connect");
    // } 
    // else {
    //     Serial.println("Connected");
    // }  
  // try{

  // }catch(Exception e){


  // }

  g_trafficLight = new TrafficLight(&g_config);
  g_blueTooth = new Bluetooth(&g_config, g_trafficLight);
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  g_trafficLight->loop();
  g_blueTooth->loop();
  delay(20);
}