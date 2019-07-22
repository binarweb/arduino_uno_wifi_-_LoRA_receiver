#include "src/LoRa/LoRa.h"

// setari
#define SERIAL_BAUDRATE 115200

const long frequency = 433E6;  // LoRa Frequency
const int csPin = 6;           // LoRa radio chip select
const int resetPin = 7;        // LoRa radio reset
// const int resetPin = -1;       // LoRa radio reset
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin
const long SPIfrequency = 8E6; // SPI Frequency

void setup() {
	Serial.begin(SERIAL_BAUDRATE);

	LoRa.setPins(csPin, resetPin, irqPin);
	// LoRa.setSPIFrequency(SPIfrequency);

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
	if (packetSize == 0) return;          // if there's no packet, return

	String incoming = "";
	while (LoRa.available()) {
		incoming += (char)LoRa.read();
	}

	Serial.println("Message: " + incoming);
	Serial.println("RSSI: " + String(LoRa.packetRssi()));
	Serial.println("Snr: " + String(LoRa.packetSnr()));
	Serial.println();
}