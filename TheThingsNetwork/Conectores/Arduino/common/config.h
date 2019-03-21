// Comment the next line to use ABP authentication on TTN. Leave it as it is to use recommended OTAA
#define OTAA

// Comment the next line if you plan NOT to use IoTAppStory's service
#define USE_IAS

#ifdef OTAA
// Valores para autenticación OTAA
static const u1_t PROGMEM DEVEUI[8]={ }; // Device EUI, hex, lsb
static const u1_t PROGMEM APPEUI[8]={ }; // Application EUI, hex, lsb
static const u1_t PROGMEM APPKEY[16] = { }; // App Key, hex, msb

void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}
#else
// Valores para autenticación ABP
static const PROGMEM u1_t NWKSKEY[16] = { }; // Network Session Key, hex, MSB
static const u1_t PROGMEM APPSKEY[16] = { }; // Application Sessoin Key, hex, MSB
static const u4_t DEVADDR = 0xfede2 ; // Device Address, 0xDecimal, MSB

void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }
#endif

const char* ssid       = "AP";
const char* password   = "pass";
const char* hostname   = "hostname";

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 30;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 14,
    .dio = {26, 33, 32}  // Pins for the Heltec ESP32 Lora board/ TTGO Lora32 with 3D metal antenna
};

// Imagen de GreenCore 50x50 px
#define greenfoot_width 50
#define greenfoot_height 50
static const unsigned char greenfoot_bits[] PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x03, 0xff, 0xff, 0xff, 0x7f, 0xf0, 0xff, 0x03, 0xff, 0xc0, 0xff, 0x3f,
   0xe0, 0xff, 0x03, 0x7f, 0xc0, 0xff, 0x1f, 0xc0, 0xff, 0x03, 0x3f, 0x80,
   0xff, 0x0f, 0xc0, 0xff, 0x03, 0x3f, 0x80, 0xff, 0x0f, 0xc0, 0xff, 0x03,
   0x3f, 0x80, 0xff, 0x0f, 0xc0, 0xff, 0x03, 0x3f, 0x00, 0xff, 0x0f, 0xc0,
   0xff, 0x03, 0x3f, 0x80, 0xff, 0x0f, 0xc0, 0xff, 0x03, 0x7f, 0x80, 0xff,
   0x07, 0xc0, 0xff, 0x03, 0x7f, 0xc0, 0xff, 0x07, 0xe0, 0xff, 0x03, 0x7f,
   0xe0, 0xff, 0x03, 0xf8, 0xff, 0x03, 0xff, 0xe1, 0xff, 0x81, 0xff, 0xff,
   0x03, 0xff, 0xf1, 0xff, 0xc0, 0xff, 0xff, 0x03, 0xff, 0xf3, 0x7f, 0xe0,
   0xff, 0xff, 0x03, 0xff, 0xe3, 0x7f, 0xf0, 0xff, 0xff, 0x03, 0xff, 0xe3,
   0x3f, 0xf8, 0xff, 0xff, 0x03, 0xff, 0xe7, 0x1f, 0xfc, 0xff, 0xff, 0x03,
   0xff, 0xc7, 0x0f, 0xfe, 0xff, 0xff, 0x03, 0xff, 0xc7, 0x07, 0xff, 0xff,
   0xff, 0x03, 0xff, 0x87, 0x83, 0xff, 0xff, 0xff, 0x03, 0xff, 0x07, 0xc0,
   0xff, 0xff, 0xff, 0x03, 0xff, 0x07, 0xc0, 0xff, 0xff, 0xff, 0x03, 0xff,
   0x07, 0xe0, 0xff, 0x1f, 0xfc, 0x03, 0xff, 0x07, 0xe0, 0xff, 0x07, 0xf8,
   0x03, 0xff, 0x03, 0xf0, 0xff, 0x07, 0xf0, 0x03, 0xff, 0x03, 0xf0, 0xff,
   0x03, 0xf0, 0x03, 0xff, 0x01, 0xf0, 0xff, 0x01, 0xf0, 0x03, 0xff, 0x00,
   0xe0, 0xff, 0x00, 0xf0, 0x03, 0xff, 0x00, 0x80, 0x1f, 0x00, 0xf0, 0x03,
   0x7f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x3f, 0x00, 0x00, 0x00, 0x00,
   0xf8, 0x03, 0x1f, 0x00, 0x00, 0xe0, 0x0f, 0xfc, 0x03, 0x0f, 0x00, 0x00,
   0xfe, 0x1f, 0xfe, 0x03, 0x07, 0x00, 0xe0, 0xff, 0xff, 0xff, 0x03, 0x03,
   0x00, 0xf8, 0xff, 0xff, 0xff, 0x03, 0x01, 0x00, 0xfe, 0xff, 0xff, 0xff,
   0x03, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0xc0, 0xff, 0xff,
   0xff, 0xff, 0x03, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0xf0,
   0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x01,
   0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0xfc, 0xff, 0xff, 0xff,
   0xff, 0x00 };
