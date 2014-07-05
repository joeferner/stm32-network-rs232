EESchema Schematic File Version 2
LIBS:stm32-network-rs232
LIBS:stm32-network-rs232-cache
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
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
L STM32-48PIN U102
U 1 1 53B5E71C
P 3950 3200
F 0 "U102" H 2950 4950 60  0000 C CNN
F 1 "STM32-48PIN" H 3200 1400 60  0000 C CNN
F 2 "" H 3950 3200 60  0000 C CNN
F 3 "" H 3950 3200 60  0000 C CNN
	1    3950 3200
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR01
U 1 1 53B5E78F
P 2500 1400
F 0 "#PWR01" H 2500 1490 20  0001 C CNN
F 1 "+3.3V" H 2500 1490 30  0000 C CNN
F 2 "" H 2500 1400 60  0000 C CNN
F 3 "" H 2500 1400 60  0000 C CNN
	1    2500 1400
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR02
U 1 1 53B5E7A3
P 2500 5000
F 0 "#PWR02" H 2500 5050 40  0001 C CNN
F 1 "GNDPWR" H 2500 4920 40  0001 C CNN
F 2 "" H 2500 5000 60  0000 C CNN
F 3 "" H 2500 5000 60  0000 C CNN
	1    2500 5000
	1    0    0    -1  
$EndComp
$Comp
L CRYSTAL X101
U 1 1 53B5E89B
P 2400 3150
F 0 "X101" H 2400 3225 30  0000 C CNN
F 1 "8MHz" H 2400 3070 30  0000 C CNN
F 2 "" H 2400 3150 60  0000 C CNN
F 3 "" H 2400 3150 60  0000 C CNN
	1    2400 3150
	0    1    1    0   
$EndComp
$Comp
L C C101
U 1 1 53B5EF93
P 2100 2900
F 0 "C101" H 2125 2950 30  0000 L CNN
F 1 "18pF" H 2125 2850 30  0000 L CNN
F 2 "" H 2100 2900 60  0000 C CNN
F 3 "" H 2100 2900 60  0000 C CNN
	1    2100 2900
	0    1    1    0   
$EndComp
$Comp
L C C102
U 1 1 53B5EFC5
P 2100 3400
F 0 "C102" H 2125 3450 30  0000 L CNN
F 1 "18pF" H 2125 3350 30  0000 L CNN
F 2 "" H 2100 3400 60  0000 C CNN
F 3 "" H 2100 3400 60  0000 C CNN
	1    2100 3400
	0    1    1    0   
$EndComp
$Comp
L C C109
U 1 1 53B5EFF7
P 4300 1000
F 0 "C109" H 4325 1050 30  0000 L CNN
F 1 "C" H 4325 950 30  0000 L CNN
F 2 "" H 4300 1000 60  0000 C CNN
F 3 "" H 4300 1000 60  0000 C CNN
	1    4300 1000
	-1   0    0    1   
$EndComp
$Comp
L C C108
U 1 1 53B5F04E
P 4100 1000
F 0 "C108" H 4125 1050 30  0000 L CNN
F 1 "C" H 4125 950 30  0000 L CNN
F 2 "" H 4100 1000 60  0000 C CNN
F 3 "" H 4100 1000 60  0000 C CNN
	1    4100 1000
	-1   0    0    1   
$EndComp
$Comp
L C C107
U 1 1 53B5F06A
P 3900 1000
F 0 "C107" H 3925 1050 30  0000 L CNN
F 1 "C" H 3925 950 30  0000 L CNN
F 2 "" H 3900 1000 60  0000 C CNN
F 3 "" H 3900 1000 60  0000 C CNN
	1    3900 1000
	-1   0    0    1   
$EndComp
$Comp
L C C106
U 1 1 53B5F087
P 3700 1000
F 0 "C106" H 3725 1050 30  0000 L CNN
F 1 "C" H 3725 950 30  0000 L CNN
F 2 "" H 3700 1000 60  0000 C CNN
F 3 "" H 3700 1000 60  0000 C CNN
	1    3700 1000
	-1   0    0    1   
