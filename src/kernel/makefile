SOURCES := $(wildcard *.c)
HEADERS := $(SOURCES:%.c=%.h)
OBJECTS := $(SOURCES:%.c=%.o)

build: $(OBJECTS)

# Todo: make this depend on headers
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $(OBJECTS_DIR)/$@