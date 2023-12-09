from __future__ import annotations

import os

from collections.abc import Callable

from typing import Optional, Union

from pathutils import with_ext, Path

includes = ['#include "', '"']
comments = ['/*', '*/']

def include_list(headers: list)->str:
    res = ''
    for h in headers:
        res += "#include " + str(h) + '\n'
    return res
    
def fmt_main(body, head='', tail=''):
	return f"""{head}
int main(int argc, char* argv[]){{
{body}
}}
{tail}
"""
StrOrPath = Union[str, Path[str]]


class Compile:
    cc = 'gcc'
    shared_arg = '-shared'
    obj_arg = '-c'
    out_key = '-o'
    headerdir_key='-I'
    def __init__(self, header_dir=''):
        if header_dir:sarg = self.headerdir_key + ' ' + header_dir
        else: sarg=''
        self.headerdir_sarg = sarg
    def mkcmd(self, *sargs):
        ls = [self.cc, self.headerdir_sarg]
        for i in sargs:
            ls.append(str(i))
        cmd = " ".join(ls)
        print(cmd)
        return cmd
    def compile(self, file: StrOrPath, outfn: StrOrPath, args: list[str] = []) -> int:
        sargs = ' '.join(args)
        
        cmd = self.mkcmd(sargs, file, self.out_key, outfn)
        return os.system(cmd)
    def compile_list(self, files: list[StrOrPath],
            output_cb: Callable[ [StrOrPath], StrOrPath],
            args: list[str] = []) -> list[StrOrPath]:
        args = args or []
        res = []
        for f in files:
            outf = output_cb(f)
            res.append(outf)
            ret = self.compile(f, outf, args)
            if res!=0:
                print(f"Compile Error for {f}")
        return res

    #def so(self, files: list): return self.compile(files, args=[self.shared_arg])
    
    def obj(self, files: list, output_cb=lambda s: with_ext(s, '.o'), args=[]): return self.compile_list(files, output_cb, [self.obj_arg]+args)
    
    def exe(self, files: list, outfn, args=[]) -> int:
        sargs = ' '.join(args)
        sfiles = ''
        for fp in files:
            sfiles += str(fp) + ' '
        
        cmd = self.mkcmd(sargs, sfiles, self.out_key, outfn)
        return os.system(cmd)
