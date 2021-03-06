/*
 * Sensor de consumo eléctrico
 * Para ESP32 TTGOv2
 * por Greencore Solutions
 * Usa serial2 y empaqueta hacia TTN
 * 
 * Debe definir APPEUI, DEVEUI, APPKEY
 */


#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Wire.h>
#include<U8g2lib.h>
#include<Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <Arduino_SNMP.h>

#include "config.h"

#ifdef USE_IAS
#define COMPDATE __DATE__ __TIME__
#define MODEBUTTON 0

#include <IOTAppStory.h>
IOTAppStory IAS(COMPDATE, MODEBUTTON);
#endif

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16); 

WiFiUDP udp;
SNMPAgent snmp = SNMPAgent("greencore");  // Starts an SMMPAgent instance with the community string 'public'
char* snmpAmp1 = "0.0";
char* snmpAmp2 = "0.0";
char* snmpAmp3 = "0.0";
char* snmpAmp4 = "0.0";

const char* prueba;

unsigned long previousMillis = 0;
const long interval = 300000;
unsigned int paqCont = 0;
uint8_t mydata[] = "00000000000000000";

// Para lectura de Serial2
String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

static osjob_t sendjob;

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void logo(){
    // LOGO
    u8g2.clearBuffer();
    u8g2.clearDisplay();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawXBMP(39,0,50,50,greenfoot_bits);
    u8g2.drawStr(5,64,"GreenCore Solutions");
    u8g2.sendBuffer();
}

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            LMIC_setLinkCheckMode(0);
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if(LMIC.dataLen) {
                // data received in rx slot after tx
                Serial.print(F("Data Received: "));
                Serial.write(LMIC.frame+LMIC.dataBeg, LMIC.dataLen);
                Serial.println();
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
         default:
            Serial.println(F("Unknown event"));
            break;
    }
}

void do_send(osjob_t* j){
    unsigned int sensorCount = 0;
    String sensorData[4] = "0000";

    // TODO: Mover a funcion
    while (Serial2.available()) {
      // get the new byte:
      char inChar = (char)Serial2.read();
      // add it to the inputString:
      if (inChar == ':') {
        sensorData[sensorCount] = inputString;
        inputString="";
        sensorCount++;        
      } else if (inChar == '\n') {
        sensorData[3]= inputString;
        Serial.print("sensorData[0]: ");
        Serial.println(sensorData[0]);
        Serial.print("sensorData[1]: ");
        Serial.println(sensorData[1]);
        Serial.print("sensorData[2]: ");
        Serial.println(sensorData[2]);
        Serial.print("sensorData[3]: ");
        Serial.println(sensorData[3]); 
        inputString = "";
        stringComplete = true;
        sensorCount = 0;
      } else {
        inputString += inChar;
      }
    } 

    // Creando paquete para sensor0-3
    int tempInteger = sensorData[0].toInt();
    mydata[1] = highByte(tempInteger);
    mydata[2] = lowByte(tempInteger);
    float tempFloat = (sensorData[0].toFloat() - tempInteger) * 100;
    tempInteger = int(tempFloat);
    mydata[3] = highByte(tempInteger);
    mydata[4] = lowByte(tempInteger);
    String str1 = String(sensorData[0]);
    str1.toCharArray(snmpAmp1, str1.length()+1);

    tempInteger = sensorData[1].toInt();
    mydata[5] = highByte(tempInteger);
    mydata[6] = lowByte(tempInteger);
    tempFloat = (sensorData[1].toFloat() - tempInteger) * 100;
    tempInteger = int(tempFloat);
    mydata[7] = highByte(tempInteger);
    mydata[8] = lowByte(tempInteger);
    String str2 = String(sensorData[1]);
    str2.toCharArray(snmpAmp2, str2.length()+1);

    tempInteger = sensorData[2].toInt();
    mydata[9] = highByte(tempInteger);
    mydata[10] = lowByte(tempInteger);
    tempFloat = (sensorData[2].toFloat() - tempInteger) * 100;
    tempInteger = int(tempFloat);
    mydata[11] = highByte(tempInteger);
    mydata[12] = lowByte(tempInteger);
    String str3 = String(sensorData[2]);
    str3.toCharArray(snmpAmp3, str3.length()+1);

    tempInteger = sensorData[3].toInt();
    mydata[13] = highByte(tempInteger);
    mydata[14] = lowByte(tempInteger);
    tempFloat = (sensorData[3].toFloat() - tempInteger) * 100;
    tempInteger = int(tempFloat);
    mydata[15] = highByte(tempInteger);
    mydata[16] = lowByte(tempInteger);
    String str4 = String(sensorData[3]);
    str4.toCharArray(snmpAmp4, str4.length()+1);

    unsigned long currentMillis = millis();
    
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
           // Prepare upstream data transmission at the next possible time.
           LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
        }
    Serial.println(F("Packet queued"));
    Serial.println(LMIC.freq);

 // INFORMACIÓN GENERAL
    char frecString[10]; 
    dtostrf(LMIC.freq/1000000.0,3,2,frecString);
    char paqString02[10]; 
    dtostrf(paqCont,2,0,paqString02);
    u8g2.clearBuffer();
    u8g2.clearDisplay();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 10, "Consumo Eléctrico");
    u8g2.drawStr(0, 20, "Frecuencia: ");
    u8g2.drawStr(68,20,frecString);
    u8g2.drawStr(100,20," MHz");
    u8g2.drawStr(0,30,"Paquete: ");
    u8g2.drawStr(50,30,paqString02);

    u8g2.drawStr(0,40,"s1: ");
    u8g2.drawStr(60,40,"s2: ");
    u8g2.drawStr(0,50,"s3: ");
    u8g2.drawStr(60,50,"s4: ");

    char sensorData0[5];
    char sensorData1[5];
    char sensorData2[5];
    char sensorData3[5];
    sensorData[0].toCharArray(sensorData0, 5);
    sensorData[1].toCharArray(sensorData1, 5);
    sensorData[2].toCharArray(sensorData2, 5);
    sensorData[3].toCharArray(sensorData3, 5);
    u8g2.drawStr(30,40,sensorData0);
    u8g2.drawStr(90,40,sensorData1);
    u8g2.drawStr(30,50,sensorData2);
    u8g2.drawStr(90,50,sensorData3);

    u8g2.sendBuffer();
    paqCont++;
}

