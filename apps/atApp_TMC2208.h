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
	static unsigned char sm1_resolution;
	static float sm1_div;
	static float sm1_angle;

	static float sm2_speed;
	static unsigned char sm2_resolution;
	static float sm2_div;
	static float sm2_angle;
	
	static SM1_State sm1_state;
	static SM2_State sm2_state;

protected:
private:
	static void App_TMC2208_Pend();
	static void App_TMC2208_Start();
	static void App_TMC2208_Restart();
	static void App_TMC2208_Execute();
	static void App_TMC2208_Suspend();
	static void App_TMC2208_Resume();
	static void App_TMC2208_End();
	static void SM1_RUN();
	static void SM2_RUN();
} atApp_TMC2208;

SM1_State App_TMC2208::sm1_state = SM1_STOP;
float App_TMC2208::sm1_speed = 0;
unsigned char App_TMC2208::sm1_resolution = 16;
float App_TMC2208::sm1_div = 8;
float App_TMC2208::sm1_angle = 0;

SM2_State App_TMC2208::sm2_state = SM2_STOP;
float App_TMC2208::sm2_speed = 0;
unsigned char App_TMC2208::sm2_resolution = 16;
float App_TMC2208::sm2_div = 8;
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

void App_TMC2208::SM1_RUN()
{
	switch (sm1_resolution)
	{
	case 2:
		gpio_put(PIN_SM1_MS1, true);
		gpio_put(PIN_SM1_MS2, false);
		break;
	case 4:
		gpio_put(PIN_SM1_MS1, false);
		gpio_put(PIN_SM1_MS2, true);
		break;
	case 8:
		gpio_put(PIN_SM1_MS1, false);
		gpio_put(PIN_SM1_MS2, false);
		break;
	case 16:
		gpio_put(PIN_SM1_MS1, true);
		gpio_put(PIN_SM1_MS2, true);
		break;
	default:;
	}
	uint8_t sm1_slice_num = pwm_gpio_to_slice_num(PIN_SM1_STEP);
	pwm_set_clkdiv(sm1_slice_num, sm1_div);
	float sm1_top_tmp = 37500000 / fabs(sm1_speed) / sm1_resolution / sm1_div - 1;
	if (sm1_top_tmp <= 65535)
	{
		int sm1_top = round(sm1_top_tmp);
		pwm_set_wrap(sm1_slice_num, sm1_top);
		pwm_set_chan_level(sm1_slice_num, PWM_CHAN_A, sm1_top >> 1);
		pwm_set_enabled(sm1_slice_num, true);
	}
}

void App_TMC2208::SM2_RUN()
{
	switch (sm2_resolution)
	{
	case 2:
		gpio_put(PIN_SM2_MS1, true);
		gpio_put(PIN_SM2_MS2, false);
		break;
	case 4:
		gpio_put(PIN_SM2_MS1, false);
		gpio_put(PIN_SM2_MS2, true);
		break;
	case 8:
		gpio_put(PIN_SM2_MS1, false);
		gpio_put(PIN_SM2_MS2, false);
		break;
	case 16:
		gpio_put(PIN_SM2_MS1, true);
		gpio_put(PIN_SM2_MS2, true);
		break;
	default:;
	}
	uint8_t sm2_slice_num = pwm_gpio_to_slice_num(PIN_SM2_STEP);
	pwm_set_clkdiv(sm2_slice_num, sm2_div);
	float sm2_top_tmp = 37500000 / fabs(sm2_speed) / sm2_resolution / sm2_div - 1;
	if (sm2_top_tmp <= 65535)
	{
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
		gpio_put(PIN_SM1_EN, false);
		int tmp;
		if (sm1_speed < 0)
		{
			tmp = -sm1_speed;
			gpio_put(PIN_SM1_DIR, true);
		}
		else
		{
			tmp = sm1_speed;
			gpio_put(PIN_SM1_DIR, false);
		}
		if ((tmp >= 40) && (tmp <= 250)) sm1_div = 1;
		else
		{
			if ((tmp < 40) && (tmp >= 5)) sm1_div = 8;
			else
				if ((tmp < 5) && (tmp >= 0.15)) sm1_div = 250;
		}
		SM1_RUN();
		break;
	case SM1_RUN_ANGLE:
		gpio_put(PIN_SM1_EN, false);
		break;
	default:;
	}

	switch (sm2_state)
	{
	case SM2_STOP:
		gpio_put(PIN_SM2_EN, true);
		break;
	case SM2_RUN_FOREVER:
		gpio_put(PIN_SM2_EN, false);
		int tmp;
		if (sm2_speed < 0)
		{
			tmp = -sm2_speed;
			gpio_put(PIN_SM2_DIR, true);
		}
		else
		{
			tmp = sm2_speed;
			gpio_put(PIN_SM2_DIR, false);
		}
		if ((tmp >= 40) && (tmp <= 250)) sm2_div = 1;
		else
		{
			if ((tmp < 40) && (tmp >= 5)) sm2_div = 8;
			else
				if ((tmp < 5) && (tmp >= 0.15)) sm2_div = 250;
		}
		SM2_RUN();
		break;
	case SM2_RUN_ANGLE:
		gpio_put(PIN_SM2_EN, false);
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