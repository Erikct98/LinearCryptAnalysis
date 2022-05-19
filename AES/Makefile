CC     		= g++
CFLAGS 		= -O3
PARFLAGS	= -fopenmp

OBJAES				= aes.o
OBJBITFIX			= bitfix.o
OBJVALIDATE			= validate.o
OBJLINSBOX			= linearizedsbox.o
OBJTRAILMC			= trailmc.o
OBJTRAILONE			= trailone.o
OBJTRAILTWO			= trailtwo.o
OBJTRAILTHREE		= trailthree.o
OBJTOOLBOX			= toolbox.o
OBJKEYRECVR			= keyrecovery.o
OBJKEYRECVR2		= keyrecovery2.o
OBJKEYRECVR3		= keyrecovery3.o
OBJKEYRECVR4		= keyrecovery4.o
OBJBLINDTRAIL		= blindtrail.o
OBJKEYGUESSFINDER	= keyguessfinder.o
OBJKEYCLASSIS		= keyclassis.o
OBJMONTECARLO		= montecarlo.o
OBJMONTECARLOV2		= montecarlov2.o
OBJDUALTRAIL		= dualtrail.o
OBJVECTORTRAIL		= vectortrail.o

EXEBITFIX			= bitfix.exe
EXEVALIDATE			= validate.exe
EXELINSBOX			= linearizedsbox.exe
EXETRAILMC			= trailmc.exe
EXETRAILONE			= trailone.exe
EXETRAILTWO			= trailtwo.exe
EXETRAILTHREE		= trailthree.exe
EXEKEYRECVR			= keyrecovery.exe
EXEKEYRECVR2		= keyrecovery2.exe
EXEKEYRECVR3		= keyrecovery3.exe
EXEKEYRECVR4		= keyrecovery4.exe
EXEBLINDTRAIL		= blindtrail.exe
EXEKEYGUESSFINDER	= keyguessfinder.exe
EXEKEYCLASSIS		= keyclassis.exe
EXEMONTECARLO		= montecarlo.exe
EXEMONTECARLOV2		= montecarlov2.exe
EXEDUALTRAIL		= dualtrail.exe
EXEVECTORTRAIL		= vectortrail.exe

_aes:
	${CC} -c -o ${OBJAES} aes128.cpp ${CFLAGS}

_toolbox:
	${CC} -c -o ${OBJTOOLBOX} toolbox.cpp ${CFLAGS}

bitfix: _aes _toolbox
	${CC} -c -o ${OBJBITFIX} bitfixing.cpp ${CFLAGS} ${PARFLAGS}
	${CC} -o ${EXEBITFIX} ${OBJBITFIX} ${OBJAES} ${OBJTOOLBOX} ${CFLAGS}

validate: _aes _toolbox
	${CC} -c -o ${OBJVALIDATE} validate.cpp ${CFLAGS}
	${CC} -o ${EXEVALIDATE} ${OBJVALIDATE} ${OBJAES} ${OBJTOOLBOX} ${CFLAGS}

linearizedsbox: _toolbox
	${CC} -c -o ${OBJLINSBOX} linearizedTrailSbox.cpp ${CFLAGS}
	${CC} -o ${EXELINSBOX} ${OBJLINSBOX} ${OBJTOOLBOX} ${CFLAGS}

trailMixColumn: _toolbox
	${CC} -c -o ${OBJTRAILMC} trailMixColumn.cpp ${CFLAGS}
	${CC} -o ${EXETRAILMC} ${OBJTRAILMC} ${OBJTOOLBOX} ${CFLAGS}

trailOneRound: _toolbox
	${CC} -c -o ${OBJTRAILONE} trailOneRound.cpp ${CFLAGS} ${PARFLAGS}
	${CC} -o ${EXETRAILONE} ${OBJTRAILONE} ${OBJTOOLBOX} ${CFLAGS} ${PARFLAGS}

trailTwoRounds: _toolbox
	${CC} -c -o ${OBJTRAILTWO} trailTwoRounds.cpp ${CFLAGS} ${PARFLAGS}
	${CC} -o ${EXETRAILTWO} ${OBJTRAILTWO} ${OBJTOOLBOX} ${CFLAGS} ${PARFLAGS}

trailThreeRounds: _toolbox
	${CC} -c -o ${OBJTRAILTHREE} trailThreeRounds.cpp ${CFLAGS} ${PARFLAGS}
	${CC} -o ${EXETRAILTHREE} ${OBJTRAILTHREE} ${OBJTOOLBOX} ${CFLAGS} ${PARFLAGS}

