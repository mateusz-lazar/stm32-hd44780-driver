#include <main.h>
#include <LCDFunctions.h>



int main()
{

	LCDInitOutputPorts();
	LCDSetupDisplay();

	LCDSend2LinesofString("Nucleo L476RG", "16x2 LCD driver");


	while(1)
	{


	}
}
