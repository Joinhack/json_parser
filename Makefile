

all: test

lib:
	cd src ; $(MAKE)

test: lib
	cd test ; $(MAKE)	

clean:
	cd src; $(MAKE) clean
	cd test; $(MAKE) clean