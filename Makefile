#finding first prime in consecutive digits of e
#Makefile
#c4tler

all: findprimeine

findprimeine: findprimeine.o
	g++  -I/usr/local/sw/include main.cpp -o findprimeine  -L/usr/local/sw/lib -lntl -lm

findprimeine.o: main.cpp
	gcc -c  -I/usr/local/sw/include main.cpp
	