void setup() {
    u8g2.begin();
    logo();
    #ifdef USE_IAS
    IAS.begin('P');
    IAS.setCallHome(true);                  // Set to true to enable calling home frequently (disabled by default)
    IAS.setCallHomeInterval(300);            // Call home interval in seconds, use 60s only for development. Please change it to at least 2 hours in production
    IAS.callHome(true);
    #else
    Serial.begin(115200);
    #endif
    Serial2.begin(115200); // Desde Arduino-Nano
    Serial.println(F("Starting"));
    WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
    WiFi.setHostname(hostname);
    WiFi.begin(ssid, password);
    MDNS.begin(hostname);
    MDNS.enableWorkstation();
    MDNS.addService("snmp", "tcp", 161);
    snmpAmp1 = (char*)malloc(6);
    memset(snmpAmp1, 0, 6);
    snmpAmp2 = (char*)malloc(6);
    memset(snmpAmp2, 0, 6);
    snmpAmp3 = (char*)malloc(6);
    memset(snmpAmp3, 0, 6);
    snmpAmp4 = (char*)malloc(6);
    memset(snmpAmp4, 0, 6);
    snmp.setUDP(&udp);
    snmp.begin();
    snmp.addStringHandler(".1.3.6.1.4.1.5.0", &snmpAmp1);
    snmp.addStringHandler(".1.3.6.1.4.1.5.1", &snmpAmp2);
    snmp.addStringHandler(".1.3.6.1.4.1.5.2", &snmpAmp3);
    snmp.addStringHandler(".1.3.6.1.4.1.5.3", &snmpAmp4);

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println(WiFi.localIP());
    }

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    #ifndef OTAA
    LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    #endif
    #ifdef CFG_us915
    LMIC_selectSubBand(1);
    //Disable FSB2-8, channels 16-72
    for (int i = 16; i < 73; i++) {
      if (i != 10)
        LMIC_disableChannel(i);
    }
    LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);
    #endif

    // tipo de sensor
    mydata[0] = 0x01;

    // Start job
    do_send(&sendjob);
}

void loop() {
  #ifdef USE_IAS
  IAS.loop();
  #endif
  os_runloop_once();
  snmp.loop();
}
