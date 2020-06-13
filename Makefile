# Root directory of the git repository
top = $(CURDIR)

# All source files
src_error = $(top)/src/roadelou_error.c
src_test = $(top)/src/roadelou_test.c
src = $(src_error) $(src_test)

# Flag to include all headers
include = -I$(top)/include/

# Flag to trigger compiler warnings
warn = -Wall -pedantic

# All intermediary object files
obj_error = $(top)/bin/error.o
obj_test = $(top)/bin/test.o
obj = $(obj_error) $(obj_test)

# Flags used to compile the object files.
obj_flags = -c $(include) -fPIC $(warn)

# The library produced after the compilation
lib = $(top)/bin/libRoadelou.so

# The compiler flags used to make the library
lib_flags = $(include) -shared -Wl,-soname,libRoadelou.so

# Source to perform all the tests
test_src_error = $(top)/test/test_error.c
test_src_test = $(top)/test/test_test.c
test_src = $(test_src_error) $(test_src_test)

# Executables used to perform all the tests.
# Must be dynamically linked against the produced library.
test_out_error = $(top)/bin/test_error.elf
test_out_test = $(top)/bin/test_test.elf
test_out = $(test_out_error) $(test_out_test)

# Compiler flags used to create the test file and link it against libRoadelou
test_flags = $(include) $(warn) -L $(top)/bin/ -lRoadelou

# A shell script to run all the tests
test_script = $(top)/bin/test.sh

.PHONY: clean install test

# Because test will already output some messages, we do not want make to add
# its own (that makes the output less readable).
.SILENT: test

$(obj): $(src)
	$(CC) $(obj_flags) $(src_error) -o $(obj_error)
	$(CC) $(obj_flags) $(src_test) -o $(obj_test)

$(lib): $(obj)
	$(CC) $(lib_flags) $(obj) -o $(lib)

$(test_out): $(lib) $(test_src)
	$(CC) $(test_flags) $(test_src_error) -o $(test_out_error)
	$(CC) -g $(test_flags) $(test_src_test) -o $(test_out_test)

test: $(test_out)
	chmod +x $(test_script)
	# Running the tests
	(cd $(top)/bin && bash $(test_script))

clean:
	# Cleaning the bin directory
	rm -f $(obj) $(lib) $(test_out)

install:
	$(info Not implemented yet)
