/**
@file

*/
/*
  Application example t

  Library::

  This version is

  Copyright:: 2021 nguyentrinhtuan1996@gmail.com
*/

#ifndef _Application_atApp_TMC2208_
#define _Application_atApp_TMC2208_
/* _____PROJECT INCLUDES____________________________________________________ */
#include "App.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "pico/time.h"
#include <cmath>
// #include "services/TMC2208.h"
/* _____DEFINITIONS__________________________________________________________ */
#define PIN_SM1_EN 6
#define PIN_SM1_DIR 7
#define PIN_SM1_STEP 8
#define PIN_SM1_MS1 9
#define PIN_SM1_MS2 10

#define PIN_SM2_EN 11
#define PIN_SM2_DIR 12
#define PIN_SM2_STEP 13
#define PIN_SM2_MS1 14
#define PIN_SM2_MS2 15

enum SM1_State
{
	SM1_STOP,
	SM1_RUN_FOREVER,
	SM1_RUN_ANGLE
};
enum SM2_State
{
	SM2_STOP,
	SM2_RUN_FOREVER,
	SM2_RUN_ANGLE
};
// enum SM1_Resolution
// {
// 	ONE_HALF = 2,
// 	ONE_QUARTER = 4,
// 	ONE_EIGHTH = 8,
// 	ONE_SIXTEENTH = 16
// };

/* _____GLOBAL VARIABLES_____________________________________________________ */
// TaskHandle_t Task_atApp_TMC2208;
void atApp_TMC2208_Task_Func(void *parameter);
///////////////////////////////////////////////Testing part//
/* _____GLOBAL FUNCTION______________________________________________________ */

/* _____CLASS DEFINITION_____________________________________________________ */

/**
 * This Application class is the application to manage the
 */
class App_TMC2208 : public Application
{
public:
	App_TMC2208();
	~App_TMC2208();

	static float sm1_speed;
	static uint8_t sm1_resolution;
	static bool sm1_dir;
	static float sm1_angle;

	static float sm2_speed;
	static uint8_t sm2_resolution;
	static bool sm2_dir;
	static float sm2_angle;

	static SM1_State sm1_state;
	static SM2_State sm2_state;

protected:
private:
	static bool sm1_irq;
	static bool sm2_irq;
	static void App_TMC2208_Pend();
	static void App_TMC2208_Start();
	static void App_TMC2208_Restart();
	static void App_TMC2208_Execute();
	static void App_TMC2208_Suspend();
	static void App_TMC2208_Resume();
	static void App_TMC2208_End();
	static void SM1_RUN();
	static void SM2_RUN();
	static void SM1_count_msteps();
	static void SM2_count_msteps();
} atApp_TMC2208;

bool App_TMC2208::sm1_irq = false;
bool App_TMC2208::sm2_irq = false;
SM1_State App_TMC2208::sm1_state = SM1_STOP;
uint8_t App_TMC2208::sm1_resolution = 16;
float App_TMC2208::sm1_speed = 0;
bool App_TMC2208::sm1_dir = false;
float App_TMC2208::sm1_angle = 0;

SM2_State App_TMC2208::sm2_state = SM2_STOP;
uint8_t App_TMC2208::sm2_resolution = 16;
float App_TMC2208::sm2_speed = 0;
bool App_TMC2208::sm2_dir = false;
float App_TMC2208::sm2_angle = 0;

App_TMC2208::App_TMC2208(/* args */)
{
	_Pend_User = *App_TMC2208_Pend;
	_Start_User = *App_TMC2208_Start;
	_Restart_User = *App_TMC2208_Restart;
	_Execute_User = *App_TMC2208_Execute;
	_Suspend_User = *App_TMC2208_Suspend;
	_Resume_User = *App_TMC2208_Resume;
	_End_User = *App_TMC2208_End;

	// change the ID of application
	ID_Application = 1;
	// change the application name
	Name_Application = (char *)"TMC2208 Application";
	// change the ID of SNM
}
/**
 * This function will be automaticaly called when the object of class is delete
 */
App_TMC2208::~App_TMC2208()
{
}

