/*
 * Generic pin interface and subclasses for objects attached to them
 *
 * Written by Adam Phelps (amp@cs.stanford.edu)
 */

#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#include "Shift.h"

/* Pin subtypes */
typedef enum 
{
  PIN_TYPE_NONE,
  PIN_TYPE_SENSOR,
  PIN_TYPE_OUTPUT
} pin_type_t;
  
/*
 * Generic pin object
 */
class Pin 
{
  public:
  Pin(byte _pin, boolean _analog, pin_type_t _type);
  Pin(byte _pin, boolean _analog);

  pin_type_t type;
  boolean analog;

  protected:
  byte pin;
};

typedef void (*pin_action_t)(int pin, int value, void *arg);

/*
 * Single-pin sensor class
 */
#define DEFAULT_DEBOUNCE_DELAY 50
class Sensor : public Pin
{
public:
  Sensor(byte _pin, boolean _pull_up, boolean _analog,
         pin_action_t _action);
  Sensor(byte _pin, boolean _pull_up, boolean _analog,
         pin_action_t _action, void *_action_arg);
  Sensor(byte _pin, boolean _pull_up, boolean _analog,
         boolean _reversed, pin_action_t _action, void *_action_arg);

  int read(void);
  int debouncedRead(void);

  pin_action_t action;
  void *action_arg;

  unsigned int curr_state;

private:
  void init(boolean _pull_up, boolean _reversed,
            pin_action_t _action, void *_action_arg);

  boolean pull_up;
  boolean reversed;

  unsigned int prev_state;

  unsigned long debounce_time;
  unsigned long debounce_delay;
};

/* Read the status of all pins in the array and perform actions if indicated */
boolean checkSensors(Pin **pins, byte num_pins, boolean debounce);

/*
 * Single-pin output class
 */
class Output : public Pin
{
public:
  Output(byte pin, byte value);
  Output(byte pin, byte value, Shift *shift);
  Output(byte pin, byte value, Shift *shift, Sensor *sensor);

  void setValue(byte value);
  void trigger(void);

private:
  void init(byte pin, byte value, Shift *shift, Sensor *sensor);
  byte _value;
  byte _next_value;
  Shift *_shift;
  Sensor *_sensor;
};

void triggerOutputs(Pin **pins, byte num_pins);



/*
 * Generic pin actions
 */

/* Set the provided Output to the indicated value */
void action_set_output(int pin, int value, void *arg);

/* Print the value of a pin */
void action_print_value(int pin, int value, void *arg);

#endif
