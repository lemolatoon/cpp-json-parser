.PHONY: configure build run test clean FORCE

configure: FORCE
	cmake -S . -B build

build: configure
	cmake --build build

run: build
	./build/src/a.out

test: build
	./build/test/googleTest

clean: FORCE
	rm -rf build