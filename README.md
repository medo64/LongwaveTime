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


### Legality of WWVB transmission ###

Transmitting on unlicensed frequencies is quite often an issue and should be
taken seriously. In USA legality of the same is coverted in [Understanding the FCC Regulations for Low-Power Non-Licensed Transmitters](https://transition.fcc.gov/Bureaus/Engineering_Technology/Documents/bulletins/oet63/oet63rev.pdf).
Short version is that you can do so as long as you keep it under 40 μV/m
measured at 300 meters. This device with its internal antenna is going to be
way under this limit.

If you plan to use it with external antenna, things get a bit trickier. I like
to use NIST's [Estimated Seasonal Signal Strength of WWVB, µV/m](https://www.nist.gov/system/files/documents/calibrations/sp432-02.pdf)
as a rule-of-the-thumb reference and as long as your watch synchronization
works only up to 10 meteres or so, you will be way under 40 µV/m as required
by FCC's rules and really unlikely to be causing any interference.

But, regardless of the rules, under no circumstances should you interfere with
official WWVB signal. This might not be encoded in rules, but it should be a
common sense.

As far as legality in other countries goes, do your own research.

---
*You can check my blog and other projects at [www.medo64.com](https://medo64.com/electronics/).*
