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
Text Notes 2550 1900 0    60   ~ 0
Atari/Commodore
$Comp
L CONN_02X03 P?
U 1 1 57882AC7
P 6900 2300
F 0 "P?" H 6900 2500 50  0000 C CNN
F 1 "CONN_02X03" H 6900 2100 50  0000 C CNN
F 2 "" H 6900 1100 50  0000 C CNN
F 3 "" H 6900 1100 50  0000 C CNN
	1    6900 2300
	1    0    0    -1  
$EndComp
Text Notes 6600 2000 0    60   ~ 0
TRS-80 CoCo
$Comp
L CONN_02X08 P?
U 1 1 57882BA8
P 10800 4750
F 0 "P?" H 10800 5200 50  0000 C CNN
F 1 "CONN_02X08" V 10800 4750 50  0000 C CNN
F 2 "" H 10800 3550 50  0000 C CNN
F 3 "" H 10800 3550 50  0000 C CNN
	1    10800 4750
	1    0    0    -1  
$EndComp
Text Notes 6200 4350 0    60   ~ 0
IBM Gameport
Text Notes 10600 4200 0    60   ~ 0
BBC Micro
Text GLabel 6650 2200 0    60   Input ~ 0
Paddle_0
Text GLabel 6650 2300 0    60   Input ~ 0
GND
Text GLabel 6650 2400 0    60   Input ~ 0
+5V
Text GLabel 7150 2200 2    60   Input ~ 0
Paddle_1
Text GLabel 7150 2300 2    60   Input ~ 0
Fire_0
Text GLabel 7150 2400 2    60   Input ~ 0
Fire_1
Text GLabel 11050 4700 2    60   Input ~ 0
GND
Text GLabel 10550 4700 0    60   Input ~ 0
Paddle_1
Text GLabel 11050 4600 2    60   Input ~ 0
GND
Text GLabel 10550 4600 0    60   Input ~ 0
GND
Text GLabel 10550 5100 0    60   Input ~ 0
Paddle_0
Text GLabel 10550 4400 0    60   Input ~ 0
+5V
Text GLabel 10550 4500 0    60   Input ~ 0
GND
Text GLabel 10550 4800 0    60   Input ~ 0
Light_Pen
Text GLabel 10550 4900 0    60   Input ~ 0
VREF
Text GLabel 10550 5000 0    60   Input ~ 0
Fire_0
Text GLabel 11050 4400 2    60   Input ~ 0
GND
Text GLabel 11050 4500 2    60   Input ~ 0
Paddle_3
Text GLabel 11050 4800 2    60   Input ~ 0
Fire_1
Text GLabel 11050 4900 2    60   Input ~ 0
Paddle_2
Text GLabel 11050 5000 2    60   Input ~ 0
VREF
$Comp
L DB9_FEMALE J?
U 1 1 59D93719
P 2900 3650
F 0 "J?" H 2900 4200 50  0000 C CNN
F 1 "DB9_FEMALE" H 2900 3075 50  0000 C CNN
F 2 "" H 2900 3650 50  0001 C CNN
F 3 "" H 2900 3650 50  0001 C CNN
	1    2900 3650
	0    1    1    0   
$EndComp
$Comp
L DB15_MALE J?
U 1 1 59D93A2C
P 6550 7250
F 0 "J?" H 6570 8100 50  0000 C CNN
F 1 "DB15_MALE" H 6550 6375 50  0000 C CNN
F 2 "" H 6550 7250 50  0001 C CNN
F 3 "" H 6550 7250 50  0001 C CNN
	1    6550 7250
	0    1    1    0   
$EndComp
$Comp
L DB15_MALE J?
U 1 1 59D93B5E
P 10800 6100
F 0 "J?" H 10820 6950 50  0000 C CNN
F 1 "DB15_MALE" H 10800 5225 50  0000 C CNN
F 2 "" H 10800 6100 50  0001 C CNN
F 3 "" H 10800 6100 50  0001 C CNN
	1    10800 6100
	0    1    1    0   
