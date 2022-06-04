/*
   OC2A 11  PB3
   OC2B  3  PD3
 */
#define _DISABLE_ARDUINO_TIMER0_INTERRUPT_HANDLER_
#include <wiring.c>
uint16_t rig;
uint16_t lef;
uint16_t mid;
bool rigSen;
bool lefSen;
bool midSen;
//PD3 left engine pwm
// PB3 right engine pwm
uint16_t sub;
uint16_t val;
void setup() {
  inite();
  adcInit();
  lef = leftSen();
  rig = rightSen();
  mid = middleSen();
  sub = lef + rig + mid;
}

void loop() {
  lef = leftSen();
  rig = rightSen();
  mid = middleSen();
  val = lef + rig + mid;
  if(lef > 800)
  {
    lefSen = true;
  }
  else
  {
    lefSen = false;
  }
  if(mid > 800)
  {
    lefSen = true;
  }
  else
  {
    lefSen = false;
  }
  if(rig > 800)
  {
    lefSen = true;
  }
  else
  {
    lefSen = false;
  }
  if((val - sub) && midSen && lefSen && rigSen)
  {
    ahead();
  }
  else if(midSen && lefSen)
  {
    turnL();
  }
  else if(midSen && rigSen)
  {
    turnR();
  }
  else if(midSen)
  {
    ahead();
  }
  else
  {
    stopEng();
  }
  sub = val;
}
void ahead()
{
  OCR2B = 255;
  OCR2A = 255;
}
void turnL()
{
  OCR2B = 150;
  OCR2A = 255;
}
void turnR()
{
  OCR2B = 255;
  OCR2A = 150;
}
void stopEng()
{
  OCR2B = 0;
  OCR2A = 0;
}
void inite()
{
  //Set Mode of The timer and set pwm mode
  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  TCCR2B &= (~(1 << WGM22));// Fast pwm mode
  //Set pwm mode
  TCCR2A |= (1 << COM2A1);
  TCCR2A &= (~(1 << COM2A0));
  TCCR2A |= (1 << COM2B1);
  TCCR2A &= (~(1 << COM2B0));
  // SET The prescaler for the frequency
  // Clock 16Mhz = 250khz = 64 = 16 mhz / 64 = 250 khz
  TCCR2B |= (1 << CS22);
  TCCR2B &= (~(1 << CS21)) & (~(1 << CS20));
  //Configure the output compare pin as o/p
  DDRB |= (1 << DDB3);
  DDRD |= (1 << DDD3);
}
void adcInit()
{
  // Set reference to AVCC
  ADMUX &= (~(1 << REFS1)); //AVcc with external capacitor at AREF pin
  ADMUX |= (1 << REFS0);
  //Set data alignment int data register
  ADMUX &= (~(1 << ADLAR)); //Right alignment 10 bits results

  // Enable adc
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  // ENABLE ADC  and Set prescaler 128
  // Clock of ADc = 16mhz / 128 = 125 khz
}
uint16_t leftSen()
{ 
  //Clear The channel
  ADMUX &= (~(1 << MUX0)) & (~(1 << MUX1)) & (~(1 << MUX2)) & (~(1 << MUX3));
  //Select The channel 
  ADMUX &= (~(1 << MUX0)) & (~(1 << MUX1)) & (~(1 << MUX2)) & (~(1 << MUX3));
  ADCSRA |= (1 << ADSC);
  while(!(ADCSRA & (1<<ADIF)));
  return ADC;
}
uint16_t rightSen()
{
  //Clear The channel
  ADMUX &= (~(1 << MUX0)) & (~(1 << MUX1)) & (~(1 << MUX2)) & (~(1 << MUX3));
  //Select The channel 
  ADMUX &= (~(1 << MUX0)) & (~(1 << MUX2)) & (~(1 << MUX3));
  ADMUX |= (1 << MUX1);
  ADCSRA |= (1 << ADSC);
  while(!(ADCSRA & (1<<ADIF)) );
  return ADC;

}
uint16_t middleSen()
{
   //Clear The channel
  ADMUX &= (~(1 << MUX0)) & (~(1 << MUX1)) & (~(1 << MUX2)) & (~(1 << MUX3));
  //Select The channel 
  ADMUX &= (~(1 << MUX1)) & (~(1 << MUX2)) & (~(1 << MUX3));
  ADMUX |= (1 << MUX0);
  //start The conversion
  ADCSRA |= (1 << ADSC);
  while(!(ADCSRA & (1<<ADIF)) );
  return ADC;
}
