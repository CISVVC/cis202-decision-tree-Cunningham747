
all:
	@g++ -o main main.cpp

full:
	@g++ -c *.cpp
	@g++ -o main *.o

full-verbose:
	@g++ -c *.cpp -Wall
	@g++ -o main *.o
run:
	@./main

clean:
	@rm main

git-commit:
	git add .
	git commit -a -m 'update'

git-submit:
	git push
