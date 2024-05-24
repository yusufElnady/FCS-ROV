#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h> 
#include "motors.h"

#define FRAME_LENGTH 8

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

IPAddress ip(192, 168, 1, 69);

unsigned int localPort = 8888;

bool switches[] = {false, false};

EthernetUDP Udp;

Motor MotorsR(MOTORSR_F, MOTORSR_B);
Motor MotorsL(MOTORSL_F, MOTORSL_B);
Motor MotorV(MOTORSV_F, MOTORSV_B);

int motors_packet[FRAME_LENGTH];

void setup() {
	Ethernet.begin(mac, ip);
	Udp.begin(localPort);

	Serial.begin(9600);

	Serial.println("Beginning...");

  pinMode(SWITCH1_PIN, OUTPUT);
  pinMode(SWITCH2_PIN, OUTPUT);
  MotorsR.init();
  MotorsL.init();
  MotorV.init();
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
}

void loop() {
	int packetSize = Udp.parsePacket();
	if (packetSize) {
		Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

		for (int i = 0; i < FRAME_LENGTH; i++) {
			motors_packet[i] = -1 * ((int)packetBuffer[i] - 50);
      Serial.print(motors_packet[i]);
		}

    Serial.println();

    for (int i = 0; i < FRAME_LENGTH; i++) {
      Serial.print(motors_packet[i]);
    }
    Serial.println();
    
    //Horizontal Motion
    if (motors_packet[LEFT_ANALOG_Y_INDEX] > DEADZONE) {
      Serial.println("left forward");
      MotorsL.forward();
    } else if (abs(motors_packet[LEFT_ANALOG_Y_INDEX]) > DEADZONE) {
      Serial.println("left backward");
      MotorsL.backward();
    } else {
      Serial.println("left idle");
      MotorsL.stop();
    }

    if (motors_packet[RIGHT_ANALOG_Y_INDEX] > DEADZONE) {
      Serial.println("right forward");
      MotorsR.forward();
    } else if (abs(motors_packet[RIGHT_ANALOG_Y_INDEX]) > DEADZONE) {
      Serial.println("right backward");
      MotorsR.backward();
    } else {
      Serial.println("right idle");
      MotorsR.stop();
    }

    if (motors_packet[R2_INDEX] == 1) {
      Serial.println("v upward");
      MotorV.forward();
    } else if (motors_packet[L2_INDEX] == 1) {
      Serial.println("v downkward");
      MotorV.backward();
    } else {
      Serial.println("v idle");
      MotorV.stop();
    }

    if (motors_packet[BUTTON1_INDEX] == 1) {
      switches[0] = !switches[0];
      digitalWrite(SWITCH1_PIN, (switches[0]) ? HIGH:LOW);
    }

    if (motors_packet[BUTTON2_INDEX] == 1) {
      switches[1] = !switches[1];
      digitalWrite(SWITCH2_PIN, (switches[1]) ? HIGH:LOW);
    }
	}
}