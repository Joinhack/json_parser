CC=gcc
AR=ar
YACC=bison
LEX=flex
LIBS=
CFLAGS="-Wall" -g


YY_PREFIX=json_yy
LL_PREFIX=json_ll
DEPS= ${LL_PREFIX}.o ${YY_PREFIX}.o dict.o setting.o arraylist.o json.o
TARGET=libjson_parser.a

all:$(TARGET)

${LL_PREFIX}.c:json.ll
	mkdir -p .lib
	$(LEX) --header-file=${LL_PREFIX}.h -o $@ $?
	$(YACC) -d -o ${YY_PREFIX}.c json.yy

${YY_PREFIX}.c:json.yy
	$(YACC) -d -o $@ $?

%.o: %.c
	$(CC) -c -o	.lib/$@ $^

$(TARGET): $(DEPS)
	cd .lib ; $(AR) -rcs $@ $^

clean:
	rm -rf .lib
	rm -rf ${YY_PREFIX}.c ${YY_PREFIX}.h ${LL_PREFIX}.c ${LL_PREFIX}.h

