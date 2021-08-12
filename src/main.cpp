#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#include "LittleFS.h"

#include "WiFiManager.h"
#include "webServer.h"
#include "updater.h"
#include "fetch.h"
#include "configManager.h"
#include "timeSync.h"
#include <TZ.h>

#define BUTTON D5

//the ISR function needs this flag
IRAM_ATTR void buttonPress() {

}

void setup() 
{
    Serial.begin(115200);

    LittleFS.begin();
    GUI.begin();
    configManager.begin();
    WiFiManager.begin(configManager.data.projectName);

    Serial.print("Syncing time for ");
    Serial.print(configManager.data.timeZone);
    timeSync.begin(configManager.data.timeZone);

    Serial.println("Hello world");
   
    //Wait for connection
    timeSync.waitForSyncResult(10000);

    if (timeSync.isSynced())
    {
        time_t now = time(nullptr);
        Serial.print(PSTR("Current time: "));
        Serial.print(asctime(localtime(&now)));
    }
    else 
    {
        Serial.println("Timeout while receiving the time");
    }

    //set up pins
    pinMode(BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON), buttonPress, FALLING);
}

void loop() 
{
    //software interrupts
    WiFiManager.loop();
    updater.loop();
    configManager.loop();

    //your code here
}