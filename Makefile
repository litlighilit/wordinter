# usage:
#  make  -> make build=build
#  make build=<build_xxx> -> build in dir <build_xxx>
# P.S. /build_*/ has been added to .gitignore

# Special build:
#  build_dbg*	will add -g
#  build_MSVC	will use MS Visuall C++ Tools

# result exe filename
target_fn?=wordinter


src=src
headerDir=src
#build dir
build?=build
CFLAGS=-std=c99

ifneq (,$(findstring build_dbg,$(build)))  # if contains build_dbg
CFLAGS+=-g
else
CFLAGS+=-O2
endif

outflag?=-o
ifeq ($(build),build_MSVC)
CC=cl
outflag=/OUT:
endif

target=$(build)/$(target_fn)

# objCache will be used for check and make dir via `cmd` command
ifeq ($(OS), Windows_NT)
SHELL=cmd
objCache=$(build)\objs
else
objCache=$(build)/objs
endif

srcs=$(wildcard $(src)/*.c)

objs=$(patsubst $(src)/%.c,$(objCache)/%.o,$(srcs)) 


# compile flag
to_obj=$(CC) -c

incDirFlag=-I

incDir=$(incDirFlag) $(headerDir)
#rm=rm
$(target):$(objs)
	$(CC) $(CFLAGS) $+ $(outflag)$@

$(objCache):
ifeq ($(OS), Windows_NT)
	if not exist $(objCache) ( md $(objCache) )
else
	mkdir -p $(objCache)
endif

$(objCache)/%.o:$(src)/%.c $(src)/%.h | $(objCache)
	$(to_obj) $(CFLAGS) $(incDir) $< $(outflag) $@



