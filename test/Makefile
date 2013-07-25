CC=gcc
CFALGS=-I ../src
LIBS=-L../src/.lib -ljson_parser

TARGET= test1 

default: $(TARGET)


test1: test1.c
	$(CC) -o $@ $^  $(CFALGS) $(LIBS)

clean:
	rm -rf $(TARGET)
