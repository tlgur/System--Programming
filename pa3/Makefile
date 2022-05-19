CC = gcc
OBJS = client.o server.o
TARGET = pa3_client.out pa3_server.out

all : $(TARGET)

pa3_client.out : client.o
	$(CC) -o $@ $^

pa3_server.out : server.o
	$(CC) -o $@ $^ -lpthread

.c.o:
	$(CC) -c -o $@ $<

clean:
	rm -rf $(TARGET) $(OBJS)
