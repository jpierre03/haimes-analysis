# Source : http://linux.leunen.com/?p=21
CC := c++
CFLAGS := -Wall -std=c++11
#CFLAGS :=
EXEC_NAME := my.bin
INCLUDES =
LIBS =

#OBJ_FILES = fichier_1.o fichier_2.o
SRC_FILES := $(wildcard *.cpp)
OBJ_FILES := $(patsubst %.cpp,%.o,$(SRC_FILES))

#INSTALL_DIR = /usr/bin
INSTALL_DIR := ${HOME}/bin

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.c
	gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

install :
	cp $(EXEC_NAME) $(INSTALL_DIR)

run:	$(EXEC_NAME)
	./$(EXEC_NAME)