$EndComp
$Comp
L CONN_02X05 J?
U 1 1 59D93F85
P 2900 2300
F 0 "J?" H 2900 2600 50  0000 C CNN
F 1 "CONN_02X05" H 2900 2000 50  0000 C CNN
F 2 "" H 2900 1100 50  0001 C CNN
F 3 "" H 2900 1100 50  0001 C CNN
	1    2900 2300
	1    0    0    -1  
$EndComp
Text Label 2650 2100 2    60   ~ 0
Button_3
Text Label 2650 2200 2    60   ~ 0
Button_4
Text Label 3300 3350 1    60   ~ 0
Up
Text Label 3200 3350 1    60   ~ 0
Fire
Text Label 3100 3350 1    60   ~ 0
Down
Text Label 3000 3350 1    60   ~ 0
+5V
Text Label 2900 3350 1    60   ~ 0
Left
Text Label 2800 3350 1    60   ~ 0
GND
Text Label 2700 3350 1    60   ~ 0
Right
Text Label 2600 3350 1    60   ~ 0
Paddle_0
Text Label 2500 3350 1    60   ~ 0
Paddle_1
Text Label 2650 2300 2    60   ~ 0
Button_1
Text Label 2650 2400 2    60   ~ 0
Button_2
Text Label 2650 2500 2    60   ~ 0
Paddle_1
Text Label 3150 2100 0    60   ~ 0
Button_0
Text Label 3150 2200 0    60   ~ 0
Pvref
Text Label 3150 2300 0    60   ~ 0
Bcommon
Text Label 3150 2400 0    60   ~ 0
Paddle_0
$Comp
L DB9_MALE J?
U 1 1 59D939A4
P 2950 7250
F 0 "J?" H 2950 7800 50  0000 C CNN
F 1 "DB9_MALE" H 2950 6675 50  0000 C CNN
F 2 "" H 2950 7250 50  0001 C CNN
F 3 "" H 2950 7250 50  0001 C CNN
	1    2950 7250
	0    1    1    0   
$EndComp
Text Label 2550 6950 1    60   ~ 0
Fire_1
Text Label 2650 6950 1    60   ~ 0
Fire_2
Text Label 2750 6950 1    60   ~ 0
+5V
Text Label 2850 6950 1    60   ~ 0
Fire_0
Text Label 2950 6950 1    60   ~ 0
GND
Text Label 3050 6950 1    60   ~ 0
Paddle_1
Text Label 3150 6950 1    60   ~ 0
Paddle_2
Text Label 3250 6950 1    60   ~ 0
Paddle_3
Text Label 3350 6950 1    60   ~ 0
Paddle_0
Wire Wire Line
	2650 2500 2500 2500
Wire Wire Line
	2500 2500 2500 3350
Wire Wire Line
	2650 2400 2450 2400
Wire Wire Line
	2450 2400 2450 2700
Wire Wire Line
	2450 2700 2700 2700
Wire Wire Line
	2700 2700 2700 3350
Wire Wire Line
	2650 2300 2400 2300
Wire Wire Line
	2400 2300 2400 2750
Wire Wire Line
	2400 2750 2900 2750
Wire Wire Line
	2900 2750 2900 3350
Wire Wire Line
	2650 2200 2350 2200
Wire Wire Line
	2350 2200 2350 2800
Wire Wire Line
	2350 2800 3100 2800
Wire Wire Line
	3100 2800 3100 3350
Wire Wire Line
	2650 2100 2300 2100
Wire Wire Line
	2300 2100 2300 2850
Wire Wire Line
	2300 2850 3300 2850
Wire Wire Line
	3300 2850 3300 3350
Wire Wire Line
	3150 2400 3400 2400
Wire Wire Line
	3400 2400 3400 2950
Wire Wire Line
	3400 2950 2600 2950
Wire Wire Line
	2600 2950 2600 3350
Wire Wire Line
	3150 2300 3450 2300
Wire Wire Line
	3450 2300 3450 3000
Wire Wire Line
	3450 3000 2800 3000
