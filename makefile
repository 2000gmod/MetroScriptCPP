MAKEFLAGS += --no-print-directory

TARGET_NAME = mscppi
TARGET = $(OUTDIR)/$(TARGET_NAME)

CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -MMD -O2
LFLAGS =
MEMPROFILER = valgrind
FORMATTER = clang-format

RUN_ON_WINDOW_COMMAND = konsole --hide-menubar --hide-tabbar --nofork -e make runWithWait
USE_NEW_WINDOW = false
PRESS_TO_EXIT_MSG = "Program complete, press any key to exit..."

MAINARGS = examples/helloworld.mtr

OBJDIR = obj
SRCDIR = src

OUTDIR = out

SOURCES := $(shell find . -name '*.cpp')
HEADERS := $(shell find . -name '*.hpp')
OBJECTS := $(subst .cpp,.o, $(subst ./src,./$(OBJDIR),$(SOURCES)))
DEPS := $(shell find . -name '*.d')

OUT_FILE = $(TARGET_NAME)_output.txt
OUT = > $(OUT_FILE)


PREFIX = [make]:
DONE = Done\n

.PHONY: default
default:
	@/usr/bin/time --format="$(PREFIX) Time: %E seconds" $(MAKE) $(TARGET)

$(TARGET): $(OBJECTS) | $(OUTDIR)
	@printf "$(PREFIX) Linking..."
	@$(CC) -o $(TARGET) $^ $(LFLAGS)
	@printf " $(DONE)"


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@printf "$(PREFIX) Compiling $<..."
	@$(CC) -c $< -o $@ $(CFLAGS) || (printf "\n\nBuild failed at file: $<.\n"; exit 1)
	@printf " $(DONE)"


include $(DEPS)

$(OUTDIR):
	@printf "$(PREFIX) Creating output directory..."
	@mkdir -p $@
	@printf " $(DONE)"

$(OBJDIR):
	@printf "$(PREFIX) Creating object directory..."
	@mkdir -p $@
	$(shell rsync -a --include='*/' --exclude='*' $(SRCDIR)/ $(OBJDIR)/)
	@printf " $(DONE)"


.PHONY: clean deepclean run debug runToFile runWithWait

run: default
	@printf "$(PREFIX) Running target: $(TARGET) $(MAINARGS)\n\n"
	@if ($(USE_NEW_WINDOW)) then $(RUN_ON_WINDOW_COMMAND) 2> /dev/null; else ($(TARGET) $(MAINARGS)); fi;

runToFile: default
	@printf "$(PREFIX) Running target: $(TARGET)\n"
	@printf "$(PREFIX) Redirecting stdout to: $(OUT_FILE)\n\n"
	@$(TARGET) $(MAINARGS) $(OUT)

runWithWait: $(TARGET)
	-@$(TARGET) $(MAINARGS)
	@printf "\n\n\n"
	@read  -r -p $(PRESS_TO_EXIT_MSG) key

clean:
	@printf "$(PREFIX) Deleting output directory..."
	@rm -rf $(OUTDIR)
	@printf " $(DONE)"

deepclean: clean
	@printf "$(PREFIX) Deleting object directory..."
	@rm -rf $(OBJDIR)
	@printf " $(DONE)"
	
format:
	@printf "$(PREFIX) Formatting all files..."
	@$(FORMATTER) -style=file $(SOURCES) $(HEADERS) -i
	@printf " $(DONE)"

memdiag: $(TARGET)
	@printf "$(PREFIX) Running $(MEMPROFILER)...\n"
	@$(MEMPROFILER) $(TARGET) $(MAINARGS)
	@printf "$(DONE)"

