CC     = g++
CFLAGS = -O3

OBJSPEEDSIMONV1	   = speedtest_v1.o
OBJSPEEDSIMONV2	   = speedtest_v2.o
OBJSPEEDSIMONV3	   = speedtest_v3.o
OBJTRAILTEST	   = trailTest.o
OBJMATSUI		   = matsui2.o
OBJDECRYPT		   = test_decrypt.o

speedtest:
	${CC} speed_test.cpp encrypt_v1.cpp -o ${OBJSPEEDSIMONV1} ${CFLAGS}
	${CC} speed_test.cpp encrypt_v2.cpp -o ${OBJSPEEDSIMONV2} ${CFLAGS}
	${CC} speed_test.cpp encrypt_v3.cpp -o ${OBJSPEEDSIMONV3} ${CFLAGS}

trailtest:
	${CC} trail_test.cpp encrypt_v3.cpp -o ${OBJTRAILTEST} ${CFLAGS}

matsui2:
	${CC} matsui2.cpp encrypt_v3.cpp -o ${OBJMATSUI} ${CFLAGS}

decrypttest:
	${CC} test_decrypt.cpp encrypt_v3.cpp -o ${OBJDECRYPT} ${CFLAGS}

clean:
	rm -f ${OBJSIMONV1} ${OBJSIMONV2} ${OBJSIMONV3} ${OBJTRAILTEST} ${OBJMATSUI} ${OBJDECRYPT}