$EndComp
$Comp
L C C105
U 1 1 53B5F0B3
P 3500 1000
F 0 "C105" H 3525 1050 30  0000 L CNN
F 1 "C" H 3525 950 30  0000 L CNN
F 2 "" H 3500 1000 60  0000 C CNN
F 3 "" H 3500 1000 60  0000 C CNN
	1    3500 1000
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR03
U 1 1 53B5F241
P 4300 700
F 0 "#PWR03" H 4300 790 20  0001 C CNN
F 1 "+3.3V" H 4300 790 30  0000 C CNN
F 2 "" H 4300 700 60  0000 C CNN
F 3 "" H 4300 700 60  0000 C CNN
	1    4300 700 
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR04
U 1 1 53B5F3BD
P 4300 1300
F 0 "#PWR04" H 4300 1350 40  0001 C CNN
F 1 "GNDPWR" H 4300 1220 40  0001 C CNN
F 2 "" H 4300 1300 60  0000 C CNN
F 3 "" H 4300 1300 60  0000 C CNN
	1    4300 1300
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR05
U 1 1 53B5F60A
P 1900 3500
F 0 "#PWR05" H 1900 3550 40  0001 C CNN
F 1 "GNDPWR" H 1900 3420 40  0001 C CNN
F 2 "" H 1900 3500 60  0000 C CNN
F 3 "" H 1900 3500 60  0000 C CNN
	1    1900 3500
	1    0    0    -1  
$EndComp
$Comp
L USB J101
U 1 1 53B5FADD
P 1200 6900
F 0 "J101" H 850 7250 60  0000 L CNN
F 1 "USB" H 850 6550 60  0000 L CNN
F 2 "" H 1150 6900 60  0000 C CNN
F 3 "" H 1150 6900 60  0000 C CNN
	1    1200 6900
	1    0    0    -1  
$EndComp
Text Label 2200 3700 0    60   ~ 0
USBDP
Text Label 2200 3800 0    60   ~ 0
USBDM
$Comp
L +3.3V #PWR06
U 1 1 53B60338
P 1500 6600
F 0 "#PWR06" H 1500 6690 20  0001 C CNN
F 1 "+3.3V" H 1500 6690 30  0000 C CNN
F 2 "" H 1500 6600 60  0000 C CNN
F 3 "" H 1500 6600 60  0000 C CNN
	1    1500 6600
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR07
U 1 1 53B603BB
P 1500 7200
F 0 "#PWR07" H 1500 7250 40  0001 C CNN
F 1 "GNDPWR" H 1500 7120 40  0001 C CNN
F 2 "" H 1500 7200 60  0000 C CNN
F 3 "" H 1500 7200 60  0000 C CNN
	1    1500 7200
	1    0    0    -1  
$EndComp
$Comp
L C C103
U 1 1 53B60469
P 2200 7200
F 0 "C103" H 2225 7250 30  0000 L CNN
F 1 "47pF" H 2225 7150 30  0000 L CNN
F 2 "" H 2200 7200 60  0000 C CNN
F 3 "" H 2200 7200 60  0000 C CNN
	1    2200 7200
	1    0    0    -1  
$EndComp
$Comp
L R R102
U 1 1 53B60486
P 1900 6700
F 0 "R102" V 1845 6700 30  0000 C CNN
F 1 "22R" V 1955 6700 30  0000 C CNN
F 2 "" H 1900 6700 60  0000 C CNN
F 3 "" H 1900 6700 60  0000 C CNN
	1    1900 6700
	0    1    1    0   
$EndComp
$Comp
L R R103
U 1 1 53B60573
P 1900 7000
F 0 "R103" V 1845 7000 30  0000 C CNN
F 1 "22R" V 1955 7000 30  0000 C CNN
F 2 "" H 1900 7000 60  0000 C CNN
F 3 "" H 1900 7000 60  0000 C CNN
	1    1900 7000
	0    1    1    0   
