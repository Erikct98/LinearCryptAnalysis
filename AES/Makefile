CC     		= g++
CFLAGS 		= -O3
PARFLAGS	= -fopenmp

OBJAES				= aes.o
OBJVALIDATE			= validate.o
OBJLINSBOX			= linearizedsbox.o
OBJTRAILMC			= trailmc.o

EXEVALIDATE			= validate.exe
EXELINSBOX			= linearizedsbox.exe
EXETRAILMC			= trailmc.exe

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

clean:
	rm -f ${OBJAES} ${OBJLINSBOX} ${EXELINSBOX} \
	${OBJVALIDATE} ${EXEVALIDATE} \
	${OBJTRAILMC} ${EXETRAILMC}