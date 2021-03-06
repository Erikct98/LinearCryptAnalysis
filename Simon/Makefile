CC     		= g++
CFLAGS 		= -O3
PARFLAGS	= -fopenmp

OBJSIMON			= simon.o
OBJTOOLBOX			= toolbox.o
OBJSPEED			= speedtest.o
OBJSPEEDPAR			= par_speedtest.o
OBJTRAIL			= trailtest.o
OBJMATSUI		    = matsui2.o
OBJDECRYPT		    = test_decrypt.o
OBJPTGEN			= plaintextgen.o
OBJLINEARIZE		= linearize.o
OBJLINEARIZEDTRAIL	= linearizedTrail.o

EXESPEED			= speedtest.exe
EXESPEEDPAR			= parspeedtest.exe
EXETRAIL			= trailtest.exe
EXEMATSUI			= matsui2.exe
EXEDECRYPT			= decrypt.exe
EXEPTGEN			= plaintextgen.exe
EXELINEARIZE 		= linearize.exe
EXELINEARIZEDTRAIL  = linearizedTrail.exe

_simon:
	${CC} -c -o ${OBJSIMON} simon.cpp ${CFLAGS}

_toolbox:
	${CC} -c -o ${OBJTOOLBOX} toolbox.cpp ${CFLAGS}

speedtest: _simon _toolbox
	${CC} -c -o ${OBJSPEED} speed_test.cpp ${CFLAGS}
	${CC} -o ${EXESPEED} ${OBJSPEED} ${OBJSIMON} ${OBJTOOLBOX} ${CFLAGS}

parspeedtest: _simon _toolbox
	${CC} -c -o ${OBJSPEEDPAR} parallel_speedtest.cpp ${CFLAGS} ${PARFLAGS}
	${CC} -o ${EXESPEEDPAR} ${OBJSPEEDPAR} ${OBJSIMON} ${OBJTOOLBOX} ${CFLAGS} ${PARFLAGS}

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

linearizedtrail: _simon _toolbox
	${CC} -c -o ${OBJLINEARIZEDTRAIL} LinearizedTrailTest.cpp ${CFLAGS} ${PARFLAGS}
	${CC} -o ${EXELINEARIZEDTRAIL} ${OBJLINEARIZEDTRAIL} ${OBJSIMON} ${OBJTOOLBOX} ${CFLAGS} ${PARFLAGS}

clean:
	rm -f ${OBJSIMON} ${OBJTOOLBOX} ${OBJSPEED} ${OBJTRAIL} ${OBJMATSUI} ${OBJDECRYPT} \
	${OBJPTGEN}	${OBJLINEARIZE} ${OBJSPEEDPAR} ${OBJLINEARIZEDTRAIL} \
	${EXESPEED} ${EXETRAIL} ${EXEMATSUI} ${EXEDECRYPT} ${EXEPTGEN} ${EXELINEARIZE} \
	${EXESPEEDPAR} ${EXELINEARIZEDTRAIL}