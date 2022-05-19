CC = gcc -g
TARGET = p8.out
OBJS = p8.o

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

.c.o:
	$(CC) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
