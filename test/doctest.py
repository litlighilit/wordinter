'doctest for c code'
from __future__ import annotations

ENC = 'utf-8'
import os,sys

from pathutils import *
import ccode as code

class Dir:
    base = Path("..").absolute()
    src = base/"src"
    
    cache = Path.cwd()/"cache"
    obj_cache = cache/"obj"
    

D = Dir()
#print(D.src)
def init():
    D.cache.mkdir(exist_ok=True)
    D.obj_cache.mkdir(exist_ok=True)

init()

class TestSep:
    op = code.comments[0] + 'doctest:'
    ed = code.comments[1]

ExitLevel = 2

def warn(msg):
    print('[WARN]', msg)
    if ExitLevel <= 2: exit(-1)

NL = "\r\n"
def uniline(line): return line.strip(NL)

def parse_cmd(uniline, deps_in, blt_headers_in):
    op = 'include('
    idx = uniline.find(op)
    if idx==-1: return
    s=uniline[idx+len(op):]
    if s[-1] == ')': s=s[:-1]
    ls = s.split(',')
    for i in ls:
        header = i.strip('"')
        if header[0]=='<':
            blt_headers_in.append(header)
        else:
            deps_in.append( Path(header) )
ARGS = [#" -D SEQ_CHECK_RANGE "
]

DEBUG = True
if DEBUG:
  ARGS.append(" -g ")

def _doctest(fp: Path) -> int:
    hfp = fp.with_suffix('.h')
    cfp = fp.with_suffix('.c')
    test_body = ''
    deps: list[Path] = []
    
    more_blt_headers: list[str] = []  # decl via include("<xxx.h>")
    
    with hfp.open(encoding=ENC) as f:
        for l in f:
            ul = uniline(l)
            if ul.startswith(TestSep.op):
                parse_cmd(ul[len(TestSep.op):], deps, more_blt_headers)
                test_body = parse_content(f)
                cuHs = set()
                parse_cusm_headers_r(cuHs, f, fp.parent)
                print(cuHs)
                for i in cuHs:
                    cf = fp.parent/with_ext(i, '.c')
                    if not cf.exists(): continue  # for header only files
                    deps.append(cf)
                break
    if not test_body:
        warn(f'no begining {TestSep.op} found')
        return -2
    hs = [ f'"{hfp.name}"' ] + more_blt_headers
    test_code = code.fmt_main(test_body, head=code.include_list(hs))
    
    t_fp = D.cache/("test_" + cfp.name)
    
    t_fp.write_text(test_code, encoding=ENC)
    
    if cfp.exists(): deps.append(cfp)
    
    objs = code.compile.obj(deps, lambda fp: D.obj_cache/(fp.with_suffix('.o').name), args=ARGS )
    
    files = objs + [ t_fp ]
    
    outfn = t_fp.with_suffix('')
    
    code.compile.exe( files, outfn, args=ARGS )
    
    ret = os.system(str(outfn))
    
    return ret


def doctest(fp: Path):
    code.compile = code.Compile(str(fp.parent))
    cwd = os.getcwd()
    ret = 1
    try:
        os.chdir(fp.parent)
        ret = _doctest(fp)
    finally:
        os.chdir(cwd)
    return ret

def capture_between(s, op, ed=None, start=0) -> str:
    'ValueError if `op not in s[start:]`'
    op_idx = s.index(op, start) + len(op)
    if ed:
        ed_idx = s.index(ed, op_idx)
        res = s[op_idx:ed_idx]
    else:
        res = s[op_idx:]
    return res
        
    
def parse_cusm_headers(f) -> list[str]:
    res = list()
    for l in f:
        op,ed = code.includes
        if l.startswith(op):
            res.append(capture_between(l, op, ed))
    return res
def parse_cusm_headers_r(res: set[str], f, dir: Path):

    oset = res.copy()

    ls = parse_cusm_headers(f)
    res.update(ls)

    if res==oset:  return

    for i in ls:
        with open(dir/i, encoding=ENC) as nf:
            parse_cusm_headers_r(res, nf, dir)
    
    return res


def parse_content(f):
    res = ''
    for l in f:
        if uniline(l) == TestSep.ed: return res
        res += l
    raise ValueError(f"in {f.name}, no end sep '{TestSep.ed}' found")

if __name__ == '__main__':
    args = sys.argv[1:]
    for fn in args:
        doctest(D.src/fn)

