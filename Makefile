BIN := main

DST_DIR := ./build
SRC_DIR := ./src

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(DST_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I ,$(INC_DIRS))

CC := gcc
CFLAGS := $(INC_FLAGS) -MMD -MP -Wall
LDFLAGS :=

# Verbose/Non-verbose build modes
V := @

REQUIRED_TOOLS := gcc find sed column mkdir rm
$(foreach tool,$(REQUIRED_TOOLS),\
    $(if $(shell command -v $(tool)),,$(error "No $(tool) in $(PATH)")))

## all: Build the main executable
all: $(DST_DIR)/$(BIN)

$(DST_DIR)/$(BIN): $(OBJS)
	$(V)$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(DST_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(V)$(CC) $(CFLAGS) -c $< -o $@

## debug: Build with debug information
debug: CFLAGS += -g --verbose
debug: V :=
debug: all

## run: Run the built executable
run: $(DST_DIR)/$(BIN)
	$(V)$(DST_DIR)/$(BIN)

## clean: Clean the build directory
clean:
	@rm -rf $(DST_DIR)

## help: Show this help message
help:
	@echo 'Targets:'
	@sed -n 's/^##//p' $(MAKEFILE_LIST) | column -t -s ':' |  sed -e 's/^/ /'

-include $(DEPS)

.PHONY: all debug run clean help
