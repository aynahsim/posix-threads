SRCS = main.c
TARGET = exe

CC = gcc
CFLAGS = -g -Wall -Werror
INCLUDE_DIR = .
LIBS = -lpthread

RM = rm
RM_FLAGS = -rf

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(SRCS) $(LIBS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean: $(TARGET)
	$(RM) $(RM_FLAGS) *.o $(TARGET)
