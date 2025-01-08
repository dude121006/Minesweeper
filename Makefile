all: build  run
	
build:
	g++ -g -Iinclude -Isrc/depend/stb_image -Isrc/depend/glm -Llib src/depend/stb_image/*.c src/*.c   -o bin/main.exe -lglfw3dll 
	
run:
	./bin/main.exe