EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 25 0
EELAYER END
$Descr USLedger 17000 11000
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L AD5282 U?
U 1 1 546E4162
P 8500 2250
F 0 "U?" H 8500 2950 60  0000 C CNN
F 1 "AD5282" H 8500 1650 60  0000 C CNN
F 2 "" H 8500 2250 60  0000 C CNN
F 3 "" H 8500 2250 60  0000 C CNN
	1    8500 2250
	1    0    0    -1  
$EndComp
$Comp
L 74HC595 U?
U 1 1 546E42CE
P 13350 2050
F 0 "U?" H 13500 2650 70  0000 C CNN
F 1 "74HC595" H 13350 1450 70  0000 C CNN
F 2 "" H 13350 2050 60  0000 C CNN
F 3 "" H 13350 2050 60  0000 C CNN
	1    13350 2050
	1    0    0    -1  
$EndComp
$Comp
L 4066 U?
U 1 1 546E43AB
P 13300 3900
F 0 "U?" H 13500 3751 40  0000 C CNN
F 1 "4066" H 13500 4050 40  0000 C CNN
F 2 "" H 13300 3900 60  0000 C CNN
F 3 "" H 13300 3900 60  0000 C CNN
	1    13300 3900
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X05 P?
U 1 1 546E4A7A
P 10950 2250
F 0 "P?" H 10950 2550 50  0000 C CNN
F 1 "CONN_02X05" H 10950 1950 50  0000 C CNN
F 2 "" H 10950 1050 60  0000 C CNN
F 3 "" H 10950 1050 60  0000 C CNN
	1    10950 2250
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X03 P?
U 1 1 546E4B08
P 4050 2700
F 0 "P?" H 4050 2900 50  0000 C CNN
F 1 "CONN_02X03" H 4050 2500 50  0000 C CNN
F 2 "" H 4050 1500 60  0000 C CNN
F 3 "" H 4050 1500 60  0000 C CNN
	1    4050 2700
	1    0    0    -1  
$EndComp
$Comp
L 74HC245 U?
U 1 1 546E4CFF
P 2550 7000
F 0 "U?" H 2650 7575 60  0000 L BNN
F 1 "74HC245" H 2600 6425 60  0000 L TNN
F 2 "" H 2550 7000 60  0000 C CNN
F 3 "" H 2550 7000 60  0000 C CNN
	1    2550 7000
	1    0    0    -1  
$EndComp
$Comp
L 4050 U?
U 1 1 546E4D2E
P 5250 6550
F 0 "U?" H 5445 6665 60  0000 C CNN
F 1 "4050" H 5440 6425 60  0000 C CNN
F 2 "" H 5250 6550 60  0000 C CNN
F 3 "" H 5250 6550 60  0000 C CNN
	1    5250 6550
	1    0    0    -1  
$EndComp
$Comp
L 4050 U?
U 1 1 546E4D57
P 5200 7050
F 0 "U?" H 5395 7165 60  0000 C CNN
F 1 "4050" H 5390 6925 60  0000 C CNN
F 2 "" H 5200 7050 60  0000 C CNN
F 3 "" H 5200 7050 60  0000 C CNN
	1    5200 7050
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 546E6040
P 2100 1450
F 0 "R?" V 2180 1450 40  0000 C CNN
F 1 "R" V 2107 1451 40  0000 C CNN
F 2 "" V 2030 1450 30  0000 C CNN
F 3 "" H 2100 1450 30  0000 C CNN
	1    2100 1450
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 546E6133
P 2900 1450
F 0 "R?" V 2980 1450 40  0000 C CNN
F 1 "R" V 2907 1451 40  0000 C CNN
F 2 "" V 2830 1450 30  0000 C CNN
F 3 "" H 2900 1450 30  0000 C CNN
	1    2900 1450
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 546E8337
P 1900 1200
F 0 "#PWR?" H 1900 1160 30  0001 C CNN
F 1 "+3.3V" H 1900 1310 30  0000 C CNN
F 2 "" H 1900 1200 60  0000 C CNN
F 3 "" H 1900 1200 60  0000 C CNN
	1    1900 1200
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 546E834B
P 3100 1200
F 0 "#PWR?" H 3100 1290 20  0001 C CNN
F 1 "+5V" H 3100 1290 30  0000 C CNN
F 2 "" H 3100 1200 60  0000 C CNN
F 3 "" H 3100 1200 60  0000 C CNN
	1    3100 1200
	1    0    0    -1  