$EndComp
$Comp
L C C104
U 1 1 53B60603
P 2400 7200
F 0 "C104" H 2425 7250 30  0000 L CNN
F 1 "47pF" H 2425 7150 30  0000 L CNN
F 2 "" H 2400 7200 60  0000 C CNN
F 3 "" H 2400 7200 60  0000 C CNN
	1    2400 7200
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR08
U 1 1 53B60B2A
P 2200 7400
F 0 "#PWR08" H 2200 7450 40  0001 C CNN
F 1 "GNDPWR" H 2200 7320 40  0001 C CNN
F 2 "" H 2200 7400 60  0000 C CNN
F 3 "" H 2200 7400 60  0000 C CNN
	1    2200 7400
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR09
U 1 1 53B60B43
P 2400 7400
F 0 "#PWR09" H 2400 7450 40  0001 C CNN
F 1 "GNDPWR" H 2400 7320 40  0001 C CNN
F 2 "" H 2400 7400 60  0000 C CNN
F 3 "" H 2400 7400 60  0000 C CNN
	1    2400 7400
	1    0    0    -1  
$EndComp
Text Label 2750 6700 2    60   ~ 0
USBDM
Text Label 2750 7000 2    60   ~ 0
USBDP
$Comp
L R R104
U 1 1 53B60C29
P 2200 6500
F 0 "R104" V 2145 6500 30  0000 C CNN
F 1 "1.5K" V 2255 6500 30  0000 C CNN
F 2 "" H 2200 6500 60  0000 C CNN
F 3 "" H 2200 6500 60  0000 C CNN
	1    2200 6500
	-1   0    0    1   
$EndComp
$Comp
L MECH M101
U 1 1 53B60D6B
P 9900 6000
F 0 "M101" H 9950 6000 60  0000 C CNN
F 1 "Mount Hole" H 10100 6000 60  0000 L CNN
F 2 "" H 9900 6000 60  0000 C CNN
F 3 "" H 9900 6000 60  0000 C CNN
	1    9900 6000
	1    0    0    -1  
$EndComp
$Comp
L MECH M102
U 1 1 53B60F6A
P 9900 6100
F 0 "M102" H 9950 6100 60  0000 C CNN
F 1 "Mount Hole" H 10100 6100 60  0000 L CNN
F 2 "" H 9900 6100 60  0000 C CNN
F 3 "" H 9900 6100 60  0000 C CNN
	1    9900 6100
	1    0    0    -1  
$EndComp
$Comp
L MECH M103
U 1 1 53B60F93
P 9900 6200
F 0 "M103" H 9950 6200 60  0000 C CNN
F 1 "Mount Hole" H 10100 6200 60  0000 L CNN
F 2 "" H 9900 6200 60  0000 C CNN
F 3 "" H 9900 6200 60  0000 C CNN
	1    9900 6200
	1    0    0    -1  
$EndComp
$Comp
L MECH M104
U 1 1 53B60FB9
P 9900 6300
F 0 "M104" H 9950 6300 60  0000 C CNN
F 1 "Mount Hole" H 10100 6300 60  0000 L CNN
F 2 "" H 9900 6300 60  0000 C CNN
F 3 "" H 9900 6300 60  0000 C CNN
	1    9900 6300
	1    0    0    -1  
$EndComp
$Comp
L MOSFET-P U101
U 1 1 53B611C2
P 2200 6100
F 0 "U101" H 2230 6220 30  0000 L CNN
F 1 "MOSFET-P" H 2225 5985 30  0000 L CNN
F 2 "" H 2200 6050 60  0000 C CNN
F 3 "" H 2200 6050 60  0000 C CNN
	1    2200 6100
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR010
U 1 1 53B613AF
P 2200 5700
F 0 "#PWR010" H 2200 5790 20  0001 C CNN
F 1 "+3.3V" H 2200 5790 30  0000 C CNN
F 2 "" H 2200 5700 60  0000 C CNN
F 3 "" H 2200 5700 60  0000 C CNN
	1    2200 5700
	1    0    0    -1  
$EndComp
$Comp
L R R101
U 1 1 53B6149B
P 1800 5900
F 0 "R101" V 1745 5900 30  0000 C CNN
F 1 "10K" V 1855 5900 30  0000 C CNN
F 2 "" H 1800 5900 60  0000 C CNN
F 3 "" H 1800 5900 60  0000 C CNN
	1    1800 5900
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR011
U 1 1 53B6163A
P 1800 5700
F 0 "#PWR011" H 1800 5790 20  0001 C CNN
F 1 "+3.3V" H 1800 5790 30  0000 C CNN
F 2 "" H 1800 5700 60  0000 C CNN
F 3 "" H 1800 5700 60  0000 C CNN
	1    1800 5700
	1    0    0    -1  
