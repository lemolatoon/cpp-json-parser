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

fmt: FORCE
	find src -name "*.cpp" | xargs clang-format -i
	find test -name "*.cpp" | xargs clang-format -i
	find include -name "*.h" | xargs clang-format -i