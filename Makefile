#############################################################################
# Makefile for building: libmathparser.so.x.x.x
# Template: lib
#############################################################################
MAKEFILE      = Makefile

####### Compiler, tools and options
MAJOR   = 1
MINOR   = 0
MICRO   = 0
VERSION       = $(MAJOR).$(MINOR).$(MICRO)
CC            = gcc
CXX           = g++
DEFINES       = -DPSSMATHPARSER_LIBRARY -DVERSION=$(VERSION)
CFLAGS        = -g -Wall -Wextra -W -D_REENTRANT -fPIC -O3 $(DEFINES)
CXXFLAGS      = -g -std=gnu++11 -Wall -Wextra -W -D_REENTRANT -fPIC -O3 $(DEFINES)
CXXFLAGS_VALGRIND = -g -std=gnu++11 -Wall -Wextra -W -D_REENTRANT -fPIC -O0 $(DEFINES)
LDFLAGS       = -shared
RM            = rm -f -r
NAME          = pssmathparser
MKDIR         = mkdir -p
OUTPUT_DIR    = ./lib/so
OUTPUT_DIR_VALGRIND = ./lib/so/valgrind
SOURCES_DIR   = ./src
TARGET_LIB    = $(OUTPUT_DIR)/lib$(NAME).so.$(VERSION)
TARGET_LIB_VALGRIND = $(OUTPUT_DIR_VALGRIND)/lib$(NAME).so.$(VERSION)

SRCS          = $(SOURCES_DIR)/pssmathparser.cpp
HDRS          = $(SOURCES_DIR)/pssmathparser.h $(SOURCES_DIR)/pssmathparser_global.h
OBJS          = $(SRCS:.c=.o)

.PHONY: all

.PHONY: clean

all: $(TARGET_LIB)

valgrind: $(TARGET_LIB_VALGRIND)

$(TARGET_LIB): $(OBJS)
	$(MKDIR) $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) ${LDFLAGS} -o $@ $^
	cd $(OUTPUT_DIR) && ln -s lib$(NAME).so.$(VERSION) lib$(NAME).so.$(MAJOR).$(MINOR)
	cd $(OUTPUT_DIR) && ln -s lib$(NAME).so.$(MAJOR).$(MINOR) lib$(NAME).so.$(MAJOR)
	cd $(OUTPUT_DIR) && ln -s lib$(NAME).so.$(MAJOR) lib$(NAME).so

$(TARGET_LIB_VALGRIND): $(OBJS)
	$(MKDIR) $(OUTPUT_DIR_VALGRIND)
	$(CXX) $(CXXFLAGS_VALGRIND) ${LDFLAGS} -o $@ $^
	cd $(OUTPUT_DIR_VALGRIND) && ln -s lib$(NAME).so.$(VERSION) lib$(NAME).so.$(MAJOR).$(MINOR)
	cd $(OUTPUT_DIR_VALGRIND) && ln -s lib$(NAME).so.$(MAJOR).$(MINOR) lib$(NAME).so.$(MAJOR)
	cd $(OUTPUT_DIR_VALGRIND) && ln -s lib$(NAME).so.$(MAJOR) lib$(NAME).so

clean:
	$(RM) $(OUTPUT_DIR)/*
	$(RM) ../build-pssmathparser*
	$(RM) ./*.pro.user*
