#include <kipr/wombat.h>

int lightSwitchPort = 0;
int up = 1900;
int down = 150;

int clawPort = 3;
int open = 1000;
int close = 1450;
int fullClose = 1600;

float rSpeed = 755;
int rMotor = 0;

float lSpeed = 750;
int lMotor = 3;

float degrees = 16;	//ticks to degrees for turning
float cm = 109.5;	//ticks to cm for moving straight

void Forward(float distance);
void Backward(float distance);
void Left(float degrees);
void Right(float degrees);
void Claw(float position);
void lightSwitch(float position);

//---------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
   	LightSwitch(down);
    Claw(fullClose);
    Forward(2);
    Right(90);				 //turn toward middle
    Forward(77);		   //move to middle
    Claw(open);
    Right(47);				//face cube
    Forward(20);					//move to cube
    Claw(close);				//grab cube
    Backward(10);				//back up away from cube
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Left(42);					//face away from ping-pong balls
    Backward(195);				//move to ping-pong ballls
    Left(155);				//Turn left in big starting box to face ping pongs
    Forward(15);
    Claw(open); 		//place cube under ping pong balls
    
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

void Forward(float distance)
{
    cmpc(rMotor);
    cmpc(lMotor);
    while(gmpc(rMotor) < distance * cm)
    {
        mav(rMotor, rSpeed);
        mav(lMotor, lSpeed);
        msleep(5);
    }
    ao();
}
         
void Backward(float distance)
{
    cmpc(rMotor);
    cmpc(lMotor);
    while(abs(gmpc(rMotor)) < distance * cm)
    {
        mav(rMotor, -rSpeed);
        mav(lMotor, -lSpeed);
        msleep(5);
    }
    ao();
}

void Left(float angle)
{
    	cmpc(rMotor);
    	msleep(5);
        while(gmpc(rMotor) < angle*degrees)
        {
            mav(rMotor, rSpeed);
    		mav(lMotor, -lSpeed);
    		msleep(5);
        }    
    	ao();
}

void Right(float angle)
{
    	cmpc(lMotor);
    	msleep(5);
        while(gmpc(lMotor) < angle*degrees)
        {
            mav(rMotor, -rSpeed);
    		mav(lMotor, lSpeed);
    		msleep(5);
        }
   		ao();
}

void Claw(float position)
{
    set_servo_position(clawPort, position);
    enable_servos();
    msleep(500);
    disable_servos();
}

void LightSwitch(float position)
{
    set_servo_position(lightSwitchPort, position);
    enable_servos();
    msleep(500);
    disable_servos();
}