void App_TMC2208::SM1_count_msteps()
{
	static uint64_t sm1_num_pulses = (uint64_t)round(sm1_angle * sm1_resolution / 1.8);
	static uint64_t sm1_count = 0;
	static uint8_t sm1_slice_num = pwm_gpio_to_slice_num(PIN_SM1_STEP);

	pwm_clear_irq(sm1_slice_num);
	sm1_count++;
	printf("sm1_count = %llu\n", sm1_count);

	if (sm1_count >= sm1_num_pulses)
	{
		pwm_set_enabled(sm1_slice_num, false);
		printf("Done\n");
		sm1_count = 0;
		sm1_state = SM1_STOP;
		irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), false);
	}
}
void App_TMC2208::SM2_count_msteps()
{
	static uint64_t sm2_num_pulses = (uint64_t)round(sm2_angle * sm2_resolution / 1.8);
	static uint64_t sm2_count = 0;
	static uint8_t sm2_slice_num = pwm_gpio_to_slice_num(PIN_SM2_STEP);

	pwm_clear_irq(sm2_slice_num);
	sm2_count++;
	printf("sm2_count = %llu\n", sm2_count);

	if (sm2_count >= sm2_num_pulses)
	{
		pwm_set_enabled(sm2_slice_num, false);
		printf("Done\n");
		sm2_count = 0;
		sm2_state = SM2_STOP;
		irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), false);
	}
}
void App_TMC2208::SM1_RUN()
{
	if (sm1_irq)
	{
		uint8_t sm1_slice_num = pwm_gpio_to_slice_num(PIN_SM1_STEP);
		pwm_clear_irq(sm1_slice_num);
		pwm_set_irq_enabled(sm1_slice_num, true);
		irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), SM1_count_msteps);
		irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), true);
	}
	bool true_resolution = true;
	gpio_put(PIN_SM1_EN, false);
	gpio_put(PIN_SM1_DIR, sm1_dir);
	if (sm1_speed > 300)
	{
		sm1_speed = 0;
		printf("sm1_speed must be less than or equal to 300 revolutions per minute.\n");
	}
	switch (sm1_resolution)
	{
	case 2:
		gpio_put(PIN_SM1_MS1, true);
		gpio_put(PIN_SM1_MS2, false);
		if (sm1_speed < 1.13)
		{
			sm1_speed = 0;
			printf("When sm1_resolution = 2, sm1_speed must be greater than or equal to 1.13 revolutions per minute.\n");
		}
		break;
	case 4:
		gpio_put(PIN_SM1_MS1, false);
		gpio_put(PIN_SM1_MS2, true);
		if (sm1_speed < 0.57)
		{
			sm1_speed = 0;
			printf("When sm1_resolution = 4, sm1_speed must be greater than or equal to 0.57 revolutions per minute.\n");
		}
		break;
	case 8:
		gpio_put(PIN_SM1_MS1, false);
		gpio_put(PIN_SM1_MS2, false);
		if (sm1_speed < 0.29)
		{
			sm1_speed = 0;
			printf("When sm1_resolution = 8, sm1_speed must be greater than or equal to 0.29 revolutions per minute.\n");
		}
		break;
	case 16:
		gpio_put(PIN_SM1_MS1, true);
		gpio_put(PIN_SM1_MS2, true);
		if (sm1_speed < 0.15)
		{
			sm1_speed = 0;
			printf("When sm1_resolution = 16, sm1_speed must be greater than or equal to 0.15 revolutions per minute.\n");
		}
		break;
	default:
		true_resolution = false;
		printf("sm1_resolution is an invalid value. Only 2, 4, 8 or 16 are allowed.\n");
	}
	if (true_resolution)
	{
		int sm1_div = 1;
		float sm1_top_tmp = 37500000 / sm1_speed / sm1_resolution / sm1_div - 1;
		while (sm1_top_tmp > 65535)
		{
			sm1_div++;
			sm1_top_tmp = 37500000 / sm1_speed / sm1_resolution / sm1_div - 1;
		}
		uint8_t sm1_slice_num = pwm_gpio_to_slice_num(PIN_SM1_STEP);
		pwm_set_clkdiv(sm1_slice_num, sm1_div);
		int sm1_top = round(sm1_top_tmp);
		pwm_set_wrap(sm1_slice_num, sm1_top);
		pwm_set_chan_level(sm1_slice_num, PWM_CHAN_A, sm1_top >> 1);
		pwm_set_enabled(sm1_slice_num, true);
	}
}

