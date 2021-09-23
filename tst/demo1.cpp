#include <iostream>
#include <cstdlib>

#include <getopt.hpp>

int
main(int argc,
     const char **argv)
{
    auto show_help = [&]() {
        std::cout << argv[0]
                  << " [-h|--help|-?] [-f=path|--file=path] [-v|--version] [-d=number|--depth=number|--max-depth=number]"
                  << std::endl;
    };

    // Simple functional api. No initialization required.

    bool        help    = getarg(false, "-h", "--help", "-?");
    int         version = getarg(0, "-v", "--version", "--show-version");
    int         depth   = getarg(0, "-d", "--depth", "--max-depth");
    std::string file    = getarg("", "-f", "--file");

    if (help || argc <= 1) {
        show_help();
        return EXIT_SUCCESS;
    }

    if (version) {
        std::cout << argv[0]
                  << " demo v1.0.0. Compiled on "
                  << __DATE__
                  << std::endl;
        return EXIT_SUCCESS;
    }

    if (depth) {
        std::cout << "provided depth: "
                  << depth
                  << std::endl;
    }

    if (!file.empty()) {
        std::cout << "provided file: ";
#ifdef _WIN32
        {
            size_t length = mbstowcs(nullptr, file.c_str(), 0);
            //wchar_t* wfile = (wchar_t*)calloc(length + 1, sizeof(wchar_t));
            wchar_t* wfile = new wchar_t[length + 1];
            size_t copied = mbstowcs(wfile, file.c_str(), length);
            wfile[copied] = L'\0';
            std::wcout << wfile;
            //free(wfile);
            delete [] wfile;
        }
#else
        std::cout << file
#endif
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
