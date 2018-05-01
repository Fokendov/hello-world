CC=gcc
LIB_DIR=../lib

all: t1 t2 t3 t4

t1: t1.c $(LIB_DIR)/libcthread.a
	$(CC) -o t1 t1.c -L$(LIB_DIR) -lcthread -Wall

t2: t2.c $(LIB_DIR)/libcthread.a
	$(CC) -o t2 t2.c -L$(LIB_DIR) -lcthread -Wall

t3: t3.c $(LIB_DIR)/libcthread.a
	$(CC) -o t3 t3.c -L$(LIB_DIR) -lcthread -lm -Wall

t4: t4.c $(LIB_DIR)/libcthread.a
	$(CC) -o t4 t4.c -L$(LIB_DIR) -lcthread -lm -Wall

clean:
	rm -rf t1 t2 t3 t4 *.o *~
