# Arduino_CMPS14_NMEA
Connects via I2C to a CMPS14, outputs NMEA0183 HDM sentences via Serial (38400 baud)

Requires:
An arduino (Uno, nano, etc) does not matter which one
CMPS14 (may work with CMPS10, CMPS12)

Wiring is Ground, 3.3V or 5V, SDA and SCL.

Only outputs the following NMEA sentences:
HDM - Heading Magnetic, including decimal degrees
XDR (Pitch) - whole degrees only
XDR (Roll) - whole degrees only

~10Hz output (delay of 100ms between loop runs)
