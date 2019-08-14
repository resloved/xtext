DESTDIR="/"
PREFIX=""

xtext:
	gcc -o xtext `pkg-config --cflags --libs pangocairo x11 xfixes` xtext.c 

install:
	@echo installing xtext
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f xtext ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/xtext

uninstall:
	@echo uninstalling xtext
	@rm -f ${DESTDIR}${PREFIX}/bin/xtext