void App_TMC2208::SM2_RUN()
{
	if (sm2_irq)
	{
		uint8_t sm2_slice_num = pwm_gpio_to_slice_num(PIN_SM2_STEP);
		pwm_clear_irq(sm2_slice_num);
		pwm_set_irq_enabled(sm2_slice_num, true);
		irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), SM2_count_msteps);
		irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), true);
	}
	bool true_resolution = true;
	gpio_put(PIN_SM2_EN, false);
	gpio_put(PIN_SM2_DIR, sm2_dir);
	if (sm2_speed > 300)
	{
		sm2_speed = 0;
		printf("sm2_speed must be less than or equal to 300 revolutions per minute.\n");
	}
	switch (sm2_resolution)
	{
	case 2:
		gpio_put(PIN_SM2_MS1, true);
		gpio_put(PIN_SM2_MS2, false);
		if (sm2_speed < 1.13)
		{
			sm2_speed = 0;
			printf("When sm2_resolution = 2, sm2_speed must be greater than or equal to 1.13 revolutions per minute.\n");
		}
		break;
	case 4:
		gpio_put(PIN_SM2_MS1, false);
		gpio_put(PIN_SM2_MS2, true);
		if (sm2_speed < 0.57)
		{
			sm2_speed = 0;
			printf("When sm2_resolution = 4, sm2_speed must be greater than or equal to 0.57 revolutions per minute.\n");
		}
		break;
	case 8:
		gpio_put(PIN_SM2_MS1, false);
		gpio_put(PIN_SM2_MS2, false);
		if (sm2_speed < 0.29)
		{
			sm2_speed = 0;
			printf("When sm2_resolution = 8, sm2_speed must be greater than or equal to 0.29 revolutions per minute.\n");
		}
		break;
	case 16:
		gpio_put(PIN_SM2_MS1, true);
		gpio_put(PIN_SM2_MS2, true);
		if (sm2_speed < 0.15)
		{
			sm2_speed = 0;
			printf("When sm2_resolution = 16, sm2_speed must be greater than or equal to 0.15 revolutions per minute.\n");
		}
		break;
	default:
		true_resolution = false;
		printf("sm2_resolution is an invalid value. Only 2, 4, 8 or 16 are allowed.\n");
	}
	if (true_resolution)
	{
		int sm2_div = 1;
		float sm2_top_tmp = 37500000 / sm2_speed / sm2_resolution / sm2_div - 1;
		while (sm2_top_tmp > 65535)
		{
			sm2_div++;
			sm2_top_tmp = 37500000 / sm2_speed / sm2_resolution / sm2_div - 1;
		}
		uint8_t sm2_slice_num = pwm_gpio_to_slice_num(PIN_SM2_STEP);
		pwm_set_clkdiv(sm2_slice_num, sm2_div);
		int sm2_top = round(sm2_top_tmp);
		pwm_set_wrap(sm2_slice_num, sm2_top);
		pwm_set_chan_level(sm2_slice_num, PWM_CHAN_B, sm2_top >> 1);
		pwm_set_enabled(sm2_slice_num, true);
	}
}

/**
 * Pend to start is the first task of this application it will do prerequisite condition. In the debit mode, task will send information of application to terminal to start the application.
 */
void App_TMC2208::App_TMC2208_Pend()
{
	// atTMC2208.Debug();
}
/**
 * This start function will init some critical function
 */
void App_TMC2208::App_TMC2208_Start()
{
	gpio_set_function(PIN_SM1_STEP, GPIO_FUNC_PWM);
	gpio_set_function(PIN_SM2_STEP, GPIO_FUNC_PWM);

	gpio_init(PIN_SM1_EN);
	gpio_init(PIN_SM1_DIR);
	gpio_init(PIN_SM1_MS1);
	gpio_init(PIN_SM1_MS2);

	gpio_init(PIN_SM2_EN);
	gpio_init(PIN_SM2_DIR);
	gpio_init(PIN_SM2_MS1);
	gpio_init(PIN_SM2_MS2);

	gpio_set_dir(PIN_SM1_EN, GPIO_OUT);
	gpio_set_dir(PIN_SM1_DIR, GPIO_OUT);
	gpio_set_dir(PIN_SM1_MS1, GPIO_OUT);
	gpio_set_dir(PIN_SM1_MS2, GPIO_OUT);

	gpio_set_dir(PIN_SM2_EN, GPIO_OUT);
	gpio_set_dir(PIN_SM2_DIR, GPIO_OUT);
	gpio_set_dir(PIN_SM2_MS1, GPIO_OUT);
	gpio_set_dir(PIN_SM2_MS2, GPIO_OUT);

	gpio_put(PIN_SM1_EN, true);
	gpio_put(PIN_SM2_EN, true);
}
/**
 * Restart function of SNM  app
 */
void App_TMC2208::App_TMC2208_Restart()
{
}
/**
 * Execute fuction of SNM app
 */
void App_TMC2208::App_TMC2208_Execute()
{
	switch (sm1_state)
	{
	case SM1_STOP:
		gpio_put(PIN_SM1_EN, true);
		break;
	case SM1_RUN_FOREVER:
		sm1_irq = false;
		SM1_RUN();
		break;
	case SM1_RUN_ANGLE:
		if (sm1_angle <= 0)
			printf("sm1_angle must be greater than 0.\n");
		else
		{
			sm1_irq = true;
			SM1_RUN();
		}
		break;
	default:;
	}

	switch (sm2_state)
	{
	case SM2_STOP:
		gpio_put(PIN_SM2_EN, true);
		break;
	case SM2_RUN_FOREVER:
		sm2_irq = false;
		SM2_RUN();
		break;
	case SM2_RUN_ANGLE:
		if (sm2_angle <= 0)
			printf("sm2_angle must be greater than 0.\n");
		else
		{
			sm2_irq = true;
			SM2_RUN();
		}
		break;
	default:;
	}

	if (atApp_TMC2208.User_Mode == APP_USER_MODE_DEBUG)
	{
	}
}
void App_TMC2208::App_TMC2208_Suspend() {}
void App_TMC2208::App_TMC2208_Resume() {}
void App_TMC2208::App_TMC2208_End() {}
#endif