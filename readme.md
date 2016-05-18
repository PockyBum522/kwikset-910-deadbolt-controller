Based on https://github.com/mogorman/closed_sesame
Seriously. This was so useful.

Wemos or ESP8266 board serial interface to Kwikset 909 or 910

Lock daughterboard pinouts (If you're looking at the lock from the indoor side, not the outside of the door) are:
1: RST  2: SDA  3: SDB  4: SCL  5: SCB  6: TXD  7: GND  8: RXD 9: GND   10: BSL 11: INT 12: CTI 13: ?? 14: ??

You want the Wemos softwareserial pins going to GND and RXD (Lock RX.) Lock TX should not be connected to Wemos RX

Have fun!