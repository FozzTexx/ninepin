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
Text Notes 2550 1350 0    60   ~ 0
Atari/Commodore
$Comp
L CONN_02X03 P?
U 1 1 57882AC7
P 14200 2200
F 0 "P?" H 14200 2400 50  0000 C CNN
F 1 "CONN_02X03" H 14200 2000 50  0000 C CNN
F 2 "" H 14200 1000 50  0000 C CNN
F 3 "" H 14200 1000 50  0000 C CNN
	1    14200 2200
	1    0    0    -1  
$EndComp
Text Notes 13900 1900 0    60   ~ 0
TRS-80 CoCo
$Comp
L CONN_02X08 P?
U 1 1 57882BA8
P 10700 1900
F 0 "P?" H 10700 2350 50  0000 C CNN
F 1 "CONN_02X08" V 10700 1900 50  0000 C CNN
F 2 "" H 10700 700 50  0000 C CNN
F 3 "" H 10700 700 50  0000 C CNN
	1    10700 1900
	1    0    0    -1  
$EndComp
Text Notes 6200 5350 0    60   ~ 0
IBM Gameport
Text Notes 10500 1350 0    60   ~ 0
BBC Micro
Text GLabel 13950 2100 0    60   Input ~ 0
Paddle_0
Text GLabel 13950 2200 0    60   Input ~ 0
GND
Text GLabel 13950 2300 0    60   Input ~ 0
+5V
Text GLabel 14450 2100 2    60   Input ~ 0
Paddle_1
Text GLabel 14450 2200 2    60   Input ~ 0
Fire_0
Text GLabel 14450 2300 2    60   Input ~ 0
Fire_1
Text GLabel 10950 1850 2    60   Input ~ 0
GND
Text GLabel 10450 1850 0    60   Input ~ 0
Paddle_1
Text GLabel 10950 1750 2    60   Input ~ 0
GND
Text GLabel 10450 1750 0    60   Input ~ 0
GND
Text GLabel 10450 2250 0    60   Input ~ 0
Paddle_0
Text GLabel 10450 1550 0    60   Input ~ 0
+5V
Text GLabel 10450 1650 0    60   Input ~ 0
GND
Text GLabel 10450 1950 0    60   Input ~ 0
Light_Pen
Text GLabel 10450 2050 0    60   Input ~ 0
VREF
Text GLabel 10450 2150 0    60   Input ~ 0
Fire_0
Text GLabel 10950 1550 2    60   Input ~ 0
GND
Text GLabel 10950 1650 2    60   Input ~ 0
Paddle_3
Text GLabel 10950 1950 2    60   Input ~ 0
Fire_1
Text GLabel 10950 2050 2    60   Input ~ 0
Paddle_2
Text GLabel 10950 2150 2    60   Input ~ 0
VREF
$Comp
L DB9_FEMALE J?
U 1 1 59D93719
P 2900 3100
F 0 "J?" H 2900 3650 50  0000 C CNN
F 1 "DB9_FEMALE" H 2900 2525 50  0000 C CNN
F 2 "" H 2900 3100 50  0001 C CNN
F 3 "" H 2900 3100 50  0001 C CNN
	1    2900 3100
	0    1    1    0   
$EndComp
$Comp
L DB15_MALE J?
U 1 1 59D93A2C
P 6550 8250
F 0 "J?" H 6570 9100 50  0000 C CNN
F 1 "DB15_MALE" H 6550 7375 50  0000 C CNN
F 2 "" H 6550 8250 50  0001 C CNN
F 3 "" H 6550 8250 50  0001 C CNN
	1    6550 8250
	0    1    1    0   
$EndComp
$Comp
L DB15_MALE J?
U 1 1 59D93B5E
P 10700 3250
F 0 "J?" H 10720 4100 50  0000 C CNN
F 1 "DB15_MALE" H 10700 2375 50  0000 C CNN
F 2 "" H 10700 3250 50  0001 C CNN
F 3 "" H 10700 3250 50  0001 C CNN
	1    10700 3250
	0    1    1    0   
$EndComp
$Comp
L CONN_02X05 J?
U 1 1 59D93F85
P 2900 1750
F 0 "J?" H 2900 2050 50  0000 C CNN
F 1 "CONN_02X05" H 2900 1450 50  0000 C CNN
F 2 "" H 2900 550 50  0001 C CNN
F 3 "" H 2900 550 50  0001 C CNN
	1    2900 1750
	1    0    0    -1  
