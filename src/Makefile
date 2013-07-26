CC=clang
AR=ar
RANLIB=ranlib
YACC=bison
LEX=flex
LIBS=
CFLAGS="-Wall" -g


YY_PREFIX=json_yy
LL_PREFIX=json_ll
DEPS=${LL_PREFIX}.o ${YY_PREFIX}.o   dict.o setting.o arraylist.o json.o
TARGET=libjson_parser.a

all:$(TARGET)

${LL_PREFIX}.c:json.ll
	mkdir -p .lib
	$(YACC) -d -o $(YY_PREFIX).c json.yy
	$(LEX) --header-file=$(LL_PREFIX).h -o $@ $?

$(YY_PREFIX).c: json.yy
	$(YACC) -d -o $(YY_PREFIX).c $^



%.o: %.c
	$(CC) $(CFLAGS) -c -o	.lib/$@ $^

$(TARGET): $(DEPS)
	cd .lib ; $(AR) -rcs $@ $^; $(RANLIB) $@

clean:
	rm -rf .lib
	rm -rf ${YY_PREFIX}.c ${YY_PREFIX}.h ${LL_PREFIX}.c ${LL_PREFIX}.h




