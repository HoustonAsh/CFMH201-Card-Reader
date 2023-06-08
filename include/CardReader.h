/*
  This is a library for the CFMH201 Card reader chip of Chafon technology.
  It is compatible with ISO1443A/Type1-4th NFC cards.

  Written by HoustonAsh, Adiya21j for SEM Industries.

  MIT License

  Copyright (c) 2023 HoustonAsh, Adiya21j

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef CARDREADER_H
#define CARDREADER_H

#include "Arduino.h"
#include <SoftwareSerial.h>

namespace CFMH201::FrameBytes {
  constexpr const byte CARD_STX = 0x02;
  constexpr const byte CARD_STATION_ID = 0x00;
  constexpr const byte CARD_READ_BUFF_LEN = 0x0A;
  constexpr const byte CARD_WRITE_BUFF_LEN = 0x1A;
  constexpr const byte CARD_CMD_READ = 0x20;
  constexpr const byte CARD_CMD_WRITE = 0x21;
  constexpr const byte CARD_KEY = 0xFF;
  constexpr const byte CARD_REQ = 0x01;
  constexpr const byte CARD_RD_NUMBER_OF_BLOCK = 0x01;
  constexpr const byte CARD_WRT_NUMBER_OF_BLOCK = 0x01;
  constexpr const byte CARD_STX_ADDRESS_BLOCK = 0x04;
  constexpr const byte CARD_BCC = 0x00;
  constexpr const byte CARD_ETX = 0x03;
  constexpr const byte CARD_PIN = 0x21;
  constexpr const byte CARD_WRITE_CMD_LEN = 0x1F;
  constexpr const byte RESP_LENGTH = 26;
  constexpr const byte CARD_READ_COMMAND_BCC = 0x2e;
  constexpr const uint8_t CARD_UID_LEN = 4;

  constexpr const byte CARD_READ_CMD_LEN = 15;
}

class CardReader {
  static const byte readCardCommand[CFMH201::FrameBytes::CARD_READ_CMD_LEN];

  static uint8_t CardUID[CFMH201::FrameBytes::CARD_UID_LEN];
  static uint8_t CardUIDold[CFMH201::FrameBytes::CARD_UID_LEN];
  uint8_t incomingBytes[CFMH201::FrameBytes::RESP_LENGTH];

  int64_t requestTime = 0;
  bool requestSent;

  typedef void (*readCardCB)(const uint8_t* CardUID);

#ifndef CARD_READER_HARDWARE_SERIAL
  SoftwareSerial serial;
#else
  HardwareSerial serial;
#endif

  readCardCB callback;
  int priority;
  int requestFrequency;

public:

#ifndef CARD_READER_HARDWARE_SERIAL
  CardReader(uint8_t rx, uint8_t tx, readCardCB callback = nullptr, uint16_t priority = 60, uint16_t requestFrequency = 200);
#else
  CardReader(readCardCB callback = nullptr, uint16_t priority = 200, uint16_t requestFrequency = 200);
#endif
  void setup();
  void process();

private:
  uint64_t startMillis;
  bool readCardBytes();
  static uint8_t calcBCC(const byte* buffer, uint8_t buffSize);
};

#endif