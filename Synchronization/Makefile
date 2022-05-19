CC = gcc
TARGET = ec.out
OBJS = ec.o

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ -lpthread

.c.o :
	$(CC) -c -o $@ $<

clean : 
	rm -rf $(TARGET) $(OBJS)
