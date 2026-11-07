CC      = clang
CFLAGS  = -Wall -Wextra -g -MMD -MP
LDLIBS  = -lncurses

TARGET  = game

SRCS    = $(wildcard *.c)
OBJS    = $(SRCS:.c=.o)
DEPS    = $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)

.PHONY: clean