Wire Wire Line
	2800 3000 2800 3350
Wire Wire Line
	3150 2200 3500 2200
Wire Wire Line
	3500 2200 3500 3050
Wire Wire Line
	3500 3050 3000 3050
Wire Wire Line
	3000 3050 3000 3350
Wire Wire Line
	3150 2100 3550 2100
Wire Wire Line
	3550 2100 3550 3100
Wire Wire Line
	3550 3100 3200 3100
Wire Wire Line
	3200 3100 3200 3350
Text Notes 2800 4350 0    60   ~ 0
Apple II
$Comp
L CONN_02X05 J?
U 1 1 59DA690F
P 2950 4750
F 0 "J?" H 2950 5050 50  0000 C CNN
F 1 "CONN_02X05" H 2950 4450 50  0000 C CNN
F 2 "" H 2950 3550 50  0001 C CNN
F 3 "" H 2950 3550 50  0001 C CNN
	1    2950 4750
	1    0    0    -1  
$EndComp
Text Label 2700 4550 2    60   ~ 0
Button_3
Text Label 2700 4650 2    60   ~ 0
Button_4
Text Label 2700 4750 2    60   ~ 0
Button_1
Text Label 2700 4850 2    60   ~ 0
Button_2
Text Label 2700 4950 2    60   ~ 0
Paddle_1
Text Label 3200 4550 0    60   ~ 0
Button_0
Text Label 3200 4650 0    60   ~ 0
Pvref
Text Label 3200 4750 0    60   ~ 0
Bcommon
Text Label 3200 4850 0    60   ~ 0
Paddle_0
$Comp
L CONN_02X05 J?
U 1 1 59DA6B1F
P 2950 5500
F 0 "J?" H 2950 5800 50  0000 C CNN
F 1 "CONN_02X05" H 2950 5200 50  0000 C CNN
F 2 "" H 2950 4300 50  0001 C CNN
F 3 "" H 2950 4300 50  0001 C CNN
	1    2950 5500
	1    0    0    -1  
$EndComp
Text Label 2700 5300 2    60   ~ 0
Button_8
Text Label 2700 5400 2    60   ~ 0
Button_9
Text Label 2700 5500 2    60   ~ 0
Button_6
Text Label 2700 5600 2    60   ~ 0
Button_7
Text Label 2700 5700 2    60   ~ 0
Paddle_3
Text Label 3200 5300 0    60   ~ 0
Button_5
Text Label 3200 5400 0    60   ~ 0
Pvref
Text Label 3200 5500 0    60   ~ 0
Bcommon
Text Label 3200 5600 0    60   ~ 0
Paddle_2
Wire Wire Line
	3200 4550 3750 4550
Wire Wire Line
	3750 4550 3750 5900
Wire Wire Line
	3750 5900 2850 5900
Wire Wire Line
	2850 5900 2850 6950
Wire Wire Line
	2550 6950 2550 5900
Wire Wire Line
	2550 5900 2100 5900
Wire Wire Line
	2100 5900 2100 4750
Wire Wire Line
	2100 4750 2700 4750
Wire Wire Line
	2650 5950 2650 6950
Wire Wire Line
	2650 5950 3800 5950
Wire Wire Line
	3800 5950 3800 5300
Wire Wire Line
	3800 5300 3200 5300
Wire Wire Line
	2750 6950 2750 6000
Wire Wire Line
	2750 6000 3850 6000
Wire Wire Line
	3850 6000 3850 4650
Wire Wire Line
	3850 4650 3200 4650
Wire Wire Line
	3200 5400 3850 5400
Connection ~ 3850 5400
Wire Wire Line
	3050 6950 3050 6100
Wire Wire Line
	3050 6100 2050 6100
Wire Wire Line
	2050 6100 2050 4950
Wire Wire Line
	2050 4950 2700 4950
Wire Wire Line
	3150 6950 3150 6100
Wire Wire Line
	3150 6100 3950 6100
Wire Wire Line
	3950 6100 3950 5600
