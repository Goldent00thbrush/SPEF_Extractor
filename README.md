# SPEF_Extractor

Done by: Aya Moemen, Marla Ebeid and Shereen Afify

Things Done in Parsing the input files(DEF/LEF): *Marla's Part*

1- Vector of struct "PINS" that contains all the PINS information (pin name, direction, state(FIXED or PLACED),pin first coordinate,pin second coordinate).

2- Array of struct "NETS" default size of 50, that contains the NETS information (net name, net connections "stored in a 2D array named *connection* of size default 8 and 2(Type and name).

3-Vector of struct "Layer" that contains information(name,resistance unit,resistance value, capacitance unit, capacitance value,edge capacitance,width,spacing) of the layers of type *ROUTING* only

4-Vector that contains the names of all layers of any type.

5-Vector of struct "VIA" that contains information about all VIAs (VIA name, resistance)

6- Design name

Things Done in Writing the output file(SPEF): *Aya's Part*

1- Header Section

2- Nmap Section of pins and ports 
