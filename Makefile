.PHONY: configure build run clean FORCE

configure: FORCE
	cmake -S . -B build

build: configure
	cmake --build build

run: build
	./build/src/a.out

clean: FORCE
	rm -rf build