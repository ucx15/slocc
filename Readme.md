# Source Lines of Code

## Description

**slocc** _(Source Lines of Code in C++)_ is a command line program which can be used to count Source lines of code in a given directory made in C++.

Currently, it only supports **PYTHON** but
 in coming days, I will add support for other languages also.

I made it because I didn't like the  [cloc](https://github.com/AlDanial/cloc) project, and wanted to try something I made as a tool in my other projects.

## Build

```
$ g++ Src/main.cpp -std=c++20 -O2 -o slocc.exe
```

## Usage

```
$ ./slocc [options] [path]
```

## Examples

```
$ ./slocc Test/
Checking: Test/

Parsing 3 Files

__________________________________________________
Lines   cmnts   Blanks  SLOC    File
__________________________________________________
1000    94      243     665     Test/a_orig.py
1000    94      243     665     Test/a_orig2.py
1000    94      243     665     Test/a_orig3.py
__________________________________________________
3000    282     729     1995    TOTAL
__________________________________________________

Took    2.2738 ms @ (3958131 lines/s)
```

## Author

[ucx15](https://github.com/ucx15/)
