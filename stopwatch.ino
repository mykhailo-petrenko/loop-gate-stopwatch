/**
 * Loop Gate Stopwatch. v.1.0.1
 *
 * Measures one lap.
 *
 * GL es 73, UR3AMP & PD5MI
 */

#define GATE_SENSOR A0
#define SCREEN_I2C_ADDRESS 0x27

#define OPTION_SENSOR_THRESHOLD_DOWN 800
#define OPTION_SENSOR_THRESHOLD_UP  850

#define REACTION_DEBOUNCE 250


/*Simple LCD stopwatch program with stop, start, reset and lap buttons.*/
#include "LiquidCrystal_I2C.h"

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(SCREEN_I2C_ADDRESS, 16, 2);

#define STATE_IDLE  0
#define STATE_RUN   1
#define STATE_STOP  2
#define STATE_RESET 3

int main_state = STATE_IDLE;
int sensorValue = 1000;


//setting hours, minutes, secound  and miliseconds to 0
unsigned int h=0;
unsigned int m=0;
unsigned int s=0;
unsigned int ms=0;

unsigned long start_ms = 0;
unsigned long now_ms = 0;
unsigned long state_changed_ms = 0;

char line0[32];
char line1[32];


unsigned long diff;

void updateTime()
{
  now_ms = millis();

  diff = now_ms - start_ms;
  s = diff / 1000;
  m = s / 60;
  ms = diff % 1000;
  s %= 60;
  m %= 60;
}

void printTime()
{
  sprintf(line0, "LAP: %02d:%02d:%03d", m, s, ms);
  lcd.setCursor(0,0);
  lcd.print(line0);
}


void stopwatch()
{
  updateTime();
  printTime();
}

void stopwatch_idle()
{
  diff = millis() / 1000;

  printTime();
  lcd.setCursor(0,1);

  if (diff % 2 == 0) {
    lcd.print("  - READY -  ");
  } else {
    lcd.print(" -  READY  - ");
  }
}

void stopwatch_stop()
{
  printTime();
  lcd.setCursor(0,1);
  lcd.print(" -- STOP -- ");
}

void stopwatch_reset()
{
  lcd.clear();

  // seting hours to 0
  h = 0;
  // seting minutes to 0
  m = 0;
  // seting seconds to 0
  s = 0;

  ms = 0;

  printTime();
}




void setup()
{
  lcd.init();
  delay(50);
  lcd.backlight();
  start_ms = millis();
}

// OPTION_DEBOUNCE
int prev_reading = HIGH;

void loop()
{
  sensorValue = analogRead(GATE_SENSOR);

  switch (main_state) {
    case STATE_IDLE:
      stopwatch_idle();

      if ((millis() - state_changed_ms) < REACTION_DEBOUNCE)
      {
        break;
      }

      if (prev_reading == LOW)
      {
        if (sensorValue > OPTION_SENSOR_THRESHOLD_UP)
        {
          prev_reading = HIGH;
        }
      } else if (sensorValue < OPTION_SENSOR_THRESHOLD_DOWN)
      {
        main_state = STATE_RUN;
        lcd.clear();
        state_changed_ms = millis();
        start_ms = millis();
        prev_reading = LOW;
      }

      break;
    case STATE_RUN:
      stopwatch();

      if ((millis() - state_changed_ms) < REACTION_DEBOUNCE)
      {
        break;
      }

      if (prev_reading == LOW)
      {
        if (sensorValue > OPTION_SENSOR_THRESHOLD_UP)
        {
          prev_reading = HIGH;
        }
      } else if (sensorValue < OPTION_SENSOR_THRESHOLD_DOWN)
      {
        main_state = STATE_STOP;
        state_changed_ms = millis();
        prev_reading = LOW;
      }


      break;
    case STATE_STOP:
      stopwatch_stop();

      delay(250);
      break;
    case STATE_RESET:
      stopwatch_reset();

      delay(50);
      break;
  }

  delay(10);
}
