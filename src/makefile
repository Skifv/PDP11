# ����������
CC=gcc
# ����� ����������
CFLAGS=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wpointer-arith -Wstack-usage=8192 -Wstrict-aliasing -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE
# ���������� � ��������� �������
SRC_DIR=source
# ���������� ��� ��������� ������
OBJ_DIR=objects
# ���������� ��� ����������� ������
EXE_DIR=executors
# ��� �������� �����
SRCS=$(wildcard $(SRC_DIR)/*.c) 
# ��������� �����
OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
# ��� ������������ �����
EXEC=pdp11.exe
# ������ ������ ������
DATA=./data/data.txt
# ����� ��� ������� ���������
# ����� ��� ������ ������ ����� ������ �����������!!
FL=-d
# ���������� ��� ������ ������������
DEP_DIR=$(OBJ_DIR)/dependencies

# ���������� ��������� ��� ����������
ifdef q
    ifeq ($(q),1)
        QUIET :=
    else
        QUIET := @
    endif
else
    QUIET := @
endif

# ����� ��� ������ � wsl
ifdef wsl
  ifeq ($(wsl),1)
    CLEAN_COMMAND := -rm -rf
	CHCP :=
  else
    CLEAN_COMMAND := -rmdir /Q /S
	CHCP := chcp 1251
  endif
else
  CLEAN_COMMAND := -rmdir /Q /S
  CHCP := chcp 1251
endif

# ������ c valgrind � ���
ifdef v
	ifeq ($(v), 1)
		VALGRIND := valgrind
	else
		VALGRIND :=
	endif
else
	VALGRIND := 
endif

# ���� �� ���������
all: $(OBJ_DIR) $(EXE_DIR) $(EXE_DIR)/$(EXEC)

# �������� ���������� executes, ���� ��� �� ����������
$(EXE_DIR):
	$(QUIET)mkdir $@

# �������� ���������� objects, ���� ��� �� ����������
$(OBJ_DIR): 
	$(QUIET)mkdir $@

# �������� ���������� ��� ������ ������������, ���� ��� �� ����������
$(DEP_DIR): | $(OBJ_DIR)
	$(QUIET)cd $(OBJ_DIR) && mkdir dependencies

# ������� ��� ���������� �������� ������ � ���������
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(DEP_DIR)
	$(QUIET)$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/$*.d -c $^ -o $@

# ������� ��� ���������� pdp11.c � pdp11.o
$(OBJ_DIR)/pdp11.o: pdp11.c | $(OBJ_DIR) $(DEP_DIR)
	$(QUIET)$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/pdp11.d -c $^ -o $@

# ������� ��� ������ ������������ �����
$(EXE_DIR)/$(EXEC): $(OBJS) $(OBJ_DIR)/pdp11.o | $(EXE_DIR) 
	$(QUIET)$(CC) $(CFLAGS) $^ -o $@
#	@echo make: compiled
#	@echo make: compiled

# ���� ��� �������� ��������� ������ � ������������ �����
clean:
	$(QUIET)$(CLEAN_COMMAND) $(OBJ_DIR) $(EXE_DIR)
#	@echo make: cleaned
#	@echo make: cleaned

# ������� ��� ������� ��������� � ���������� �����������
run: $(EXE_DIR)/$(EXEC)
	$(QUIET)$(CHCP)
	$(VALGRIND) ./$(EXE_DIR)/$(EXEC) $(FL) $(DATA)
#	@echo make: executed
#	@echo make: executed

# ������� ��� ����������� �������
help:
	@echo ""
	@echo "Usage: make [options] [additional options]"
	@echo "Options:"
	@echo "  all     - compile the program (default)"
	@echo "  clean   - remove object files and executable"
	@echo "  run     - compile and run the program"
	@echo "  help    - display this help message"
	@echo ""
	@echo "Additional options:"
	@echo "  v=1     - enable Valgrind for memory checking"
	@echo "  v=0     - disable Valgrind for memory checking (default)"
	@echo "  wsl=1   - use WSL commands"
	@echo "  wsl=0   - use PowerShell commands (default)"
	@echo "  q=1     - enable noisy mode"
	@echo "  q=0     - enable quiet mode (default)"
	@echo ""

# ��������� ������������
-include $(OBJ_DIR)/*.d