$EndComp
Text Label 2650 1550 2    60   ~ 0
Button_3
Text Label 2650 1650 2    60   ~ 0
Button_4
Text Label 3300 2800 1    60   ~ 0
Up
Text Label 3200 2800 1    60   ~ 0
Fire
Text Label 3100 2800 1    60   ~ 0
Down
Text Label 3000 2800 1    60   ~ 0
+5V
Text Label 2900 2800 1    60   ~ 0
Left
Text Label 2800 2800 1    60   ~ 0
GND
Text Label 2700 2800 1    60   ~ 0
Right
Text Label 2600 2800 1    60   ~ 0
Paddle_0
Text Label 2500 2800 1    60   ~ 0
Paddle_1
Text Label 2650 1750 2    60   ~ 0
Button_1
Text Label 2650 1850 2    60   ~ 0
Button_2
Text Label 2650 1950 2    60   ~ 0
Paddle_1
Text Label 3150 1550 0    60   ~ 0
Button_0
Text Label 3150 1650 0    60   ~ 0
Pvref
Text Label 3150 1750 0    60   ~ 0
Bcommon
Text Label 3150 1850 0    60   ~ 0
Paddle_0
$Comp
L DB9_MALE J?
U 1 1 59D939A4
P 2950 8250
F 0 "J?" H 2950 8800 50  0000 C CNN
F 1 "DB9_MALE" H 2950 7675 50  0000 C CNN
F 2 "" H 2950 8250 50  0001 C CNN
F 3 "" H 2950 8250 50  0001 C CNN
	1    2950 8250
	0    1    1    0   
$EndComp
Text Label 2550 7950 1    60   ~ 0
Fire_1
Text Label 2650 7950 1    60   ~ 0
Fire_2
Text Label 2750 7950 1    60   ~ 0
+5V
Text Label 2850 7950 1    60   ~ 0
Fire_0
Text Label 2950 7950 1    60   ~ 0
GND
Text Label 3050 7950 1    60   ~ 0
Paddle_1
Text Label 3150 7950 1    60   ~ 0
Paddle_2
Text Label 3250 7950 1    60   ~ 0
Paddle_3
Text Label 3350 7950 1    60   ~ 0
Paddle_0
Text Notes 2800 5350 0    60   ~ 0
Apple II
$Comp
L CONN_02X05 J?
U 1 1 59DA690F
P 2950 5750
F 0 "J?" H 2950 6050 50  0000 C CNN
F 1 "CONN_02X05" H 2950 5450 50  0000 C CNN
F 2 "" H 2950 4550 50  0001 C CNN
F 3 "" H 2950 4550 50  0001 C CNN
	1    2950 5750
	1    0    0    -1  
$EndComp
Text Label 2700 5550 2    60   ~ 0
Button_3
Text Label 2700 5650 2    60   ~ 0
Button_4
Text Label 2700 5750 2    60   ~ 0
Button_1
Text Label 2700 5850 2    60   ~ 0
Button_2
Text Label 2700 5950 2    60   ~ 0
Paddle_1
Text Label 3200 5550 0    60   ~ 0
Button_0
Text Label 3200 5650 0    60   ~ 0
Pvref
Text Label 3200 5750 0    60   ~ 0
Bcommon
Text Label 3200 5850 0    60   ~ 0
Paddle_0
$Comp
L CONN_02X05 J?
U 1 1 59DA6B1F
P 2950 6500
F 0 "J?" H 2950 6800 50  0000 C CNN
F 1 "CONN_02X05" H 2950 6200 50  0000 C CNN
F 2 "" H 2950 5300 50  0001 C CNN
F 3 "" H 2950 5300 50  0001 C CNN
	1    2950 6500
	1    0    0    -1  
