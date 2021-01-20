#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/**
*   Address of the slave on the bus.
*/
#define TWI_ADDR     8

/**
*   Enable pin for the channel on the H-bridge.
*/
#define PIN_ENABLE_A 7

/**
*  Enable pin for the channel on the H-bridge.
*/
#define PIN_ENABLE_B 8

/**
*   PWN-Pin for the speed control on the H-bridge.
*/
#define PIN_PWM 9

/**
*  Encoder feedback from the H-bridge.
*  Must be a interrupt pin.
*/
#define PHASE_B     (PIND & 1<<PD2)

/**
*  Encoder feedback from the H-bridge.
*  Must be a interrupt pin.
*/
#define PHASE_A     (PIND & 1<<PD3)

/**
*  Input pin for the Home sensor.
*/
#define PIN_HOME 4

/**
 *
 */
#define ZERO_OFFSET 1345


#endif // CONFIGURATION_H
