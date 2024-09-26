SRC = src
CC = cc
CFLAGS = -std=c99 -Wall -Wextra

.PHONY: clean xpf

xpf: $(SRC)/main.c
	@echo Compiling...
	$(CC) $(CFLAGS) -o $@ $<
	strip $@
	@echo Done!

install: xpf
	@echo Installing...
	@mkdir -p /usr/local/bin
	@cp $< /usr/local/bin/
	@echo Done!

clean:
	@echo Cleaning up...
	rm -rf xpf
	@echo Done!
