# TrySpace Component Demonstration
This repository demonstrates a component in the TrySpace environment.

## Overview
This repository includes a command line interface (CLI), flight software (FSW), ground software (GSW), and simulation (SIM) directories.

The demo component is a UART device that is speak when spoken to.
Each command that is successfully interpretted is echoed back.
If additional telemetry is to be generated, it will follow.
The specific command format is followed:
* uint16, header, 0xC0FF
* uint16, command
  * (0) No operation
  * (1) Get housekeeping
  * (2) Get data
  * (3) Set configuration
* uint16, payload
  * Unused except for set configuration command
* uint16, trailer, 0xFEFE

Response formats:
* Housekeeping
  * uint16, header, 0xC0FF 
  * uint16, command counter
  * uint16, configuration
  * uint16, trailer, 0xFEFE
* Data
  * uint16, header, 0xC0FF
  * uint16, data channel 1
  * uint16, data channel 2
  * uint16, data channel 3
  * uint16, trailer, 0xFEFE

### Command Line Interface
The CLI can be configured to connect to either the hardware or simulation.
This enables direct checkouts these without interferance.

### Flight Software
The core Flight System (cFS) flight software application receives commands from the software bus.
Two message IDs exist for commands:
* 0x1A00 - Commands
  * (0) No operation
  * (1) Reset counters
  * (2) Enable
  * (3) Disable
* 0x1A01 - Requests
  * (0) Request telemetry

Two message IDs exist for telemetry:
* 0x0A00 - Application Housekeeping
* 0x0A01 - Component Telemetry

### Ground Software
...

### Simulation
...
