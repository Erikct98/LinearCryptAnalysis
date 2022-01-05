CC     = g++
CFLAGS = -Os

OBJVALIDATE		= validate.o
OBJTRAIL   		= trail.o

validate:
	${CC} validate.cpp AESesque.cpp -o ${OBJVALIDATE} ${CFLAGS}

trail:
	${CC} trail.cpp AESesque.cpp -o ${OBJTRAIL} ${CFLAGS}

clean:
	rm -f ${OBJVALIDATE} ${OBJTRAIL}