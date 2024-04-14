CC=gcc

all:
	$(CC) ./src/Main.cpp -o ./connect4 -ggdb -lstdc++ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

prod:
	$(CC) ./src/main.c -o ./build/snake -std=c99 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -O3

wasm:	
	emcc -o snake.html ./src/main.c

clear:
	rm -rf ./build/* ./build/core.*
