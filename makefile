CC = g++
CFLAGS = -Wall -g
LEX = flex
YACC = bison
LFLAGS = -lfl
YFLAGS = -d

TARGET = tree_builder

.PHONY: all clean test

.SILENT: all clean test $(TARGET) lex.yy.c tree_builder.tab.c tree_builder.tab.h

all: $(TARGET)

lex.yy.c: tree_builder.l
	$(LEX) tree_builder.l

tree_builder.tab.c tree_builder.tab.h: tree_builder.y
	$(YACC) $(YFLAGS) tree_builder.y

$(TARGET): lex.yy.c tree_builder.tab.c
	$(CC) $(CFLAGS) -o $(TARGET) lex.yy.c tree_builder.tab.c $(LFLAGS)

test: test_head.cpp
	$(CC) $(CFLAGS) -o test test_head.cpp

clean:
	rm -f lex.yy.c tree_builder.tab.c tree_builder.tab.h $(TARGET) test