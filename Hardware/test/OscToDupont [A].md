### Oscilloscope To DuPont ###

|  # | Part                                      | RefDes  |
|---:|-------------------------------------------|---------|
|  2 | J BNC male                                | J1-J2   |
|  1 | J JST XH 2.54mm (4w)                      | J3      |
|  1 | P Dupont 2.54mm (2w)                      | P1      |
|  1 | W Cable                                   |         |

Wires from 4-pin JST XH connector are terminated into 2-pin Dupont connector
as pairs 1-2 and 3-4.


#### Board Size ####

|       |      Dimensions | Area    | Thickness |
|-------|-----------------|---------|-----------|
| PCB   |  28.0 x 23.0 mm | 1.0 in² |    1.6 mm |


### Usage ###

Connect signal generator to `SIG`, spectrum analyzer to `OSC`, and remaining
connector to PCB. Generate 60 kHz square wave and use spectrum analyzer to see
where the dip happens.
