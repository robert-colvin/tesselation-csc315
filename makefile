OBJS=frisbee.o singly.o
LIBS=-lglut -lGLU -lGL
CC=g++
CFLAG=-O3
DEPS=structs.h

#p1 : $(OBJS)
#	$(CC) $(OBJS) -o Frisbee

frisbee.o: frisbee.cpp $(DEPS)
	$(CC) frisbee.cpp $(LIBS) $(CFLAG) -o Frisbee

singly.o: singly.cpp 
	$(CC) singly.cpp -c $(LIBS) $(CFLAG)

clean:
	\rm *.o #*~ p1
