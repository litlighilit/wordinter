
from pathlib import Path

def with_ext(s: str, ext):
    return s.rsplit('.',1)[0] + ext

