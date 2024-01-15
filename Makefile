# The GPLv3 License (GPLv3)
#
# Copyright (c) 2024 Vladislav 'ElCapitan' Nazarov (AT PROJECT)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
COM      	:= cc
CFLAGS 		:= -Wall -Wextra -g -DLOG_USE_COLOR -std=c89 -Oz
LFLAGS   	:=  
SRCDIRS	  := src
BUILDDR		:= build
OUT      	:= out
MAIN	    := server
FILES			:= files

SOURCES   := $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJECTS	  := $(patsubst $(SRCDIRS)/%.c, $(BUILDDR)/%.o, $(SOURCES))
OUTFILE		:= $(OUT)/$(MAIN)

ifdef DEBUG
CFLAGS += -Dkugisaki_DEBUG
endif

all: dirs $(MAIN)
	cp -r $(FILES)/* $(OUT)/
	@echo Done!

dirs :
	@mkdir -p $(BUILDDR)
	@mkdir -p $(OUT)

$(MAIN): $(OBJECTS)
	$(COM) $(CFLAGS) -o $(OUTFILE) $(OBJECTS) $(LFLAGS)

$(BUILDDR)/%.o: $(SRCDIRS)/%.c
	$(COM) $(CFLAGS) -c $<  -o $@

$(OUT):
	mkdir -p $(OUT)

clean:
	rm -rf $(OUT)/*
	rm -f $(BUILDDR)/*

