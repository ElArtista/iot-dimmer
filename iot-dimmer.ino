/*
 * IoT Dimmer
 */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// Blink state
unsigned long prevMs = 0;

// The setup function runs once when you press reset or power the board
void setup() {
    // Setup serial and print boot message
    Serial.begin(115200);
    Serial.println("Booting");

    // Setup WiFi in station mode and connect to given AP
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Wait for WiFi to connect
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    // Print assigned IP address
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Over the Air updates
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_FS
            type = "filesystem";
        }
        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        }
    });
    ArduinoOTA.begin();

    // Initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
}

// The loop function runs over and over again forever
void loop() {
    ArduinoOTA.handle();

    unsigned long curMs = millis();
    if (curMs - prevMs >= 1000) {
        prevMs = curMs;
        const int led = LED_BUILTIN;
        int ledState = digitalRead(led);
        digitalWrite(led, ledState == LOW ? HIGH : LOW);
    }
}
