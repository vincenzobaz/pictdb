CC = gcc
CFLAGS += -Wall -std=c99 -g
LDLIBS += -lssl -lcrypto
SRCS = db_utils.c pictDBM.c db_list.c error.c db_create.c db_delete.c
#correcteur: utilisez les .o et pas les .c
pictDBM: $(SRCS)
	$(CC) $(LDLIBS) $(CFLAGS) -o pictDBM $(SRCS)

all: pictDBM

test: pictDBM
	# Try to save output to variable. Probably bad end of file. http://blog.melski.net/2010/11/15/shell-commands-in-gnu-make/
	#OUTPUT := $(shell ./pictDBM list testDB01.pictdb_static && ./pictDBM list testDB02.pictdb_static >> test_output.txt)
	#diff -s -w $(OUTPUT) correct_output.txt
	./pictDBM list testDB01.pictdb_static > test_output.txt
	./pictDBM list testDB02.pictdb_static >> test_output.txt
	# -e only show differing parts -s tells if file equal -q tells if file differ
	diff -y -w test_output.txt correct_output.txt
	diff -s -w test_output.txt correct_output.txt
	diff -q -w test_output.txt correct_output.txt
