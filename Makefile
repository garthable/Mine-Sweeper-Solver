COMPILER := g++
CVERSION := -std=c++20
CFLAGS := -g -O3
EXECUTABLENAME := runner

standard:
	@mkdir -p build
	cd build; \
	$(COMPILER) $(CFLAGS) $(CVERSION) -I ./../include*.cpp ../src/*.cpp -o $(EXECUTABLENAME)

clean:
	rm -f -r build/*