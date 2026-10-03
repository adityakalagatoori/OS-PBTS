# Priority-Based Task Execution Using Arduino

Efficient real-time scheduling for mission-critical embedded systems, implemented on an
Arduino Uno. This project demonstrates core Operating Systems concepts — priority
scheduling and preemption — on resource-constrained hardware, using simple, easy-to-read
code.

**Department:** CSE - Cyber Security

**Team:**
- Kalagatori Vaishnav Aditya — CH.SC.U4CYS25012
- Tejpratap Kumar Sah — CH.SC.U4CYS25052

## Problem Statement

The standard Arduino `void loop()` model executes tasks sequentially and treats them
equally. In a system handling a flame sensor, motion sensor, buttons, buzzer, LEDs and an
LCD, this means a safety-critical event (e.g. fire detection) can be delayed behind a
routine LCD refresh — unacceptable latency in a safety-critical application.

## Solution

A single, simple sketch (`firmware/PBTS/PBTS.ino`):
- The flame and motion sensors use hardware interrupts (`attachInterrupt`) so a critical
  event is captured the instant it happens, instead of waiting for a polling loop.
- The main `loop()` checks tasks in priority order — fire first, then motion, then
  button, then the LCD — and only runs a task if nothing higher-priority is pending.
- No extra libraries or abstractions: just flags, `if`/`else if`, and plain functions.

## Priority Levels

| Level | Task | Trigger |
|---|---|---|
| 0 | Fire Alarm | Flame sensor interrupt |
| 1 | Motion | Motion sensor interrupt |
| 2 | Button | Polled digital input |
| 3 | LCD Refresh | Every 1s, only if nothing else pending |

## Hardware

- Arduino Uno (ATmega328P)
- Flame sensor, PIR motion sensor, push button
- Buzzer, 2x status LEDs, 16x2 LCD (HD44780, parallel mode)
- See [docs/wiring.md](docs/wiring.md) for the full pin map and circuit notes.

## Repository Structure

```
firmware/PBTS/PBTS.ino     Arduino sketch (all logic, single file)
docs/wiring.md              Pin mapping and circuit notes
docs/technical_report.md    Full technical report
```

## Build & Flash

1. Open `firmware/PBTS/PBTS.ino` in the Arduino IDE (or `arduino-cli`).
2. Install the built-in `LiquidCrystal` library (bundled with the IDE).
3. Wire the circuit per [docs/wiring.md](docs/wiring.md).
4. Select **Arduino Uno** as the board, select the correct COM port, and upload.
5. Open the Serial Monitor at 9600 baud to observe task logs.

## Results

- Fire-alarm response preempts all other work within microseconds of interrupt trigger.
- Clear priority ordering demonstrated on constrained hardware with simple, readable code.
