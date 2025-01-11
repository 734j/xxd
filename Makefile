CC=clang++
CFLAGS_TESTBIN=-O0 -Wfatal-errors -Wall -Werror -Wextra -g3 -fsanitize=address -fsanitize=leak -Wpedantic -Wformat=2 -Wshadow -fno-common -std=c++20 -D__TEST_STREAM_STDERR
CFLAGS=-O3 -flto -march=native -DNDEBUG -fomit-frame-pointer -s -static -std=c++20
TARGET=xxd
TESTTARGET=xxd-TESTING
INSTALL_DIRECTORY=/usr/local/bin
MAKEFLAGS += 
SRCS=xxd.cpp

all: release
clean:
	rm -f $(TARGET)
	rm -f test/$(TESTTARGET)

tests:
	$(CC) $(CFLAGS_TESTBIN) $(SRCS) -o test/$(TESTTARGET)

install:
	cp $(TARGET) $(INSTALL_DIRECTORY)

release:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)
