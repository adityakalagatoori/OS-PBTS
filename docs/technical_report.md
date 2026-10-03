# Technical Report: Priority-Based Task Execution Using Arduino

**Department:** CSE — Cyber Security

**Team Members:**
- Kalagatori Vaishnav Aditya — CH.SC.U4CYS25012
- Tejpratap Kumar Sah — CH.SC.U4CYS25052

---

## 1. Abstract

This project implements priority-based task execution on an Arduino Uno to demonstrate
core Operating Systems concepts — priority scheduling and preemption — in a
resource-constrained embedded environment, using a single, simple sketch. The system
replaces the conventional sequential `void loop()` execution model with priority-ordered
checks that guarantee safety-critical tasks (fire detection) run ahead of routine tasks
(LCD updates).

## 2. Problem Statement

Standard Arduino sketches execute all logic sequentially inside `loop()`, treating every
operation with equal urgency. In a multi-sensor, multi-actuator system — sensor reads,
buzzer control, display updates — this causes critical tasks to be delayed behind routine
processing. In safety-critical applications (e.g. an overheating alarm or a braking
signal), such delay can cause unacceptable latency and potential hardware or safety
failure.

## 3. Objectives

1. Ensure the highest-urgency task always runs first, using the simplest code that
   achieves this on a resource-constrained microcontroller.
2. Reduce latency: ensure critical safety tasks respond within microseconds of trigger
   detection, using hardware interrupts rather than polling.
3. Keep the implementation easy to read and verify: no custom libraries, classes, or
   generic abstractions — just flags and priority-ordered `if`/`else if` checks.

## 4. System Architecture

```
Sensor / Input -> Interrupt / Poll -> Priority Check (loop) -> Task Function -> Output Devices
```

- **Sensor/Input**: Flame sensor, PIR motion sensor, push button.
- **Interrupt/Poll**: Flame and motion sensors trigger hardware interrupts (ISRs) that set
  a flag; the button is polled directly each loop.
- **Priority Check**: `loop()` checks flags in strict priority order (fire, then motion,
  then button, then LCD) and runs the first one that is ready.
- **Task Function**: A plain function (e.g. `runFireTask()`) performs the actual work.
- **Output Devices**: Buzzer, LEDs, 16x2 LCD.

## 5. Hardware Components

| Component | Role |
|---|---|
| Arduino Uno (ATmega328P) | Microcontroller |
| Flame sensor | Highest-priority interrupt source (fire detection) |
| PIR motion sensor | Second-priority interrupt source |
| Push button | User input, polled |
| Buzzer | Alarm output |
| LEDs (x2) | Fire / motion status indicators |
| 16x2 LCD (HD44780) | Routine status display, lowest priority |

Full pin mapping is documented in `docs/wiring.md`.

## 6. Operating System Concepts Applied

- **Priority Scheduling**: Tasks have a fixed priority order (fire > motion > button >
  LCD). `loop()` always checks and runs the highest-priority ready task first, which is
  the essence of priority scheduling.
- **Preemption via Interrupts**: The flame and motion sensors use `attachInterrupt()`, so
  a critical event is captured by the CPU immediately, independent of what the main loop
  is currently doing — the flag is set the instant the event occurs, and is acted on as
  soon as the current task function returns.
- **Polling vs. Event-Driven Tasks**: The button is handled by polling (simple, low
  priority, no latency requirement), while fire/motion use interrupts (urgent, latency
  -sensitive) — illustrating the trade-off an OS designer makes between polling and
  interrupt-driven I/O.

## 7. Execution Methodology

1. **Setup**: Pins are configured, the LCD is initialized, and interrupts are attached to
   the flame and motion sensor pins.
2. **Event Capture**: An ISR only sets a `volatile bool` flag (e.g. `fireReady = true`) —
   kept intentionally minimal so the interrupt returns quickly.
3. **Priority Check**: On every pass of `loop()`, flags are checked in order of priority:
   fire first, motion second, button third, LCD last.
4. **Task Execution**: The first ready task's function runs to completion before the loop
   re-checks priorities, so higher-priority events are never missed, only briefly
   deferred.

## 8. Software Design

- `firmware/PBTS/PBTS.ino` — single-file sketch: pin setup, interrupt handlers, task
  functions, and the priority-ordered main loop.
- No external scheduler library or custom data structures are used — priority is
  expressed directly as the order of `if`/`else if` checks, keeping the code simple and
  easy to verify.

## 9. Results

- Fire-alarm response preempts all other work: verified via Serial Monitor logs showing
  `[P0] Fire alarm` entries ahead of pending `[P3] LCD refresh` entries during sustained
  motion/button activity.
- The LCD task only ever runs when no higher-priority task is pending, confirming correct
  priority ordering.
- The implementation fits comfortably within the ATmega328P's resources (2KB SRAM, 16MHz
  clock) with no additional libraries beyond `LiquidCrystal`.

## 10. Applications

- **Home Automation**: Prioritizing gas-leak/security alerts over routine lighting and
  display updates.
- **Robotic Control**: Guaranteeing collision-avoidance and braking routines preempt
  telemetry logging.
- **Medical Systems**: Ensuring vital-sign monitors and patient alerts are processed
  without queue lag.

## 11. Conclusion

The project demonstrates that the core idea behind priority scheduling — always running
the most urgent ready task first — can be implemented on a minimal microcontroller with
very simple code: hardware interrupts for latency-sensitive events, and a priority-ordered
sequence of checks in the main loop. This approach is directly applicable to
safety-critical embedded systems where response latency cannot be compromised by routine
processing.

## 12. Future Work

- Add more priority levels and tasks as the application grows.
- Add a safeguard so a low-priority task (e.g. LCD) is guaranteed to run periodically even
  under continuous high-priority activity.
- Explore a full RTOS (e.g. FreeRTOS for AVR) for applications needing true multitasking.
