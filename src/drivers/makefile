SOURCES := $(wildcard *.c)
HEADERS := $(SOURCES:%.c=%.h)
OBJECTS := $(SOURCES:%.c=%.o)

build: $(OBJECTS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $(OBJECTS_DIR)/$@