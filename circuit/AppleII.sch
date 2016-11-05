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
$Descr USLetter 11000 8500
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
L CONN_02X05 P?
U 1 1 57B9D9EA
P 2400 2200
F 0 "P?" H 2400 2500 50  0000 C CNN
F 1 "CONN_02X05" H 2400 1900 50  0000 C CNN
F 2 "" H 2400 1000 50  0000 C CNN
F 3 "" H 2400 1000 50  0000 C CNN
	1    2400 2200
	1    0    0    -1  
$EndComp
Text Label 2650 2000 0    60   ~ 0
Button_4
Text Label 2650 2100 0    60   ~ 0
Button_2
Text Label 2650 2200 0    60   ~ 0
Button_0
Text Label 2650 2300 0    60   ~ 0
Bcommon
Text Label 2650 2400 0    60   ~ 0
PGND
Text Label 2150 2000 2    60   ~ 0
Button_3
Text Label 2150 2100 2    60   ~ 0
Button_1
Text Label 2150 2200 2    60   ~ 0
Paddle_1
Text Label 2150 2300 2    60   ~ 0
Pvref
Text Label 2150 2400 2    60   ~ 0
Paddle_0
$Comp
L CONN_02X05 P?
U 1 1 57B9DA8B
P 4650 2200
F 0 "P?" H 4650 2500 50  0000 C CNN
F 1 "CONN_02X05" H 4650 1900 50  0000 C CNN
F 2 "" H 4650 1000 50  0000 C CNN
F 3 "" H 4650 1000 50  0000 C CNN
	1    4650 2200
	1    0    0    -1  
$EndComp
Text Label 4900 2000 0    60   ~ 0
Button_4
Text Label 4900 2100 0    60   ~ 0
Button_2
Text Label 4900 2200 0    60   ~ 0
Button_0
Text Label 4900 2300 0    60   ~ 0
Bcommon
Text Label 4900 2400 0    60   ~ 0
PGND
Text Label 4400 2000 2    60   ~ 0
Button_3
Text Label 4400 2100 2    60   ~ 0
Button_1
Text Label 4400 2200 2    60   ~ 0
Paddle_1
Text Label 4400 2300 2    60   ~ 0
Pvref
Text Label 4400 2400 2    60   ~ 0
Paddle_0
Wire Wire Line
	2150 2000 1550 2000
Wire Wire Line
	1550 2000 1550 1700
Wire Wire Line
	1550 1700 3900 1700
Wire Wire Line
	3900 1700 3900 2000
Wire Wire Line
	3900 2000 4400 2000
Wire Wire Line
	2150 2100 1500 2100
Wire Wire Line
	1500 2100 1500 1650
Wire Wire Line
	1500 1650 3850 1650
Wire Wire Line
	3850 1650 3850 2100
Wire Wire Line
	3850 2100 4400 2100
Wire Wire Line
	2150 2300 1450 2300
Wire Wire Line
	1450 2300 1450 1600
Wire Wire Line
	1450 1600 3800 1600
Wire Wire Line
	3800 1600 3800 2300
Wire Wire Line
	3800 2300 4400 2300
Wire Wire Line
	2650 2000 3750 2000
Wire Wire Line
	3750 2000 3750 2650
Wire Wire Line
	3750 2650 5400 2650
Wire Wire Line
	5400 2650 5400 2000
Wire Wire Line
	5400 2000 4900 2000
Wire Wire Line
	2650 2100 3700 2100
Wire Wire Line
	3700 2100 3700 2700
Wire Wire Line
	3700 2700 5450 2700
Wire Wire Line
	5450 2700 5450 2100
Wire Wire Line
	5450 2100 4900 2100
Wire Wire Line
	2650 2200 3650 2200
Wire Wire Line
	3650 2200 3650 2750
Wire Wire Line
	3650 2750 5500 2750
Wire Wire Line
	5500 2750 5500 2200
Wire Wire Line
	5500 2200 4900 2200
