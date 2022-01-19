CC     		= g++
CFLAGS 		= -O3
PARFLAGS	= -fopenmp

OBJAES				= aes.o
OBJLINSBOX			= linearizedsbox.o

EXELINSBOX			= linearizedsbox.exe

_aes:
	${CC} -c -o ${OBJAES} aes128.cpp ${CFLAGS}

linearizedsbox: _aes
	${CC} -c -o ${OBJLINSBOX} linearizedTrailSbox.cpp ${CFLAGS}
	${CC} -o ${EXELINSBOX} ${OBJLINSBOX} ${OBJAES} ${CFLAGS}

clean:
	rm -f ${OBJAES} ${OBJLINSBOX} ${EXELINSBOX}