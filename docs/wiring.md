# Wiring / Pin Map

| Component | Arduino Pin | Notes |
|---|---|---|
| Flame sensor (digital out) | D2 | INT0, interrupt on RISING edge |
| Motion sensor (PIR, digital out) | D3 | INT1, interrupt on RISING edge |
| Push button | D4 | `INPUT_PULLUP`, active LOW |
| Buzzer | D8 | Driven via `tone()` |
| Fire LED | D9 | Active HIGH |
| Motion LED | D10 | Active HIGH |
| LCD RS | D12 | |
| LCD E | D11 | |
| LCD D4 | D5 | |
| LCD D5 | D6 | |
| LCD D6 | D7 | |
| LCD D7 | D13 | |
| LCD VSS, RW, K | GND | |
| LCD VDD, A | 5V (A via ~220Ω resistor) | |
| LCD V0 | Potentiometer wiper (contrast) | |

## Power

- Arduino Uno powered via USB (development) or external 9V supply (deployment).
- All sensor/peripheral grounds common with Arduino GND.

## Notes

- D2 and D3 are used for the flame and motion sensors specifically because they are the
  only pins on the Uno capable of external interrupts (`INT0`/`INT1`), which is required
  for true preemptive triggering independent of the main scheduling loop.
- A 220Ω series resistor is recommended on the buzzer and LEDs to limit current.
