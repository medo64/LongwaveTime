### Longwave Time Protocol ###

This device presents itself as a serial port. Serial port parameters are
115200,N,8,1.

For purpose of protocol `<CR>` and `<LF>` end of line characters can be used
interchangeably. Each response will end with `<CR><LF>`. If command has no
arguments, it will return the current status.

Commands that resulted in error will always end in `!` `<BEL>` `<CR>` `<LF>`.


#### Set Protocol (P) #####

This command will set the protocol.

|          | Text                           |
|----------|--------------------------------|
| Syntax   | `P{1:protocol}<LF>`            |
| Response | `P{1:protocol}<CR><LF>`        |
| Reject   | `P!<BEL><CR><LF>`              |


The following protocol values are allowed:

| Value | Protocol |  Carrier |    Frequency |
|:-----:|----------|---------:|-------------:|
|   0   | None     |        - |            - |
|   W   | WWVB     | 60.0 kHz | 60,000.00 Hz |
|   D   | DCF77    | 77.5 kHz | 77,419.35 Hz |
|   M   | MSF      | 60.0 kHz | 60,000.00 Hz |
|   4   | JJY      | 40.0 kHz | 40,000.00 Hz |
|   6   | JJY      | 60.0 kHz | 60,000.00 Hz |

Please note that 77.5 kHz cannot be correctly represented by device without a
small carrier deviation. This does not impact auto-incrementing time but only
the carrier. All watches I tested with were able to tolerate this.


Here are some usage examples:

| Example            | Send               | Receive            | Notes                             |
|--------------------|--------------------|--------------------|-----------------------------------|
| Query              | `P<LF>`            | `PW<CR><LF>`       | Using WWVB protocol               |
| Set WWVB           | `PW<LF>`           | `PW<CR><LF>`       | Set protocol to WWVB              |
| Set DCF77          | `PD<LF>`           | `PD<CR><LF>`       | Set protocol to DCF77             |
| Turn off           | `P0<LF>`           | `P0<CR><LF>`       | No carrier or time-keeping        |
| Unknown protocol   | `Px<LF>`           | `F!<BEL><CR><LF>`  |                                   |


#### Raw Data (R) #####

Sets bit data. Data is always set for the buffer not in use.

Buffers are called X and Y.

|          | Text                           | Notes                                               |
|----------|--------------------------------|-----------------------------------------------------|
| Syntax   | `R{59-61:data}<LF>`            |                                                     |
| Response | `R{2:buffer}<CR><LF>`          | Current buffer, followed by buffer user for command |
| Reject   | `R!<BEL><CR><LF>`              |                                                     |

If one sets 59 or 61 bit, leap second will be assumed.

Bits sets using this command superseed data set by any other command.



The following bits are supported for [WWVB](https://en.wikipedia.org/wiki/WWVB):

| Data  | Value        | Description                                   |
|:-----:|--------------|-----------------------------------------------|
|   0   | Bit 0        | 200 ms reduced power, 800 ms full power       |
|   1   | Bit 1        | 500 ms full power, 500 ms reduced power       |
|   M   | Marker       | 800 ms reduced power, 200 ms full power       |

The following bits are supported for [DCF77](https://en.wikipedia.org/wiki/DCF77):

| Data  | Value        | Description                                   |
|:-----:|--------------|-----------------------------------------------|
|   0   | Bit 0        | 100 ms reduced power, 900 ms full power       |
|   1   | Bit 1        | 200 ms reduced power, 800 ms full power       |
|   M   | Minute mark  | no amplitude modulation                       |

The following bits are supported for [MSF](https://en.wikipedia.org/wiki/Time_from_NPL_(MSF)):

| Data  | Value        | Description                                   |
|:-----:|--------------|-----------------------------------------------|
|   0   | Bits 00      | Carrier is encoded as follows _~~~~~~~~~      |
|   1   | Bits 01      | Carrier is encoded as follows _~_~~~~~~~      |
|   2   | Bits 10      | Carrier is encoded as follows __~~~~~~~~      |
|   3   | Bits 11      | Carrier is encoded as follows ___~~~~~~~      |
|   M   | Minute mark  | Carrier off for 500 ms, carrier on for 500 ms |

The following bits are supported for [JJY](https://en.wikipedia.org/wiki/JJY):

| Data  | Value        | Description                                   |
|:-----:|--------------|-----------------------------------------------|
|   0   | Bit 0        | 800 ms full power, 200 ms reduced power       |
|   1   | Bit 1        | 500 ms full power, 500 ms reduced power       |
|   M   | Marker       | 200 ms full power, 800 ms reduced power       |


Here are some usage examples:

| Example            | Send               | Receive            | Notes                                                            |
|--------------------|--------------------|--------------------|------------------------------------------------------------------|
| Query              | `R<LF>`            | `RYX<CR><LF>`      | Buffer Y is currently in use and X is ready                      |
| Query              | `R<LF>`            | `RY~<CR><LF>`      | Buffer Y is currently in use and no new buffer is ready          |
| Set                | `R123...<LF>`      | `R~X<CR><LF>`      | New data was written in buffer X and there was no current buffer |
| Not enough bits    | `R011<LF>`         | `R!<BEL><CR><LF>`  | Must have 59-61 bits (60 usually)                                |
| Unknown protocol   | `Rx<LF>`           | `R!<BEL><CR><LF>`  |                                                                  |


#### Start (S) #####

This command will synchronize time and start sending data if one is not already
sent. Input can be given in either seconds or tenths of second while output is
always in tenths of second.

Buffer is always switched either when `S` is called or when a full minute has
elapsed.

|          | Text                           |
|----------|--------------------------------|
| Syntax   | `S{2-3:second}<LF>`            |
| Response | `S{3:second}<CR><LF>`          |
| Reject   | `S!<BEL><CR><LF>`              |

Second can be any number from 00-60.


Here are some usage examples:

| Example            | Send               | Receive            | Notes                                     |
|--------------------|--------------------|--------------------|-------------------------------------------|
| Query              | `S<LF>`            | `S103<CR><LF>`     | Returns current tenth of a second (10.2s) |
| Query              | `S<LF>`            | `S---<CR><LF>`     | Returning `-` when not active             |
| Sync time          | `S42<LF>`          | `S420<CR><LF>`     | Syncs device to second 42                 |
| Unknown second     | `S61<LF>`          | `S!<BEL><CR><LF>`  |                                           |


##### Version (V) #####

Returns software version.

|          | Text                                                            |
|----------|-----------------------------------------------------------------|
| Syntax   | `V<LF>`                                                         |
| Response | `V{2:major}.{2:minor}<CR><LF>`                                  |
| Reject   | `V!<BEL><CR><LF>`                                               |

Here are some usage examples:

| Example            | Send               | Receive            | Notes                             |
|--------------------|--------------------|--------------------|-----------------------------------|
| Query              | `V<LF>`            | `V2112<CR><LF>`    | Version is 21.12                  |
| Unknown parameters | `Vx<LF>`           | `V!<BEL><CR><LF>`  |                                   |



### Example Flow ###

When starting up:
  1) Set protocol using `P` command
  2) Set data for the current minute using `R` command (assuming second is <59)
  3) Wait for the start of next second and use `S` command to synchronize time

For the every minute after startup:
  1) At second `59` set data for the next minute using `R` command
  2) At second `0` apply new data using `S` command
  3) Rinse & repeat
