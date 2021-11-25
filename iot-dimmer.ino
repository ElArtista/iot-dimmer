/*
 * IoT Dimmer
 */

#include <ESP8266WiFi.h>

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

    // Initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
}

// The loop function runs over and over again forever
void loop() {
    unsigned long curMs = millis();
    if (curMs - prevMs >= 1000) {
        prevMs = curMs;
        const int led = LED_BUILTIN;
        int ledState = digitalRead(led);
        digitalWrite(led, ledState == LOW ? HIGH : LOW);
    }
}
