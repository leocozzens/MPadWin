CC = gcc
CFLAGS = -Iinclude
SRC = src
OBJ = obj
BINDIR = bin
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
BIN = $(BINDIR)/main.exe # For windows compatability

SUBMITNAME = minPadWin.zip
RM = del /Q
ZIP = powershell Compress-Archive

all: $(BIN)

release: CFLAGS=-O2
release: clean
release: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -lgdi32

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $(OBJ)/$@	

link: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) -lgdi32

clean:
	$(RM) $(BINDIR)
	$(RM) $(OBJ)

new: clean
new: all

submit:
	$(RM) $(SUBMITNAME)
	$(ZIP) $(SUBMITNAME) -Path $(BIN)

run:
	$(BIN)
	powershell echo $$?