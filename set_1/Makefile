EXE = set1
SRC_DIR = src
OBJ_DIR = obj
BLD_DIR = build
RM = rm -rf

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS += -Iinclude  # default "include" directory

.PHONY: all clean

all: $(EXE)

$(BLD_DIR)/$(EXE): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ -o $@

$(EXE): %: $(BLD_DIR)/$(EXE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
	$(RM) $(BLD_DIR)
