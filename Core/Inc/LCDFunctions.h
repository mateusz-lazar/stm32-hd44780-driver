#ifndef LCDFunctionsHeader

#define LCDFunctionsHeader

#include <stdio.h>

#define LCDRSPin 10
#define LCDRSPort GPIOA
#define LCDRWPin 3
#define LCDRWPort GPIOB
#define LCDEPin 5
#define LCDEPort GPIOB
#define LCDD0Pin 10
#define LCDD0Port GPIOB
#define LCDD1Pin 8
#define LCDD1Port GPIOA
#define LCDD2Pin 9
#define LCDD2Port GPIOA
#define LCDD3Pin 6
#define LCDD3Port GPIOB
#define LCDD4Pin 7
#define LCDD4Port GPIOA
#define LCDD5Pin 6
#define LCDD5Port GPIOA
#define LCDD6Pin 9
#define LCDD6Port GPIOB
#define LCDD7Pin 8
#define LCDD7Port GPIOB
#define timeDelayBeforeEnable 400
#define timeDelayBeforeDisable 800
#define LCDInstruction_SetTo8bit_Mode2lines 0b00111000
#define LCDInstruction_TurnOnDisplay 0b00001110
#define LCDInstruction_IncrementPositionByOne 0b00000110
#define LCDInstruction_ClearDisplay 0b00000001
#define LCDInstruction_MoveTo2ndLine 0b11000000


void notExactTimeDelay(int timeDelay)
{
	volatile int i;
	for(i = 0; i < timeDelay; i++)
	{

	}
}
void SetPortAndPinForOutput(GPIO_TypeDef *port, int pinNumber)
{
	if(port == GPIOA)
	{
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	}
	if(port == GPIOB)
	{
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	}
	if(port == GPIOC)
	{
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	}
	if(port == GPIOD)
	{
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	}
	if(port == GPIOE)
	{
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	}
	if(port == GPIOF)
	{
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
	}
	if(port == GPIOG)
	{
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOGEN;
	}
	if(port == GPIOH)
	{
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN;
	}

	port->MODER &= ~(1 << ((pinNumber*2)+1));
	port->MODER |= (1 << pinNumber*2);

	port->OTYPER &= ~(1 << pinNumber);

	port->OSPEEDR |= (1 << ((pinNumber*2) + 1)) | (1 << pinNumber*2);

	port->PUPDR &= ~(1 << pinNumber*2);
	port->PUPDR &= ~(1 << ((pinNumber*2) + 1));
}
void SendBitToPortAndPin(GPIO_TypeDef *port, int pinNumber, uint8_t bitState)
{
	if(bitState)
		{
			port->BSRR = (1 << pinNumber);
		}
		else
		{
			port->BRR = (1 << pinNumber);
		}
}
void LCDInitOutputPorts()
{
	SetPortAndPinForOutput(LCDRSPort, LCDRSPin);
	SetPortAndPinForOutput(LCDRWPort, LCDRWPin);
	SetPortAndPinForOutput(LCDEPort, LCDEPin);
	SetPortAndPinForOutput(LCDD0Port, LCDD0Pin);
	SetPortAndPinForOutput(LCDD1Port, LCDD1Pin);
	SetPortAndPinForOutput(LCDD2Port, LCDD2Pin);
	SetPortAndPinForOutput(LCDD3Port, LCDD3Pin);
	SetPortAndPinForOutput(LCDD4Port, LCDD4Pin);
	SetPortAndPinForOutput(LCDD5Port, LCDD5Pin);
	SetPortAndPinForOutput(LCDD6Port, LCDD6Pin);
	SetPortAndPinForOutput(LCDD7Port, LCDD7Pin);
}
void LCDEnable()
{
	notExactTimeDelay(timeDelayBeforeEnable);
	SendBitToPortAndPin(LCDEPort, LCDEPin, 1);
}
void LCDSetToWrite()
{
	SendBitToPortAndPin(LCDRWPort, LCDRWPin, 0);
}
void LCDSetToRead()
{
	SendBitToPortAndPin(LCDRWPort, LCDRWPin, 1);
}
void LCDInstructionMode()
{
	SendBitToPortAndPin(LCDRSPort, LCDRSPin, 0);
}
void LCDCharacterMode()
{
	SendBitToPortAndPin(LCDRSPort, LCDRSPin, 1);
}
void LCDSendByteToTheLCDDataPins(char character)
{
	SendBitToPortAndPin(LCDD0Port, LCDD0Pin, character & 0b00000001);
	SendBitToPortAndPin(LCDD1Port, LCDD1Pin, character & 0b00000010);
	SendBitToPortAndPin(LCDD2Port, LCDD2Pin, character & 0b00000100);
	SendBitToPortAndPin(LCDD3Port, LCDD3Pin, character & 0b00001000);
	SendBitToPortAndPin(LCDD4Port, LCDD4Pin, character & 0b00010000);
	SendBitToPortAndPin(LCDD5Port, LCDD5Pin, character & 0b00100000);
	SendBitToPortAndPin(LCDD6Port, LCDD6Pin, character & 0b01000000);
	SendBitToPortAndPin(LCDD7Port, LCDD7Pin, character & 0b10000000);
	notExactTimeDelay(timeDelayBeforeDisable);
	SendBitToPortAndPin(LCDEPort, LCDEPin, 0);
}
void LCDSendACharacter(char character)
{
	LCDSetToWrite();
	LCDCharacterMode();
	LCDEnable();
	LCDSendByteToTheLCDDataPins(character);
}
void LCDSendAnInstruction(char character)
{
	LCDSetToWrite();
	LCDInstructionMode();
	LCDEnable();
	LCDSendByteToTheLCDDataPins(character);
}

void LCDSendAString(char* text)
{
	while(*text)
	{
		LCDSendACharacter(*text++);
	}

}

void LCDSetupDisplay()
{
	notExactTimeDelay(100000);
	LCDSendAnInstruction(LCDInstruction_SetTo8bit_Mode2lines);
	notExactTimeDelay(20000);
	LCDSendAnInstruction(LCDInstruction_SetTo8bit_Mode2lines);
	notExactTimeDelay(2000);
	LCDSendAnInstruction(LCDInstruction_SetTo8bit_Mode2lines);

	LCDSendAnInstruction(LCDInstruction_SetTo8bit_Mode2lines);
	LCDSendAnInstruction(LCDInstruction_ClearDisplay);
	LCDSendAnInstruction(LCDInstruction_IncrementPositionByOne);
	LCDSendAnInstruction(LCDInstruction_TurnOnDisplay);

}

void LCDSend2LinesofString(char* Line1Text, char* Line2Text)
{
	LCDSendAString(Line1Text);
	LCDSendAnInstruction(LCDInstruction_MoveTo2ndLine);
	LCDSendAString(Line2Text);
}

void LCDSendAnInteger(int IntegerToBeDisplayed, uint8_t MaxLengthOfNumber)
{
	char StringNumber[MaxLengthOfNumber + 1];
	snprintf(StringNumber, MaxLengthOfNumber + 1, "%d", IntegerToBeDisplayed);
	LCDSendAString(StringNumber);
}

void LCDSendAFloatingPointNumber(float FLoatingPointNumberToBeDisplayed, uint8_t MaxLengthOfNumber)
{
	char StringNumber[MaxLengthOfNumber + 1];
	snprintf(StringNumber, MaxLengthOfNumber + 1, "%f", FLoatingPointNumberToBeDisplayed);
	LCDSendAString(StringNumber);
}
#endif
