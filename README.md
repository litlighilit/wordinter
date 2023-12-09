

## factors

### runtime
- allow any numbers of files in input directory
- allow any amount of file contents
- allow run via either stdin-based REPL or CLI

### compile
- allow both linux and windows
- build via `make`
- using git as VCS
- self-write a framework for `doctest`
- all headers are correctly wrapped by condtion preprocess directives to avoid multi-included

## difficulty

### cmdline
to ensure MSVC-compatiablity, I'm enforced to say goodbye to `getopt`, which means it comes harder to parse cmdline... and I have to write quite a few code myself...

And due to this is only and has to be a C project, I'm limited to not use some useful and powerful libraries like [cmdline](https://github.com/tanakh/cmdline), [gflags](https://github.com/gflags/gflags), ... as they are for C++...


### directory IO
Also, still due to MSVC-compatiablity, I have to write a inner abstract code shell for the functionality of `listDir` (see "fsutil.h")


### sequence & string


while a large number of programming languages own builtin sequance type: like Python's `list`, C++'s `vector`, Rust's `vec`, Haskell's `List`, Java's `ArrayList`, Go's slice, ... No need to go on enumerating, otherwise it'll be a quite long list! All in all, as C has no builtin sequence, then I had to write on my own.

What's more, as a result of the absence of useful `generics` in traditional C (though `_Generics` is added since c11, it's limited and does little fover to write extensive `generics`), I have to implement my `seq` type all by macros...

Meanwhile, out of the same reason, 
while C++'s std::string useful, c's `char*` or `char[]` is really hard to manipulate.

Get fed up with the weakness of the c's `<string.h>` and lack of some highly-needed string functions like `split`, `join`, `+`, I finally decided to implement my own string type, named `CharSeq`(see "strimpl.h")

 and needed string utilities (see "strutils.h")
