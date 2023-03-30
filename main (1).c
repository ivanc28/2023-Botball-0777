#include <kipr/wombat.h>
	int RMotor = 0;
    float RSpeed = 740;
    int LMotor = 3;
    float LSpeed = 760;
	int RTurnSpeed = 575;
	int LTurnSpeed = 575;
	int RTrackSpeed = 280;
	int LTrackSpeed = 280;
	int armPort = 1;
	int clawPort = 2;
	int armPosLeft = 400;
	int armPosLeftMid = 635;
	int armPosMiddle = 970;
	int armPosRightMid = 1330;
	int armPosRight = 1650;
	int clawPosOpen = 350;
	int clawPosClose = 1390;

	int IRPort = 0;
	int IRRange = 3300; // higher is dark, lower is light
    
	float cm = 65; // ticks per centimeter
    float degree = 12.2f;  // ticks per degree

	int greenChannel = 0;
	int redChannel = 1;
    
	void Move(int dir, int distance); // dir = 1 is forward, dir = -1 is backward
    void Turn(int right, float ticks); // right = 1 turn right, right = 0 turn left
	void Servo(int port, int pos);
	void SlowServo(int port, int pos);
	void FollowLine(int distance);
	void BackSense();
    void TurnTrack(int right, float angle);
	void CameraAlign(int channel, int dir);

int main()
{       
    Servo(clawPort, clawPosOpen);
    Servo(armPort, armPosMiddle);
    msleep(500);
    Turn(1, 25);
    Move(1, 44); //move towards first 2 poms
    Servo(armPort, armPosRight); // pushes green pom to the right
    Turn(1, 90);
    Turn(0, 15);
    FollowLine(21);
    SlowServo(armPort, armPosLeft); //pushes 3rd pom (red) to the left
    FollowLine( 18);
    SlowServo(armPort, armPosRight); //pushes 4th pom (green) to the right
    FollowLine( 31);
    SlowServo(armPort, armPosLeft); //pushes 1st set of double poms (red) to the left
    FollowLine( 30);
    SlowServo(armPort, armPosRight); //pushes 2nd set of double poms (green) to the right
    Move(1, 33);
    SlowServo(armPort, armPosLeft); //pushes 3rd set of double poms (red) to the left
    FollowLine( 30);
    SlowServo(armPort, armPosRight); //pushes 4th set of double poms (green) to the right
    FollowLine(26); // move up to first stack
    Servo(clawPort, clawPosClose); //grabs top pom (green) of 1st stack
    msleep(500);
    FollowLine(3);
    msleep(500);
    //Servo(armPort, armPosLeftMid); //pushes bottom of stack (red) to the left
    //msleep(400);
    SlowServo(armPort, armPosLeft); 
    msleep(500);
    Servo(clawPort, clawPosOpen); //drops green pom and puts on right side of sort
    FollowLine(25);
    Servo(clawPort, clawPosClose); //grabs top pom (red) of 2nd stack
    msleep(500);
    SlowServo(armPort, armPosRight); // pushes bottom of stack (green) to the right;
    msleep(500);
    Servo(clawPort, clawPosOpen); //drops red pom and puts on right side of sort
    msleep(200);
    Turn(1, 90);
    
    // FINISHED SORTING
    Move(1, 15);
    Turn(0, 40); // shake poms so that they are all touching surface
    Turn(1, 80);
    Turn(0, 40);
    
    Move(-1, 50);
    Turn(1, 90);
    Move(1, 120);
    Turn(0, 90);
    Move(1, 40);
    Servo(armPort, armPosLeft);
    Servo(clawPort, clawPosClose);
    Turn(0, 40); // shake poms again
    Turn(1, 80);
    Turn(0, 40);
    Move(-1, 80);
    
    
    disable_servos();
    
    
    //Turn(1, 90);    
}
void Move(int dir, int distance)
{
    cmpc(RMotor);
    cmpc(LMotor);
    
    while(abs(gmpc(RMotor)) < (distance * cm))
    {
        mav(RMotor, RSpeed * dir);
        mav(LMotor, LSpeed * dir);
    }
    ao();
}

void Turn(int right, float angle)
{
    cmpc(RMotor);
    cmpc(LMotor);
    if (right == 1)
    {
        while(gmpc(LMotor) < (angle * degree))
        {
            mav(RMotor, -RTurnSpeed);
            mav(LMotor, LTurnSpeed);
        }
        ao();
    }
    else
    {
        while(gmpc(RMotor) < (angle * degree))
        {
            mav(RMotor, RTurnSpeed);
            mav(LMotor, -LTurnSpeed);
        }
        ao();
    }
}

void Servo(int port, int pos)
{
    enable_servos();
    set_servo_position(port, pos);
    msleep(50);
    
}

void SlowServo(int port, int pos)
{
    enable_servos();
    int currentPos = get_servo_position(port);
    if(pos > currentPos)
    {
        while(currentPos < pos)
        {
            currentPos++;
            set_servo_position(port, currentPos);
            msleep(1);
        }
    }
    if(pos < currentPos)
    {
        while(currentPos > pos)
        {
            currentPos--;
            set_servo_position(port, currentPos);
            msleep(1);
        }
    }
    set_servo_position(port, pos);
    msleep(50);
}

void FollowLine(int distance) 
{
    cmpc(RMotor);
    while(abs(gmpc(RMotor)) < distance * cm)
    {       
        if(analog(IRPort) > IRRange)
        {
            mav(RMotor, RSpeed + 150);
            mav(LMotor, LSpeed - 150);
        }
        else if(analog(IRPort) < IRRange)
        {
            mav(RMotor, RSpeed - 100);
            mav(LMotor, LSpeed + 100);
        }
    }
    msleep(5);
    mav(RMotor, 0);
    mav(LMotor, 0);
}

void BackSense()
{
    while(digital(0) == 0 || digital(9) == 0)
    {
        mav(RMotor, -RSpeed);
        mav(LMotor, -LSpeed);
    }
}

void TurnTrack(int right, float angle)
{
    cmpc(RMotor);
    cmpc(LMotor);
    if (right == 1)
    {
        while(gmpc(LMotor) < (angle * degree))
        {
            mav(RMotor, -RTrackSpeed);
            mav(LMotor, LTrackSpeed);
        }
        ao();
    }
    else
    {
        while(gmpc(RMotor) < (angle * degree))
        {
            mav(RMotor, RTrackSpeed);
            mav(LMotor, -LTrackSpeed);
        }
        ao();
    }
}

void CameraAlign(int channel, int dir)
{
    camera_open();
    camera_update();
    if (dir == 0)
    {
        while(get_object_center_column(channel, 0) < 75 || get_object_center_column(channel, 0) > 85 || get_object_count(channel) == 0)
        {
            camera_update();
            //mav(RMotor, RTrackSpeed);
            //mav(LMotor, -LTrackSpeed);  
            TurnTrack(0, 1);
        }
    }
    else
    {
        while(get_object_center_column(channel, 0) > 85 || get_object_center_column(channel, 0) < 75 || get_object_count(channel) == 0)
        {
            camera_update();
            //mav(RMotor, -RTrackSpeed);
            //mav(LMotor, LTrackSpeed);   
            TurnTrack(1, 1);
        }
    }
    camera_close();
}
        
