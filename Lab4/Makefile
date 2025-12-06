CC = gcc
CFLAGS = -Wall -Iinc
LDFLAGS_DL = -ldl
LDFLAGS_STATIC = -L.
PICFLAGS = -fPIC

PROG1 = prog1
PROG2 = prog2

LIB1_NAME = liblib1.so
LIB2_NAME = liblib2.so

LIB_OBJS = lib1.o lib2.o

.PHONY: all clean libs

all: $(LIB1_NAME) $(LIB2_NAME) $(PROG1) $(PROG2)


$(LIB1_NAME): lib1.o
	$(CC) -shared lib1.o -o $(LIB1_NAME)
	

$(LIB2_NAME): lib2.o
	$(CC) -shared lib2.o -o $(LIB2_NAME)
	

lib1.o: src/lib1.c inc/contract.h
	$(CC) $(CFLAGS) $(PICFLAGS) -c src/lib1.c -o lib1.o

lib2.o: src/lib2.c inc/contract.h
	$(CC) $(CFLAGS) $(PICFLAGS) -c src/lib2.c -o lib2.o


$(PROG1): src/main_static.c $(LIB1_NAME) inc/contract.h
	$(CC) $(CFLAGS) src/main_static.c -o $(PROG1) $(LDFLAGS_STATIC) -llib1
	@echo "Собрана: $(PROG1) (Линковка с lib1)"

$(PROG2): src/main_dynamic.c inc/contract.h
	$(CC) $(CFLAGS) src/main_dynamic.c -o $(PROG2) $(LDFLAGS_DL)
	@echo "Собрана: $(PROG2) (Динамическая загрузка)"

clean:
	rm -f $(PROG1) $(PROG2) *.o *.so
	@echo "Удалены все объектные, библиотечные и исполняемые файлы."