$EndComp
$Comp
L BSS138 Q?
U 1 1 546E8FD3
P 2500 1600
F 0 "Q?" H 2500 1451 40  0000 R CNN
F 1 "BSS138" H 2500 1750 40  0000 R CNN
F 2 "SOT-23" H 2370 1702 29  0000 C CNN
F 3 "" H 2500 1600 60  0000 C CNN
	1    2500 1600
	0    1    1    0   
$EndComp
Text Label 3800 2600 2    60   ~ 0
/SRQ
Text Label 3800 2700 2    60   ~ 0
ATN
Text Label 3800 2800 2    60   ~ 0
DATA
Text Label 4300 2600 0    60   ~ 0
GND
Text Label 4300 2700 0    60   ~ 0
CLK
Text Label 4300 2800 0    60   ~ 0
/RESET
$Comp
L R R?
U 1 1 546EADC9
P 2100 2150
F 0 "R?" V 2180 2150 40  0000 C CNN
F 1 "R" V 2107 2151 40  0000 C CNN
F 2 "" V 2030 2150 30  0000 C CNN
F 3 "" H 2100 2150 30  0000 C CNN
	1    2100 2150
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 546EADCF
P 2900 2150
F 0 "R?" V 2980 2150 40  0000 C CNN
F 1 "R" V 2907 2151 40  0000 C CNN
F 2 "" V 2830 2150 30  0000 C CNN
F 3 "" H 2900 2150 30  0000 C CNN
	1    2900 2150
	1    0    0    -1  
$EndComp
$Comp
L BSS138 Q?
U 1 1 546EADD5
P 2500 2300
F 0 "Q?" H 2500 2151 40  0000 R CNN
F 1 "BSS138" H 2500 2450 40  0000 R CNN
F 2 "SOT-23" H 2370 2402 29  0000 C CNN
F 3 "" H 2500 2300 60  0000 C CNN
	1    2500 2300
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 546EAFFD
P 2100 2850
F 0 "R?" V 2180 2850 40  0000 C CNN
F 1 "R" V 2107 2851 40  0000 C CNN
F 2 "" V 2030 2850 30  0000 C CNN
F 3 "" H 2100 2850 30  0000 C CNN
	1    2100 2850
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 546EB003
P 2900 2850
F 0 "R?" V 2980 2850 40  0000 C CNN
F 1 "R" V 2907 2851 40  0000 C CNN
F 2 "" V 2830 2850 30  0000 C CNN
F 3 "" H 2900 2850 30  0000 C CNN
	1    2900 2850
	1    0    0    -1  
$EndComp
$Comp
L BSS138 Q?
U 1 1 546EB009
P 2500 3000
F 0 "Q?" H 2500 2851 40  0000 R CNN
F 1 "BSS138" H 2500 3150 40  0000 R CNN
F 2 "SOT-23" H 2370 3102 29  0000 C CNN
F 3 "" H 2500 3000 60  0000 C CNN
	1    2500 3000
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 546EB013
P 2100 3550
F 0 "R?" V 2180 3550 40  0000 C CNN
F 1 "R" V 2107 3551 40  0000 C CNN
F 2 "" V 2030 3550 30  0000 C CNN
F 3 "" H 2100 3550 30  0000 C CNN
	1    2100 3550
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 546EB019
P 2900 3550
F 0 "R?" V 2980 3550 40  0000 C CNN
F 1 "R" V 2907 3551 40  0000 C CNN
F 2 "" V 2830 3550 30  0000 C CNN
F 3 "" H 2900 3550 30  0000 C CNN
	1    2900 3550
	1    0    0    -1  
$EndComp
$Comp
L BSS138 Q?
U 1 1 546EB01F
P 2500 3700
F 0 "Q?" H 2500 3551 40  0000 R CNN
F 1 "BSS138" H 2500 3850 40  0000 R CNN
F 2 "SOT-23" H 2370 3802 29  0000 C CNN
F 3 "" H 2500 3700 60  0000 C CNN
	1    2500 3700
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 546EB1F3
P 2100 4250
F 0 "R?" V 2180 4250 40  0000 C CNN
F 1 "R" V 2107 4251 40  0000 C CNN
F 2 "" V 2030 4250 30  0000 C CNN
F 3 "" H 2100 4250 30  0000 C CNN
	1    2100 4250
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 546EB1F9
P 2900 4250
F 0 "R?" V 2980 4250 40  0000 C CNN
F 1 "R" V 2907 4251 40  0000 C CNN
F 2 "" V 2830 4250 30  0000 C CNN
F 3 "" H 2900 4250 30  0000 C CNN
	1    2900 4250
	1    0    0    -1  
