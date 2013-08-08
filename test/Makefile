CC=gcc
CFLAGS=-I ../src   -g

TARGET= test1 cstr_test thread_test

LIBS=../src/.lib/libjson_parser.a

all: $(TARGET)

test1: test1.o
	$(CC) -o $@ $< $(LIBS)

cstr_test: cstr_test.o
	$(CC) -o $@ $< $(LIBS)

thread_test: thread_test.o
	$(CC) -o $@ $< $(LIBS) -lpthread

clean:
	rm -rf $(TARGET) *.o
