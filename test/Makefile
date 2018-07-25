#############################################################################
# Makefile for building: main that performs test1
# Template: exe
#############################################################################
MAKEFILE      = Makefile

####### Compiler, tools and options
CC            = gcc
CXX           = g++
CFLAGS        = -g -Wall -Wextra -W
CXXFLAGS      = -g -std=gnu++11 -Wall -Wextra
CXXFLAGS_VALGRIND = -g -std=gnu++11 -Wall -Wextra -O0
INCLUDES      = -I$(PWD)/../src/
LDFLAGS       = -Wl,-R$(PWD)/../lib/so -L$(PWD)/../lib/so
LDFLAGS_VALGRIND = -Wl,-R$(PWD)/../lib/so/valgrind -L$(PWD)/../lib/so/valgrind
LIBS          = -lpssmathparser
RM            = rm -f -r
MKDIR         = mkdir -p

OUTPUT_DIR    = ../bin
SOURCES_DIR   = ./

T1	          = test1
TAR1          = $(OUTPUT_DIR)/$(T1)
SRC1          = $(SOURCES_DIR)/$(T1).cpp
OBJ1          = $(SRC1:.c=.o)

T2	          = test2
TAR2          = $(OUTPUT_DIR)/$(T2)
SRC2          = $(SOURCES_DIR)/$(T2).cpp
OBJ2          = $(SRC2:.c=.o)

T3	          = test3
TAR3          = $(OUTPUT_DIR)/$(T3)
SRC3          = $(SOURCES_DIR)/$(T3).cpp
OBJ3          = $(SRC3:.c=.o)

T4	          = test4
TAR4          = $(OUTPUT_DIR)/$(T4)
SRC4          = $(SOURCES_DIR)/$(T4).cpp
OBJ4          = $(SRC4:.c=.o)

T5	          = test5
TAR5          = $(OUTPUT_DIR_VALGRIND)/$(T5)
SRC5          = $(SOURCES_DIR)/$(T5).cpp
OBJ5          = $(SRC5:.c=.o)

.PHONY: all

.PHONY: $(T1)

.PHONY: $(T2)

.PHONY: $(T3)

.PHONY: $(T4)

.PHONY: $(T5)

.PHONY: clean

all: $(TAR1) $(TAR2) $(TAR3) $(TAR4) $(TAR5)

$(T1) : $(TAR1)

$(T2) : $(TAR2)

$(T3) : $(TAR3)

$(T4) : $(TAR4)

$(T5) : $(TAR5)

$(TAR1) : $(OBJ1)
	$(MKDIR) $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(INCLUDES) $(LIBS)

$(TAR2) : $(OBJ2)
	$(MKDIR) $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(INCLUDES) $(LIBS)

$(TAR3) : $(OBJ3)
	$(MKDIR) $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(INCLUDES) $(LIBS)

$(TAR4) : $(OBJ4)
	$(MKDIR) $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(INCLUDES) $(LIBS)

$(TAR5) : $(OBJ5)
	$(MKDIR) $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS_VALGRIND) $(LDFLAGS_VALGRIND) -o $@ $^ $(INCLUDES) $(LIBS)

clean:
	$(RM) $(OUTPUT_DIR)/*
	$(RM) $(OUTPUT_DIR)/../build-test*
	$(RM) $(SOURCES_DIR)/*.pro.user
