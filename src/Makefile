CC=gcc
AR=ar
RANLIB=ranlib
YACC=bison
LEX=flex
LDFLAGS=
CFLAGS="-Wall" -g -c99


YY_PREFIX=json_yy
LL_PREFIX=json_ll
DEPS=${LL_PREFIX}.o ${YY_PREFIX}.o dict.o setting.o arraylist.o json.o cstr.o codec.o
TARGET=libjson_parser.a
OUTDIR=.lib

all:$(TARGET)

${LL_PREFIX}.c:json.ll
	mkdir -p $(OUTDIR)
	$(YACC) -d -o $(YY_PREFIX).c json.yy
	$(LEX) --header-file=$(LL_PREFIX).h -o $@ $?

$(YY_PREFIX).c: json.yy
	$(YACC) -d -o $(YY_PREFIX).c $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o	$(OUTDIR)/$@ $^

$(TARGET): $(DEPS)
	cd $(OUTDIR) ; $(AR) -rcs $@ $^; $(RANLIB) $@

clean:
	rm -rf .lib
	rm -rf ${YY_PREFIX}.c ${YY_PREFIX}.h ${LL_PREFIX}.c ${LL_PREFIX}.h