$EndComp
Text Label 2700 6300 2    60   ~ 0
Button_8
Text Label 2700 6400 2    60   ~ 0
Button_9
Text Label 2700 6500 2    60   ~ 0
Button_6
Text Label 2700 6600 2    60   ~ 0
Button_7
Text Label 2700 6700 2    60   ~ 0
Paddle_3
Text Label 3200 6300 0    60   ~ 0
Button_5
Text Label 3200 6400 0    60   ~ 0
Pvref
Text Label 3200 6500 0    60   ~ 0
Bcommon
Text Label 3200 6600 0    60   ~ 0
Paddle_2
$Comp
L R R?
U 1 1 59DAF48A
P 1850 7350
F 0 "R?" V 1930 7350 50  0000 C CNN
F 1 "470" V 1850 7350 50  0000 C CNN
F 2 "" V 1780 7350 50  0001 C CNN
F 3 "" H 1850 7350 50  0001 C CNN
	1    1850 7350
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 59DAF4C5
P 1850 7550
F 0 "R?" V 1930 7550 50  0000 C CNN
F 1 "470" V 1850 7550 50  0000 C CNN
F 2 "" V 1780 7550 50  0001 C CNN
F 3 "" H 1850 7550 50  0001 C CNN
	1    1850 7550
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 59DAF503
P 1850 7750
F 0 "R?" V 1930 7750 50  0000 C CNN
F 1 "470" V 1850 7750 50  0000 C CNN
F 2 "" V 1780 7750 50  0001 C CNN
F 3 "" H 1850 7750 50  0001 C CNN
	1    1850 7750
	0    1    1    0   
$EndComp
$Comp
L CONN_02X05 J?
U 1 1 59DAFB53
P 6500 5750
F 0 "J?" H 6500 6050 50  0000 C CNN
F 1 "CONN_02X05" H 6500 5450 50  0000 C CNN
F 2 "" H 6500 4550 50  0001 C CNN
F 3 "" H 6500 4550 50  0001 C CNN
	1    6500 5750
	1    0    0    -1  
$EndComp
Text Label 6250 5550 2    60   ~ 0
Button_3
Text Label 6250 5650 2    60   ~ 0
Button_4
Text Label 6250 5750 2    60   ~ 0
Button_1
Text Label 6250 5850 2    60   ~ 0
Button_2
Text Label 6250 5950 2    60   ~ 0
Paddle_1
Text Label 6750 5550 0    60   ~ 0
Button_0
Text Label 6750 5650 0    60   ~ 0
Pvref
Text Label 6750 5750 0    60   ~ 0
Bcommon
Text Label 6750 5850 0    60   ~ 0
Paddle_0
$Comp
L CONN_02X05 J?
U 1 1 59DAFB62
P 6500 6500
F 0 "J?" H 6500 6800 50  0000 C CNN
F 1 "CONN_02X05" H 6500 6200 50  0000 C CNN
F 2 "" H 6500 5300 50  0001 C CNN
F 3 "" H 6500 5300 50  0001 C CNN
	1    6500 6500
	1    0    0    -1  
$EndComp
Text Label 6250 6300 2    60   ~ 0
Button_8
Text Label 6250 6400 2    60   ~ 0
Button_9
Text Label 6250 6500 2    60   ~ 0
Button_6
Text Label 6250 6600 2    60   ~ 0
Button_7
Text Label 6250 6700 2    60   ~ 0
Paddle_3
Text Label 6750 6300 0    60   ~ 0
Button_5
Text Label 6750 6400 0    60   ~ 0
Pvref
Text Label 6750 6500 0    60   ~ 0
Bcommon
Text Label 6750 6600 0    60   ~ 0
Paddle_2
Text Label 5850 7950 1    60   ~ 0
+5V
Text Label 6050 7950 1    60   ~ 0
Button_0
Text Label 6250 7950 1    60   ~ 0
Joystick_0X
Text Label 6450 7950 1    60   ~ 0
GND
Text Label 6650 7950 1    60   ~ 0
GND
Text Label 6850 7950 1    60   ~ 0
Joystick_0Y
Text Label 7050 7950 1    60   ~ 0
Button_1
Text Label 7250 7950 1    60   ~ 0
+5V
Text Label 5950 7950 1    60   ~ 0
+5V
Text Label 6150 7950 1    60   ~ 0
Button_3
Text Label 6350 7950 1    60   ~ 0
Joystick_1X
Text Label 6550 7950 1    60   ~ 0
GND
Text Label 6750 7950 1    60   ~ 0
Joystick_1Y
Text Label 6950 7950 1    60   ~ 0
Button_2
Text Label 7150 7950 1    60   ~ 0
+5V
Text Notes 6400 1350 0    60   ~ 0
ColecoVision
$Comp
L CONN_02X05 J?
U 1 1 5A04E0DF
P 6550 1750
F 0 "J?" H 6550 2050 50  0000 C CNN
F 1 "CONN_02X05" H 6550 1450 50  0000 C CNN
F 2 "" H 6550 550 50  0001 C CNN
F 3 "" H 6550 550 50  0001 C CNN
	1    6550 1750
	1    0    0    -1  
