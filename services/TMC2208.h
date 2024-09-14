/**
@file
*/
/*
  Service_atTMC2208.h

  This version is

  Copyright:: 2021 nguyentrinhtuan1996@gmail.com
*/

#ifndef _Service_atTMC2208_
#define _Service_atTMC2208_
/* _____PROJECT INCLUDES____________________________________________________ */
#include "Service.h"
/* _____DEFINETIONS__________________________________________________________ */

/* _____GLOBAL VARIABLES_____________________________________________________ */

/* _____GLOBAL FUNCTION______________________________________________________ */

/* _____CLASS DEFINITION_____________________________________________________ */
/**
 * This Service class is the Service to manage the 
 */
class TMC2208 : public Service
{
public:
    TMC2208();
    ~TMC2208();
    
protected:
     
private:
    static void  TMC2208_Start();
    static void  TMC2208_Execute();    
    static void  TMC2208_End();
} atTMC2208 ;
/**
 * This function will be automaticaly called when a object is created by this class
 */
TMC2208::TMC2208(/* args */)
{
    _Start_User      = *TMC2208_Start;
    _Execute_User    = *TMC2208_Execute;
    _End_User        = *TMC2208_End;

    // change the ID of Service
    ID_Service = 1;
    // change the Service name
    Name_Service = (char*)"TMC2208 Service";
    // change the ID of SNM
}
/**
 * This function will be automaticaly called when the object of class is delete
 */
TMC2208::~TMC2208()
{
    
}
/**
 * This start function will init some critical function 
 */
void  TMC2208::TMC2208_Start()
{
    
}  

/**
 * Execute fuction of SNM app
 */
void  TMC2208::TMC2208_Execute()
{
    printf("Hello world!\n");
    // sleep_ms(500); 
    if(atTMC2208.User_Mode == SER_USER_MODE_DEBUG)
    {
        
    }   
}    
void  TMC2208::TMC2208_End(){}

#endif


