# SPEF_Extractor

Done by: Aya Moemen, Marla Ebeid and Shereen Afify

Things Done in Parsing the input files(DEF/LEF): *Marla's Part*

1- Vector of struct "PINS" that contains all the PINS information (pin name, direction, state(FIXED or PLACED),pin first coordinate,pin second coordinate).

2- Array of struct "NETS" default size of 50, that contains the NETS information (net name, net connections "stored in a 2D array named *connection* of size default 8 and 2(Type and name).

3-Vector of struct "Layer" that contains information(name,resistance unit,resistance value, capacitance unit, capacitance value,edge capacitance,width,spacing) of the layers of type *ROUTING* only

4-Vector that contains the names of all layers of any type.

5-Vector of struct "VIA" that contains information about all VIAs (VIA name, resistance)

6- Design name

Things Done in Calculating the R and C : *Shereen's Part*

- For the resistance part: 

1- Two main functions are constructed for calculating the resistances: the first one is for calculating the resistance per layer and the second one is for calculating the via resistance. both of them return the float resistance required value.

2- The function calculating the layer resistance takes the resistance, width and length values from each struct in the layers vector and plug it into the equation to return the resistance value for the layer.

3- The vias resistance function returns the via resistance value for each via.

- For the capacitance part: 

2- The function calculating the wire capacitance takes the capacitance, width and spacing values from each struct in the layers vector and plug it into the equation to return the capacitance value for the layer.

Things Done in Writing the output file(SPEF): *Aya's Part*

1- Header Section

2- Nmap Section of pins and ports 
