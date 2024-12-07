all: build  run
	
build:
	g++ -g -Iinclude -Llib src/*.c  -o bin/main.exe -lglfw3dll 
	
run:
	./bin/main.exe