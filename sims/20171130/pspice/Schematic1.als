* Schematics Aliases *

.ALIASES
R_R9            R9(1=0 2=$N_0001 )
R_R1            R1(1=$N_0003 2=$N_0002 )
X_U1            U1(OUT=$N_0001 REF=$N_0011 GND=0 DB7=$N_0002 DB6=$N_0004
+  DB5=$N_0005 DB4=$N_0006 DB3=$N_0007 DB2=$N_0008 DB1=$N_0009 DB0=$N_0010
+  DPWR=$G_DPWR DGND=$G_DGND )
V_V2            V2(+=$N_0011 -=0 )
R_R4            R4(1=$N_0003 2=$N_0006 )
R_R5            R5(1=$N_0003 2=$N_0007 )
R_R7            R7(1=$N_0003 2=$N_0008 )
R_R6            R6(1=$N_0003 2=$N_0009 )
R_R8            R8(1=$N_0003 2=$N_0010 )
V_V1            V1(+=$N_0003 -=0 )
R_R2            R2(1=$N_0003 2=$N_0004 )
R_R3            R3(1=$N_0003 2=$N_0005 )
_    _($G_DPWR=$G_DPWR)
_    _($G_DGND=$G_DGND)
.ENDALIASES

