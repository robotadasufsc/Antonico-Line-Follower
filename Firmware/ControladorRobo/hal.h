#pragma once

#include <Arduino.h>

/*
+-----+
+----[PWR]-------------------| USB |--+
|                            +-----+  |
|           GND/RST2  [ ] [ ]         |
|         MOSI2/SCK2  [ ] [ ]  SCL[ ] |   D0
|            5V/MISO2 [ ] [ ]  SDA[ ] |   D1
|                             AREF[ ] |
|                              GND[ ] |
| [ ]N/C                        13[ ]~|   B7
| [ ]IOREF                      12[ ]~|   B6
| [ ]RST                        11[ ]~|   B5
| [ ]3V3      +----------+      10[ ]~|   B4
| [ ]5v       | ARDUINO  |       9[ ]~|   H6
| [ ]GND      |   MEGA   |       8[ ]~|   H5
| [ ]GND      +----------+            |
| [ ]Vin                         7[ ]~|   H4
|                                6[ ]~|   H3
| [ ]A0                          5[ ]~|   E3
| [ ]A1                          4[ ]~|   G5
| [ ]A2                     INT5/3[ ]~|   E5
| [ ]A3                     INT4/2[ ]~|   E4
| [ ]A4                       TX>1[ ]~|   E1
| [ ]A5                       RX<0[ ]~|   E0
| [ ]A6                               |
| [ ]A7                     TX3/14[ ] |   J1
|                           RX3/15[ ] |   J0
| [ ]A8                     TX2/16[ ] |   H1
| [ ]A9                     RX2/17[ ] |   H0
| [ ]A10               TX1/INT3/18[ ] |   D3
| [ ]A11               RX1/INT2/19[ ] |   D2
| [ ]A12           I2C-SDA/INT1/20[ ] |   D1
| [ ]A13           I2C-SCL/INT0/21[ ] |   D0
| [ ]A14                              |
| [ ]A15                              |   Ports:
|                RST SCK MISO         |    22=A0  23=A1
|         ICSP   [ ] [ ] [ ]          |    24=A2  25=A3
|                [ ] [ ] [ ]          |    26=A4  27=A5
|                GND MOSI 5V          |    28=A6  29=A7
| G                                   |    30=C7  31=C6
| N 5 5 4 4 4 4 4 3 3 3 3 3 2 2 2 2 5 |    32=C5  33=C4
| D 2 0 8 6 4 2 0 8 6 4 2 0 8 6 4 2 V |    34=C3  35=C2
|         ~ ~                         |    36=C1  37=C0
| @ # # # # # # # # # # # # # # # # @ |    38=D7  39=G2
| @ # # # # # # # # # # # # # # # # @ |    40=G1  41=G0
|           ~                         |    42=L7  43=L6
| G 5 5 4 4 4 4 4 3 3 3 3 3 2 2 2 2 5 |    44=L5  45=L4
| N 3 1 9 7 5 3 1 9 7 5 3 1 9 7 5 3 V |    46=L3  47=L2
| D                                   |    48=L1  49=L0    SPI:
|                                     |    50=B3  51=B2     50=MISO 51=MOSI
|     2560                ____________/    52=B1  53=B0     52=SCK  53=SS
\_______________________/
*/

//LED PINS
#define GREENLED 22
#define YELLOWLED 23

//SWITCH PINS
#define SWITCH1 52
#define SWITCH2 53

//LED Control
#define greenLedOff()      digitalWrite(GREENLED, HIGH)
#define greenLedOn()       digitalWrite(GREENLED, LOW)
#define greenLedToggle()   digitalWrite(GREENLED, !digitalRead(GREENLED))
#define yellowLedOff()    digitalWrite(YELLOWLED, HIGH)
#define yellowLedOn()     digitalWrite(YELLOWLED, LOW)
#define yellowLedToggle() digitalWrite(YELLOWLED, !digitalRead(YELLOWLED))

//// H-Bridge Pins
#define RIGHT_WHEEL_FORWARD_PIN 8
#define RIGHT_WHEEL_BACKWARD_PIN 7
#define RIGHT_WHEEL_SPEED_PIN 9

#define LEFT_WHEEL_FORWARD_PIN 6
#define LEFT_WHEEL_BACKWARD_PIN 10
#define LEFT_WHEEL_SPEED_PIN 5

// Encoder
#define RIGHT_ENCODER_A 21
#define RIGHT_ENCODER_B 20  // B is out of phase by 90 degrees

#define LEFT_ENCODER_A 18
#define LEFT_ENCODER_B 19 // B is out of phase by 90 degrees

#define ENCODER_RESOLUTION 3200