$EndComp
Text Label 6300 1550 2    60   ~ 0
Button_3
Text Label 6300 1650 2    60   ~ 0
Button_4
Text Label 6300 1750 2    60   ~ 0
Button_1
Text Label 6300 1850 2    60   ~ 0
Button_2
Text Label 6300 1950 2    60   ~ 0
Paddle_1
Text Label 6800 1550 0    60   ~ 0
Button_0
Text Label 6800 1650 0    60   ~ 0
Pvref
Text Label 6800 1750 0    60   ~ 0
Bcommon
Text Label 6800 1850 0    60   ~ 0
Paddle_0
$Comp
L CONN_02X05 J?
U 1 1 5A04E0EE
P 6550 2500
F 0 "J?" H 6550 2800 50  0000 C CNN
F 1 "CONN_02X05" H 6550 2200 50  0000 C CNN
F 2 "" H 6550 1300 50  0001 C CNN
F 3 "" H 6550 1300 50  0001 C CNN
	1    6550 2500
	1    0    0    -1  
$EndComp
Text Label 6300 2300 2    60   ~ 0
Button_8
Text Label 6300 2400 2    60   ~ 0
Button_9
Text Label 6300 2500 2    60   ~ 0
Button_6
Text Label 6300 2600 2    60   ~ 0
Button_7
Text Label 6300 2700 2    60   ~ 0
Paddle_3
Text Label 6800 2300 0    60   ~ 0
Button_5
Text Label 6800 2400 0    60   ~ 0
Pvref
Text Label 6800 2500 0    60   ~ 0
Bcommon
Text Label 6800 2600 0    60   ~ 0
Paddle_2
$Comp
L DB9_FEMALE J?
U 1 1 5A04EF6A
P 6550 4300
F 0 "J?" H 6550 4850 50  0000 C CNN
F 1 "DB9_FEMALE" H 6550 3725 50  0000 C CNN
F 2 "" H 6550 4300 50  0001 C CNN
F 3 "" H 6550 4300 50  0001 C CNN
	1    6550 4300
	0    1    1    0   
$EndComp
Text Label 6950 4000 1    60   ~ 0
Up
Text Label 6850 4000 1    60   ~ 0
Fire
Text Label 6750 4000 1    60   ~ 0
Down
Text Label 6650 4000 1    60   ~ 0
NC
Text Label 6550 4000 1    60   ~ 0
Left
Text Label 6450 4000 1    60   ~ 0
Select_2
Text Label 6350 4000 1    60   ~ 0
Right
Text Label 6250 4000 1    60   ~ 0
NC
Text Label 6150 4000 1    60   ~ 0
Select_1
$Comp
L D D?
U 1 1 5A04F9F2
P 7550 2500
F 0 "D?" H 7550 2600 50  0000 C CNN
F 1 "D" H 7550 2400 50  0000 C CNN
F 2 "" H 7550 2500 50  0001 C CNN
F 3 "" H 7550 2500 50  0001 C CNN
	1    7550 2500
	0    1    1    0   
$EndComp
$Comp
L D D?
U 1 1 5A04FC18
P 7850 2500
F 0 "D?" H 7850 2600 50  0000 C CNN
F 1 "D" H 7850 2400 50  0000 C CNN
F 2 "" H 7850 2500 50  0001 C CNN
F 3 "" H 7850 2500 50  0001 C CNN
	1    7850 2500
	0    1    1    0   
$EndComp
Text Notes 9750 5300 0    60   ~ 0
Amiga/Sega Master
$Comp
L CONN_02X05 J?
U 1 1 5A2C2807
P 9900 5700
F 0 "J?" H 9900 6000 50  0000 C CNN
F 1 "CONN_02X05" H 9900 5400 50  0000 C CNN
F 2 "" H 9900 4500 50  0001 C CNN
F 3 "" H 9900 4500 50  0001 C CNN
	1    9900 5700
	1    0    0    -1  