$EndComp
Text Label 1400 6100 0    60   ~ 0
USBDIS
Text Label 5900 2400 2    60   ~ 0
USBDIS
Text Notes 1550 4400 0    60   ~ 0
Flash Memory\nBOOT0=0 BOOT1=X
Text Notes 950  4100 0    60   ~ 0
RST has internal pull-up
NoConn ~ 2600 2200
NoConn ~ 2600 2700
Text Label 5900 2700 2    60   ~ 0
SWDIO
Text Label 5900 2800 2    60   ~ 0
SWCLK
Text Label 5900 2500 2    60   ~ 0
USART1TX
Text Label 5900 2600 2    60   ~ 0
USART1RX
$Comp
L FTDI-PLUG P101
U 1 1 53B62672
P 8400 1400
F 0 "P101" H 8400 2050 60  0000 L CNN
F 1 "DEBUG" H 8400 1250 60  0000 L CNN
F 2 "" H 8400 1200 60  0000 C CNN
F 3 "" H 8400 1200 60  0000 C CNN
	1    8400 1400
	1    0    0    -1  
$EndComp
Text Label 7400 1200 0    60   ~ 0
USART1RX
Text Label 7400 1300 0    60   ~ 0
USART1TX
$Comp
L GNDPWR #PWR012
U 1 1 53B62BA7
P 8000 1600
F 0 "#PWR012" H 8000 1650 40  0001 C CNN
F 1 "GNDPWR" H 8000 1520 40  0001 C CNN
F 2 "" H 8000 1600 60  0000 C CNN
F 3 "" H 8000 1600 60  0000 C CNN
	1    8000 1600
	1    0    0    -1  
$EndComp
NoConn ~ 8100 1000
NoConn ~ 8100 1100
NoConn ~ 8100 1400
$Comp
L SWD P102
U 1 1 53B63010
P 8400 2600
F 0 "P102" H 8400 3250 60  0000 L CNN
F 1 "SWD" H 8400 2450 60  0000 L CNN
F 2 "" H 8100 2500 60  0000 C CNN
F 3 "" H 8100 2500 60  0000 C CNN
	1    8400 2600
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR013
U 1 1 53B63110
P 8000 2800
F 0 "#PWR013" H 8000 2850 40  0001 C CNN
F 1 "GNDPWR" H 8000 2720 40  0001 C CNN
F 2 "" H 8000 2800 60  0000 C CNN
F 3 "" H 8000 2800 60  0000 C CNN
	1    8000 2800
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR014
U 1 1 53B63160
P 8000 1900
F 0 "#PWR014" H 8000 1990 20  0001 C CNN
F 1 "+3.3V" H 8000 1990 30  0000 C CNN
F 2 "" H 8000 1900 60  0000 C CNN
F 3 "" H 8000 1900 60  0000 C CNN
	1    8000 1900
	1    0    0    -1  
$EndComp
Text Label 2200 4100 0    60   ~ 0
nRST
Text Label 7400 2500 0    60   ~ 0
nRST
Text Label 7400 2400 0    60   ~ 0
SWDIO
Text Label 7400 2200 0    60   ~ 0
SWCLK
NoConn ~ 8100 2600
$Comp
L FTDI-PLUG P103
U 1 1 53B63918
P 10300 1400
F 0 "P103" H 10300 2050 60  0000 L CNN
F 1 "RS232-TAP" H 10300 1250 60  0000 L CNN
F 2 "" H 10300 1200 60  0000 C CNN
F 3 "" H 10300 1200 60  0000 C CNN
	1    10300 1400
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR015
U 1 1 53B6399D
P 9900 1600
F 0 "#PWR015" H 9900 1650 40  0001 C CNN
F 1 "GNDPWR" H 9900 1520 40  0001 C CNN
F 2 "" H 9900 1600 60  0000 C CNN
F 3 "" H 9900 1600 60  0000 C CNN
	1    9900 1600
	1    0    0    -1  
