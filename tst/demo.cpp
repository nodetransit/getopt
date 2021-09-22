#include <iostream>
#include <cstdlib>

#include <getopt.hpp>

int main( int argc, const char **argv ) {

    auto show_help = [&]() {
        std::cout << argv[0] << " [-h|--help|-?] [-f=path|--file=path] [-v|--version] [-d=number|--depth=number|--max-depth=number]" << std::endl;
        exit(0);
    };

    // Simple functional api. No initialization required.

    bool help        = getarg( false, "-h", "--help", "-?" );
    int version      = getarg( 0, "-v", "--version", "--show-version" );
    int depth        = getarg( 0, "-d", "--depth", "--max-depth");
    std::string file = getarg( "", "-f", "--file" );

    if( help || argc <= 1 ) {
        show_help();
    }

    if( version ) {
        std::cout << argv[0] << " demo v1.0.0. Compiled on " << __DATE__ << std::endl;
    }

    if( depth ) {
        std::cout << "provided depth: " << depth << std::endl;
    }

    if( !file.empty() ) {
        std::cout << "provided file: " << file << std::endl;
    }

    // OOP map-based api. Explicit (argc, argv) initialization required.

    struct getopt args( argc, argv );

    if( args.has("-h") || args.has("--help") || args.has("-?") || args.size() == 1 ) {
        show_help();
    }

    if( args.has("-v") || args.has("--version") ) {
        std::cout << args["0"] << " demo v1.0.0. Compiled on " << __DATE__ << std::endl;
    }

    if( args.has("-d") || args.has("--depth") || args.has("--max-depth") ) {
        std::string arg = args["-d"];
        if( arg.empty() ) arg = args["--depth"];
        if( arg.empty() ) arg = args["--max-depth"];
        int depth = atoi( arg.c_str() );
        std::cout << "provided depth: " << depth << std::endl;
    }

    if( args.has("-f") || args.has("--file") ) {
        std::string arg = args["-f"];
        if( arg.empty() ) arg = args["--file"];
        std::string fname = arg;
        std::cout << "provided file: " << fname << std::endl;
    }

    std::cout << "---" << std::endl;
    std::cout << args.cmdline() << std::endl;
    //std::cout << args.size() << " provided args: " << args.str() << std::endl;
}
