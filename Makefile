.PHONY: configure build run test clean FORCE

configure: FORCE
	cmake -GNinja -S . -B build $(CMAKE_ARGS)

build: configure
	ninja -C build

run: build
	./build/src/a.out

test: build
	./build/test/googleTest

clean: FORCE
	rm -rf build