Wire Wire Line
	3950 5600 3200 5600
Wire Wire Line
	3250 6950 3250 6150
Wire Wire Line
	3250 6150 2000 6150
Wire Wire Line
	2000 6150 2000 5700
Wire Wire Line
	2000 5700 2700 5700
Wire Wire Line
	3350 6950 3350 6150
Wire Wire Line
	3350 6150 4000 6150
Wire Wire Line
	4000 6150 4000 4850
Wire Wire Line
	4000 4850 3200 4850
$Comp
L R R?
U 1 1 59DAF48A
P 1850 6350
F 0 "R?" V 1930 6350 50  0000 C CNN
F 1 "470" V 1850 6350 50  0000 C CNN
F 2 "" V 1780 6350 50  0001 C CNN
F 3 "" H 1850 6350 50  0001 C CNN
	1    1850 6350
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 59DAF4C5
P 1850 6550
F 0 "R?" V 1930 6550 50  0000 C CNN
F 1 "470" V 1850 6550 50  0000 C CNN
F 2 "" V 1780 6550 50  0001 C CNN
F 3 "" H 1850 6550 50  0001 C CNN
	1    1850 6550
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 59DAF503
P 1850 6750
F 0 "R?" V 1930 6750 50  0000 C CNN
F 1 "470" V 1850 6750 50  0000 C CNN
F 2 "" V 1780 6750 50  0001 C CNN
F 3 "" H 1850 6750 50  0001 C CNN
	1    1850 6750
	0    1    1    0   
$EndComp
Wire Wire Line
	2000 6350 2850 6350
Connection ~ 2850 6350
Wire Wire Line
	2950 6950 2950 6900
Wire Wire Line
	2950 6900 1500 6900
Wire Wire Line
	1500 6900 1500 6350
Wire Wire Line
	1500 6350 1700 6350
Wire Wire Line
	1700 6550 1500 6550
Connection ~ 1500 6550
Wire Wire Line
	1700 6750 1500 6750
Connection ~ 1500 6750
Wire Wire Line
	2000 6550 2650 6550
Connection ~ 2650 6550
Wire Wire Line
	2000 6750 2550 6750
Connection ~ 2550 6750
Wire Wire Line
	3200 4750 3850 4750
Connection ~ 3850 4750
Wire Wire Line
	3200 5500 3850 5500
Connection ~ 3850 5500
$Comp
L CONN_02X05 J?
U 1 1 59DAFB53
P 6500 4750
F 0 "J?" H 6500 5050 50  0000 C CNN
F 1 "CONN_02X05" H 6500 4450 50  0000 C CNN
F 2 "" H 6500 3550 50  0001 C CNN
F 3 "" H 6500 3550 50  0001 C CNN
	1    6500 4750
	1    0    0    -1  
$EndComp
Text Label 6250 4550 2    60   ~ 0
Button_3
Text Label 6250 4650 2    60   ~ 0
Button_4
Text Label 6250 4750 2    60   ~ 0
Button_1
Text Label 6250 4850 2    60   ~ 0
Button_2
Text Label 6250 4950 2    60   ~ 0
Paddle_1
Text Label 6750 4550 0    60   ~ 0
Button_0
Text Label 6750 4650 0    60   ~ 0
Pvref
Text Label 6750 4750 0    60   ~ 0
Bcommon
Text Label 6750 4850 0    60   ~ 0
Paddle_0
$Comp
L CONN_02X05 J?
U 1 1 59DAFB62
P 6500 5500
F 0 "J?" H 6500 5800 50  0000 C CNN
F 1 "CONN_02X05" H 6500 5200 50  0000 C CNN
F 2 "" H 6500 4300 50  0001 C CNN
F 3 "" H 6500 4300 50  0001 C CNN
	1    6500 5500
	1    0    0    -1  
