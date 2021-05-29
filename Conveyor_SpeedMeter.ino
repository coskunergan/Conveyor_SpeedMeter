/*
    Conveyor Speed Meter Unit ( meter / minute )

    Created on: May 29, 2021

    Author: Coskun ERGAN

    V-1.0
*/
#define TIMEOUT_PERCENT   50
#define CALIBRATION       7.00f
#define MAGNET_POLARITY   4
#define DEBOUNCE_MS       100
//----------------------
const byte interruptPin = 2;
float MeterPerMinute;
//----------------------
volatile uint8_t TimeIndex;
volatile uint16_t Speed;
volatile uint16_t TimeoutMs;
volatile uint32_t TimerMs;
volatile uint32_t TimeLogs[MAGNET_POLARITY];
/***********************************************/
void Timer_Init(void)
{
  //timer1 1Hz' e ayarlanıyor
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  //OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (değer 65536 dan küçük)
  OCR1A = 15;//~1mS
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
  attachInterrupt(digitalPinToInterrupt(interruptPin), IntPin_ISR, FALLING);
  Serial.begin(9600);
  Timer_Init();
}
/***********************************************/
/***********************************************/
/***********************************************/
void loop()
{
  if (Speed)
  {
    MeterPerMinute = 100000 / (Speed * CALIBRATION);
  }
  else
  {
    MeterPerMinute = 0;
  }
  Serial.println(MeterPerMinute);
  delay(1000);
}
/***********************************************/
void IntPin_ISR()
{
  uint32_t temp;
  static uint32_t last_time = 0;
  if (TimerMs > (last_time + DEBOUNCE_MS))
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    TimeIndex++;
    TimeIndex %= MAGNET_POLARITY;
    if (TimeLogs[TimeIndex])
    {
      Speed = (TimerMs - TimeLogs[TimeIndex]);
    }
    else
    {
      temp = TimeLogs[(TimeIndex - 1) % MAGNET_POLARITY];
      if (temp)
      {
        Speed = (TimerMs - temp) * MAGNET_POLARITY;
      }
    }
    TimeLogs[TimeIndex] = TimerMs;
    if (Speed)
    {
      TimeoutMs = (Speed / MAGNET_POLARITY);
      TimeoutMs += ((TimeoutMs * TIMEOUT_PERCENT) / 100);
    }
  }
  last_time = TimerMs;
}
/***********************************************/
ISR(TIMER1_COMPA_vect)
{
  TimerMs++;
  if (TimeoutMs)
  {
    if (--TimeoutMs == 0)
    {
      TimerMs = 0;
      Speed = 0;
      for (int i = 0; i < MAGNET_POLARITY; i++)
      {
        TimeLogs[i] = 0;
      }
    }
  }
}
/***********************************************/
/***********************************************/
/***********************************************/
