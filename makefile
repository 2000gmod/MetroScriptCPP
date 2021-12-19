MAKEFLAGS += --no-print-directory

TARGET = $(OUTDIR)/mscppi
MAINARGS = -m

CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -MMD


OBJDIR = obj
SRCDIR = src

OUTDIR = out

SOURCES := $(shell find . -name '*.cpp')
OBJECTS := $(subst .cpp,.o, $(subst ./src,./$(OBJDIR),$(SOURCES)))
DEPS := $(shell find . -name '*.d')

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
	@clang-format-13 -style=file $(SOURCES) $(HEADERS) -i
	@printf " $(DONE)"

