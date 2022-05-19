CC = gcc
TARGET = e10.out
OBJS = e10.o

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ -lpthread

.c.o :
	$(CC) -c -o $@ $<

clean : 
	rm -rf $(TARGET) $(OBJS)
