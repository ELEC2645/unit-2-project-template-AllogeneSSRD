# makefile for building the program. Each of these can be run from the command line like "make hello.out".
# "make clean" deletes the exectuable to build again 
# "make test" builds the main file and then runs the test script. This is what the autograder uses
# 
# Note to students: You dont need to fully understand this! 

# the -lm at the end links the math library

main.exe:
	gcc main.c funcs.c -o main.exe -lm
#	C:\cygwin64\bin\gcc.exe main.c funcs.c -o main_cygwin.exe -lm
# 	gcc .\test\test_input_float.c -o test_input_float.exe -lm

main.out:
	gcc main.c funcs.c -o main.out -lm

clean:
	-rm main.out

test: clean main.out
	bash test.sh

test_funcs:
	gcc -o test_funcs test/test_funcs.c test/cutest/CuTest.c funcs.c -lm
# windows
	.\test_funcs.exe
# linux
# 	./test_funcs.exe
