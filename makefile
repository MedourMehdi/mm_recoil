_CC = m68k-atari-mint-gcc
_CXX = m68k-atari-mint-g++
SRC_DIR := ./
OBJ_DIR := ./build
OBJ_DIR_C := ./build
BIN_DIR := ./bin

SRC_C := $(wildcard $(SRC_DIR)/*/recoil/*.c)

LIB_FREETYPE := -lfreetype -lbz2

SRC := $(wildcard $(SRC_DIR)/*.cpp) \
  $(wildcard $(SRC_DIR)/*/*.cpp) \
  $(wildcard $(SRC_DIR)/*/zview/*.cpp) \
  $(wildcard $(SRC_DIR)/*/dither/*.cpp) \

# BIN := $(BIN_DIR)/mm_recoil68k.prg
BIN := $(BIN_DIR)/mm_recoil.prg
# BIN := $(BIN_DIR)/mm_recoilCF.prg

OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJ_C := $(SRC_C:$(SRC_DIR)/%.c=$(OBJ_DIR_C)/%.o)

_CPPFLAGS := -I./

_CFLAGS   := -m68020-60 -fomit-frame-pointer -fno-strict-aliasing -O2 
# _CFLAGS   := -mcfv4e -fomit-frame-pointer -fno-strict-aliasing -O2 
# _CFLAGS   := -m68000 -fomit-frame-pointer -fno-strict-aliasing -O2
_LDFLAGS  :=

_LDLIBS   := -lgem -lpng -lz -lm -lyuv $(LIB_FREETYPE) -lpthread

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ) $(OBJ_C) | $(BIN_DIR)
	$(_CXX) $(_LDFLAGS) $^ $(_LDLIBS) -o $@
	m68k-atari-mint-strip $(BIN)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(_CXX) $(_CPPFLAGS) $(_CFLAGS) -c $< -o $@

$(OBJ_DIR_C)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR_C)
	@mkdir -p $(@D)
	$(_CC) $(_CPPFLAGS) $(_CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $(@D)

clean:
	@$(RM) -rv $(BIN) $(OBJ_DIR)

clean-objects:
	@$(RM) -rv $(OBJ_DIR)

-include $(OBJ:.o=.d)
-include $(OBJ_C:.o=.d)