$EndComp
NoConn ~ 10000 1000
NoConn ~ 10000 1100
NoConn ~ 10000 1400
Text Label 9300 1200 0    60   ~ 0
USART2RX
Text Label 9300 1300 0    60   ~ 0
USART2TX
Text Label 5900 1900 2    60   ~ 0
USART2RX
Text Label 5900 1800 2    60   ~ 0
USART2TX
$Comp
L C C112
U 1 1 53B7F617
P 7450 3750
F 0 "C112" H 7475 3800 30  0000 L CNN
F 1 "0.1uF" H 7475 3700 30  0000 L CNN
F 2 "" H 7450 3750 60  0000 C CNN
F 3 "" H 7450 3750 60  0000 C CNN
	1    7450 3750
	1    0    0    -1  
$EndComp
$Comp
L MAX3221 U103
U 1 1 53B7FEE1
P 8400 4300
F 0 "U103" H 7900 5350 60  0000 L CNN
F 1 "MAX3221" H 7900 3150 60  0000 L CNN
F 2 "" H 8200 4000 60  0000 C CNN
F 3 "" H 8200 4000 60  0000 C CNN
	1    8400 4300
	1    0    0    -1  
$EndComp
$Comp
L C C113
U 1 1 53B8009A
P 7450 4150
F 0 "C113" H 7475 4200 30  0000 L CNN
F 1 "0.1uF" H 7475 4100 30  0000 L CNN
F 2 "" H 7450 4150 60  0000 C CNN
F 3 "" H 7450 4150 60  0000 C CNN
	1    7450 4150
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR016
U 1 1 53B803A1
P 7500 5600
F 0 "#PWR016" H 7500 5650 40  0001 C CNN
F 1 "GNDPWR" H 7500 5520 40  0001 C CNN
F 2 "" H 7500 5600 60  0000 C CNN
F 3 "" H 7500 5600 60  0000 C CNN
	1    7500 5600
	1    0    0    -1  
$EndComp
$Comp
L C C111
U 1 1 53B805D4
P 7200 5400
F 0 "C111" H 7225 5450 30  0000 L CNN
F 1 "0.1uF" H 7225 5350 30  0000 L CNN
F 2 "" H 7200 5400 60  0000 C CNN
F 3 "" H 7200 5400 60  0000 C CNN
	1    7200 5400
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR017
U 1 1 53B806FF
P 7200 5600
F 0 "#PWR017" H 7200 5650 40  0001 C CNN
F 1 "GNDPWR" H 7200 5520 40  0001 C CNN
F 2 "" H 7200 5600 60  0000 C CNN
F 3 "" H 7200 5600 60  0000 C CNN
	1    7200 5600
	1    0    0    -1  
$EndComp
$Comp
L C C110
U 1 1 53B80C41
P 6900 5400
F 0 "C110" H 6925 5450 30  0000 L CNN
F 1 "0.1uF" H 6925 5350 30  0000 L CNN
F 2 "" H 6900 5400 60  0000 C CNN
F 3 "" H 6900 5400 60  0000 C CNN
	1    6900 5400
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR018
U 1 1 53B80C7E
P 6900 5600
F 0 "#PWR018" H 6900 5650 40  0001 C CNN
F 1 "GNDPWR" H 6900 5520 40  0001 C CNN
F 2 "" H 6900 5600 60  0000 C CNN
F 3 "" H 6900 5600 60  0000 C CNN
	1    6900 5600
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR019
U 1 1 53B80DCA
P 7200 3200
F 0 "#PWR019" H 7200 3290 20  0001 C CNN
F 1 "+3.3V" H 7200 3290 30  0000 C CNN
F 2 "" H 7200 3200 60  0000 C CNN
F 3 "" H 7200 3200 60  0000 C CNN
	1    7200 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 2000 2600 2000
Wire Wire Line
	2500 1400 2500 2000
Wire Wire Line
	2600 1900 2500 1900
Connection ~ 2500 1900
Wire Wire Line
	2600 1800 2500 1800
Connection ~ 2500 1800
Wire Wire Line
	2600 1700 2500 1700
Connection ~ 2500 1700
Wire Wire Line
	2600 1600 2500 1600
Connection ~ 2500 1600
Wire Wire Line
	2600 4500 2500 4500
Wire Wire Line
	2500 4300 2500 5000
