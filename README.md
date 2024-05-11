## Desc
A files-based search engine interpreter,

supporting:

- access file content in paragraphs
- query all occupations of one word
- count the number of words within one paragraphs
- ...(see `src/interpreter.c`)

## Factors

### Runtime
- allow any numbers of files in input directory
- allow any amount of file contents
- bound checks are round:
	- paragraph index
	- word index
- allow run via either stdin-based REPL or CLI
- when CLI:
	- can handle filenames that contains spaces (given surrounded by quotation marks)

### Compile-time
- target both POSIX and Windows
- build via `make`
- compatiable with C++ (can be compiled via C++ compiler)
- build document via `doxygen`
- self-write a framework for `doctest`
- several self-written modules can be used as frameworks, serving for other projects and depends nothing other than this project's modules.

## Difficulties for implementations

### cmdline
To ensure MSVC-compatiablity, I'm enforced to say goodbye to `getopt`, which means it comes harder to parse cmdline... and I have to write quite a few code myself...

And due to this is only and has to be a C project, I'm limited to not use some useful and powerful libraries like [cmdline](https://github.com/tanakh/cmdline), [gflags](https://github.com/gflags/gflags), ... as they are for C++...


### directory IO
Also, still due to MSVC-compatiablity, I have to write a inner abstract code shell for the functionality of `listDir` (see "fsutil.h")


### sequence & string
While a large number of programming languages own builtin sequance type: like Python's `list`, C++'s `vector`, Rust's `vec`, Haskell's `List`, Java's `ArrayList`, Go's slice, there's no builtin sequence in C, so I had to write my own sequence type.

What's more, as a result of the absence of useful `generics` in traditional C (though `_Generics` is added since c11, it's limited and does little faver to write extensive `generics`), I have to implement my `Seq` type all by macros...

Meanwhile, out of the same reason, 
while C++'s std::string useful, c's `char*` or `char[]` is really hard to manipulate.

Get fed up with the weakness of the c's `<string.h>` and lack of some highly-needed string functions like `split`, `join`, `+`, I finally decided to implement my own string type, named `CharSeq`(see "strimpl.h")

 and some needed string utilities (see "strutils.h")
