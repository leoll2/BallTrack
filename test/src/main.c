#include "CUnit/Basic.h"
#include "CUnit/Console.h"
#include "CUnit/Automated.h"

#include "mock.h"
#include "servo.h"

static CU_pSuite pSuite = NULL;


/* ########################################################################
 * SUITE 'SERVO':
 * Test servomotor commands. Ensure that operative limits are not violated.
 * ######################################################################## */

int init_suite_servo(void)
{
	return 0;
}

int clean_suite_servo(void)
{
	return 0;
}

/* Test ServoMotor0 */
void test_servo0(void)
{
	Servo_SetPosition(SERVO_Y, 0);
	CU_ASSERT_EQUAL(servo0_pulse, 1310)
	Servo_SetPosition(SERVO_Y, 90);
	CU_ASSERT_EQUAL(servo0_pulse, 1571);
	Servo_SetPosition(SERVO_Y, 100);
	CU_ASSERT_EQUAL(servo0_pulse, 1600);
	Servo_SetPosition(SERVO_Y, 110);
	CU_ASSERT_EQUAL(servo0_pulse, 1600);
	Servo_SetPosition(SERVO_Y, -90);
	CU_ASSERT_EQUAL(servo0_pulse, 1049);
	Servo_SetPosition(SERVO_Y, -100);
	CU_ASSERT_EQUAL(servo0_pulse, 1020);
	Servo_SetPosition(SERVO_Y, -110);
	CU_ASSERT_EQUAL(servo0_pulse, 1020);
}


/* Test ServoMotor1 */
void test_servo1(void)
{
	Servo_SetPosition(SERVO_X, 0);
	CU_ASSERT_EQUAL(servo1_pulse, 1550)
	Servo_SetPosition(SERVO_X, 90);
	CU_ASSERT_EQUAL(servo1_pulse, 1820);
	Servo_SetPosition(SERVO_X, 100);
	CU_ASSERT_EQUAL(servo1_pulse, 1850);
	Servo_SetPosition(SERVO_X, 110);
	CU_ASSERT_EQUAL(servo1_pulse, 1850);
	Servo_SetPosition(SERVO_X, -90);
	CU_ASSERT_EQUAL(servo1_pulse, 1280);
	Servo_SetPosition(SERVO_X, -100);
	CU_ASSERT_EQUAL(servo1_pulse, 1250);
	Servo_SetPosition(SERVO_X, -110);
	CU_ASSERT_EQUAL(servo1_pulse, 1250);
}


/* Initialize CUnit registry and suites. */
CU_ErrorCode init_cunit(void)
{
	// Initialize test registry
	if (CUE_SUCCESS != CU_initialize_registry())
	  return CU_get_error();

	// Configure suite 'SERVO'
	pSuite = CU_add_suite("Suite_servo", init_suite_servo, clean_suite_servo);
	if (NULL == pSuite) {
	  CU_cleanup_registry();
	  return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite, "test_servo0", test_servo0)) ||
	   (NULL == CU_add_test(pSuite, "test_servo1", test_servo1)))
	{
	  CU_cleanup_registry();
	  return CU_get_error();
	}


	return 0;
}

/* Cleanup CUnit registry */
CU_ErrorCode cleanup_cunit(void)
{
	// Clean up registry and return
	CU_cleanup_registry();
	return CU_get_error();
}


void run_cunit_tests(void)
{
	// Run all tests using console interface
	CU_console_run_tests();
}


int main()
{
	init_cunit();

	run_cunit_tests();

	return cleanup_cunit();
}
