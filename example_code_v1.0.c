/*
 * Robox.c
 *
 * Created: 04/07/2017 19:59:26
 * Author : Matt
 */ 

// PORTA bit 0: HOME, POS1, POS2, POS3, POS4, POS5...
// PORTB GO, SW6, SW5, SW4, SW3, SW2, SW1,SW0

#define F_CPU 1000000UL  // 1 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
 
void initialise();
void disconnectServoB();
void disconnectServoA();
void connectServoB();
void connectServoA();
void setMotorB(int);
void setMotorA(int);

int main(void)
{
	initialise();

	PORTD = 0; //turn on both LEDs

    while (1) 
    {

    }
}

ISR(PCINT1_vect) 
{
	//PORTB pin change interrupt
};

ISR(PCINT0_vect)
{
	//PORTA pin change interrupt
};

void disconnectServoB()
{
	TCCR1A &= ~((1 <<  COM1B0) | (1 << COM1B1)); //disconnect timer 1 from Pin C1
}

void disconnectServoA()
{
	TCCR1A &= ~((1 <<  COM1A0) | (1 << COM1A1)); //disconnect timer 1 from Pin C1
}

void connectServoB()
{
	TCCR1A |= (0 <<  COM1B0) | (1 << COM1B1);
}

void connectServoA()
{
	TCCR1A |= (0 <<  COM1A0) | (1 << COM1A1);
}

void setMotorB(int position)
{
	OCR1B = position;
}

void setMotorA(int position)
{
	OCR1A = position; //send 1000us pulses
}

void initialise()
{
	sei(); //enable global interrupts
	DDRD = 0b00000011; //set LEDs as outputs
	DDRC = 0x00; //set port C as inputs
	DDRB = 0x00; //Set port B as inputs
	DDRA = 0x00; //Set port A as inputs
	PUEA = 0xff; //Turn on pull ups for port A
	PUEB = 0xff; // turn on pull ups for port B
	
	PCMSK0 = 0xff; //enable pin change interrupts on port a
	PCMSK1 = 0xff; //enable pin change interrupts on port b
	PCICR = 0x3; //enable interrupts on port a and port b
	
	//setup servo timer
	ICR1 = 20000; //set pulse period to 20ms
	TCCR1A |= (1 << WGM11) | (0 << WGM10) | (1 << COM1A1) | (0 <<  COM1A0) | (1 << COM1B1) | (0 <<  COM1B0); //set timer to clear OC1A and OC1B on compare match
	TCCR1B |= (1 << WGM13) | (1 << WGM12); //set to fast PWM mode (TOP = ICR1)
	TOCPMSA0 |= (1 << TOCC0S1) | (0 << TOCC0S0) | (1 << TOCC1S1) | (1 << TOCC1S0); //route timer 1 output compare A to Pin C0 and output compare B to Pin C1
	TOCPMCOE |= (1 << TOCC0OE) | (1 << TOCC1OE); //enable output compare mux for Pin C0 and Pin C1
	OCR1A = FINGER_BACKWARD;

	TCCR1B |= (1 << CS10) || (0 << CS11) || (0 << CS12); //set timer1 clock as CLK/1
	DDRC = 0x3; //set servos as outputs in GPIO
}

