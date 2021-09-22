getopt <a href="https://travis-ci.org/r-lyeh/getopt"><img src="https://api.travis-ci.org/r-lyeh/getopt.svg?branch=master" align="right" /></a><a href="LICENSE"><img src="https://img.shields.io/badge/license-zlib/libpng-blue.svg" align="right" /></a>
====

Simple command-line options handler (C++11).

## Features 
- [x] Handy, one line per option.
- [x] Tiny, cross-platform, self-contained, header-only.
- [x] ZLIB/libPNG licensed.

## Showcase
```c++
#> cat sample.cc
#include <iostream>
#include <string>
#include "getopt.hpp"
int main() {
	// - No initialization required: (argc, argv) pair automatically retrieved.
	// - First argument is default option value, then all option indentifiers follow.
    bool help = getarg( false, "-h", "--help", "-?" );
    int version = getarg( 0, "-v", "--version", "--show-version" );
    int depth = getarg( 1, "-d", "--depth", "--max-depth");
    std::string file = getarg( "", "-f", "--file" );
    // [...]
    std::cout << help << ',' << file << ',' << version << std::endl;
}

#> g++ sample.cc && ./a.out --file=/a/b/c --depth=123
0, /a/b/c, 123
```



## Documentation

Two APIs provided:

1. Simple functional api `getarg(...)`. 
   - No initialization required: (argc, argv) pair automatically retrieved.
   - First argument is default option value, then all option indentifiers follow.

```c++
int main() {
    bool help = getarg( false, "-h", "--help", "-?" );
    int version = getarg( 0, "-v", "--version", "--show-version" );
    int depth = getarg( 1, "-d", "--depth", "--max-depth");
    std::string file = getarg( "", "-f", "--file" );
    [...]
}
```

2. Simple OOP map-based api `getopt class`. Initialization (argc, argv) pair required.

   This getopt class is a `std::map` replacement where key/value are `std::string` types.
   Given invokation `'./app.out --user=me --pass=123 -h'` this class delivers not only:
   `map[0] = "./app.out"`, `map[1] = "--user=me"`, `map[2]="--pass=123"`, `map[3]='-h'`
   but also, `map["--user"]="me"`, `map["--pass"]="123"` and also, `map["-h"]=true`

   Additional API:
   - .cmdline() for a print app invocation string
   - .str() for pretty map printing
   - .size() number of arguments (equivalent to argc), rather than `std::map.size()`

```c++
int main( int argc, const char **argv ) {
    getopt args( argc, argv );
    if( args.has("-h") 
        || args.has("--help") 
        || args.has("-?") 
        || args.size() == 1 ) {
        std::cout << args["0"] 
                  << " [-?|-h|--help] [-v|--version] [--depth=number]"
                  << std::endl;
        return 0;
    }
    if( args.has("-v") 
        || args.has("--version") ) {
        std::cout << args["0"]
                  << " sample v1.0.0. Compiled on "
                   << __DATE__ << std::endl;
    }
    if( args.has("--depth") ) {
        int depth = atoi( args["--depth"].c_str() );
        std::cout << "depth set to "
                  << depth 
                  << std::endl;
    }
    [...]
}
```


## Changelog
- v1.0.0 (2016/04/18): Initial version

