# best-hood-control
New electronic for Best (model Etna) hood based on Arduino

Component list:

* Arduino Nano
* Relay shield with 4 relays 5V, 10A
* Power module AC 230V to DC 5V, 3.5W
* Wires
* Wago: COMPACT Splicing Connectors

URL:

http://www.dx.com/p/improved-version-nano-3-0-atmel-atmega328p-mini-usb-board-for-arduino-452880
http://www.dx.com/p/ac-dc220v-to-5v-buck-3-5w-isolation-switch-power-supply-module-480550
http://www.dx.com/p/5v-4-channel-high-level-trigger-relay-module-for-arduino-red-157213

Wiring Arduino:

D2 - Relay Module IN4
D3 - Relay Module IN3
D4 - Relay Module IN2
D5 - Relay Module IN1
D6 - Pin3 to user board
D7 - Pin4 to user board
D8 - Pin5 to user board
D9 - Pin6 to user board
D10 - Pin7 to user board
D11 - Pin8 to user board
D12 - Pin9 to user board
D13 - Pin10 to user board

Wiring Relay shield:

Relay1 - Speed 1 (white)
Relay2 - Speed 2 (orange)
Relay3 - Speed 3 (black)
Relay4 - Light

User board (buttons and 7-segment):

pin1 : +5V
pin2 : null
pin3..10: Arduino

Wiring motor:

black: speed 3
orange: speed 2
white: speed 1
blue + brown: capacitor
red: N