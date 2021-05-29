/*
    Conveyor Speed Meter Unit ( meter / second )

    Created on: May 29, 2021

    Author: Coskun ERGAN

    V-1.0
*/
/***********************************************/
float Speed;
const byte interruptPin = 2;
/***********************************************/
void Timer_Init(void)
{
  //timer1 1Hz' e ayarlanıyor
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (değer 65536 dan küçük)
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();
}
/***********************************************/
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), IntPin_ISR, CHANGE);
  Serial.begin(9600);
  Timer_Init();
}
/***********************************************/
/***********************************************/
/***********************************************/
void loop()
{
  Speed = 1;

  Serial.println(Speed);

  delay(1000);// wait for a second

}
/***********************************************/
void IntPin_ISR()
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
/***********************************************/
ISR(TIMER1_COMPA_vect)
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
/***********************************************/
/***********************************************/
/***********************************************/