keyrecovery: _toolbox
	${CC} -c -o ${OBJKEYRECVR} keyrecovery.cpp ${CFLAGS}
	${CC} -o ${EXEKEYRECVR} ${OBJKEYRECVR} ${OBJTOOLBOX} ${CFLAGS}

keyrecovery2: _toolbox
	${CC} -c -o ${OBJKEYRECVR2} keyrecoveryv2.cpp ${CFLAGS}
	${CC} -o ${EXEKEYRECVR2} ${OBJKEYRECVR2} ${OBJTOOLBOX} ${CFLAGS}

keyrecovery3: _toolbox _aes
	${CC} -c -o ${OBJKEYRECVR3} keyrecoveryv3.cpp ${CFLAGS}
	${CC} -o ${EXEKEYRECVR3} ${OBJKEYRECVR3} ${OBJAES} ${OBJTOOLBOX} ${CFLAGS}

keyrecovery4: _toolbox _aes
	${CC} -c -o ${OBJKEYRECVR4} keyrecoveryv4.cpp ${CFLAGS}
	${CC} -o ${EXEKEYRECVR4} ${OBJKEYRECVR4} ${OBJAES} ${OBJTOOLBOX} ${CFLAGS}

blindtrail: _toolbox
	${CC} -c -o ${OBJBLINDTRAIL} blindtrail.cpp ${CFLAGS}
	${CC} -o ${EXEBLINDTRAIL} ${OBJBLINDTRAIL} ${OBJTOOLBOX} ${CFLAGS}

keyguessfinder: _toolbox
	${CC} -c -o ${OBJKEYGUESSFINDER} keyguessfinder.cpp ${CFLAGS}
	${CC} -o ${EXEKEYGUESSFINDER} ${OBJKEYGUESSFINDER} ${OBJTOOLBOX} ${CFLAGS}

keyclassis: _toolbox
	${CC} -c -o ${OBJKEYCLASSIS} keyclassification.cpp ${CFLAGS}
	${CC} -o ${EXEKEYCLASSIS} ${OBJKEYCLASSIS} ${OBJTOOLBOX} ${CFLAGS}

montecarlo: _toolbox
	${CC} -c -o ${OBJMONTECARLO} montecarlo.cpp ${CFLAGS}
	${CC} -o ${EXEMONTECARLO} ${OBJMONTECARLO} ${OBJTOOLBOX} ${CFLAGS}

montecarlov2: _toolbox _aes
	${CC} -c -o ${OBJMONTECARLOV2} montecarlov2.cpp ${CFLAGS}
	${CC} -o ${EXEMONTECARLOV2} ${OBJMONTECARLOV2} ${OBJTOOLBOX} ${OBJAES} ${CFLAGS}

dualtrail: _toolbox
	${CC} -c -o ${OBJDUALTRAIL} dualtrail.cpp ${CFLAGS}
	${CC} -o ${EXEDUALTRAIL} ${OBJDUALTRAIL} ${OBJTOOLBOX} ${CFLAGS}

vectortrail: _toolbox
	${CC} -c -o ${OBJVECTORTRAIL} vectortrail.cpp ${CFLAGS}
	${CC} -o ${EXEVECTORTRAIL} ${OBJVECTORTRAIL} ${OBJTOOLBOX} ${CFLAGS}

clean:
	rm -f ${OBJAES} ${OBJTOOLBOX} \
	${OBJLINSBOX} ${EXELINSBOX} \
	${OBJBITFIX} ${EXEBITFIX} \
	${OBJVALIDATE} ${EXEVALIDATE} \
	${OBJTRAILMC} ${EXETRAILMC} \
	${OBJTRAILONE} ${EXETRAILONE} \
	${OBJTRAILTWO} ${EXETRAILTWO} \
	${OBJTRAILTHREE} ${EXETRAILTHREE} \
	${OBJKEYRECVR} ${EXEKEYRECVR} \
	${OBJKEYRECVR2} ${EXEKEYRECVR2} \
	${OBJKEYRECVR3} ${EXEKEYRECVR3} \
	${OBJKEYRECVR4} ${EXEKEYRECVR4} \
	${OBJBLINDTRAIL} ${EXEBLINDTRAIL} \
	${OBJKEYGUESSFINDER} ${EXEKEYGUESSFINDER} \
	${OBJKEYCLASSIS} ${EXEKEYCLASSIS} \
	${OBJMONTECARLO} ${EXEMONTECARLO} \
	${OBJDUALTRAIL} ${EXEDUALTRAIL} \
	${OBJVECTORTRAIL} ${EXEVECTORTRAIL}
