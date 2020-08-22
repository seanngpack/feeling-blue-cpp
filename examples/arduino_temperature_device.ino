#include <Arduino.h>
#include "main.h"

/*
  This is a faux temperature sensing device called SwagTemperature. It has 3 main features:
  battery indication, celcius & kelvin support, and temperature reading.
*/

// create service
BLEService device_service("19B10000-E8F2-537E-4F6C-D104768A1214");

const char * battery_level_uuid = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char * temp_units_uuid = "19B10002-E8F2-537E-4F6C-D104768A1214";
const char * current_temp_uuid = "19B10003-E8F2-537E-4F6C-D104768A1214";
// create battery characteristic that relays current battery level
BLECharacteristic battery_level_char(battery_level_uuid, BLERead, 20u);
// create String type characteristic that sets the units of the temperature. Can read or write.
// This characteristic converts byte data to string. We're using this to demonstrate that you don't have to always
// deal in bytes with the ArduinoBLE library.
BLEStringCharacteristic temp_units_char(temp_units_uuid, BLERead | BLEWrite, 20u);
// create temperature characteristic that has notifications enabled
BLECharacteristic current_temp_char(current_temp_uuid, BLERead | BLENotify, 20u);

char* default_unit = "fahrenh";

// how often the current_temp_char will update
const unsigned long interval = 1000; // in ms
unsigned long start_time;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    // begin initialization
    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");
        while (1);
    }

    // set the local name peripheral advertises
    BLE.setLocalName("SwagTemperature");
    // set the UUID for the service this peripheral advertises
    BLE.setAdvertisedService(device_service);

    // add the characteristic to the service
    device_service.addCharacteristic(battery_level_char);
    device_service.addCharacteristic(current_temp_char);
    device_service.addCharacteristic(temp_units_char);

    // add service
    BLE.addService(device_service);

    // Event handlers for connection info
    BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

    // Event handlers for characteristics
    temp_units_char.setEventHandler(BLEWritten, tempCharacteristicWritten);
    temp_units_char.setEventHandler(BLERead, tempCharacteristicRead);

    // set an initial value for characteristics
    battery_level_char.setValue((char*)int_to_bytes(100));
    Serial.println(battery_level_char.valueLength());
    temp_units_char.setValue("fahrenheit");
    current_temp_char.setValue(0);

    // start advertising
    BLE.advertise();

    Serial.println(("Bluetooth device active, waiting for connections..."));
    start_time = millis();
}

void loop() {
    // poll for BLE events
    BLE.poll();
    if(millis() >= start_time + interval) {
        updateTemp(current_temp_char); // call updateTemp every interval
        start_time = millis();
    }
}

// update the temperature every second
void updateTemp(BLECharacteristic characteristic) {
    float rand = random(-100, 101) / 100.0;
    float currentTempF = rand + 80;
    float currentTemp;

    String c = "celsius";
    if (temp_units_char.value() == c) {
        currentTemp = (currentTempF-32) * .5556;
    }
    else {
        currentTemp = currentTempF;
    }

    // this should trigger a notify
    characteristic.writeValue(float_to_bytes(currentTemp), sizeof(float));
}

/*******************
 *
 *  EVENT HANDLERS
 *
 *******************/

void blePeripheralConnectHandler(BLEDevice central) {
    // central connected event handler
    Serial.print("Connected event, central: ");
    Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
    // central disconnected event handler
    Serial.print("Disconnected event, central: ");
    Serial.println(central.address());
}

void printByteArray(const byte data[], int length) {
    Serial.print("printing byte array: ");
    for (int i = 0; i < length; i++) {
        Serial.print(data[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void tempCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
    Serial.print("Your computer wrote to temp_unit_char!");
}

void tempCharacteristicRead(BLEDevice central, BLECharacteristic characteristic) {
    Serial.print("temp_units_char just got read!");
}

/*******************
 *
 * HELPER FUNCTIONS
 *
 *******************/

uint8_t *float_to_bytes(float f) {
    uint8_t *swag = reinterpret_cast<uint8_t*>(&f);;
    return swag;
}

float bytes_to_float(const uint8_t* bytes) {
    float f;
    memcpy(&f, bytes, sizeof(f));
    return f;
}

const uint8_t *int_to_bytes(int data) {
    uint8_t *byte_vector;
    byte_vector[0] = data & 0xff;
    byte_vector[1] = (data>> 8) & 0xff;
    byte_vector[2] = (data>> 16) & 0xff;
    byte_vector[3] = (data>> 24) & 0xff;
    return byte_vector;
}