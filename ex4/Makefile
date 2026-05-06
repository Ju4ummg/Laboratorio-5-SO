CC = gcc
CFLAGS = -Wall
TARGET = servidor

all: $(TARGET)

$(TARGET): servidor.c
	$(CC) $(CFLAGS) -o $(TARGET) servidor.c

clean:
	rm -f $(TARGET) /tmp/tradutor_socket