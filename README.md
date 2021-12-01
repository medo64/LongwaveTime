[LongwaveTime](https://medo64.com/longwavetime/)
================================================

This USB device will show up on your system as a serial device and allow for
modulation of WWVB and DCF77 time synchronization signal using either onboard
or external antenna.

Actual antenna output stage is a variation on well-known [Radio time station
transmitter by Henner Zeller](https://github.com/hzeller/txtempus) setup which
sends the signal out with a bare minimum of components. Combine that rather
simplistic design with PCB antenna and you have yourself a really localized
device.

Mind you, this is by design as the primary use case for this device is
experimentation and it's not intended to compete with official WWVB or DCF77
signal. Onboard antenna should have enough power to work with watch on top of
it and no more.

Of course, with an external 60 kHz (WWVB) or 75 kHz (DCF77) antenna, signal will
propagate a bit further but you should really only consider this if your home
is not covered with official signal.

---
*You can check my blog and other projects at [www.medo64.com](https://medo64.com/electronics/).*
