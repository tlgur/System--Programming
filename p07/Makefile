CC = gcc -g
TARGET = t
OBJS = t.o

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

.c.o:
	$(CC) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