$EndComp
$Comp
L BSS138 Q?
U 1 1 546EB1FF
P 2500 4400
F 0 "Q?" H 2500 4251 40  0000 R CNN
F 1 "BSS138" H 2500 4550 40  0000 R CNN
F 2 "SOT-23" H 2370 4502 29  0000 C CNN
F 3 "" H 2500 4400 60  0000 C CNN
	1    2500 4400
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR?
U 1 1 546EC072
P 7350 1000
F 0 "#PWR?" H 7350 1090 20  0001 C CNN
F 1 "+5V" H 7350 1090 30  0000 C CNN
F 2 "" H 7350 1000 60  0000 C CNN
F 3 "" H 7350 1000 60  0000 C CNN
	1    7350 1000
	1    0    0    -1  
$EndComp
Text Label 10700 2050 2    60   ~ 0
Up
Text Label 10700 2150 2    60   ~ 0
Left
Text Label 10700 2250 2    60   ~ 0
B_Pot
Text Label 10700 2350 2    60   ~ 0
Power
Text Label 10700 2450 2    60   ~ 0
A_Pot
Text Label 11200 2050 0    60   ~ 0
Down
Text Label 11200 2150 0    60   ~ 0
Right
Text Label 11200 2250 0    60   ~ 0
Button_1
Text Label 11200 2350 0    60   ~ 0
GND
$Comp
L AD5282 U?
U 1 1 546EDC34
P 8500 3750
F 0 "U?" H 8500 4450 60  0000 C CNN
F 1 "AD5282" H 8500 3150 60  0000 C CNN
F 2 "" H 8500 3750 60  0000 C CNN
F 3 "" H 8500 3750 60  0000 C CNN
	1    8500 3750
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 546EEAA8
P 7100 1000
F 0 "#PWR?" H 7100 960 30  0001 C CNN
F 1 "+3.3V" H 7100 1110 30  0000 C CNN
F 2 "" H 7100 1000 60  0000 C CNN
F 3 "" H 7100 1000 60  0000 C CNN
	1    7100 1000
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P?
U 1 1 546EFFE3
P 6500 1500
F 0 "P?" H 6500 1650 50  0000 C CNN
F 1 "CONN_01X02" V 6600 1500 50  0000 C CNN
F 2 "" H 6500 1500 60  0000 C CNN
F 3 "" H 6500 1500 60  0000 C CNN
	1    6500 1500
	0    -1   -1   0   
$EndComp
Text Label 6550 1700 0    60   ~ 0
SDA
Text Label 6450 1700 2    60   ~ 0
SCL
$Comp
L GND #PWR?
U 1 1 546F76FB
P 9300 8400
F 0 "#PWR?" H 9300 8400 30  0001 C CNN
F 1 "GND" H 9300 8330 30  0001 C CNN
F 2 "" H 9300 8400 60  0000 C CNN
F 3 "" H 9300 8400 60  0000 C CNN
	1    9300 8400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 1700 2100 1700
Wire Wire Line
	2700 1700 3800 1700
Wire Wire Line
	2450 1200 2450 1400
Wire Wire Line
	1900 1200 2450 1200
Wire Wire Line
	2300 2400 2100 2400
Wire Wire Line
	2700 2400 3400 2400
Wire Wire Line
	2450 1900 2450 2100
Wire Wire Line
	1900 1900 2450 1900
Wire Wire Line
	2300 3100 2100 3100
Wire Wire Line
	2700 3100 3400 3100
Wire Wire Line
	2450 2600 2450 2800
Wire Wire Line
	1900 2600 2450 2600
Wire Wire Line
	2300 3800 2100 3800
Wire Wire Line
	2700 3800 4300 3800
Wire Wire Line
	2450 3300 2450 3500
Wire Wire Line
	1900 3300 2450 3300
Wire Wire Line
	2300 4500 2100 4500
Wire Wire Line
	2700 4500 4900 4500
Wire Wire Line
	2450 4000 2450 4200
Wire Wire Line
	1900 4000 2450 4000
Wire Wire Line
	3100 1200 3100 4000
Wire Wire Line
	3100 4000 2900 4000
Wire Wire Line
	1900 1200 1900 4000