$EndComp
Text Label 9650 5500 2    60   ~ 0
Button_3
Text Label 9650 5600 2    60   ~ 0
Button_4
Text Label 9650 5700 2    60   ~ 0
Button_1
Text Label 9650 5800 2    60   ~ 0
Button_2
Text Label 9650 5900 2    60   ~ 0
Paddle_1
Text Label 10150 5500 0    60   ~ 0
Button_0
Text Label 10150 5600 0    60   ~ 0
Pvref
Text Label 10150 5700 0    60   ~ 0
Bcommon
Text Label 10150 5800 0    60   ~ 0
Paddle_0
$Comp
L CONN_02X05 J?
U 1 1 5A2C2816
P 9900 6450
F 0 "J?" H 9900 6750 50  0000 C CNN
F 1 "CONN_02X05" H 9900 6150 50  0000 C CNN
F 2 "" H 9900 5250 50  0001 C CNN
F 3 "" H 9900 5250 50  0001 C CNN
	1    9900 6450
	1    0    0    -1  
$EndComp
Text Label 9650 6250 2    60   ~ 0
Button_8
Text Label 9650 6350 2    60   ~ 0
Button_9
Text Label 9650 6450 2    60   ~ 0
Button_6
Text Label 9650 6550 2    60   ~ 0
Button_7
Text Label 9650 6650 2    60   ~ 0
Paddle_3
Text Label 10150 6250 0    60   ~ 0
Button_5
Text Label 10150 6350 0    60   ~ 0
Pvref
Text Label 10150 6450 0    60   ~ 0
Bcommon
Text Label 10150 6550 0    60   ~ 0
Paddle_2
$Comp
L DB9_FEMALE J?
U 1 1 5A2C2825
P 9900 8250
F 0 "J?" H 9900 8800 50  0000 C CNN
F 1 "DB9_FEMALE" H 9900 7675 50  0000 C CNN
F 2 "" H 9900 8250 50  0001 C CNN
F 3 "" H 9900 8250 50  0001 C CNN
	1    9900 8250
	0    1    1    0   
$EndComp
Text Label 10300 7950 1    60   ~ 0
Up
Text Label 10200 7950 1    60   ~ 0
Button_1
Text Label 10100 7950 1    60   ~ 0
Down
Text Label 10000 7950 1    60   ~ 0
NC
Text Label 9900 7950 1    60   ~ 0
Left
Text Label 9800 7950 1    60   ~ 0
GND
Text Label 9700 7950 1    60   ~ 0
Right
Text Label 9600 7950 1    60   ~ 0
Button_2
Text Label 9500 7950 1    60   ~ 0
NC
Wire Wire Line
	2650 1950 2500 1950
Wire Wire Line
	2500 1950 2500 2800
Wire Wire Line
	2200 1850 2650 1850
Wire Wire Line
	2200 2150 2700 2150
Wire Wire Line
	2700 2150 2700 2800
Wire Wire Line
	2150 1750 2650 1750
Wire Wire Line
	2150 2200 2900 2200
Wire Wire Line
	2900 2200 2900 2800
Wire Wire Line
	2100 1650 2650 1650
Wire Wire Line
	2100 2250 3100 2250
Wire Wire Line
	3100 2250 3100 2800
Wire Wire Line
	2050 1550 2650 1550
Wire Wire Line
	2050 2300 3300 2300
Wire Wire Line
	3300 2300 3300 2800
Wire Wire Line
	3150 1850 3400 1850
Wire Wire Line
	3400 1850 3400 2400
Wire Wire Line
	3400 2400 2600 2400
Wire Wire Line
	2600 2400 2600 2800
Wire Wire Line
	3150 1750 3600 1750
Wire Wire Line
	2800 2450 3600 2450
Wire Wire Line
	2800 2450 2800 2800
Wire Wire Line
	3150 1650 3650 1650
Wire Wire Line
	3000 2500 3650 2500
Wire Wire Line
	3000 2500 3000 2800
Wire Wire Line
	3150 1550 3700 1550
Wire Wire Line
	3200 2550 3700 2550
Wire Wire Line
	3200 2550 3200 2800
Wire Wire Line
	3200 5550 3750 5550
Wire Wire Line
	3750 5550 3750 6900
Wire Wire Line
	3750 6900 2850 6900
Wire Wire Line
	2850 6900 2850 7950
Wire Wire Line
	2550 7950 2550 6900
Wire Wire Line
	2550 6900 2100 6900
Wire Wire Line
	2100 6900 2100 5750
Wire Wire Line
	2100 5750 2700 5750
Wire Wire Line
	2650 6950 2650 7950
Wire Wire Line
	2650 6950 3800 6950
Wire Wire Line
	3800 6950 3800 6300
Wire Wire Line
	3800 6300 3200 6300
