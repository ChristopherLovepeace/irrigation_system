# irrigation_system
TUM Plant a Seed - irrigation system

Our project aims to make an automated watering system for the garden beds on the main campus of the TUM.
We use a Raspberry pi to send commands to an arduino MKR1010. The microcontroller then makes measurements for temperature, humidity and soil moisture in the garden bed where it is placed. The data is send back to the RPi as a struct and used to make decisions on whether the irrigation should be activated for that bed and for how long.

Latest:
Working on the prototype system, which will only be used on one of the garden beds, and be in operation for one week, during which time we will monitor its status.

Future Plans:
We hope for master-user status at the LRZ to make use of a VM on the eduroam network.
