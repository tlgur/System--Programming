CC = gcc
TARGET= lab3

all : $(TARGET)
       
$(TARGET): p.o
	$(CC) -o $@ $^

.c.o:
	$(CC) -c -o $@ $<

clean :	
	rm *.o $(TARGET)
