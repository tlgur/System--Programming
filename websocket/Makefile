CC = gcc
OBJS = client.o server.o
TARGET = client.out server.out


all : $(TARGET)

client.out : client.o
	$(CC) -o $@ $^

server.out : server.o
	$(CC) -o $@ $^

.c.o:
	$(CC) -c -o $@ $<

clean:
	rm -rf $(TARGET) $(OBJS)
