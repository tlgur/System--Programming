CC = gcc
TARGET = e6.out
OBJS = p6.o

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

.c.o:
	$(CC) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
