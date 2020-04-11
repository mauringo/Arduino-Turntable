// For the UNO et al.
#define PERIOD 250   // 250 cycles = 15.625us for each half of waveform, 32kHz
#define HALF 125     // half the period is the default PWM threshold - gives square wave.
#define MAXAMP 31
int a=999;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
void setup_timers ()
{
  TCCR1A = 0xF2 ;  // phase correct (mode 1010, ICR1 controls period)
  TCCR1B = 0x11 ;  // prescale by 1, change to 0x12 for prescale by 8
  TIMSK1 = 0x01 ;  // overflow interrupt
  TCCR2A = 0x31 ;  // phase correct (mode 101, OCR2A controls period)
  TCCR2B = 0x09 ; // prescale by 1, change to 0x0A for prescale by 8
  
  ICR1  = PERIOD ;    // 31.25us cycle time, 32kHz PWM but 64kHz drive pulses (differential)
  OCR2A = PERIOD ;
  
  OCR1A = HALF-100 ;    // example U drive
  OCR1B = HALF ;
  OCR2B = HALF+100 ;    // example W drive
  
  GTCCR = 0x83 ; // clear and halt prescalers
  TCNT1 = 0xFFFF ;  // synchronize counters exactly.
  TCNT2 = 0 ;
  GTCCR = 0x00 ; // allow prescalers to fly
}


void setup ()
{
  setup_cosines () ;
  setup_timers () ;
   Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode (9, OUTPUT) ; // OC1A pin = U drive
  pinMode (10, OUTPUT) ; // OC1B pin = V drive
  pinMode (3, OUTPUT) ; // OC2B pin = W drive
}

volatile byte u = HALF ;
volatile byte v = HALF ;
volatile byte w = HALF ;


ISR (TIMER1_OVF_vect)   // overflow triggers at BOTTOM, update all the compare regs, which are sampled at TOP
{
  OCR1A = u ;
  OCR1B = v ;
  OCR2B = w ;
}

#define  DEG_360  0x200
#define  DEG_120  0x0AB

char cosine_tab [DEG_360+1] ;  // fails to work if less than 0x202.

void setup_cosines ()
{
  for (int i = 0 ; i < 0x202 ; i++)
  {
    float a = i * 6.283185 / 0x200 ;
    if (i <= DEG_360)
      cosine_tab [i] = round (127.0 * cos (a)) ;
  }
}

unsigned int phase = 0 ;
int freq  = 1 ;
int amplitude = MAXAMP ;


int my_cosine (int ph)
{
  ph &= DEG_360-1 ;
  int res = cosine_tab [ph] ;
  res *= amplitude ;
  return (res + 0x0F) >> 5 ;
}

void loop ()
{
  phase += freq ;
  int newu = my_cosine (phase) ;
  int newv = my_cosine (phase - DEG_120) ;
  int neww = - newu - newv ;
  newu += HALF ;
  newv += HALF ;
  neww += HALF ;
  u = newu ;  // no masking of interrupts needed as u,v,w variables are single byte each.
  v = newv ;
  w = neww ;
  delayMicroseconds (a) ;
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
   a=inputString.toInt();
   inputString="";
   inChar='1';
   return;
    }
  }
  

}  


