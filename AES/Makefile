CC     		= g++
CFLAGS 		= -O3
PARFLAGS	= -fopenmp

OBJAES				= aes.o
OBJVALIDATE			= validate.o
OBJLINSBOX			= linearizedsbox.o
OBJTRAILMC			= trailmc.o
OBJTRAILONE			= trailone.o
OBJTOOLBOX			= toolbox.o

EXEVALIDATE			= validate.exe
EXELINSBOX			= linearizedsbox.exe
EXETRAILMC			= trailmc.exe
EXETRAILONE			= trailone.exe

_aes:
	${CC} -c -o ${OBJAES} aes128.cpp ${CFLAGS}

_toolbox:
	${CC} -c -o ${OBJTOOLBOX} toolbox.cpp ${CFLAGS}

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

clean:
	rm -f ${OBJAES} ${OBJTOOLBOX} \
	${OBJLINSBOX} ${EXELINSBOX} \
	${OBJVALIDATE} ${EXEVALIDATE} \
	${OBJTRAILMC} ${EXETRAILMC} \
	${OBJTRAILONE} ${EXETRAILONE}