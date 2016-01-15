/*******************************************************************
* Definition of the request and command ID's of the protocol for the
* communication between a control program and the emulator of
* the washing machine.
*/
#ifndef _PROTOCOL_H
#define _PROTOCOL_H

// Request IDs
#define MACHINE_REQ           (uint8_t) 0x01
#define DOOR_LOCK_REQ         (uint8_t) 0x02
#define WATER_VALVE_REQ       (uint8_t) 0x03
#define SOAP_DISPENSER_REQ    (uint8_t) 0x04
#define PUMP_REQ              (uint8_t) 0x05
#define WATER_LEVEL_REQ       (uint8_t) 0x06
#define HEATING_UNIT_REQ      (uint8_t) 0x07
#define TEMPERATURE_REQ       (uint8_t) 0x08
#define GET_RPM_REQ           (uint8_t) 0x09
#define SET_RPM_REQ           (uint8_t) 0x0A
#define SIGNAL_LED_REQ        (uint8_t) 0x0B

// Command IDs
#define STATUS_CMD            (uint8_t) 0x01

#define START_CMD             (uint8_t) 0x10
#define STOP_CMD              (uint8_t) 0x20
#define PAUSE_CMD             (uint8_t) 0x40

#define ON_CMD                (uint8_t) 0x10
#define OFF_CMD               (uint8_t) 0x20

#define OPEN_CMD              (uint8_t) 0x10
#define CLOSE_CMD             (uint8_t) 0x20
#define LOCK_CMD              (uint8_t) 0x40
#define UNLOCK_CMD            (uint8_t) 0x80

// Door status IDs
#define OPENED                (uint8_t) 0x01
#define CLOSED                (uint8_t) 0x02
#define LOCKED                (uint8_t) 0x04
#define ON                    (uint8_t) 0x08
#define OFF                   (uint8_t) 0x10

// Machine states
#define HALTED                (uint8_t) 0x01
#define IDLE                  (uint8_t) 0x02
#define RUNNING               (uint8_t) 0x04
#define STOPPED               (uint8_t) 0x08
#define FAILED                (uint8_t) 0xFF

#endif // _PROTOCOL_H