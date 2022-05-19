CC = gcc -g
TARGET = t
OBJS = ex3.o

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

.c.o:
	$(CC) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