$EndComp
Text Label 6250 5300 2    60   ~ 0
Button_8
Text Label 6250 5400 2    60   ~ 0
Button_9
Text Label 6250 5500 2    60   ~ 0
Button_6
Text Label 6250 5600 2    60   ~ 0
Button_7
Text Label 6250 5700 2    60   ~ 0
Paddle_3
Text Label 6750 5300 0    60   ~ 0
Button_5
Text Label 6750 5400 0    60   ~ 0
Pvref
Text Label 6750 5500 0    60   ~ 0
Bcommon
Text Label 6750 5600 0    60   ~ 0
Paddle_2
Text Label 5850 6950 1    60   ~ 0
+5V
Text Label 6050 6950 1    60   ~ 0
Button_0
Text Label 6250 6950 1    60   ~ 0
Joystick_0X
Text Label 6450 6950 1    60   ~ 0
GND
Text Label 6650 6950 1    60   ~ 0
GND
Text Label 6850 6950 1    60   ~ 0
Joystick_0Y
Text Label 7050 6950 1    60   ~ 0
Button_1
Text Label 7250 6950 1    60   ~ 0
+5V
Text Label 5950 6950 1    60   ~ 0
+5V
Text Label 6150 6950 1    60   ~ 0
Button_3
Text Label 6350 6950 1    60   ~ 0
Joystick_1X
Text Label 6550 6950 1    60   ~ 0
GND
Text Label 6750 6950 1    60   ~ 0
Joystick_1Y
Text Label 6950 6950 1    60   ~ 0
Button_2
Text Label 7150 6950 1    60   ~ 0
+5V
Wire Wire Line
	6050 6950 6050 5900
Wire Wire Line
	6050 5900 7300 5900
Wire Wire Line
	7300 5900 7300 4550
Wire Wire Line
	7300 4550 6750 4550
Wire Wire Line
	6750 4750 7350 4750
Wire Wire Line
	7350 4750 7350 5950
Wire Wire Line
	7350 5500 6750 5500
Wire Wire Line
	7350 5950 6450 5950
Wire Wire Line
	6450 5950 6450 6950
Connection ~ 7350 5500
Wire Wire Line
	6550 6950 6550 5950
Connection ~ 6550 5950
Wire Wire Line
	6650 6950 6650 5950
Connection ~ 6650 5950
Wire Wire Line
	7050 6950 7050 6000
Wire Wire Line
	7050 6000 5750 6000
Wire Wire Line
	5750 6000 5750 4750
Wire Wire Line
	5750 4750 6250 4750
Wire Wire Line
	6950 6950 6950 6050
Wire Wire Line
	6950 6050 7400 6050
Wire Wire Line
	7400 6050 7400 5300
Wire Wire Line
	7400 5300 6750 5300
Wire Wire Line
	6150 6950 6150 6050
Wire Wire Line
	6150 6050 5700 6050
Wire Wire Line
	5700 6050 5700 5500
Wire Wire Line
	5700 5500 6250 5500
Wire Wire Line
	7250 6950 7250 6100
Wire Wire Line
	7250 6100 7450 6100
Wire Wire Line
	7450 6100 7450 4650
Wire Wire Line
	7450 5400 6750 5400
Wire Wire Line
	7450 4650 6750 4650
Connection ~ 7450 5400
Wire Wire Line
	6750 4850 7500 4850
Wire Wire Line
	7500 4850 7500 6150
Wire Wire Line
	7500 6150 6250 6150
Wire Wire Line
	6250 6150 6250 6950
Wire Wire Line
	6250 4950 5650 4950
Wire Wire Line
	5650 4950 5650 6200
Wire Wire Line
	5650 6200 6850 6200
Wire Wire Line
	6850 6200 6850 6950
Wire Wire Line
	6750 5600 7550 5600
Wire Wire Line
	7550 5600 7550 6250
Wire Wire Line
	7550 6250 6350 6250
Wire Wire Line
	6350 6250 6350 6950
Wire Wire Line
	6250 5700 5600 5700
Wire Wire Line
	5600 5700 5600 6300
Wire Wire Line
	5600 6300 6750 6300
Wire Wire Line
	6750 6300 6750 6950
$EndSCHEMATC
