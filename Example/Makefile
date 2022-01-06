CC     = g++
CFLAGS = -O3

OBJSPEED		= speedtest.o
OBJVALIDATE		= validate.o
OBJTRAIL   		= trail.o

validate:
	${CC} validate.cpp AESesque.cpp -o ${OBJVALIDATE} ${CFLAGS}

speedtest:
	${CC} speedtest.cpp AESesque.cpp -o ${OBJSPEED} ${CFLAGS}

trail:
	${CC} trail.cpp AESesque.cpp -o ${OBJTRAIL} ${CFLAGS}

clean:
	rm -f ${OBJVALIDATE} ${OBJTRAIL}