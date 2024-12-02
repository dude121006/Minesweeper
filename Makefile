all:
	g++ -Iinclude -Llib src/*.c  -o bin/main.exe -lglfw3dll 
	./bin/main.exe