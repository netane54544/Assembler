# Compiler and compiler flags
CC = gcc
CFLAGS = -std=c90 -Wall -ansi -pedantic

# Source files
SRCS = Lexer.c Lexer.h main.c Parser.c Parser.h Token.h \
       PreProcessor/pre-processor.c PreProcessor/pre-processor.h \
       PreProcessor/toolFunctions.c PreProcessor/toolFunctions.h \
       Utils/Base64.h Utils/Base64.c Utils/DecimalToBinary.c Utils/DecimalToBinary.h \
       Utils/FileSystem.c Utils/FileSystem.h Utils/Globals.c Utils/Globals.h Utils/Strdup.c Utils/Strdup.h Utils/CStrcat.c Utils/CStrcat.h Utils/CustomStrtok.c Utils/CustomStrtok.h \
       LinkedLists/Linked_List_AsmLines.c LinkedLists/Linked_List_AsmLines.h \
       LinkedLists/Linked_List_ParseBit.c LinkedLists/Linked_List_ParseBit.h \
       LinkedLists/Linked_List_Symbols.c LinkedLists/Linked_List_Symbols.h \
       LinkedLists/Linked_List_Token.c LinkedLists/Linked_List_Token.h \
       LinkedLists/linked-macro.h LinkedLists/linked-macro.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = Assembler

# Default target
all: $(TARGET)

# Compile source files into object files
#There is a problem with the linked lists the pointers are ok and the convertion is ok,
#but the gcc compailer thinks the conversion is not ok, but it should be ok.
#This is why I removed incompatible-pointer-types warning for this files, you can check without the flag at the makefile for this file and find out the converstion is ok */
%.o: %.c 
	$(CC) $(CFLAGS) -Wno-incompatible-pointer-types -c $< -o $@

# Link object files into the executable
$(TARGET): $(OBJS) 
	$(CC) $(OBJS) -o $(TARGET)

# Clean up intermediate and executable files
clean: rm -f 
	$(OBJS) $(TARGET)
