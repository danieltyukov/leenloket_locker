#ifndef INCLUDE_TINY_CODE_READER_H
#define INCLUDE_TINY_CODE_READER_H

#include <stdint.h>

#include <Wire.h>

#define TINY_CODE_READER_I2C_ADDRESS (0x0c)

#define TINY_CODE_READER_REG_LED_STATE        (0x01)

#define TINY_CODE_READER_CONTENT_BYTE_COUNT (254)
typedef struct __attribute__ ((__packed__)) {
    uint16_t content_length;
    uint8_t content_bytes[TINY_CODE_READER_CONTENT_BYTE_COUNT];
} tiny_code_reader_results_t;

inline bool tiny_code_reader_read(tiny_code_reader_results_t* results) {
    const int maxBytesPerChunk = 64;
    const int totalBytes = sizeof(tiny_code_reader_results_t);
    int8_t* results_bytes = (int8_t*)(results);
    int index = 0;
    while (index < totalBytes) {
        const int bytesRemaining = totalBytes - index;
        const int bytesThisChunk = min(bytesRemaining, maxBytesPerChunk);
        const int endIndex = index + bytesThisChunk;
        const bool isLastChunk = (bytesRemaining <= maxBytesPerChunk);
        Wire.requestFrom(TINY_CODE_READER_I2C_ADDRESS, bytesThisChunk, isLastChunk);
        for (; index < endIndex; ++index) {
            if (Wire.available() < 1) {
                Serial.print("Only ");
                Serial.print(index);
                Serial.print(" bytes available on I2C, but we need ");
                Serial.println(bytesThisChunk);
                return false;
            }
            results_bytes[index] = Wire.read();
        }
    }
    if (results->content_length >= TINY_CODE_READER_CONTENT_BYTE_COUNT) {
        results->content_length = (TINY_CODE_READER_CONTENT_BYTE_COUNT - 1);
    }
    results->content_bytes[results->content_length] = 0;
    return true;
}

// Writes the value to the sensor register over the I2C bus.
inline void person_sensor_write_reg(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(TINY_CODE_READER_I2C_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

#endif  // INCLUDE_TINY_CODE_READER_H
