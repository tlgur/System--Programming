CC = gcc
TARGET = main.out
OBJS = main.o plus.o minus.o

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^

main.o:
	$(CC) -c main.c

plus.o:
	$(CC) -c plus.c

minus.o:
	$(CC) -c minus.c

clean:
	rm -rf $(TARGET) $(OBJS) 