Wire Wire Line
	2750 7950 2750 7000
Wire Wire Line
	2750 7000 3850 7000
Wire Wire Line
	3850 7000 3850 5650
Wire Wire Line
	3850 5650 3200 5650
Wire Wire Line
	3200 6400 3850 6400
Connection ~ 3850 6400
Wire Wire Line
	3050 7950 3050 7100
Wire Wire Line
	3050 7100 2050 7100
Wire Wire Line
	2050 7100 2050 5950
Wire Wire Line
	2050 5950 2700 5950
Wire Wire Line
	3150 7950 3150 7100
Wire Wire Line
	3150 7100 3950 7100
Wire Wire Line
	3950 7100 3950 6600
Wire Wire Line
	3950 6600 3200 6600
Wire Wire Line
	3250 7950 3250 7150
Wire Wire Line
	3250 7150 2000 7150
Wire Wire Line
	2000 7150 2000 6700
Wire Wire Line
	2000 6700 2700 6700
Wire Wire Line
	3350 7950 3350 7150
Wire Wire Line
	3350 7150 4000 7150
Wire Wire Line
	4000 7150 4000 5850
Wire Wire Line
	4000 5850 3200 5850
Wire Wire Line
	2000 7350 2850 7350
Connection ~ 2850 7350
Wire Wire Line
	2950 7950 2950 7900
Wire Wire Line
	2950 7900 1500 7900
Wire Wire Line
	1500 7900 1500 7350
Wire Wire Line
	1500 7350 1700 7350
Wire Wire Line
	1700 7550 1500 7550
Connection ~ 1500 7550
Wire Wire Line
	1700 7750 1500 7750
Connection ~ 1500 7750
Wire Wire Line
	2000 7550 2650 7550
Connection ~ 2650 7550
Wire Wire Line
	2000 7750 2550 7750
Connection ~ 2550 7750
Wire Wire Line
	3200 5750 3850 5750
Connection ~ 3850 5750
Wire Wire Line
	3200 6500 3850 6500
Connection ~ 3850 6500
Wire Wire Line
	6050 7950 6050 6900
Wire Wire Line
	6050 6900 7300 6900
Wire Wire Line
	7300 6900 7300 5550
Wire Wire Line
	7300 5550 6750 5550
Wire Wire Line
	6750 5750 7350 5750
Wire Wire Line
	7350 5750 7350 6950
Wire Wire Line
	7350 6500 6750 6500
Wire Wire Line
	7350 6950 6450 6950
Wire Wire Line
	6450 6950 6450 7950
Connection ~ 7350 6500
Wire Wire Line
	6550 7950 6550 6950
Connection ~ 6550 6950
Wire Wire Line
	6650 7950 6650 6950
Connection ~ 6650 6950
Wire Wire Line
	7050 7950 7050 7000
Wire Wire Line
	7050 7000 5750 7000
Wire Wire Line
	5750 7000 5750 5750
Wire Wire Line
	5750 5750 6250 5750
Wire Wire Line
	6950 7950 6950 7050
Wire Wire Line
	6950 7050 7400 7050
Wire Wire Line
	7400 7050 7400 6300
Wire Wire Line
	7400 6300 6750 6300
Wire Wire Line
	6150 7950 6150 7050
Wire Wire Line
	6150 7050 5700 7050
Wire Wire Line
	5700 7050 5700 6500
Wire Wire Line
	5700 6500 6250 6500
Wire Wire Line
	7250 7950 7250 7100
Wire Wire Line
	7250 7100 7450 7100
Wire Wire Line
	7450 7100 7450 5650
Wire Wire Line
	7450 6400 6750 6400
Wire Wire Line
	7450 5650 6750 5650
Connection ~ 7450 6400
Wire Wire Line
	6750 5850 7500 5850
Wire Wire Line
	7500 5850 7500 7150
Wire Wire Line
	7500 7150 6250 7150
Wire Wire Line
	6250 7150 6250 7950
Wire Wire Line
	6250 5950 5650 5950
Wire Wire Line
	5650 5950 5650 7200
Wire Wire Line
	5650 7200 6850 7200
Wire Wire Line
	6850 7200 6850 7950
Wire Wire Line
	6750 6600 7550 6600
Wire Wire Line
	7550 6600 7550 7250
Wire Wire Line
	7550 7250 6350 7250
Wire Wire Line
	6350 7250 6350 7950
