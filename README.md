# SolarCollectorController

This is Arduino code for a device that controls two circulatory pumps and an electronic water valve based on the conditions of three temperature sensors. 

It is used in Solar Collector installation that heats up antifreeze in outside light absorbers from solar light, then uses that antifreeze
to heat up a buffer tank (heat exchanger)and then hot water from the heat exchanger is pumped to the boiler to heat up water for regular use. 
Since the boiler is also connected to the solid fuel heating system (that is used in the winter to heat the boiler and the whole house), 
valve is used to control the direction that hot water from the heat the exchanger is going, so it would not flow to the main house heating system

Implemented UART comm logic to be able to read and write parameters from external source like single board computer. Next time would probably use Modbus RTU protocol.
More: https://github.com/TautrimasM/arduinoCommunicator
