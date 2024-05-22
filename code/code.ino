#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h> 
#include "motors.h"

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

IPAddress ip(192, 168, 1, 69);

unsigned int localPort = 8888;

EthernetUDP Udp;

Motor Motor1(MOTOR1_F, MOTOR1_B);
Motor Motor2(MOTOR2_F, MOTOR2_B);
Motor Motor3(MOTOR3_F, MOTOR3_B);
Motor Motor4(MOTOR4_F, MOTOR4_B);
Motor MotorsVFront(FRONT_VERTICAL_F, FRONT_VERTICAL_B);
Motor MotorsVRear(REAR_VERTICAL_F, REAR_VERTICAL_B);

int motors_packet[4];
int accessories_packet[12];

void setup() {
	Ethernet.begin(mac, ip);
	Udp.begin(localPort);

	Serial.begin(9600);

	Serial.println("Beginning...");

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A7, OUTPUT);
}

void loop() {
	int packetSize = Udp.parsePacket();
	if (packetSize) {
		Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

		for (int i = 0; i < 4; i++) {
			motors_packet[i] = (int)packetBuffer[i] - 50;
      Serial.print(motors_packet[i]);
		}

    Serial.println();
//
//		for (int i = 4; i < packetSize; i++) {
////			Serial.print(packetBuffer[i]);
//			accessories_packet[i] = packetBuffer[i-4] - '0'
//		}

    for (int i = 0; i < 4; i++) {
      Serial.print(motors_packet[i]);
    }
    Serial.println();
    
    //Horizontal Motion
    if (motors_packet[LEFT_ANALOG_Y_INDEX] > DEADZONE) {
      Motor1.forward();
      Motor2.forward();
      Motor3.forward();
      Motor4.forward();
    } else if (abs(motors_packet[LEFT_ANALOG_Y_INDEX]) > DEADZONE) {
      Motor1.backward();
      Motor2.backward();
      Motor3.backward();
      Motor4.backward();
    } else if (motors_packet[LEFT_ANALOG_X_INDEX] > DEADZONE) {
      Motor1.forward();
      Motor2.forward();
      Motor3.forward();
      Motor4.forward();
    } else if (abs(motors_packet[LEFT_ANALOG_X_INDEX]) > DEADZONE) {
      Motor1.backward();
      Motor2.backward();
      Motor3.backward();
      Motor4.backward();
    }
    
    // Vertical Motion
    if (accessories_packet[UP_BUTTON_INDEX] || motors_packet[RIGHT_ANALOG_Y_INDEX] > DEADZONE) {
      MotorsVFront.forward();
      MotorsVRear.forward();
    } else if (accessories_packet[DOWN_BUTTON_INDEX] || abs(motors_packet[RIGHT_ANALOG_Y_INDEX]) > DEADZONE) {
      MotorsVFront.backward();
      MotorsVRear.backward();
    } else {
      MotorsVFront.stop();
      MotorsVRear.stop();
    }
	}
}
