#include <SPI.h>
#include <WiFi.h>

#define DIME 2  // Serial Port 2
#define PENNY 3  // Serial Port 3
#define NICKEL 4  // Serial Port 4
#define QUARTER 5  // Serial Port 5
#define LOONIE 6 // Serial Port 6
#define TOONIE 7  // Serial Port 7

#define DEFAULT_LONGPRESS_LEN    25  // Min nr of loops for a long press
#define DELAY                    20  // Delay per loop in ms

char ssid[] = "yourNetwork";      //  your network SSID (name)
char pass[] = "secretPassword";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiClient client;
char server[] = "www.arduino.cc";
unsigned long lastConnectionTime = 0;            // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds
//////////////////////////////////////////////////////////////////////////////
enum { EV_NONE=0, EV_SHORTPRESS, EV_LONGPRESS };
//////////////////////////////////////////////////////////////////////////////
// Class definition
class ButtonHandler {
  public:
    // Constructor
    ButtonHandler(int pin, int longpress_len=DEFAULT_LONGPRESS_LEN);
    // Initialization done after construction, to permit static instances
    void init();
    // Handler, to be called in the loop()
    int handle();
  protected:
    boolean was_pressed;     // previous state
    int pressed_counter;     // press running duration
    const int pin;           // pin to which button is connected
    const int longpress_len; // longpress duration
};
ButtonHandler::ButtonHandler(int p, int lp)
: pin(p), longpress_len(lp)
{
}
void ButtonHandler::init()
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH); // pull-up
  was_pressed = false;
  pressed_counter = 0;
}
int ButtonHandler::handle()
{
  int event;
  int now_pressed = !digitalRead(pin);
  if (!now_pressed && was_pressed) {
    // handle release event
    if (pressed_counter < longpress_len)
      event = EV_SHORTPRESS;
    else
      event = EV_LONGPRESS;
  }
  else
    event = EV_NONE;
  // update press running duration
  if (now_pressed)
    ++pressed_counter;
  else
    pressed_counter = 0;
  // remember state, and we're done
  was_pressed = now_pressed;
  return event;
}

//////////////////////////////////////////////////////////////////////////////

// Instantiate button objects
ButtonHandler button1(DIME);
ButtonHandler button2(PENNY);
ButtonHandler button3(NICKEL);
ButtonHandler button4(QUARTER);
ButtonHandler button5(LOONIE);
ButtonHandler button6(TOONIE);

void setup()
{
  Serial.begin(9600);

  // init buttons pins; I suppose it's best to do here
  button1.init();
  button2.init();
  button3.init();
  button4.init();
  button5.init();
  button6.init();
}

void print_event(const char* button_name, int event)
{ 
  if (event)
    Serial.print(button_name);
    Serial.print(".SL"[event]);
}

void loop()
{
  // handle button
  int event1 = button1.handle();
  int event2 = button2.handle();
  int event3 = button3.handle();
  int event4 = button4.handle();
  int event5 = button5.handle();
  int event6 = button6.handle();

  // do other things
  print_event("dime", event1);
  print_event("penny", event2);
  print_event("nickel", event3);
  print_event("quarter", event4);
  print_event("loonie", event5);
  print_event("toonie", event6);

  // add newline sometimes
  static int counter = 0;
  if ((++counter & 0x1f) == 0)
    Serial.println();

  delay(DELAY);
}
