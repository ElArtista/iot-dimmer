/*
 * IoT Dimmer
 */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* server = MQTT_SERVER;
const char* topic = MQTT_TOPIC;

WiFiClient espClient;
PubSubClient client(espClient);

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

    // Setup mqtt connection
    client.setServer(server, 1883);
    client.setCallback([](char* topic, byte* payload, unsigned int length){
        Serial.print("Message arrived on topic [");
        Serial.print(topic);
        Serial.print("] ");
        for (int i = 0; i < length; i++) {
            Serial.print((char)payload[i]);
        }
        Serial.println();

        // Switch on the LED if an 1 was received as first character
        if ((char)payload[0] == '1') {
            // Turn the LED on (Note that LOW is the voltage level
            // but actually the LED is on; this is because
            // it is active low on the ESP-01)
            digitalWrite(BUILTIN_LED, LOW);
        } else {
            // Turn the LED off by making the voltage HIGH
            digitalWrite(BUILTIN_LED, HIGH);
        }
    });
}

void reconnect() {
    // Create a random client ID
    String clientId = "esp8266-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientId.c_str())) {
        Serial.println(" connected.");
        client.subscribe(topic);
    } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
    }
}

// The loop function runs over and over again forever
void loop() {
    // Handle ota updates
    ArduinoOTA.handle();

    // Try to reconnect if not connected
    if (!client.connected()) {
        reconnect();
    }

    // Handle mqtt traffic
    client.loop();
}
