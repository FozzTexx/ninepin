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
LIBS:Joysticks-cache
EELAYER 25 0
EELAYER END
$Descr USLedger 17000 11000
encoding utf-8
Sheet 1 1
Title "Ninepin Interface"
Date ""
Rev ""
Comp "http://insentricity.com"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CONN_02X05 P?
U 1 1 57882C64
P 2900 2300
F 0 "P?" H 2900 2600 50  0000 C CNN
F 1 "CONN_02X05" H 2900 2000 50  0000 C CNN
F 2 "" H 2900 1100 50  0000 C CNN
F 3 "" H 2900 1100 50  0000 C CNN
	1    2900 2300
	1    0    0    -1  
$EndComp
Text Notes 2550 1900 0    60   ~ 0
Atari/Commodore
Text GLabel 2650 2100 0    60   Input ~ 0
Up
Text GLabel 2650 2200 0    60   Input ~ 0
Left
Text GLabel 2650 2300 0    60   Input ~ 0
Paddle_1
Text GLabel 2650 2400 0    60   Input ~ 0
+5V
Text GLabel 2650 2500 0    60   Input ~ 0
Paddle_0
Text GLabel 3150 2100 2    60   Input ~ 0
Down
Text GLabel 3150 2200 2    60   Input ~ 0
Right
Text GLabel 3150 2300 2    60   Input ~ 0
Fire
Text GLabel 3150 2400 2    60   Input ~ 0
GND
$Comp
L CONN_02X05 P?
U 1 1 57882C65
P 2900 3600
F 0 "P?" H 2900 3900 50  0000 C CNN
F 1 "CONN_02X05" H 2900 3300 50  0000 C CNN
F 2 "" H 2900 2400 50  0000 C CNN
F 3 "" H 2900 2400 50  0000 C CNN
	1    2900 3600
	1    0    0    -1  
$EndComp
Text Notes 2750 3200 0    60   ~ 0
Apple II
Text GLabel 2650 3400 0    60   Input ~ 0
Fire_1
Text GLabel 2650 3500 0    60   Input ~ 0
GND
Text GLabel 2650 3600 0    60   Input ~ 0
Paddle_0
Text GLabel 2650 3700 0    60   Input ~ 0
Fire_0
Text GLabel 2650 3800 0    60   Input ~ 0
Paddle_3
Text GLabel 3150 3400 2    60   Input ~ 0
+5V
Text GLabel 3150 3500 2    60   Input ~ 0
Paddle_2
Text GLabel 3150 3600 2    60   Input ~ 0
Fire_2
Text GLabel 3150 3700 2    60   Input ~ 0
Paddle_1
$Comp
L CONN_02X03 P?
U 1 1 57882AC7
P 4900 2200
F 0 "P?" H 4900 2400 50  0000 C CNN
F 1 "CONN_02X03" H 4900 2000 50  0000 C CNN
F 2 "" H 4900 1000 50  0000 C CNN
F 3 "" H 4900 1000 50  0000 C CNN
	1    4900 2200
	1    0    0    -1  
$EndComp
Text Notes 4600 1900 0    60   ~ 0
TRS-80 CoCo
$Comp
L CONN_02X08 P?
U 1 1 57882B68
P 4900 3750
F 0 "P?" H 4900 4200 50  0000 C CNN
F 1 "CONN_02X08" V 4900 3750 50  0000 C CNN
F 2 "" H 4900 2550 50  0000 C CNN
F 3 "" H 4900 2550 50  0000 C CNN
	1    4900 3750
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X08 P?
U 1 1 57882BA8
P 6750 2450
F 0 "P?" H 6750 2900 50  0000 C CNN
F 1 "CONN_02X08" V 6750 2450 50  0000 C CNN
F 2 "" H 6750 1250 50  0000 C CNN
F 3 "" H 6750 1250 50  0000 C CNN
	1    6750 2450
	1    0    0    -1  
$EndComp
Text Notes 4600 3200 0    60   ~ 0
IBM Gameport
Text Notes 6550 1900 0    60   ~ 0
BBC Micro
Text GLabel 4650 2100 0    60   Input ~ 0
Paddle_0
Text GLabel 4650 2200 0    60   Input ~ 0
GND
Text GLabel 4650 2300 0    60   Input ~ 0
+5V
Text GLabel 5150 2100 2    60   Input ~ 0
Paddle_1
Text GLabel 5150 2200 2    60   Input ~ 0
Fire_0
Text GLabel 5150 2300 2    60   Input ~ 0
Fire_1
Text GLabel 4650 3400 0    60   Input ~ 0
+5V
Text GLabel 4650 3500 0    60   Input ~ 0
Paddle_0
Text GLabel 4650 3600 0    60   Input ~ 0
GND
Text GLabel 4650 3700 0    60   Input ~ 0
Fire_1
Text GLabel 4650 3800 0    60   Input ~ 0
+5V
Text GLabel 4650 3900 0    60   Input ~ 0
Paddle_2
Text GLabel 4650 4000 0    60   Input ~ 0
Paddle_3
Text GLabel 4650 4100 0    60   Input ~ 0
+5V
Text GLabel 5150 3400 2    60   Input ~ 0
Fire_0
Text GLabel 5150 3500 2    60   Input ~ 0
GND
Text GLabel 5150 3600 2    60   Input ~ 0
Paddle_1
Text GLabel 5150 3700 2    60   Input ~ 0
+5V
Text GLabel 5150 3800 2    60   Input ~ 0
Fire_3
Text GLabel 5150 3900 2    60   Input ~ 0
GND
Text GLabel 5150 4000 2    60   Input ~ 0
Fire_2
Text GLabel 7000 2400 2    60   Input ~ 0
GND
Text GLabel 6500 2400 0    60   Input ~ 0
Paddle_1
Text GLabel 7000 2300 2    60   Input ~ 0
GND
Text GLabel 6500 2300 0    60   Input ~ 0
GND
Text GLabel 6500 2800 0    60   Input ~ 0
Paddle_0
Text GLabel 6500 2100 0    60   Input ~ 0
+5V
Text GLabel 6500 2200 0    60   Input ~ 0
GND
Text GLabel 6500 2500 0    60   Input ~ 0
Light_Pen
Text GLabel 6500 2600 0    60   Input ~ 0
VREF
Text GLabel 6500 2700 0    60   Input ~ 0
Fire_0
Text GLabel 7000 2100 2    60   Input ~ 0
GND
Text GLabel 7000 2200 2    60   Input ~ 0
Paddle_3
Text GLabel 7000 2500 2    60   Input ~ 0
Fire_1
Text GLabel 7000 2600 2    60   Input ~ 0
Paddle_2
Text GLabel 7000 2700 2    60   Input ~ 0
VREF
$EndSCHEMATC
