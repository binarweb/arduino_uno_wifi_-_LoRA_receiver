#include "src/LoRa/LoRa.h"

// setari
#define SERIAL_BAUDRATE 9600
#define MAX_MESSAGE 20

const long frequency = 433E6;  // LoRa Frequency
const int csPin = 6;           // LoRa radio chip select
const int resetPin = 7;        // LoRa radio reset
// const int resetPin = -1;       // LoRa radio reset
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin
const long SPIfrequency = 8E6; // SPI Frequency

void setup() {
	Serial.begin(SERIAL_BAUDRATE);

	LoRa.setPins(csPin, resetPin, irqPin);
	LoRa.setSPIFrequency(SPIfrequency);

	if (!LoRa.begin(frequency)) {
		Serial.println("LoRa init failed. Check your connections.");
		while(1); // if failed, do nothing
	}

	Serial.println("LoRa init succeeded.");
}

void loop() {
	onReceive(LoRa.parsePacket());
}

void onReceive(int packetSize) {
	if (packetSize == 0) return; // if there's no packet, return

	uint8_t incoming_int[MAX_MESSAGE] = {0};
	char incoming_ascii[MAX_MESSAGE] = {0};
	uint16_t i = 0, j = 0, k = 0;
	char c;
	int ci;
	while (LoRa.available()) {
		ci = LoRa.read();

		incoming_int[i++] = ci;

		c = (char)ci;
		if (c == 10 /*LF*/ || c == 13 /*CR*/) {
			incoming_ascii[j++] = 124; /*|*/
		} else {
			incoming_ascii[j++] = c;
		}

		k++;
		if (k == MAX_MESSAGE) break;
	}

	for (i = 0; i < MAX_MESSAGE; i++) {
		Serial.print(incoming_int[i], DEC);
		Serial.print(" ");
	}
	Serial.println("");

	for (i = 0; i < MAX_MESSAGE; i++) {
		Serial.print(incoming_ascii[i]);
		Serial.print(" ");
	}
	Serial.println("");

	Serial.println("");
}