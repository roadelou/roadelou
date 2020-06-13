# Root directory of the git repository
top = $(CURDIR)

# All source files
src_error = $(top)/src/roadelou_error.c
src = $(src_error)

# Flag to include all headers
include = -I$(top)/include/

# Flag to trigger compiler warnings
warn = -Wall -pedantic

# All intermediary object files
obj_error = $(top)/bin/error.o
obj = $(obj_error)

# Flags used to compile the object files.
obj_flags = -c $(include) -fPIC $(warn)

# The library produced after the compilation
lib = $(top)/bin/libRoadelou.so

# The compiler flags used to make the library
lib_flags = $(include) -shared -Wl,-soname,libRoadelou.so

# Source to perform all the tests
test_src = $(top)/test/test_error.c

# Executable used to perform all the tests.
# Must be dynamically linked against the produced library.
test_out = $(top)/bin/test_libRoadelou.elf

# Compiler flags used to create the test file and link it against libRoadelou
test_flags = $(include) $(warn) -L $(top)/bin/ -lRoadelou

# A shell script to run all the tests
test_script = $(top)/bin/test.sh

.PHONY: clean install test

# Because test will already output some messages, we do not want make to add its own (that makes the output less readable).
.SILENT: test

$(obj): $(src)
	$(CC) $(obj_flags) $(src_error) -o $(obj_error)

$(lib): $(obj)
	$(CC) $(lib_flags) $(obj) -o $(lib)

$(test_out): $(lib) $(test_src)
	$(CC) $(test_flags) $(test_src) -o $(test_out)

test: $(test_out)
	chmod +x $(test_script)
	# Running the tests
	$(test_script)

clean:
	# Cleaning the bin directory
	rm -f $(obj) $(lib) $(test_out)

install:
	$(info Not implemented yet)
