CC := gcc
CFLAGS := -Wall -Wextra -pedantic -std=c99
TARGET = futurerestore_latest
OBJS = futurerestore_latest.o
COMMANDS = -o futurerestore_latest -lcurl

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(COMMANDS)

.PHAUNY: clean

clean:
	$(RM) $(OBJS) $(TARGET)
