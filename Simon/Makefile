CC     = g++
CFLAGS = -O3

OBJSIMON			= simon.o
OBJTOOLBOX			= toolbox.o
OBJSPEED			= speedtest.o
OBJTRAIL			= trailtest.o
OBJMATSUI		    = matsui2.o
OBJDECRYPT		    = test_decrypt.o
OBJPTGEN			= plaintextgen.o
OBJLINEARIZE		= linearize.o

EXESPEED			= speedtest.exe
EXETRAIL			= trailtest.exe
EXEMATSUI			= matsui2.exe
EXEDECRYPT			= decrypt.exe
EXEPTGEN			= plaintextgen.exe
EXELINEARIZE 		= linearize.exe

_simon:
	${CC} -c -o ${OBJSIMON} simon.cpp ${CFLAGS}

_toolbox:
	${CC} -c -o ${OBJTOOLBOX} toolbox.cpp ${CFLAGS}

speedtest: _simon _toolbox
	${CC} -c -o ${OBJSPEED} speed_test.cpp ${CFLAGS}
	${CC} -o ${EXESPEED} ${OBJSPEED} ${OBJSIMON} ${OBJTOOLBOX} ${CFLAGS}

trailtest: _simon _toolbox
	${CC} -c -o ${OBJTRAIL} trail_test.cpp ${CFLAGS}
	${CC} -o ${EXETRAIL} ${OBJTRAIL} ${OBJSIMON} ${OBJTOOLBOX} ${CFLAGS}

matsui2: _simon _toolbox
	${CC} -c -o ${OBJMATSUI} matsui2.cpp ${CFLAGS}
	${CC} -o ${EXEMATSUI} ${OBJMATSUI} ${OBJSIMON} ${OBJTOOLBOX} ${CFLAGS}

decrypt: _simon _toolbox
	${CC} -c -o ${OBJDECRYPT} test_decrypt.cpp ${CFLAGS}
	${CC} -o ${EXEDECRYPT} ${OBJDECRYPT} ${OBJSIMON} ${OBJTOOLBOX} ${CFLAGS}

plaintextgen: _simon _toolbox
	${CC} -c -o ${OBJPTGEN} plaintextgen.cpp ${CFLAGS}
	${CC} -o ${EXEPTGEN} ${OBJPTGEN} ${OBJSIMON} ${OBJTOOLBOX} ${CFLAGS}

linearize: _simon _toolbox
	${CC} -c -o ${OBJLINEARIZE} linearization.cpp ${CFLAGS}
	${CC} -o ${EXELINEARIZE} ${OBJLINEARIZE} ${OBJSIMON} ${OBJTOOLBOX} ${CFLAGS}

clean:
	rm -f ${OBJSIMON} ${OBJTOOLBOX} ${OBJSPEED} ${OBJTRAIL} ${OBJMATSUI} ${OBJDECRYPT}\
	${OBJPTGEN}	${OBJLINEARIZE} ${EXESPEED} ${EXETRAIL} ${EXEMATSUI} ${EXEDECRYPT}\
	${EXEPTGEN} ${EXELINEARIZE}