CC ?= gcc
CFLAGS ?= 
LINKERFLAGS ?= -static
BUILDFOLDER ?= ./build/
INSTALLFOLDER ?= /usr/local/bin/

all: $(BUILDFOLDER) $(BUILDFOLDER)htb

$(BUILDFOLDER)htb: $(BUILDFOLDER)htb.o
	$(CC) -o $(BUILDFOLDER)htb $(LINKERFLAGS) $(BUILDFOLDER)htb.o

$(BUILDFOLDER)htb.o: ./src/htb.c
	$(CC) -o $(BUILDFOLDER)htb.o -c $(CFLAGS) ./src/htb.c

$(BUILDFOLDER):
	-mkdir $(BUILDFOLDER)

install: $(BUILDFOLDER)htb
	mv $(BUILDFOLDER)htb $(INSTALLFOLDER)htb

clean: all
	rm $(BUILDFOLDER)htb.o

