
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#include <unistd.h>
#endif



int wait(int sec){
	if(waitingFlag==0){
		waitingFlag=1;
	}
	if(waitingFlag!=sec){
		waitingFlag++;
		return;
	}
	waitingFlag=0;
	return waitingFlag;
}

void activateLinearMotor(){
	linearMotor=1;
	linearMotor=0;
}

void activateRotaryMotor(){
	rotaryMotor=1;
	rotaryMotor=0;
}

void gripperOpen(){
	gripper=0;
}

void gripperClose()
{
	if(isSliderStateOpen()==1){
		gripper=1;
	}
}

void sliderExtend(){
	if(isSliderStateClose()==1){
		slider=1;
	}
}

void sliderClose(){
	if(isSliderStateOpen()==1){
		slider=0;
	}
}

int isSliderStateOpen(){
	return isSliderStateBit==0?1:0;
}

int isSliderStateClose(){
	return isSliderStateBit==1?1:0;
}

int isMotionComplete(){
	if(iRotaryMotionComplete==1 && iLinearMotionComplete==1){
		return 1;
	}
	else{
		return 0;
	}
}

void setAxis(int pos2,int pos1,int pos0){
	controlPos0=pos0;
	controlPos1=pos1;
	controlPos2=pos2;
}

void gotoPosSingleAxis(int bitSeq){
	int bit =0; int i=0;
	int a[2]= {0,0,0};
	for(i=0;i<3;i++){
		bit=(bitSeq & (1<<i))!=0;
		a[i]=bit;
	}
	setAxis(a[2],a[1],a[0]);
}

void gotoPosDoubleAxis(int linearBitSeq, int rotyaryBitSeq){
	gotoPosSingleAxis(linearBitSeq);
	gotoPosSingleAxis(rotyaryBitSeq);
}

void resetAxisValue(){
	controlPos0=0;
	controlPos1=0;
	controlPos2=0;
}

void motorMove(int pos, int linear){
	if(flag!=prevFlag && isMotionComplete()==1){
		gotoPosSingleAxis(pos);
		if(linear==1){
			linearMotor=1;
		}
		else{
			rotaryMotor=1;
		}
		prevFlag=flag;
	}
	else if(flag==prevFlag){
		flag++;
		resetAxisValue();
		if(linear==1){
			linearMotor=0;
		}
		else{
			rotaryMotor=0;
		}
	} 
}

void linearMotorMove(int pos){
	motorMove(pos,1);
}

void rotaryMotorMove(int pos){
	motorMove(pos,0);
}

void sliderPositionChange(int sliderOpen){
	if(flag!=prevFlag && isMotionComplete()==1){
		if(sliderOpen==1){
			sliderExtend();
		}
		else{
			sliderClose();
		}
		prevFlag=flag;   
	}
	
	
	else if(flag==prevFlag && sliderOpen==1 && isSliderStateOpen()==1){
		flag++;
	}
	
	else if(flag==prevFlag && sliderOpen==0 && isSliderStateClose()==1){
		flag++;
	}
}

void gripperPositionChange(int gripperOp){
	if(flag!=prevFlag && isMotionComplete()==1){
		if(wait(5)>0){ //2 second
			return;
		}
		if(gripperOp==1){
			gripperOpen();
		}
		else{
			gripperClose();
		}
		prevFlag=flag;
	}
	else if(flag==prevFlag){
		flag++;
	}
}

void _INIT ProgramInit(void)
{
	flag=1;
	prevFlag=0;
	test=0;
	
	sourceX = 6;
	sourceY = 7;
	
	destinationX = 0;
	destinationY = 2;
}

void _CYCLIC ProgramCyclic(void)
{
	if(waitingFlag==1){
		return;
	}
	if(flag==1){
		rotaryMotorMove(7); /// homing
	}
	else if(flag==2){
		linearMotorMove(0); /// homing
	}
	
	else if(flag==3){
		linearMotorMove(sourceY);
	}
	else if(flag==4){
		rotaryMotorMove(sourceX);
	}
	
	else if(flag==5){
		sliderPositionChange(1);
	}
	
	else if(flag==6){
		gripperPositionChange(0);
	}
	
	else if(flag==7){
		linearMotorMove(sourceY-1);
	}
	
	else if(flag==8){
		sliderPositionChange(0);
	}
	
	else if(flag==9){
		rotaryMotorMove(destinationX);
	}
	
	else if(flag==10){
		linearMotorMove(destinationY);
	}
	
	else if(flag==11){
		sliderPositionChange(1);
	}
	
	else if(flag==12){
		gripperPositionChange(1);
	}
		
	else if(flag==13){
		sliderPositionChange(0);
	}
	
	else if(flag==14){
		destinationX++;
		flag= 3;
		if(destinationX==6){
			flag=50;
		}
	}

	
}

void _EXIT ProgramExit(void)
{

}

