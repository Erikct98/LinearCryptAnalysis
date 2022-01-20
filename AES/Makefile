CC     		= g++
CFLAGS 		= -O3
PARFLAGS	= -fopenmp

OBJAES				= aes.o
OBJVALIDATE			= validate.o
OBJLINSBOX			= linearizedsbox.o

EXEVALIDATE			= validate.exe
EXELINSBOX			= linearizedsbox.exe

_aes:
	${CC} -c -o ${OBJAES} aes128.cpp ${CFLAGS}

validate: _aes
	${CC} -c -o ${OBJVALIDATE} validate.cpp ${CFLAGS}
	${CC} -o ${EXEVALIDATE} ${OBJVALIDATE} ${OBJAES} ${CFLAGS}

linearizedsbox: _aes
	${CC} -c -o ${OBJLINSBOX} linearizedTrailSbox.cpp ${CFLAGS}
	${CC} -o ${EXELINSBOX} ${OBJLINSBOX} ${OBJAES} ${CFLAGS}

clean:
	rm -f ${OBJAES} ${OBJLINSBOX} ${EXELINSBOX} \
	${OBJVALIDATE} ${EXEVALIDATE}