Wire Wire Line
	6250 6700 5600 6700
Wire Wire Line
	5600 6700 5600 7300
Wire Wire Line
	5600 7300 6750 7300
Wire Wire Line
	6750 7300 6750 7950
Wire Wire Line
	5800 3350 6350 3350
Wire Wire Line
	6350 3350 6350 4000
Wire Wire Line
	5750 3400 6550 3400
Wire Wire Line
	6550 3400 6550 4000
Wire Wire Line
	5700 3450 6750 3450
Wire Wire Line
	6750 3450 6750 4000
Wire Wire Line
	5650 3500 6950 3500
Wire Wire Line
	6950 3500 6950 4000
Wire Wire Line
	6450 3650 7300 3650
Wire Wire Line
	6450 3650 6450 4000
Wire Wire Line
	6850 3750 7850 3750
Wire Wire Line
	6850 3750 6850 4000
Wire Wire Line
	6800 1550 7850 1550
Wire Wire Line
	6800 1750 7300 1750
Wire Wire Line
	5650 1550 6300 1550
Wire Wire Line
	5800 3350 5800 1850
Wire Wire Line
	5800 1850 6300 1850
Wire Wire Line
	5750 3400 5750 1750
Wire Wire Line
	5750 1750 6300 1750
Wire Wire Line
	5700 3450 5700 1650
Wire Wire Line
	5700 1650 6300 1650
Wire Wire Line
	5650 3500 5650 1550
Wire Wire Line
	7300 1750 7300 3650
Wire Wire Line
	7550 3750 7550 2650
Wire Wire Line
	7550 2350 7550 2300
Wire Wire Line
	7550 2300 6800 2300
Wire Wire Line
	7850 3750 7850 2650
Connection ~ 7550 3750
Wire Wire Line
	7850 1550 7850 2350
Wire Wire Line
	6150 4000 6150 3200
Wire Wire Line
	6150 3200 7250 3200
Wire Wire Line
	7250 3200 7250 2500
Wire Wire Line
	7250 2500 6800 2500
Wire Wire Line
	2200 2150 2200 1850
Wire Wire Line
	2150 2200 2150 1750
Wire Wire Line
	2100 2250 2100 1650
Wire Wire Line
	2050 2300 2050 1550
Wire Wire Line
	3600 2450 3600 1750
Wire Wire Line
	3650 2500 3650 1650
Wire Wire Line
	3700 2550 3700 1550
Wire Wire Line
	9150 7300 9700 7300
Wire Wire Line
	9700 7300 9700 7950
Wire Wire Line
	9100 7350 9900 7350
Wire Wire Line
	9900 7350 9900 7950
Wire Wire Line
	9050 7400 10100 7400
Wire Wire Line
	10100 7400 10100 7950
Wire Wire Line
	9000 7450 10300 7450
Wire Wire Line
	10300 7450 10300 7950
Wire Wire Line
	9800 7600 9800 7950
Wire Wire Line
	11200 7700 10200 7700
Wire Wire Line
	10200 7700 10200 7950
Wire Wire Line
	10150 5500 11200 5500
Wire Wire Line
	10150 5700 10650 5700
Wire Wire Line
	9000 5500 9650 5500
Wire Wire Line
	9150 7300 9150 5800
Wire Wire Line
	9150 5800 9650 5800
Wire Wire Line
	9100 7350 9100 5700
Wire Wire Line
	9100 5700 9650 5700
Wire Wire Line
	9050 7400 9050 5600
Wire Wire Line
	9050 5600 9650 5600
Wire Wire Line
	9000 7450 9000 5500
Wire Wire Line
	10650 5700 10650 7600
Wire Wire Line
	10900 6250 10900 7500
Wire Wire Line
	10900 6250 10150 6250
Wire Wire Line
	11200 5500 11200 7700
Wire Wire Line
	10150 6450 10650 6450
Connection ~ 10650 6450
Wire Wire Line
	10650 7600 9800 7600
Wire Wire Line
	9600 7950 9600 7500
Wire Wire Line
	9600 7500 10900 7500
Text Notes 13150 5300 0    60   ~ 0
MSX
$Comp
L CONN_02X05 J?
U 1 1 5A845C4A
P 13300 5700
F 0 "J?" H 13300 6000 50  0000 C CNN
F 1 "CONN_02X05" H 13300 5400 50  0000 C CNN
F 2 "" H 13300 4500 50  0001 C CNN
F 3 "" H 13300 4500 50  0001 C CNN
	1    13300 5700
	1    0    0    -1  