Wire Wire Line
	2600 4600 2500 4600
Connection ~ 2500 4600
Wire Wire Line
	2600 4700 2500 4700
Connection ~ 2500 4700
Wire Wire Line
	2600 4800 2500 4800
Connection ~ 2500 4800
Wire Wire Line
	2200 3400 2600 3400
Wire Wire Line
	2400 3400 2400 3300
Wire Wire Line
	2200 2900 2600 2900
Wire Wire Line
	2400 2900 2400 3000
Wire Wire Line
	3500 900  3500 800 
Wire Wire Line
	3500 800  4300 800 
Wire Wire Line
	4300 700  4300 900 
Wire Wire Line
	4100 900  4100 800 
Connection ~ 4100 800 
Wire Wire Line
	3900 900  3900 800 
Connection ~ 3900 800 
Wire Wire Line
	3700 900  3700 800 
Connection ~ 3700 800 
Connection ~ 4300 800 
Wire Wire Line
	4300 1100 4300 1300
Wire Wire Line
	3500 1200 4300 1200
Wire Wire Line
	3500 1200 3500 1100
Connection ~ 4300 1200
Wire Wire Line
	3700 1100 3700 1200
Connection ~ 3700 1200
Wire Wire Line
	3900 1100 3900 1200
Connection ~ 3900 1200
Wire Wire Line
	4100 1100 4100 1200
Connection ~ 4100 1200
Connection ~ 2400 2900
Wire Wire Line
	2000 2900 1900 2900
Wire Wire Line
	1900 2900 1900 3500
Wire Wire Line
	2000 3400 1900 3400
Connection ~ 1900 3400
Connection ~ 2400 3400
Wire Wire Line
	2600 3700 2200 3700
Wire Wire Line
	2600 3800 2200 3800
Wire Wire Line
	1400 6700 1500 6700
Wire Wire Line
	1500 6700 1500 6600
Wire Wire Line
	1400 7000 1500 7000
Wire Wire Line
	1500 7000 1500 7200
Wire Wire Line
	1400 7100 1500 7100
Connection ~ 1500 7100
Wire Wire Line
	1400 6800 1600 6800
Wire Wire Line
	1600 6800 1600 6700
Wire Wire Line
	1600 6700 1800 6700
Wire Wire Line
	1800 7000 1600 7000
Wire Wire Line
	1600 7000 1600 6900
Wire Wire Line
	1600 6900 1400 6900
Wire Wire Line
	2000 7000 2750 7000
Wire Wire Line
	2200 6600 2200 7100
Connection ~ 2200 7000
Wire Wire Line
	2000 6700 2750 6700
Wire Wire Line
	2400 7100 2400 6700
Connection ~ 2400 6700
Wire Wire Line
	2200 7300 2200 7400
Wire Wire Line
	2400 7300 2400 7400
Wire Wire Line
	2200 5700 2200 5900
Wire Wire Line
	2200 6400 2200 6300
Wire Wire Line
	1400 6100 2000 6100
Wire Wire Line
	1800 6100 1800 6000
Wire Wire Line
	1800 5700 1800 5800
Connection ~ 1800 6100
Wire Wire Line
	5400 2400 5900 2400
Wire Wire Line
	2600 4300 2500 4300
Connection ~ 2500 4500
Wire Wire Line
	5400 2700 5900 2700
Wire Wire Line
	5400 2800 5900 2800
Wire Wire Line
	5400 2500 5900 2500
Wire Wire Line
	5400 2600 5900 2600
Wire Wire Line
	8100 1200 7400 1200
Wire Wire Line
	8100 1300 7400 1300
Wire Wire Line
	8100 900  8000 900 
Wire Wire Line
	8000 900  8000 1600
Wire Wire Line
	8100 2100 8000 2100
Wire Wire Line
	8000 2100 8000 1900
Wire Wire Line
	8100 2300 8000 2300
Wire Wire Line
	8000 2300 8000 2800
Wire Wire Line
	2600 4100 2200 4100
Wire Wire Line
	8100 2500 7400 2500
Wire Wire Line
	8100 2200 7400 2200
Wire Wire Line
	8100 2400 7400 2400
Wire Wire Line
	10000 900  9900 900 
