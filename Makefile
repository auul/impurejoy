OUT := joy
LIBFLAGS := -lm -lreadline

CC := gcc
CFLAGS := -O1
SRCDIR := src
OBJDIR := build

SRCFILES := $(wildcard $(SRCDIR)/*.c)
OBJFILES := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCFILES))

all: $(OBJDIR) $(OUT)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OUT): $(OBJFILES)
	$(CC) -o $(OUT) $(LIBFLAGS) $(OBJFILES)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(OBJDIR) $(OUT)
	./$(OUT)

clean:
	rm -fr $(OBJDIR)

.phony:
	all clean