Connection ~ 2100 4000
Connection ~ 1900 3300
Connection ~ 2100 3300
Connection ~ 1900 2600
Connection ~ 2100 2600
Connection ~ 1900 1900
Connection ~ 2100 1900
Connection ~ 2100 1200
Wire Wire Line
	2900 1200 3100 1200
Wire Wire Line
	2900 1900 3100 1900
Connection ~ 3100 1900
Wire Wire Line
	2900 2600 3100 2600
Connection ~ 3100 2600
Wire Wire Line
	2900 3300 3100 3300
Connection ~ 3100 3300
Wire Wire Line
	3800 1700 3800 2600
Connection ~ 2900 1700
Wire Wire Line
	3400 2400 3400 2700
Wire Wire Line
	3400 2700 3800 2700
Connection ~ 2900 2400
Wire Wire Line
	3400 3100 3400 2800
Wire Wire Line
	3400 2800 3800 2800
Connection ~ 2900 3100
Wire Wire Line
	4300 3800 4300 2800
Connection ~ 2900 3800
Wire Wire Line
	4900 4500 4900 2700
Wire Wire Line
	4900 2700 4300 2700
Connection ~ 2900 4500
Wire Wire Line
	9950 2050 9950 2250
Wire Wire Line
	9950 2250 10700 2250
Wire Wire Line
	9400 3550 9950 3550
Wire Wire Line
	9950 3550 9950 2450
Wire Wire Line
	9950 2450 10700 2450
Wire Wire Line
	7800 2350 7600 2350
Wire Wire Line
	7800 2450 7600 2450
Connection ~ 7600 2450
Wire Wire Line
	7600 2600 7800 2600
Connection ~ 7600 2600
Wire Wire Line
	7800 4100 7600 4100
Connection ~ 7600 4100
Wire Wire Line
	7800 3950 7600 3950
Connection ~ 7600 3950
Wire Wire Line
	7350 1000 7350 6900
Wire Wire Line
	7350 3850 7800 3850
Wire Wire Line
	7800 1800 7100 1800
Wire Wire Line
	7100 1000 7100 6500
Wire Wire Line
	7100 3300 7800 3300
Connection ~ 7100 1800
Wire Wire Line
	9200 1800 9500 1800
Wire Wire Line
	9200 1900 9400 1900
Wire Wire Line
	9200 2350 9400 2350
Wire Wire Line
	9200 2450 9300 2450
Wire Wire Line
	9500 3300 9200 3300
Wire Wire Line
	9200 3400 9400 3400
Wire Wire Line
	9400 3400 9400 3850
Wire Wire Line
	9400 3850 9200 3850
Connection ~ 9400 3550
Wire Wire Line
	9200 3950 9300 3950
Connection ~ 9300 3950
Wire Wire Line
	7800 2700 7600 2700
Connection ~ 7600 2700
Wire Wire Line
	7800 4200 7600 4200
Connection ~ 7600 4200
Wire Wire Line
	7800 1950 7100 1950
Connection ~ 7100 1950
Wire Wire Line
	7100 3450 7800 3450
Connection ~ 7100 3300
Wire Wire Line
	7800 3200 7350 3200
Connection ~ 7350 3200
Wire Wire Line
	7800 1700 7350 1700
Connection ~ 7350 1700
Wire Wire Line
	6450 1700 6450 6650
Wire Wire Line
	6450 2100 7800 2100
Wire Wire Line
	6450 3600 7800 3600
Connection ~ 6450 2100
Wire Wire Line
	6550 1700 6550 6750
Wire Wire Line
	6550 2200 7800 2200
Wire Wire Line
	6550 3700 7800 3700
Connection ~ 6550 2200
Wire Wire Line
	11200 2350 11350 2350
Wire Wire Line
	9400 2350 9400 1900
$Comp
L AD5282 U?
U 1 1 546FA187
P 8500 5300
F 0 "U?" H 8500 6000 60  0000 C CNN
F 1 "AD5282" H 8500 4700 60  0000 C CNN
F 2 "" H 8500 5300 60  0000 C CNN
F 3 "" H 8500 5300 60  0000 C CNN
	1    8500 5300
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X05 P?
U 1 1 546FA18D
P 10950 5300
F 0 "P?" H 10950 5600 50  0000 C CNN
F 1 "CONN_02X05" H 10950 5000 50  0000 C CNN
F 2 "" H 10950 4100 60  0000 C CNN
F 3 "" H 10950 4100 60  0000 C CNN
	1    10950 5300
	1    0    0    -1  
