CC     		= g++
CFLAGS 		= -O3
PARFLAGS	= -fopenmp

OBJAES				= aes.o
OBJVALIDATE			= validate.o
OBJLINSBOX			= linearizedsbox.o
OBJTRAILMC			= trailmc.o
OBJTRAILONE			= trailone.o

EXEVALIDATE			= validate.exe
EXELINSBOX			= linearizedsbox.exe
EXETRAILMC			= trailmc.exe
EXETRAILONE			= trailone.exe

_aes:
	${CC} -c -o ${OBJAES} aes128.cpp ${CFLAGS}

validate: _aes
	${CC} -c -o ${OBJVALIDATE} validate.cpp ${CFLAGS}
	${CC} -o ${EXEVALIDATE} ${OBJVALIDATE} ${OBJAES} ${CFLAGS}

linearizedsbox: _aes
	${CC} -c -o ${OBJLINSBOX} linearizedTrailSbox.cpp ${CFLAGS}
	${CC} -o ${EXELINSBOX} ${OBJLINSBOX} ${OBJAES} ${CFLAGS}

trailMixColumn:
	${CC} -c -o ${OBJTRAILMC} trailMixColumn.cpp ${CFLAGS}
	${CC} -o ${EXETRAILMC} ${OBJTRAILMC} ${CFLAGS}

trailOneRound:
	${CC} -c -o ${OBJTRAILONE} trailOneRound.cpp ${CFLAGS} ${PARFLAGS}
	${CC} -o ${EXETRAILONE} ${OBJTRAILONE} ${CFLAGS} ${PARFLAGS}

clean:
	rm -f ${OBJAES} ${OBJLINSBOX} ${EXELINSBOX} \
	${OBJVALIDATE} ${EXEVALIDATE} \
	${OBJTRAILMC} ${EXETRAILMC} \
	${OBJTRAILONE} ${EXETRAILONE}