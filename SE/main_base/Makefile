##########################################################################
#	This program is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>.
##########################################################################


##########################################################################
#
#	Makefile to make alsmost every single c or c++ project
#
##########################################################################
CC=g++

##########################################################################
#
#	Build options
#
##########################################################################
BUILD=-O3 -Wall -std=c++0x

##########################################################################
#
#	Libraries -lmylib or "-LPATH" -lmylib
#
##########################################################################
LIBS=-lpthread -lwiringPi
LIBS+=-lsfml-graphics -lsfml-system -lsfml-window -lsfml-network

##########################################################################
#
#	Include paths "-IPATH"
#
##########################################################################
CFLAGS=

##########################################################################
#
#	Binary name
#
##########################################################################
EXEC=base_soft

##########################################################################
#
#	Build folder to store the .o (can be empty "BUILDPATH=.")
#
##########################################################################
BUILDPATH=build

##########################################################################
#
#	Source folder to find the sources to compile ./ sources: "SRCPATH=."
#	Will automatically search for sub directories sources
#
##########################################################################
SRCPATH=src




##########################################################################
##########################################################################
##
##
##	Do not modify after this point
##
##
##########################################################################
##########################################################################
SRC=$(wildcard $(SRCPATH)/*.cpp) $(wildcard $(SRCPATH)/**/*.cpp)
OBJ=$(addprefix $(BUILDPATH)/,$(SRC:$(SRCPATH)/%.cpp=%.o))
DIRS=$(sort $(addprefix $(BUILDPATH)/,$(dir $(SRC:$(SRCPATH)/%=%))))

all: mkbdirs $(EXEC)

mkbdirs:
	@mkdir -p $(DIRS)

new: clean all

run:
	./$(EXEC)
	
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(BUILD)

$(BUILDPATH)/%.o: $(SRCPATH)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS) $(BUILD)
	
.PHONY: mkbdirs all clean $(EXEC) $(BUILDPATH)/%.o new run
	
clean:
	@rm -rf $(BUILDPATH)/*.o
	@rm -rf $(BUILDPATH)/**/*.o
	@rm -rf $(EXEC)