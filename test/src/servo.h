/* ACE RC S1903 servomotors
 *
 * Few details available at:
 * 		https://servodatabase.com/servo/ace-rc/s1903
 */

#ifndef SERVO_H
#define SERVO_H

typedef enum{SERVO_X, SERVO_Y} ServoID; // servomotor naming

void Servo_Config(uint32_t period, uint16_t prescaler);
void Servo_SetPosition(ServoID servo_id, int32_t angle_pct);

#endif // SERVO_H
