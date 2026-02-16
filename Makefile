CC = gcc
CFLAGS = -g

TARGET = app
SRC = main.c

$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
