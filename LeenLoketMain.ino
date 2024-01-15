#include <WiFi.h>
#include <FastLED.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "tiny_code_reader.h"

#define LED_PIN A3
#define LOCK_PIN D12
#define NUM_LEDS 8

#define WIFI_SSID "Mz"
#define WIFI_PASSWORD "vtgd0060"

// Insert Firebase project API Key
#define API_KEY "AIzaSyARR033aiPUAJB165QTnjsR45pj1U2m_GU"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://tue-leenloket-default-rtdb.europe-west1.firebasedatabase.app"

#include <Arduino.h>
#include <U8g2lib.h>
#include <Keypad.h>

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ D13, /* data=*/ D11, /* CS=*/ D10, /* reset=*/ D8);

const int ROW_NUM    = 4; //four rows
const int COLUMN_NUM = 4; //four columns

const unsigned char LeenLoket [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfc, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0xc0, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0xfc, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0x00, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0x00, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x0f, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x3f, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x81, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x8f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xdf, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xbf, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x04, 0x1e, 0x0f, 0x40, 0x0c, 0xf0, 0x40, 0x98, 0xcf, 0x1f, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0c, 0x1f, 0x8f, 0xc0, 0x0c, 0xfc, 0xc3, 0x9c, 0xcf, 0x1f, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0c, 0x03, 0x83, 0xc1, 0x0c, 0x0e, 0xc7, 0x8c, 0x01, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0c, 0x03, 0x81, 0xc3, 0x0c, 0x06, 0xc6, 0x86, 0x01, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0c, 0x03, 0x83, 0xc7, 0x0c, 0x07, 0xcc, 0x83, 0x01, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0c, 0x1f, 0x8f, 0xcf, 0x0c, 0x03, 0xcc, 0x83, 0x07, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0c, 0x07, 0x87, 0xfd, 0x0c, 0x03, 0xcc, 0x83, 0x03, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0c, 0x03, 0x81, 0xf9, 0x0c, 0x07, 0xcc, 0x86, 0x01, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0c, 0x03, 0x81, 0xf1, 0x0c, 0x06, 0xce, 0x8c, 0x01, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0c, 0x03, 0x81, 0xe1, 0x0c, 0x0e, 0xc7, 0x9c, 0x01, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x7c, 0x3f, 0x9f, 0xc1, 0x7c, 0xfc, 0xc3, 0x98, 0x0f, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3c, 0x3e, 0x9f, 0x00, 0x7c, 0xf0, 0x40, 0xb0, 0x0f, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char LeenLoketMini [] PROGMEM = {
  0x00, 0x04, 0x00, 0x80, 0x1c, 0x00, 0x80, 0x79, 0x00, 0xc0, 0xf0, 0x01, 0xc0, 0xc0, 0x03, 0x60,
  0x80, 0x01, 0x60, 0xc0, 0x01, 0x30, 0xc0, 0x00, 0x30, 0xe0, 0x00, 0x18, 0x60, 0x00, 0x1c, 0x30,
  0x00, 0x0c, 0x30, 0x00, 0x0e, 0x18, 0x00, 0x06, 0x18, 0x00, 0x0f, 0x0c, 0x00, 0x3e, 0x0c, 0x00,
  0xf8, 0x06, 0x00, 0xe0, 0x04, 0x00, 0x80, 0x00, 0x00
};


char keys[ROW_NUM][COLUMN_NUM] = {
  {'*', '1', '4', '7'},
  {'0', '2', '5', '8'},
  {'#', '3', '6', '9'},
  {'D', 'A', 'B', 'C'}
};

byte pin_rows[ROW_NUM] = {D9, D8, D7, D6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {D5, D4, D3, D2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

/**
   These are structures for storing Database received data in, and accessing it in other functions
*/
struct Item {
  int Category;
  String Description;
  String ItemID;
  int LockerID;
  String Status;
};
struct Reservation {
  String ItemID;
  String UserID;
  String Status;
  String EndDate;
};
struct User {
  String Name;
};
struct Validation {
  String Name;
  String State;
  String Date;
};

CRGB leds[8];

String RFID1 = "04 37 85 6C DF 61 81";  // RFID tag
String RFID2 = "04 ED 6A 6C DF 61 80";  // RFID tag

//Define Firebase Data object
FirebaseData fbdo;
FirebaseJson json;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

/**
   Throw an error, call this function when an error happens.

   @param code Error code, use a different number for each different type of error for tracability
   @param error Additional error message
*/
void throwErrorState(int code, String error = "") {
  Serial.println("ERROR Thrown error code: " + code);
  Serial.println("ERROR " + error);
  /* TODO: Make sure either the LEDS blink integer "code" times, and/or the LCD shows the errorcode */
}

/**
   Connect to WiFi, modify password and SSID accordingly
*/
void setupWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    /* TODO: Change LED to blinking red? or display connecting... on LCD */
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

/**
   Initialize the Firebase
*/
void setupFireBase() {
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  auth.user.email = "machine1@leenloket.nl";
  auth.user.password = "LeenLoketM1PW";

  /* Sign up */
  /*if (Firebase.signUp(&config, &auth, "machine1@leenloket.nl", "LeenLoketM1PW")) {
    signupOK = true;
    }
    else {
    Serial.println("Failed to authenticate");
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }

    /* Assign the callback function for the long running token generation task */
  //config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
}

/**
   Initialize the time server connection
*/
void setupTime() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov"); // Set up time using NTP

  time_t now = time(nullptr); // Wait for time to be set
  while (now < 100000) {
    delay(1000);
    now = time(nullptr);
    Serial.println("Waiting for time to be set...");
  }
}

/**
   Provide the itemID, returns the Item object with information about the item.
*/
Item getItem(String itemID) {
  Item output = {0, "", "", 0, ""};
  if (Firebase.RTDB.getJSON(&fbdo, "/Items/" + String(itemID) + "/", &json)) {
    Serial.println("Data retrieved successfully!");  /* Debug code */
    json.toString(Serial, true);  /* Debug code */
    Serial.println();  /* Debug code */
    size_t count = json.iteratorBegin();
    for (size_t i = 0; i < count; i++) {
      FirebaseJson::IteratorValue value = json.valueAt(i);
      if (value.key == "CategoryID") {
        output.Category = value.value.toInt();
      }
      else if (value.key == "Description") {
        output.Description = String(value.value);
      }
      else if (value.key == "ItemID") {
        output.ItemID = String(value.value);
      }
      else if (value.key == "LockerID") {
        output.LockerID = value.value.toInt();
      }
      else if (value.key == "Status") {
        output.Status = String(value.value);
      }
    }
    json.iteratorEnd();
  } else {
    Serial.println("Failed to retrieve data");
    throwErrorState(2, fbdo.errorReason());
  }
  return output;
}
User getUser(String userID) {
  User output = {""};
  if (Firebase.RTDB.getJSON(&fbdo, "/Users/" + userID + "/", &json)) {
    Serial.println("Data retrieved successfully!");  /* Debug code */
    json.toString(Serial, true);  /* Debug code */
    Serial.println();  /* Debug code */
    size_t count = json.iteratorBegin();
    for (size_t i = 0; i < count; i++) {
      FirebaseJson::IteratorValue value = json.valueAt(i);
      if (value.key == "Name") {
        output.Name = String(value.value);
      }
    }
    json.iteratorEnd();
  } else {
    Serial.println("Failed to retrieve data");
    throwErrorState(2, fbdo.errorReason());
  }
  return output;
}
Reservation getReservation(String reservationID) {
  Reservation output = {"", "", "", ""};
  if (Firebase.RTDB.getJSON(&fbdo, "/Reservations/" + reservationID + "/", &json)) {
    Serial.println("resID2 " + reservationID);
    Serial.println("Data retrieved successfully!");  /* Debug code */
    json.toString(Serial, true);  /* Debug code */
    Serial.println();  /* Debug code */
    size_t count = json.iteratorBegin();
    for (size_t i = 0; i < count; i++) {
      FirebaseJson::IteratorValue value = json.valueAt(i);
      if (value.key == "UserID") {
        output.UserID = String(value.value);
      }
      else if (value.key == "ItemID") {
        output.ItemID = String(value.value);
      }
      else if (value.key == "Status") {
        output.Status = String(value.value);
      }
      else if (value.key == "EndDate") {
        output.EndDate = String(value.value);
      }
    }
    json.iteratorEnd();
  } else {
    Serial.println("Failed to retrieve data");
    throwErrorState(2, fbdo.errorReason());
  }
  return output;
}

// Function to remove quotation marks from a string
String sanitizeID(String id) {
  id.replace("\"", ""); // Replace quotation marks with nothing
  return id;
}

/**
   Set the item/reservation status, with input itemID and new status
*/
void setItemStatus(String itemID, String Status) {
  itemID = sanitizeID(itemID);
  if (Firebase.RTDB.setString(&fbdo, "/Items/" + String(itemID) + "/Status/", Status)) {
    Serial.println("Set item status of ID: " + String(itemID) + " to: " + Status);
  }
  else {
    throwErrorState(3, fbdo.errorReason());
  }
}
void setReservationStatus(String reservationID, String Status) {
  reservationID = sanitizeID(reservationID);
  if (Firebase.RTDB.setString(&fbdo, "/Reservations/" + String(reservationID) + "/Status/", Status)) {
    Serial.println("Set reservation status of ID: " + String(reservationID) + " to: " + Status);
  }
  else {
    throwErrorState(3, fbdo.errorReason());
  }
}

int countColons(String str) {
  int count = 0;
  for (int i = 0; i < str.length(); i++) {
    if (str.charAt(i) == ':') {
      count++;
    }
  }
  return count;
}

/**
   Verify the number typed in using the numpad, returns "NULL" if no user was found that reserved an item.

   @param code The number typed in using the keypad. HAS TO BE AN INTEGER!
   @return int Returns the reservationID, Use this to find the name of the person attached to this reservation using getReservationInformation();
*/
Validation validateCode(String code) {
  bool success = false;
  String reservationID = "-1";
  String userName = "NULL";
  Validation output = {"NULL", "NULL", "NULL"};

  if (Firebase.RTDB.getJSON(&fbdo, "/Codes", &json)) {
    Serial.println("Data retrieved successfully!");  /* Debug code */
    json.toString(Serial, true);  /* Debug code */
    Serial.println();  /* Debug code */
    size_t count = json.iteratorBegin(); // This is the number of reservations currently open
    Serial.println(count);
    for (size_t i = 0; i < count; i++) { // Loop through all open reservations, and check if the code matches
      String reservationID_temp = "-1";
      FirebaseJson::IteratorValue value = json.valueAt(i); // Get the i-th reservation
      String key = value.key;
      String v = value.value;

      if (key == "PINCode") {
        v.replace("\"", "");
      }
      else if (key == "ReservationID") {
        reservationID_temp = String(value.value);
        if (success) {
          reservationID = reservationID_temp;
          Serial.println("break");
          break;
        }
      }
      if (v.equals(code)) {
        // We have found a reservation linked to the entered code, now we open the locker and make the LED green, and show that on the LCD, and update the database.
        success = true;
      }
    }
    json.iteratorEnd();
  } else {
    Serial.println("Failed to retrieve data");
    throwErrorState(2, fbdo.errorReason());
  }
  reservationID.replace("\"", "");
  if (success && reservationID != "-1") {
    Reservation res = getReservation(reservationID);
    String userID_from_reservation = res.UserID;
    String itemID_from_reservation = res.ItemID;
    userID_from_reservation.replace("\"", "");
    itemID_from_reservation.replace("\"", "");
    Serial.println(itemID_from_reservation);

    res.Status.replace("\"", "");
    res.EndDate.replace("\"", "");
    if (countColons(res.EndDate) < 2) {
      res.EndDate += ":00";
    }

    /*
        Case 1: reservation != "Complete" AND time before EndDate                                   This is an in time pickup    RESULT: Open locker
        Case 2: reservation != "Complete" AND time after EndDate                                    This is a too late pickup    RESULT: Show message only
        Case 3: reservation == "Complete" and time before EndDate AND ItemStatus == "Unavailable"   In time return               RESULT: Open Locker
        Case 3: reservation == "Complete" and time after EndDate AND ItemStatus == "Unavailable"    Late return                  RESULT: Open locker
    */


    Item item = getItem(itemID_from_reservation);
    Serial.println(item.Status);
    item.Status.replace("\"", "");

    Serial.println(item.Status);

    Serial.println("A " + String(res.Status.equals("Completed")));
    Serial.println("B " + String(isNowBeforeTarget(res.EndDate)));
    Serial.println("C " + String(item.Status.equals("Unavailable")));
    /*
       Case 1
    */
    if (!res.Status.equals("Completed") && isNowBeforeTarget(res.EndDate)) {
      output.State = "INTIME_PICKUP";
      setItemStatus(itemID_from_reservation, "Unavailable");
      setReservationStatus(reservationID, "Completed");
      userName = getUser(userID_from_reservation).Name;
      userName.replace("\"", "");
    }
    /*
       Case 2
    */
    else if (!res.Status.equals("Completed") && !isNowBeforeTarget(res.EndDate)) {
      output.State = "LATE_PICKUP";
      setReservationStatus(reservationID, "Completed");
    }
    /*
       Case 3
    */
    else if (res.Status.equals("Completed") && isNowBeforeTarget(res.EndDate) && item.Status.equals("Unavailable")) {
      output.State = "INTIME_RETURN";
      setItemStatus(itemID_from_reservation, "Available");
      userName = getUser(userID_from_reservation).Name;
      userName.replace("\"", "");
    }
    /*
       Case 4
    */
    else if (res.Status.equals("Completed") && !isNowBeforeTarget(res.EndDate) && item.Status.equals("Unavailable")) {
      output.State = "LATE_RETURN";
      setItemStatus(itemID_from_reservation, "Available");
    }
    Serial.println(output.State);

    /*if (!res.Status.equals("Complete") && isNowBeforeTarget(res.EndDate)){ //Check if item hasnt already been picked up & if the pickup time is before the reservation end date
      setReservationStatus(reservationID, "Complete");
      setItemStatus(itemID_from_reservation, "Taken");
      userName = getUser(userID_from_reservation).Name;
      userName.replace("\"","");
      }
      else {
      Item item = getItem(itemID_from_reservation);
      if (item.Status = "Taken"){ // The user is returning the item after the deadline
        userName = "LATE";
        Serial.println("Item returned too late!");
      }
      else {
        userName = "PASSED";
        Serial.println("Item already taken / Reservation already passed!");
      }
      }*/
    output.Name = userName;
    output.Date = res.EndDate;
    return output;
  }
  else {
    return output;
  }
}

/**
   Blink the LED for error signalling

   @param int The number for the error code. Number of blinks.
*/
void errorBlinkRed(int number) {
  while (number > 0) {
    for (int k = 0; k < NUM_LEDS; k++) {
      leds[k] = 0xFD0303;
      FastLED.show();
    }
    delay(200);
    for (int k = 0; k < NUM_LEDS; k++) {
      leds[k] = CRGB::Black;
      FastLED.show();
    }
    delay(200);
    number = number - 1;
  }
  delay(1000);
}

void errorBlinkGreen(int number) {  //Blink green for correct reading RFID1  - Uncomment if not used
  while (number > 0) {
    for (int k = 0; k < NUM_LEDS; k++) {
      leds[k] = 0x4ce91f;
      FastLED.show();
    }
    delay(200);
    for (int k = 0; k < NUM_LEDS; k++) {
      leds[k] = CRGB::Black;
      FastLED.show();
    }
    delay(200);
    number = number - 1;
  }
  delay(1000);
}

/**
   Make the LED Green
*/
void ledGreen() {
  for (int i = 0; i < 8; i++) {
    leds[i] = 0x4ce91f;
  }
  FastLED.show();
}
/**
   Make the LED Red
*/
void ledRed() {
  for (int i = 0; i < 8; i++) {
    leds[i] = 0xFD0303;
  }
  FastLED.show();
}
/**
   Make the LED Blue
*/
void ledBlue() {
  for (int i = 0; i < 8; i++) {
    leds[i] = 0x235cb8;
  }
  FastLED.show();
}

/**
   Lock and unlock the electromagnet relay.
*/
void Lock() {
  digitalWrite(LOCK_PIN, LOW); // We do the opposite (LOW for locked) because when the arduino happens to power down for some reason, the lock will remain locked.
  Serial.println("Locked!");
}
void Unlock() {
  digitalWrite(LOCK_PIN, HIGH);
  Serial.println("Unlocked!");
}

/**
   Function to convert a string time in this form: 2023-12-02T09:00:00 (ISO8601) to epoch time
*/
time_t convertDateTimeToEpoch(String dateTimeString) {
  // Parse the date-time string
  int year, month, day, hour, minute, second;
  sscanf(dateTimeString.c_str(), "%d-%d-%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second);

  // Set the time struct
  struct tm tmTime;
  tmTime.tm_year = year - 1900;
  tmTime.tm_mon = month - 1;
  tmTime.tm_mday = day;
  tmTime.tm_hour = hour;
  tmTime.tm_min = minute;
  tmTime.tm_sec = second;

  // Convert to epoch time
  time_t epochTime = mktime(&tmTime);
  Serial.println(epochTime);
  return epochTime;
}

/**
   Function to compare the current time to a time String in the format: 2023-12-02T09:00:00 (ISO8601)
*/
bool isNowBeforeTarget(String dateTimeString) {
  time_t targetTime = convertDateTimeToEpoch(dateTimeString);

  // Get the current epoch time
  time_t currentTime = time(nullptr) + 3600;
  // Compare the target time with the current time
  Serial.println(currentTime);
  if (currentTime <= targetTime) {
    return true;
  }
  else {
    return false;
  }
}

bool checkRFID() {
  if (nfc.tagPresent()) {
    NfcTag tag = nfc.read();
    tag.print();
    Serial.print("RFID Reader has detected UID: ");
    String UID = tag.getUidString();
    Serial.println(UID);
    if (RFID1.equals(UID)) {
      errorBlinkGreen(3);  // for demonstration blink green if correct RFID1 is scanned
      ledBlue();
      return true;
    } else if (RFID2.equals(UID)) {  // for demonstration blink red if (incorrect) RFID2 is scanned
      errorBlinkRed(3);
      ledBlue();
      return false;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

void resetLCD(bool PIN = true) {
  do {
    u8g2.clearBuffer();
    u8g2.clear();

    u8g2.setFont(u8g2_font_NokiaSmallBold_tf);
    u8g2.drawStr(1, 9, "Welcome to LeenLoket");

    u8g2.drawXBMP( 128 - 18, 0, 19, 19, LeenLoketMini);

    u8g2.setFont(u8g2_font_t0_11_tr);
    if (PIN) {
      u8g2.drawStr(1, 20, "Enter your PIN:");
    };

    u8g2.setFont(u8g2_font_smallsimple_tr);
    u8g2.drawStr(1, 63, "* Reset  |  # Enter | A Scan");
    u8g2.drawLine(0, 55, 128, 55);
  } while ( u8g2.nextPage() );
}

bool qrCodeReaderActive = false; // To keep track of QR code reader state

void activateQRCodeReader(bool activate) {
  qrCodeReaderActive = activate;
  if (activate) {
    u8g2.clear();
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_t0_11_tr);
      u8g2.drawStr(1, 20, "Scanning...");
    } while ( u8g2.nextPage() );
  } else {
    resetLCD();
  }
}

Validation updateStatusFromQRCode(String reservationID) {
  String userName = "NULL";
  Validation output = {"NULL", "NULL", "NULL"};
  reservationID.replace("\"", "");
  Reservation res = getReservation(reservationID);
  String userID_from_reservation = res.UserID;
  String itemID_from_reservation = res.ItemID;
  userID_from_reservation.replace("\"", "");
  itemID_from_reservation.replace("\"", "");
  Serial.println(itemID_from_reservation);

  res.Status.replace("\"", "");
  res.EndDate.replace("\"", "");
  if (countColons(res.EndDate) < 2) {
    res.EndDate += ":00";
  }

  /*
      Case 1: reservation != "Complete" AND time before EndDate                                   This is an in time pickup    RESULT: Open locker
      Case 2: reservation != "Complete" AND time after EndDate                                    This is a too late pickup    RESULT: Show message only
      Case 3: reservation == "Complete" and time before EndDate AND ItemStatus == "Unavailable"   In time return               RESULT: Open Locker
      Case 3: reservation == "Complete" and time after EndDate AND ItemStatus == "Unavailable"    Late return                  RESULT: Open locker
  */


  Item item = getItem(itemID_from_reservation);
  Serial.println(item.Status);
  item.Status.replace("\"", "");

  Serial.println(item.Status);

  Serial.println("A " + String(res.Status.equals("Completed")));
  Serial.println("B " + String(isNowBeforeTarget(res.EndDate)));
  Serial.println("C " + String(item.Status.equals("Unavailable")));
  /*
     Case 1
  */
  if (!res.Status.equals("Completed") && isNowBeforeTarget(res.EndDate)) {
    output.State = "INTIME_PICKUP";
    setItemStatus(itemID_from_reservation, "Unavailable");
    setReservationStatus(reservationID, "Completed");
    userName = getUser(userID_from_reservation).Name;
    userName.replace("\"", "");
  }
  /*
     Case 2
  */
  else if (!res.Status.equals("Completed") && !isNowBeforeTarget(res.EndDate)) {
    output.State = "LATE_PICKUP";
    setReservationStatus(reservationID, "Completed");
  }
  /*
     Case 3
  */
  else if (res.Status.equals("Completed") && isNowBeforeTarget(res.EndDate) && item.Status.equals("Unavailable")) {
    output.State = "INTIME_RETURN";
    setItemStatus(itemID_from_reservation, "Available");
    userName = getUser(userID_from_reservation).Name;
    userName.replace("\"", "");
  }
  /*
     Case 4
  */
  else if (res.Status.equals("Completed") && !isNowBeforeTarget(res.EndDate) && item.Status.equals("Unavailable")) {
    output.State = "LATE_RETURN";
    setItemStatus(itemID_from_reservation, "Available");
  }
  Serial.println(output.State);

  /*if (!res.Status.equals("Complete") && isNowBeforeTarget(res.EndDate)){ //Check if item hasnt already been picked up & if the pickup time is before the reservation end date
    setReservationStatus(reservationID, "Complete");
    setItemStatus(itemID_from_reservation, "Taken");
    userName = getUser(userID_from_reservation).Name;
    userName.replace("\"","");
    }
    else {
    Item item = getItem(itemID_from_reservation);
    if (item.Status = "Taken"){ // The user is returning the item after the deadline
      userName = "LATE";
      Serial.println("Item returned too late!");
    }
    else {
      userName = "PASSED";
      Serial.println("Item already taken / Reservation already passed!");
    }
    }*/
  output.Name = userName;
  output.Date = res.EndDate;
  return output;
}

void setup() {
  Serial.begin(9600);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, 8);

  ledRed();

  Wire.begin();
  u8g2.begin();
  nfc.begin();
  u8g2.clearBuffer();
  // (DECENT u8g2_font_t0_14_tr) (VERY GOOD u8g2_font_t0_11_tr) (FITS u8g2_font_mercutio_sc_nbp_t_all) (VERY GOOD SMALL u8g2_font_smallsimple_tr) ( VERY GOOD BOLD u8g2_font_NokiaSmallBold_tf)
  u8g2.firstPage();
  do {
    u8g2.drawXBMP( 0, 0, 128, 64, LeenLoket);
  } while ( u8g2.nextPage() );

  setupWifi();
  setupFireBase();
  setupTime();

  resetLCD();
  ledGreen();

  pinMode(LOCK_PIN, OUTPUT);
}

String input_password;

void loop() {
  if (qrCodeReaderActive) {
    tiny_code_reader_results_t results = {};
    if (!tiny_code_reader_read(&results)) {
      Serial.println("No QR code reader results found on the i2c bus");
      delay(200);
      return;
    }

    if (results.content_length == 0) {
      Serial.println("No code found");
    } else {
      String scannedCode = String((char*)results.content_bytes);
      Serial.print("Found '");
      Serial.print(scannedCode);
      Serial.println("'");

      // Update status in database
      Validation check = updateStatusFromQRCode(scannedCode);
      String Name = check.Name;
      String Date = check.Date;
      Date.replace("T", " at ");

      if (check.State.equals("INTIME_PICKUP")) {
        u8g2.clear();
        resetLCD(false); // RESET without the "enter your PIN" text
        ledBlue();
        Unlock();
        delay(500);
        Lock();

        do {
          u8g2.setFont(u8g2_font_t0_11_tr);
          u8g2.setCursor(1, 20);
          u8g2.print("Thank you for");
          u8g2.setCursor(1, 32);
          u8g2.print("using LeenLoket");
          u8g2.setCursor(1, 44);
          u8g2.setFont(u8g2_font_NokiaSmallBold_tf);
          u8g2.print(Name + "!");
          u8g2.sendBuffer();
        } while ( u8g2.nextPage() );
        delay(4500);
        u8g2.clear();
        resetLCD(false); // RESET without the "enter your PIN" text
        do {
          u8g2.setFont(u8g2_font_t0_11_tr);
          u8g2.setCursor(1, 20);
          u8g2.print("Please return the");
          u8g2.setCursor(1, 32);
          u8g2.print("item before: ");
          u8g2.setCursor(1, 44);
          u8g2.setFont(u8g2_font_NokiaSmallBold_tf);
          u8g2.print(Date);
          u8g2.sendBuffer();
        } while ( u8g2.nextPage() );
        delay(4500);
        ledGreen();
        resetLCD();
      }
      else if (check.State.equals("LATE_PICKUP")) {
        u8g2.clear();
        resetLCD(false); // RESET without the "enter your PIN" text

        do {
          u8g2.setFont(u8g2_font_t0_11_tr);
          u8g2.setCursor(1, 20);
          u8g2.print("The reservation");
          u8g2.setCursor(1, 32);
          u8g2.print("date has expired!");
          u8g2.sendBuffer();
        } while ( u8g2.nextPage() );
        errorBlinkRed(5);
        ledGreen();
        delay(1000);
        resetLCD();
      }
      else if (check.State.equals("INTIME_RETURN")) {
        u8g2.clear();
        resetLCD(false); // RESET without the "enter your PIN" text
        ledBlue();
        Unlock();
        delay(500);
        while (!checkRFID()) {}
        Lock();

        do {
          u8g2.setFont(u8g2_font_t0_11_tr);
          u8g2.setCursor(1, 20);
          u8g2.print("Thank you for");
          u8g2.setCursor(1, 32);
          u8g2.print("using LeenLoket");
          u8g2.setCursor(1, 44);
          u8g2.setFont(u8g2_font_NokiaSmallBold_tf);
          u8g2.print(Name + "!");
          u8g2.sendBuffer();
        } while ( u8g2.nextPage() );
        delay(4500);
        u8g2.clear();
        ledGreen();
        resetLCD();
      }
      else if (check.State.equals("LATE_RETURN")) {
        u8g2.clear();
        resetLCD(false); // RESET without the "enter your PIN" text
        do {
          u8g2.setFont(u8g2_font_t0_11_tr);
          u8g2.setCursor(1, 20);
          u8g2.print("The item has been");
          u8g2.setCursor(1, 32);
          u8g2.print("returned late.");
          u8g2.sendBuffer();
        } while ( u8g2.nextPage() );
        ledBlue();
        Unlock();
        delay(500);
        while (!checkRFID()) {}
        Lock();
        ledGreen();
        delay(1000);
        resetLCD();
      }
      else if (check.State.equals("NULL")) {
        u8g2.clear();
        resetLCD(false); // RESET without the "enter your PIN" text

        do {
          u8g2.setFont(u8g2_font_t0_11_tr);
          u8g2.setCursor(1, 20);
          u8g2.print("Incorrect code!");
          u8g2.setCursor(1, 32);
          u8g2.print("Please try again.");
          u8g2.sendBuffer();
        } while ( u8g2.nextPage() );
        errorBlinkRed(5);
        ledGreen();
        resetLCD();
      }
      activateQRCodeReader(false); // Deactivate QR code reader after scanning
    }
  }
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    if (key == '*') {
      input_password = "";
      u8g2.clear();
      resetLCD();
    }
    else if (key == '#') {
      Serial.println(input_password);
      Validation check = validateCode(input_password);
      String Name = check.Name;
      String Date = check.Date;
      Date.replace("T", " at ");

      if (check.State.equals("INTIME_PICKUP")) {
        input_password = "";

        u8g2.clear();
        resetLCD(false); // RESET without the "enter your PIN" text
        ledBlue();
        Unlock();
        delay(500);
        Lock();

        do {
          u8g2.setFont(u8g2_font_t0_11_tr);
          u8g2.setCursor(1, 20);
          u8g2.print("Thank you for");
          u8g2.setCursor(1, 32);
          u8g2.print("using LeenLoket");
          u8g2.setCursor(1, 44);
          u8g2.setFont(u8g2_font_NokiaSmallBold_tf);
          u8g2.print(Name + "!");
          u8g2.sendBuffer();
        } while ( u8g2.nextPage() );
        delay(4500);
        u8g2.clear();
        resetLCD(false); // RESET without the "enter your PIN" text
        do {
          u8g2.setFont(u8g2_font_t0_11_tr);
          u8g2.setCursor(1, 20);
          u8g2.print("Please return the");
          u8g2.setCursor(1, 32);
          u8g2.print("item before: ");
          u8g2.setCursor(1, 44);
          u8g2.setFont(u8g2_font_NokiaSmallBold_tf);
          u8g2.print(Date);
          u8g2.sendBuffer();
        } while ( u8g2.nextPage() );
        delay(4500);
        ledGreen();
        resetLCD();
      }
      else if (check.State.equals("LATE_PICKUP")) {
        input_password = "";
        u8g2.clear();
        resetLCD(false); // RESET without the "enter your PIN" text

        do {
          u8g2.setFont(u8g2_font_t0_11_tr);
          u8g2.setCursor(1, 20);
          u8g2.print("The reservation");
          u8g2.setCursor(1, 32);
          u8g2.print("date has expired!");
          u8g2.sendBuffer();
        } while ( u8g2.nextPage() );
        errorBlinkRed(5);
        ledGreen();
        delay(1000);
        resetLCD();
      }
      else if (check.State.equals("INTIME_RETURN")) {
        input_password = "";

        u8g2.clear();
        resetLCD(false); // RESET without the "enter your PIN" text
        ledBlue();
        Unlock();
        delay(500);
        while (!checkRFID()) {}  // wait until correct rfid is scanned
        Lock();

        do {
          u8g2.setFont(u8g2_font_t0_11_tr);
          u8g2.setCursor(1, 20);
          u8g2.print("Thank you for");
          u8g2.setCursor(1, 32);
          u8g2.print("using LeenLoket");
          u8g2.setCursor(1, 44);
          u8g2.setFont(u8g2_font_NokiaSmallBold_tf);
          u8g2.print(Name + "!");
          u8g2.sendBuffer();
        } while ( u8g2.nextPage() );
        delay(4500);
        u8g2.clear();
        ledGreen();
        resetLCD();
      }
      else if (check.State.equals("LATE_RETURN")) {
        input_password = "";
        u8g2.clear();
        resetLCD(false); // RESET without the "enter your PIN" text
        do {
          u8g2.setFont(u8g2_font_t0_11_tr);
          u8g2.setCursor(1, 20);
          u8g2.print("The item has been");
          u8g2.setCursor(1, 32);
          u8g2.print("returned late.");
          u8g2.sendBuffer();
        } while ( u8g2.nextPage() );
        ledBlue();
        Unlock();
        delay(500);
        while (!checkRFID()) {}  // wait until correct rfid is scanned
        Lock();
        ledGreen();
        delay(1000);
        resetLCD();
      }
      else if (check.State.equals("NULL")) {
        input_password = "";
        u8g2.clear();
        resetLCD(false); // RESET without the "enter your PIN" text

        do {
          u8g2.setFont(u8g2_font_t0_11_tr);
          u8g2.setCursor(1, 20);
          u8g2.print("Incorrect code!");
          u8g2.setCursor(1, 32);
          u8g2.print("Please try again.");
          u8g2.sendBuffer();
        } while ( u8g2.nextPage() );
        errorBlinkRed(5);
        ledGreen();
        resetLCD();
      }
    }
    else if (key == 'A') {
      if (qrCodeReaderActive) {
        activateQRCodeReader(false); // Deactivate QR code reader
      } else {
        activateQRCodeReader(true); // Activate QR code reader
      }
    }
    else {
      input_password += key;
      String stars = "";
      for (int i = 0; i < input_password.length(); i++) {
        stars += "*";
      }
      do {
        u8g2.setFont(u8g2_font_t0_11_tr);
        u8g2.setCursor(1, 30);
        u8g2.print(stars);
        u8g2.sendBuffer();
      } while ( u8g2.nextPage() );
    }
  }
}
