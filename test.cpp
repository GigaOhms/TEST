#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define REF_VOLTAGE 1100.0 // internal bandgap reference voltage, in millivolts
#define ACS758_SENS 40     // ACS758 sensitivity = 40mV/A (for ACS758-200B version)

#define CUR_SEL 2 // current type select pushbutton pin
// define current type
#define AC 0    // AC current
#define DC 1    // DC current
#define AC_DC 2 // AC+DC current (AC current with DC offset)
// variables
byte current_type = AC; // current type according to previous 3 definitions
const uint16_t n = 256; // number of samples
float _array[n];        // sample array with 'n' elements
float dc_offset;        // auto-calibration dc offset

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup(void)
{
  lcd.init(); // initialize the lcd
  lcd.begin(16, 2);
  lcd.backlight();
  Serial.begin(9600);
  pinMode(CUR_SEL, INPUT_PULLUP);
  //  lcd.begin(16, 2);         // set up the LCD's number of columns and rows
  // ADC configuration
  ADMUX |= (1 << REFS0); // set ADC +ive vlotage reference to AVCC
  // ADC auto triggering is enabled, ADC clock division factor set to 128 (ADC clock = 125kHz)
  ADCSRA |= (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // do auto-calibration to the circuit (no current should flow through the ACS758 sensor)
  ADMUX |= (1 << MUX1); // select analog channel 2 (A2) as input to ADC
  dc_offset = 0;
  get_smaples();
  for (uint16_t j = 0; j < n; j++)
    dc_offset += _array[j];
  dc_offset /= n;

  attachInterrupt(digitalPinToInterrupt(CUR_SEL), current_type_set, FALLING); // interrupt enable
}

volatile bool button = 0; // variable for CUR_SEL button press
// set current type: AC, DC or AC+DC
void current_type_set()
{
  button = 1;
  detachInterrupt(digitalPinToInterrupt(CUR_SEL)); // interrupt disable
  loop();
}

// button debounce function
bool debounce()
{
  byte count = 0;
  for (byte i = 0; i < 5; i++)
  {
    if (!digitalRead(CUR_SEL))
      count++;
    delay(10);
  }

  if (count > 2)
    return 1;
  else
    return 0;
}

// analog data acquisition function
void get_smaples()
{
  // clear sample array
  for (uint16_t i = 0; i < n; i++)
    _array[i] = 0.0;
  ADCSRA |= (1 << ADEN) | (1 << ADSC); // enable ADC module and start converion (free running mode)
  // ignore the first reading
  while ((ADCSRA & (1 << ADIF)) == 0)
    ;              // wait for ADIF to be set (conversion complete)
  ADCSRA = ADCSRA; // reset ADIF flag bit
  // fill samples array with 12-bit data (add another 2 bits using oversampling technique)
  for (uint16_t i = 0; i < n; i++)
  {
    for (uint16_t j = 0; j < 16; j++)
    {
      while ((ADCSRA & (1 << ADIF)) == 0)
        ;                    // wait for ADIF to be set (conversion complete)
      ADCSRA |= (1 << ADIF); // reset ADIF bit by writing 1 to it
      _array[i] += ADCW;
    }
  }
  ADCSRA &= ~((1 << ADEN) | (1 << ADSC)); // stop conversion and disable ADC module, reset ADIF bit also
  for (uint16_t i = 0; i < n; i++)
    _array[i] /= 4.0;
}

// main loop function
void loop()
{

  if (button)
  { // if current type button is pressed
    button = 0;
    if (debounce())
    {
      current_type++;
      if (current_type > 2)
        current_type = 0;
    }
    attachInterrupt(digitalPinToInterrupt(CUR_SEL), current_type_set, FALLING); // interrupt enable
    return;
  }

  // get digital representation of the internal bangap reference voltage
  float bgref_voltage = 0;                          // Arduino internal reference voltage variable
  ADMUX |= (1 << MUX3) | (1 << MUX2) | (1 << MUX1); // select internal bandgap refernce as input to ADC
  get_smaples();                                    // get samples
  for (uint16_t i = 0; i < n; i++)
    bgref_voltage += _array[i];
  bgref_voltage /= n; // average value

  // read ACS758 output voltage
  float acs758_voltage = 0;              // ACS758 sensor output voltage variable
  ADMUX &= ~((1 << MUX3) | (1 << MUX2)); // select analog channel 2 (A2) as input to ADC
  get_smaples();

  if (current_type == AC || current_type == AC_DC)
  { // AC or AC+DC type
    float _offset = 0;
    if (current_type == AC)
    {                                  // AC signal
      for (uint16_t i = 0; i < n; i++) // caculate signal average value (dc offset)
        _offset += _array[i];
      _offset = _offset / n;
    }
    else                   // AC+DC signal
      _offset = dc_offset; // the dc offset is the pre-calibrated one

    // calculate signal RMS value (digital representation)
    for (uint16_t i = 0; i < n; i++)
      acs758_voltage += sq(_array[i] - _offset);
    acs758_voltage = acs758_voltage / n;
    acs758_voltage = sqrt(acs758_voltage);
  }

  else
  { // DC type
    for (uint16_t i = 0; i < n; i++)
      acs758_voltage += _array[i] - dc_offset; // remove the pre-calibrated DC offset
    acs758_voltage /= n;                       // average value
  }

  // calculate actual ACS758 sensor output voltage
  acs758_voltage = acs758_voltage * REF_VOLTAGE / bgref_voltage;

  // now we can calculate current passing through the ACS758 sensor (in amps)
  float acs758_current = acs758_voltage / ACS758_SENS;

  // print data on the LCD and serial monitor
  //  lcd.setCursor(0, 0);
  if (current_type == AC)
  {
    lcd.clear();
    lcd.print("AC Current =    ");
    Serial.print("AC Current = ");
  }
  else if (current_type == DC)
  {
    lcd.clear();
    lcd.print("DC Current =    ");
    Serial.print("DC Current = ");
  }
  else
  {
    lcd.clear();
    lcd.print("AC+DC Current = ");
    Serial.print("AC+DC Current = ");
  }

  //  lcd.setCursor(0, 1);
  if (acs758_current < -0.01)
  {
    lcd.clear();
    lcd.print('-');
    Serial.print('-');
  }

  acs758_current = abs(acs758_current);
  char _buffer[9];
  if (acs758_current < 10.0) // if current < 10.0 Amps
    sprintf(_buffer, "%1u.%02u A  ", (uint8_t)acs758_current, (uint8_t)(acs758_current * 100) % 100);
  else if (acs758_current < 100.0) // if 10.0 <= current < 100.0 Amps
    sprintf(_buffer, "%2u.%02u A ", (uint8_t)acs758_current, (uint8_t)(acs758_current * 100) % 100);
  else // current is >= 100.0 Amps
    sprintf(_buffer, "%3u.%02u A", (uint8_t)acs758_current, (uint8_t)(acs758_current * 100) % 100);

  lcd.setCursor(0, 1);
  lcd.print(_buffer);
  Serial.println(_buffer);
  Serial.println();
}
// end of code.
