.PHONY: all clean

define COMPILE_IT
	mkdir -p build
	gcc $(1)
endef

# clang needs -stdlib=libc++
COMPILER=$(shell (((gcc --version 2> /dev/null | grep clang) > /dev/null) && echo "clang"))

ifeq ($(COMPILER), clang)
STDLIBOPT=-stdlib=libc++
HDR="<experimental/coroutine>"
USING="std::experimental"
else
STDLIBOPT=
HDR="<coroutine>"
USING="std"
endif

EXE=build/foo
SRCS=$(shell find . -name '*.cpp' | sed -e 's,^\./,,')
OBJS=$(addprefix build/,$(SRCS:.cpp=.o))

all: $(EXE)

build/%.o: %.cpp
	$(call COMPILE_IT, -O0 -g --std=c++20 -Wall -Werror -pthread -c -o $@ $< -DCORO_HDR=$(HDR) -DCORO_USING=$(USING))

$(EXE): $(OBJS)
	gcc -lstdc++ -pthread $(STDLIBOPT) -o $@ $<

clean:
	rm -rf build
