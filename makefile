MAKEFLAGS += --no-print-directory

TARGET_NAME = mscppi
TARGET = $(OUTDIR)/$(TARGET_NAME)

CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -MMD -O3
MEMPROFILER = valgrind
FORMATTER = clang-format

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
	$(MAKE) $(TARGET)

$(TARGET): $(OBJECTS) | $(OUTDIR)
	@printf "$(PREFIX) Linking..."
	@$(CC) -o $(TARGET) $^ $(CFLAGS) 
	@printf " $(DONE)"


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@printf "$(PREFIX) Compiling $<..."
	@$(CC) -c $< -o $@ $(CFLAGS)
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


.PHONY: clean deepclean run debug

run: $(TARGET)
	@printf "$(PREFIX) Running target: $(TARGET)\n\n"
	@$(TARGET) $(MAINARGS)

runToFile: $(TARGET)
	@printf "$(PREFIX) Running target: $(TARGET)\n"
	@printf "$(PREFIX) Redirecting stdout to: $(OUT_FILE)\n\n"
	@$(TARGET) $(MAINARGS) $(OUT)
	

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
