# Компилятор
CC=gcc
# Опции компиляции
CFLAGS=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wpointer-arith -Wstack-usage=8192 -Wstrict-aliasing -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE
# Директория с исходными файлами
SRC_DIR=source
# Директория для объектных файлов
OBJ_DIR=objects
# Директория для исполняемых файлов
EXE_DIR=executors
# Все исходные файлы
SRCS=$(wildcard $(SRC_DIR)/*.c) 
# Объектные файлы
OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
# Имя исполняемого файла
EXEC=pdp11.exe
# Откуда читать данные
DATA=./data/data.txt
# Флаги при запуске программы
# ФЛАГИ ДЛЯ ТЕСТОВ ТОЛЬКО ПОСЛЕ ФЛАГОВ ЛОГИРОВАНИЯ!!
FL=-d #--testall
# Директория для файлов зависимостей
DEP_DIR=$(OBJ_DIR)/dependencies

# Отключение сообщений при компиляции
ifdef q
    ifeq ($(q),1)
        QUIET :=
    else
        QUIET := @
    endif
else
    QUIET := @
endif

# Нужно для работы в wsl
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

# Запуск c valgrind и без
ifdef v
	ifeq ($(v), 1)
		VALGRIND := valgrind
	else
		VALGRIND :=
	endif
else
	VALGRIND := 
endif

# Цель по умолчанию
all: $(OBJ_DIR) $(EXE_DIR) $(EXE_DIR)/$(EXEC)

# Создание директории executes, если она не существует
$(EXE_DIR):
	$(QUIET)mkdir $@

# Создание директории objects, если она не существует
$(OBJ_DIR): 
	$(QUIET)mkdir $@

# Создание директории для файлов зависимостей, если она не существует
$(DEP_DIR): | $(OBJ_DIR)
	$(QUIET)cd $(OBJ_DIR) && mkdir dependencies

# Правило для компиляции исходных файлов в объектные
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(DEP_DIR)
	$(QUIET)$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/$*.d -c $^ -o $@

# Правило для компиляции pdp11.c в pdp11.o
$(OBJ_DIR)/pdp11.o: pdp11.c | $(OBJ_DIR) $(DEP_DIR)
	$(QUIET)$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/pdp11.d -c $^ -o $@

# Правило для сборки исполняемого файла
$(EXE_DIR)/$(EXEC): $(OBJS) $(OBJ_DIR)/pdp11.o | $(EXE_DIR) 
	$(QUIET)$(CC) $(CFLAGS) $^ -o $@
#	@echo make: compiled
#	@echo make: compiled

# Цель для удаления объектных файлов и исполняемого файла
clean:
	$(QUIET)$(CLEAN_COMMAND) $(OBJ_DIR) $(EXE_DIR)
#	@echo make: cleaned
#	@echo make: cleaned

# Правило для запуска программы с указанными параметрами
run: $(EXE_DIR)/$(EXEC)
	$(QUIET)$(CHCP)
	$(VALGRIND) ./$(EXE_DIR)/$(EXEC) $(FL) $(DATA)
#	@echo make: executed
#	@echo make: executed

# Правило для отображения справки
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

# Включение зависимостей
-include $(OBJ_DIR)/*.d
