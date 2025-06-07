all: build

build: main

main:
	mkdir -p build && cd build && cmake .. && make && src/matrix_app ../src/config_cpu.json

clean:
	rm -rf build

rebuild: clean build