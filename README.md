# Loop Gate Stopwatch (Arduino DIY)

Gate automatic stopwatch has one goal:

    To provitte a reliable and accurate timing system 
    for evaluating the performance of line-following cars 
    in completing a predefined track.

## State machine

- **Idle**. Waits until car crosses the laser beam.
- **Run**. Stopwatch counts time (up to ms) and shows it on the screen.
- **Stop**. Display time (stopwatch stops) when a car crosses laser bean second time.

## Components

- Arduino UNO
- Laser module connected to +3V and GND.
- LCD Screen connected via I2C.
- Photo resistor connected as a divider with resistor (10k) to A0 (analog input).

## Prototype

![Gate Start](doc/IMG_8711.png)

![Gate Stop](doc/IMG_8713.png)
