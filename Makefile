
# result exe filepath
target_fn=main


src=src
headerDir=src
#build dir
build=build
target=$(build)/$(target_fn)

objCache=$(build)/objs

srcs=$(wildcard $(src)/*.c)

objs=$(patsubst $(src)/%.c,$(objCache)/%.o,$(srcs)) 


# compile flag
to_obj=$(CC) -c

incDirFlag=-I

incDir=$(incDirFlag) $(headerDir)
#rm=rm
$(target):$(objs)
	$(CC) $(CFLAGS) $+ -o $@


$(objCache)/%.o:$(src)/%.c
	$(to_obj) $(CFLAGS) $(incDir) $^ -o $@



