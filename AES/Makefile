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
OBJKEYRECVR3		= keyrecovery3.o
OBJBLINDTRAIL		= blindtrail.o

EXEBITFIX			= bitfix.exe
EXEVALIDATE			= validate.exe
EXELINSBOX			= linearizedsbox.exe
EXETRAILMC			= trailmc.exe
EXETRAILONE			= trailone.exe
EXETRAILTWO			= trailtwo.exe
EXETRAILTHREE		= trailthree.exe
EXEKEYRECVR			= keyrecovery.exe
EXEKEYRECVR3		= keyrecovery3.exe
EXEBLINDTRAIL		= blindtrail.exe

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

keyrecovery3: _toolbox _aes
	${CC} -c -o ${OBJKEYRECVR3} keyrecoveryv3.cpp ${CFLAGS}
	${CC} -o ${EXEKEYRECVR3} ${OBJKEYRECVR3} ${OBJAES} ${OBJTOOLBOX} ${CFLAGS}

blindtrail: _toolbox
	${CC} -c -o ${OBJBLINDTRAIL} blindtrail.cpp ${CFLAGS}
	${CC} -o ${EXEBLINDTRAIL} ${OBJBLINDTRAIL} ${OBJTOOLBOX} ${CFLAGS}


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
	${OBJKEYRECVR3} ${EXEKEYRECVR3} \
	${OBJBLINDTRAIL} ${EXEBLINDTRAIL}