$EndComp
Text Label 10700 5100 2    60   ~ 0
Up
Text Label 10700 5200 2    60   ~ 0
Left
Text Label 10700 5300 2    60   ~ 0
B_Pot
Text Label 10700 5400 2    60   ~ 0
Power
Text Label 10700 5500 2    60   ~ 0
A_Pot
Text Label 11200 5100 0    60   ~ 0
Down
Text Label 11200 5200 0    60   ~ 0
Right
Text Label 11200 5300 0    60   ~ 0
Button_1
Text Label 11200 5400 0    60   ~ 0
GND
$Comp
L AD5282 U?
U 1 1 546FA19C
P 8500 6800
F 0 "U?" H 8500 7500 60  0000 C CNN
F 1 "AD5282" H 8500 6200 60  0000 C CNN
F 2 "" H 8500 6800 60  0000 C CNN
F 3 "" H 8500 6800 60  0000 C CNN
	1    8500 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 5100 9950 5300
Wire Wire Line
	9950 5300 10700 5300
Wire Wire Line
	9400 6600 9950 6600
Wire Wire Line
	9950 6600 9950 5500
Wire Wire Line
	9950 5500 10700 5500
Wire Wire Line
	7800 5400 7600 5400
Wire Wire Line
	7600 5400 7600 7750
Wire Wire Line
	7600 7750 11350 7750
Connection ~ 9300 7750
Wire Wire Line
	7800 5500 7600 5500
Connection ~ 7600 5500
Wire Wire Line
	7800 5650 7600 5650
Connection ~ 7600 5650
Wire Wire Line
	7800 7150 7600 7150
Connection ~ 7600 7150
Wire Wire Line
	7800 7000 7600 7000
Connection ~ 7600 7000
Wire Wire Line
	7350 6900 7800 6900
Wire Wire Line
	7800 4850 7100 4850
Wire Wire Line
	7100 6350 7800 6350
Connection ~ 7100 4850
Wire Wire Line
	9200 4850 9500 4850
Wire Wire Line
	9200 4950 9400 4950
Wire Wire Line
	9200 5400 9400 5400
Wire Wire Line
	9200 5500 9300 5500
Wire Wire Line
	9500 6350 9200 6350
Wire Wire Line
	9200 6450 9400 6450
Wire Wire Line
	9400 6450 9400 6900
Wire Wire Line
	9400 6900 9200 6900
Connection ~ 9400 6600
Wire Wire Line
	9200 7000 9300 7000
Connection ~ 9300 7000
Wire Wire Line
	7800 5750 7600 5750
Connection ~ 7600 5750
Wire Wire Line
	7800 7250 7600 7250
Connection ~ 7600 7250
Wire Wire Line
	7800 5000 7100 5000
Connection ~ 7100 5000
Wire Wire Line
	7100 6500 7800 6500
Connection ~ 7100 6350
Wire Wire Line
	7800 6250 7350 6250
Connection ~ 7350 6250
Wire Wire Line
	7800 4750 7350 4750
Connection ~ 7350 4750
Wire Wire Line
	6450 5150 7800 5150
Wire Wire Line
	6450 6650 7800 6650
Wire Wire Line
	6550 5250 7800 5250
Wire Wire Line
	6550 6750 7800 6750
Wire Wire Line
	11200 5400 11350 5400
Wire Wire Line
	11350 5400 11350 7750
Wire Wire Line
	9400 5400 9400 4950
Wire Wire Line
	9950 2050 9400 2050
Connection ~ 9400 2050
Wire Wire Line
	10700 2350 9500 2350
Wire Wire Line
	9500 1800 9500 3300
Connection ~ 9500 2350
Wire Wire Line
	9300 2450 9300 8400
Wire Wire Line
	9950 5100 9400 5100
Connection ~ 9400 5100
Wire Wire Line
	9500 4850 9500 6350
Wire Wire Line
	10700 5400 9500 5400
Connection ~ 9500 5400
Connection ~ 7350 3850
Connection ~ 7100 3450
Connection ~ 6450 3600
Connection ~ 6450 5150
Connection ~ 6550 3700
Connection ~ 6550 5250
Wire Wire Line
	7600 4500 11350 4500
Wire Wire Line
	7600 2350 7600 4500
Wire Wire Line
	11350 4500 11350 2350
$EndSCHEMATC