$EndComp
Text Label 13050 5500 2    60   ~ 0
Button_3
Text Label 13050 5600 2    60   ~ 0
Button_4
Text Label 13050 5700 2    60   ~ 0
Button_1
Text Label 13050 5800 2    60   ~ 0
Button_2
Text Label 13050 5900 2    60   ~ 0
Paddle_1
Text Label 13550 5500 0    60   ~ 0
Button_0
Text Label 13550 5600 0    60   ~ 0
Pvref
Text Label 13550 5700 0    60   ~ 0
Bcommon
Text Label 13550 5800 0    60   ~ 0
Paddle_0
$Comp
L CONN_02X05 J?
U 1 1 5A845C59
P 13300 6450
F 0 "J?" H 13300 6750 50  0000 C CNN
F 1 "CONN_02X05" H 13300 6150 50  0000 C CNN
F 2 "" H 13300 5250 50  0001 C CNN
F 3 "" H 13300 5250 50  0001 C CNN
	1    13300 6450
	1    0    0    -1  
$EndComp
Text Label 13050 6250 2    60   ~ 0
Button_8
Text Label 13050 6350 2    60   ~ 0
Button_9
Text Label 13050 6450 2    60   ~ 0
Button_6
Text Label 13050 6550 2    60   ~ 0
Button_7
Text Label 13050 6650 2    60   ~ 0
Paddle_3
Text Label 13550 6250 0    60   ~ 0
Button_5
Text Label 13550 6350 0    60   ~ 0
Pvref
Text Label 13550 6450 0    60   ~ 0
Bcommon
Text Label 13550 6550 0    60   ~ 0
Paddle_2
$Comp
L DB9_FEMALE J?
U 1 1 5A845C68
P 13300 8250
F 0 "J?" H 13300 8800 50  0000 C CNN
F 1 "DB9_FEMALE" H 13300 7675 50  0000 C CNN
F 2 "" H 13300 8250 50  0001 C CNN
F 3 "" H 13300 8250 50  0001 C CNN
	1    13300 8250
	0    1    1    0   
$EndComp
Text Label 13700 7950 1    60   ~ 0
Up
Text Label 13600 7950 1    60   ~ 0
Button_1
Text Label 13500 7950 1    60   ~ 0
Down
Text Label 13400 7950 1    60   ~ 0
Button_2
Text Label 13300 7950 1    60   ~ 0
Left
Text Label 13200 7950 1    60   ~ 0
Output
Text Label 13100 7950 1    60   ~ 0
Right
Text Label 13000 7950 1    60   ~ 0
GND
Text Label 12900 7950 1    60   ~ 0
+5V
Wire Wire Line
	12550 7300 13100 7300
Wire Wire Line
	13100 7300 13100 7950
Wire Wire Line
	12500 7350 13300 7350
Wire Wire Line
	13300 7350 13300 7950
Wire Wire Line
	12450 7400 13500 7400
Wire Wire Line
	13500 7400 13500 7950
Wire Wire Line
	12400 7450 13700 7450
Wire Wire Line
	13700 7450 13700 7950
Wire Wire Line
	14600 7700 13600 7700
Wire Wire Line
	13600 7700 13600 7950
Wire Wire Line
	13550 5500 14600 5500
Wire Wire Line
	13550 5700 14050 5700
Wire Wire Line
	12400 5500 13050 5500
Wire Wire Line
	12550 7300 12550 5800
Wire Wire Line
	12550 5800 13050 5800
Wire Wire Line
	12500 7350 12500 5700
Wire Wire Line
	12500 5700 13050 5700
Wire Wire Line
	12450 7400 12450 5600
Wire Wire Line
	12450 5600 13050 5600
Wire Wire Line
	12400 7450 12400 5500
Wire Wire Line
	14050 5700 14050 7600
Wire Wire Line
	14300 6250 14300 7500
Wire Wire Line
	14300 6250 13550 6250
Wire Wire Line
	14600 5500 14600 7700
Wire Wire Line
	13550 6450 14050 6450
Connection ~ 14050 6450
Wire Wire Line
	14050 7600 13000 7600
Wire Wire Line
	13000 7600 13000 7950
Wire Wire Line
	14300 7500 13400 7500
Wire Wire Line
	13400 7500 13400 7950
$EndSCHEMATC
