CC     = g++
CFLAGS = -O3

OBJAESESQUE		= aesesque.o
OBJSPEED		= speedtest.o
OBJVALIDATE		= validate.o
OBJTRAIL   		= trail.o

EXEVALIDATE		= validate.exe
EXESPEED		= speedtest.exe
EXETRAIL		= trail.exe

_aes:
	${CC} -c -o ${OBJAESESQUE} AESesque.cpp ${CFLAGS}

validate: _aes
	${CC} -c -o ${OBJVALIDATE} validate.cpp ${CFLAGS}
	${CC} -o ${EXEVALIDATE} ${OBJAESESQUE} ${OBJVALIDATE} ${CFLAGS}

speedtest: _aes
	${CC} -c -o ${OBJSPEED} speedtest.cpp ${CFLAGS}
	${CC} -o ${EXESPEED} ${OBJAESESQUE} ${OBJSPEED} ${CFLAGS}

trail: _aes
	${CC} -c -o ${OBJTRAIL} trail.cpp ${CFLAGS}
	${CC} -o ${EXETRAIL} ${OBJAESESQUE} ${OBJTRAIL} ${CFLAGS}

clean:
	rm -f ${OBJAESESQUE} ${OBJSPEED} ${OBJVALIDATE} ${OBJTRAIL} ${EXEVALIDATE} ${EXESPEED} ${EXETRAIL}