.PHONY: configure build run test clean FORCE
	
CLANG_FORMAT=clang-format-14

CMAKE_ARGS+=-DCMAKE_TOOLCHAIN_FILE=$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake

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
	find src -name "*.cpp" | xargs $(CLANG_FORMAT) -i
	find test -name "*.cpp" | xargs $(CLANG_FORMAT) -i
	find include -name "*.h" | xargs $(CLANG_FORMAT) -i