/*
 * IoT Dimmer
 */

#include <ESP8266WiFi.h>

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

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
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(1000);                     // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(1000);                     // wait for a second
}
