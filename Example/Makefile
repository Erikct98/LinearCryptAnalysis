CC     = g++
CFLAGS = -O3

OBJVALIDATE		= validate.o
OBJTRAIL   		= trail.o

validate:
	${CC} validate.cpp AESesque.cpp -o ${OBJVALIDATE} ${CFLAGS}

trail:
	${CC} trail.cpp AESesque.cpp -o ${OBJTRAIL} ${CFLAGS}

clean:
	rm -f ${OBJVALIDATE} ${OBJTRAIL}