Wire Wire Line
	2650 2300 3600 2300
Wire Wire Line
	3600 2300 3600 2800
Wire Wire Line
	3600 2800 5550 2800
Wire Wire Line
	5550 2800 5550 2300
Wire Wire Line
	5550 2300 4900 2300
$Comp
L C C?
U 1 1 57B9DB58
P 2800 3200
F 0 "C?" H 2825 3300 50  0000 L CNN
F 1 ".1uf" H 2825 3100 50  0000 L CNN
F 2 "" H 2838 3050 50  0000 C CNN
F 3 "" H 2800 3200 50  0000 C CNN
	1    2800 3200
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57B9DB7D
P 3200 3200
F 0 "C?" H 3225 3300 50  0000 L CNN
F 1 ".1uf" H 3225 3100 50  0000 L CNN
F 2 "" H 3238 3050 50  0000 C CNN
F 3 "" H 3200 3200 50  0000 C CNN
	1    3200 3200
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57B9DB9C
P 3000 3500
F 0 "C?" H 3025 3600 50  0000 L CNN
F 1 ".1uf" H 3025 3400 50  0000 L CNN
F 2 "" H 3038 3350 50  0000 C CNN
F 3 "" H 3000 3500 50  0000 C CNN
	1    3000 3500
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57B9DC0E
P 2800 3950
F 0 "C?" H 2825 4050 50  0000 L CNN
F 1 ".1uf" H 2825 3850 50  0000 L CNN
F 2 "" H 2838 3800 50  0000 C CNN
F 3 "" H 2800 3950 50  0000 C CNN
	1    2800 3950
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57B9DC14
P 3200 3950
F 0 "C?" H 3225 4050 50  0000 L CNN
F 1 ".1uf" H 3225 3850 50  0000 L CNN
F 2 "" H 3238 3800 50  0000 C CNN
F 3 "" H 3200 3950 50  0000 C CNN
	1    3200 3950
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57B9DC1A
P 3000 4250
F 0 "C?" H 3025 4350 50  0000 L CNN
F 1 ".1uf" H 3025 4150 50  0000 L CNN
F 2 "" H 3038 4100 50  0000 C CNN
F 3 "" H 3000 4250 50  0000 C CNN
	1    3000 4250
	0    1    1    0   
$EndComp
Wire Wire Line
	3350 3200 5100 3200
Wire Wire Line
	5100 3200 5100 2400
Wire Wire Line
	5100 2400 4900 2400
Wire Wire Line
	3150 3500 3500 3500
Wire Wire Line
	3500 3200 3500 4250
Connection ~ 3500 3200
Wire Wire Line
	3500 3950 3350 3950
Connection ~ 3500 3500
Wire Wire Line
	3500 4250 3150 4250
Connection ~ 3500 3950
Wire Wire Line
	3050 3200 2950 3200
Wire Wire Line
	3050 3950 2950 3950
Wire Wire Line
	2150 2400 1650 2400
Wire Wire Line
	1650 2400 1650 3200
Wire Wire Line
	1650 3200 2650 3200
Wire Wire Line
	2850 3500 2550 3500
Wire Wire Line
	2550 3500 2550 3200
Connection ~ 2550 3200
Wire Wire Line
	2150 2200 1550 2200
Wire Wire Line
	1550 2200 1550 3950
Wire Wire Line
	1550 3950 2650 3950
Wire Wire Line
	2850 4250 2550 4250
Wire Wire Line
	2550 4250 2550 3950
Connection ~ 2550 3950
Wire Wire Line
	4400 2200 3850 2200
Wire Wire Line
	3850 2200 3850 3700
Wire Wire Line
	3850 3700 1550 3700
Connection ~ 1550 3700
Wire Wire Line
	4400 2400 3900 2400
Wire Wire Line
	3900 2400 3900 2950
Wire Wire Line
	3900 2950 1650 2950
Connection ~ 1650 2950
$EndSCHEMATC
