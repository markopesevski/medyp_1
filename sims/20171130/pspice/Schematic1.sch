*version 9.1 3315502881
u 87
U? 2
R? 10
? 2
V? 3
@libraries
@analysis
.TRAN 1 0 1 0
+0 0ns
+1 10ms
+3 1ms
.OP 0 
@targets
@attributes
@translators
a 0 u 13 0 0 0 hln 100 PCBOARDS=PCB
a 0 u 13 0 0 0 hln 100 PSPICE=PSPICE
a 0 u 13 0 0 0 hln 100 XILINX=XILINX
@setup
unconnectedPins 0
connectViaLabel 0
connectViaLocalLabels 0
NoStim4ExtIFPortsWarnings 1
AutoGenStim4ExtIFPorts 1
@index
pageloc 1 0 4682 
@status
n 0 117:10:30:11:40:26;1512038426 e 
s 2832 117:10:30:11:40:26;1512038426 e 
*page 1 0 970 720 iA
@ports
port 36 GND_ANALOG 540 340 h
port 37 GND_ANALOG 370 340 h
@parts
part 11 r 630 310 v
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R9
a 0 ap 9 0 15 0 hln 100 REFDES=R9
a 0 u 13 0 15 25 hln 100 VALUE=10k
part 3 r 400 240 h
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R1
a 0 ap 9 0 15 0 hln 100 REFDES=R1
part 2 DAC8break 500 240 h
a 0 sp 11 0 -24 120 hln 100 PART=DAC8break
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=U1
a 0 ap 9 0 10 8 hln 100 REFDES=U1
part 74 VDC 590 300 h
a 0 sp 0 0 22 37 hln 100 PART=VDC
a 0 a 0:13 0 0 0 hln 100 PKGREF=V2
a 1 ap 9 0 24 7 hcn 100 REFDES=V2
a 1 u 13 0 -11 18 hcn 100 DC=1.5
part 6 r 400 270 h
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R4
a 0 ap 9 0 15 0 hln 100 REFDES=R4
part 7 r 400 280 h
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R5
a 0 ap 9 0 15 0 hln 100 REFDES=R5
part 9 r 400 290 h
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R7
a 0 ap 9 0 15 0 hln 100 REFDES=R7
part 8 r 400 300 h
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R6
a 0 ap 9 0 15 0 hln 100 REFDES=R6
part 10 r 400 310 h
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R8
a 0 ap 9 0 15 0 hln 100 REFDES=R8
part 38 VDC 370 200 h
a 0 sp 0 0 22 37 hln 100 PART=VDC
a 0 a 0:13 0 0 0 hln 100 PKGREF=V1
a 1 ap 9 0 24 7 hcn 100 REFDES=V1
a 1 u 13 0 -11 18 hcn 100 DC=3
part 4 r 400 250 h
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R2
a 0 ap 9 0 15 0 hln 100 REFDES=R2
part 5 r 400 260 h
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R3
a 0 ap 9 0 15 0 hln 100 REFDES=R3
part 1 titleblk 970 720 h
a 1 s 13 0 350 10 hcn 100 PAGESIZE=A
a 1 s 13 0 180 60 hcn 100 PAGETITLE=
a 1 s 13 0 300 95 hrn 100 PAGENO=1
a 1 s 13 0 340 95 hrn 100 PAGECOUNT=1
part 34 nodeMarker 620 270 h
a 0 s 0 0 0 0 hln 100 PROBEVAR=
a 0 a 0 0 4 22 hlb 100 LABEL=1
@conn
w 13
s 440 240 500 240 12
w 15
s 500 250 440 250 14
w 17
s 440 260 500 260 16
w 19
s 500 270 440 270 18
w 21
s 440 280 500 280 20
w 23
s 500 290 440 290 22
w 25
s 440 300 500 300 24
w 27
s 500 310 440 310 26
w 29
s 570 270 620 270 28
s 620 270 630 270 35
w 31
s 630 310 630 340 30
s 630 340 590 340 32
s 590 340 540 340 75
w 77
s 590 300 570 300 76
w 52
s 380 340 370 340 55
s 370 340 370 240 57
w 78
s 400 240 380 240 51
s 380 300 380 310 70
s 400 310 380 310 71
s 380 290 380 300 67
s 380 300 400 300 68
s 380 280 380 290 64
s 400 290 380 290 65
s 380 270 380 280 61
s 400 280 380 280 62
s 400 270 380 270 59
s 380 270 380 240 80
s 380 240 380 230 82
s 370 200 390 200 41
s 390 200 390 230 43
s 390 250 400 250 45
s 390 250 390 260 47
s 390 260 400 260 49
s 390 230 390 250 86
s 380 230 390 230 84
@junction
j 500 240
+ p 2 DB7
+ w 13
j 440 240
+ p 3 2
+ w 13
j 440 250
+ p 4 2
+ w 15
j 500 250
+ p 2 DB6
+ w 15
j 440 260
+ p 5 2
+ w 17
j 500 260
+ p 2 DB5
+ w 17
j 440 270
+ p 6 2
+ w 19
j 500 270
+ p 2 DB4
+ w 19
j 440 280
+ p 7 2
+ w 21
j 500 280
+ p 2 DB3
+ w 21
j 440 290
+ p 9 2
+ w 23
j 500 290
+ p 2 DB2
+ w 23
j 440 300
+ p 8 2
+ w 25
j 500 300
+ p 2 DB1
+ w 25
j 440 310
+ p 10 2
+ w 27
j 500 310
+ p 2 DB0
+ w 27
j 630 270
+ p 11 2
+ w 29
j 570 270
+ p 2 OUT
+ w 29
j 630 310
+ p 11 1
+ w 31
j 540 340
+ p 2 GND
+ w 31
j 620 270
+ p 34 pin1
+ w 29
j 540 340
+ s 36
+ p 2 GND
j 540 340
+ s 36
+ w 31
j 390 250
+ w 78
+ w 78
j 400 240
+ p 3 1
+ w 78
j 370 340
+ s 37
+ w 52
j 370 240
+ p 38 -
+ w 52
j 380 280
+ w 78
+ w 78
j 380 290
+ w 78
+ w 78
j 380 300
+ w 78
+ w 78
j 590 340
+ p 74 -
+ w 31
j 570 300
+ p 2 REF
+ w 77
j 590 300
+ p 74 +
+ w 77
j 400 270
+ p 6 1
+ w 78
j 400 280
+ p 7 1
+ w 78
j 400 290
+ p 9 1
+ w 78
j 400 300
+ p 8 1
+ w 78
j 400 310
+ p 10 1
+ w 78
j 380 270
+ w 78
+ w 78
j 380 240
+ w 78
+ w 78
j 370 200
+ p 38 +
+ w 78
j 400 250
+ p 4 1
+ w 78
j 400 260
+ p 5 1
+ w 78
j 390 230
+ w 78
+ w 78
@attributes
a 0 s 0:13 0 0 0 hln 100 PAGETITLE=
a 0 s 0:13 0 0 0 hln 100 PAGENO=1
a 0 s 0:13 0 0 0 hln 100 PAGESIZE=A
a 0 s 0:13 0 0 0 hln 100 PAGECOUNT=1
@graphics