Wire Wire Line
	9900 900  9900 1600
Wire Wire Line
	5400 1800 5900 1800
Wire Wire Line
	5900 1900 5400 1900
Wire Wire Line
	10000 1200 9300 1200
Wire Wire Line
	10000 1300 9300 1300
Wire Wire Line
	7600 3600 7450 3600
Wire Wire Line
	7450 3600 7450 3650
Wire Wire Line
	7600 3900 7450 3900
Wire Wire Line
	7450 3900 7450 3850
Wire Wire Line
	7600 4000 7450 4000
Wire Wire Line
	7450 4000 7450 4050
Wire Wire Line
	7600 4300 7450 4300
Wire Wire Line
	7450 4300 7450 4250
Wire Wire Line
	7600 5300 7500 5300
Wire Wire Line
	7500 4900 7500 5600
Wire Wire Line
	7600 5200 7200 5200
Wire Wire Line
	7200 5200 7200 5300
Wire Wire Line
	7200 5500 7200 5600
Wire Wire Line
	7600 5100 6900 5100
Wire Wire Line
	6900 5100 6900 5300
Wire Wire Line
	6900 5500 6900 5600
Wire Wire Line
	7600 3400 7200 3400
Wire Wire Line
	7200 3200 7200 4800
Wire Wire Line
	7200 4700 7600 4700
Connection ~ 7200 3400
Wire Wire Line
	7200 4800 7600 4800
Connection ~ 7200 4700
Wire Wire Line
	7600 4900 7500 4900
Connection ~ 7500 5300
NoConn ~ 9200 4900
Text Label 6700 4500 0    60   ~ 0
USART1TX
Text Label 6700 4600 0    60   ~ 0
USART1RX
Wire Wire Line
	7600 4600 6700 4600
Wire Wire Line
	7600 4500 6700 4500
$Comp
L RS-232-DB-9-DTE P104
U 1 1 53B8374E
P 10300 4900
F 0 "P104" H 10100 5450 60  0000 L CNN
F 1 "RS-232-DB-9-DTE" H 10100 4250 60  0000 L CNN
F 2 "" H 10100 4300 60  0000 C CNN
F 3 "" H 10100 4300 60  0000 C CNN
	1    10300 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 4600 9800 4600
Wire Wire Line
	9200 4700 9800 4700
$Comp
L GNDPWR #PWR020
U 1 1 53B8429D
P 9700 5600
F 0 "#PWR020" H 9700 5650 40  0001 C CNN
F 1 "GNDPWR" H 9700 5520 40  0001 C CNN
F 2 "" H 9700 5600 60  0000 C CNN
F 3 "" H 9700 5600 60  0000 C CNN
	1    9700 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 4900 9700 4900
Wire Wire Line
	9700 4900 9700 5600
Wire Wire Line
	9800 5400 9700 5400
Connection ~ 9700 5400
NoConn ~ 9800 4500
NoConn ~ 9800 5300
NoConn ~ 9800 4800
NoConn ~ 9800 5000
NoConn ~ 9800 5100
NoConn ~ 9800 5200
$Comp
L C C?
U 1 1 53B851FE
P 8300 5900
F 0 "C?" H 8325 5950 30  0000 L CNN
F 1 "0.1uF" H 8325 5850 30  0000 L CNN
F 2 "" H 8300 5900 60  0000 C CNN
F 3 "" H 8300 5900 60  0000 C CNN
	1    8300 5900
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 53B8524D
P 8300 6100
F 0 "#PWR?" H 8300 6150 40  0001 C CNN
F 1 "GNDPWR" H 8300 6020 40  0001 C CNN
F 2 "" H 8300 6100 60  0000 C CNN
F 3 "" H 8300 6100 60  0000 C CNN
	1    8300 6100
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 53B852CF
P 8300 5700
F 0 "#PWR?" H 8300 5790 20  0001 C CNN
F 1 "+3.3V" H 8300 5790 30  0000 C CNN
F 2 "" H 8300 5700 60  0000 C CNN
F 3 "" H 8300 5700 60  0000 C CNN
	1    8300 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 5700 8300 5800
Wire Wire Line
	8300 6000 8300 6100
$EndSCHEMATC
