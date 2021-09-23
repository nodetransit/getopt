#include "getopt.hpp"


namespace getopt_utils {
// token split

size_t
split(std::vector<std::string> &tokens,
      const std::string &self,
      const std::string &delimiters)
{
    std::string str;
    tokens.clear();

    for (auto &ch : self) {
        if (delimiters.find_first_of(ch) != std::string::npos) {
            if (str.size()) {
                tokens.push_back(str), str = "";
            }
            tokens.push_back(std::string() + ch);
        } else {
            str += ch;
        }
    }

    return str.empty() ?
           tokens.size() :
           (tokens.push_back(str), tokens.size());
}

// portable cmdline

std::vector<std::string>
cmdline()
{
    std::vector<std::string> args;
    std::string              arg;
#ifdef _WIN32
    int argv;
    auto *list = ::CommandLineToArgvW(::GetCommandLineW(), &argv);

    if(list) {
        for(int i = 0; i < argv; ++i) {
            size_t length = wcstombs(NULL, list[i], 0);
            //char* buff = (char*)calloc(length + 1, sizeof(char));
            char* buff = new char[length + 1];
            size_t copied = wcstombs(buff, list[i], length);
            buff[copied] = '\0';
            args.push_back(std::string(buff));

            //std::string ws(buff);
            //args.push_back(std::string(ws.begin(), ws.end()));

            //free(buff);
            delete [] buff;
        }
        ::LocalFree(list);
    }
#else
    pid_t pid = getpid();

    char fname[32] = {0};
    sprintf(fname, "/proc/%d/cmdline", pid);
    std::ifstream ifs(fname);
    if (ifs.good()) {
        std::stringstream ss;
        ifs >> ss.rdbuf();
        arg = ss.str();
    }

    for (auto end = arg.size(), i = end - end; i < end; ++i) {
        auto st = i;
        while (i < arg.size() && arg[i] != '\0') {
            ++i;
        }
        args.push_back(arg.substr(st, i - st));
    }
#endif

    return args;
}

}
