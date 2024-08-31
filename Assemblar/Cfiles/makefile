CC = gcc
CXXFLAGS = -Wall -ansi -pedantic -g -I  ../Headers
LDFLAGS = 

APPNAME = assembler # assembler = AssemblyCompiler

.PHONY: clean main valgrind

# compiler targets
main: assembler.o code_helper.o Errors.o first_and_second_pass.o first_pass_helper.o link_list_strct.o pre_assemblar.o symbol_table.o util.o code_helper.o
	$(CC) $(LDFLAGS) -o $(APPNAME) $^

assembler.o: assembler.c
	$(CC) $(CXXFLAGS) -c $< -o $@

Errors.o: Errors.c
	$(CC) $(CXXFLAGS) -c $< -o $@

first_and_second_pass.o: first_and_second_pass.c
	$(CC) $(CXXFLAGS) -c $< -o $@

first_pass_helper.o: first_pass_helper.c
	$(CC) $(CXXFLAGS) -c $< -o $@

link_list_strct.o: link_list_strct.c
	$(CC) $(CXXFLAGS) -c $< -o $@

pre_assemblar.o: pre_assemblar.c
	$(CC) $(CXXFLAGS) -c $< -o $@

util.o: util.c
	$(CC) $(CXXFLAGS) -c $< -o $@

symbol_table.o: symbol_table.c
	$(CC) $(CXXFLAGS) -c $< -o $@

code_helper.o: code_helper.c
	$(CC) $(CXXFLAGS) -c $< -o $@



# valgrind
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind.out.txt ./$(APPNAME) $(ARGS)

clean:
	rm -f ./*.o valgrind.out.txt $(APPNAME)
