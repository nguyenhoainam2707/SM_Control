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
#include "services/TMC2208.h"
/* _____DEFINITIONS__________________________________________________________ */

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
	float m1_speed = 0;
	float m2_speed = 0;
	bool m1_run = false;
	bool m2_run = false;
protected:
private:
  	static void  App_TMC2208_Pend();
	static void  App_TMC2208_Start();
	static void  App_TMC2208_Restart();
	static void  App_TMC2208_Execute();
	static void  App_TMC2208_Suspend();
	static void  App_TMC2208_Resume();	  
	static void  App_TMC2208_End();
} atApp_TMC2208 ;
/**
 * This function will be automaticaly called when a object is created by this class
 */
App_TMC2208::App_TMC2208(/* args */)
{
  	_Pend_User 	     = *App_TMC2208_Pend;
	_Start_User 	 = *App_TMC2208_Start;
	_Restart_User 	 = *App_TMC2208_Restart;
	_Execute_User 	 = *App_TMC2208_Execute;
	_Suspend_User	 = *App_TMC2208_Suspend;
	_Resume_User	 = *App_TMC2208_Resume;
	_End_User	     = *App_TMC2208_End;

	// change the ID of application
	ID_Application = 1;
	// change the application name
	Name_Application = (char*)"TMC2208 Application";
	// change the ID of SNM
}
/**
 * This function will be automaticaly called when the object of class is delete
 */
App_TMC2208::~App_TMC2208()
{
	
}
/**
 * Pend to start is the first task of this application it will do prerequisite condition. In the debit mode, task will send information of application to terminal to start the application.
 */
void  App_TMC2208::App_TMC2208_Pend()
{
    atService_XYZ.Debug();
}
/**
 * This start function will init some critical function 
 */
void  App_TMC2208::App_TMC2208_Start()
{
	// init atXYZ Service in the fist running time
	atService_XYZ.Run_Service();
}  
/**
 * Restart function of SNM  app
 */
void  App_TMC2208::App_TMC2208_Restart()
{

}
/**
 * Execute fuction of SNM app
 */
void  App_TMC2208::App_TMC2208_Execute()
{	
	atService_XYZ.Run_Service();
    if(atApp_TMC2208.User_Mode == APP_USER_MODE_DEBUG)
    {
		
    }   
}
void  App_TMC2208::App_TMC2208_Suspend(){}
void  App_TMC2208::App_TMC2208_Resume(){}	  
void  App_TMC2208::App_TMC2208_End(){}
// void atApp_TMC2208_Task_Func(void *parameter)
// {
//   while (1)
//   {
//     atApp_TMC2208.Run_Application(APP_RUN_MODE_AUTO);
//     vTaskDelay(1000/ portTICK_PERIOD_MS);
//   }
// }
#endif