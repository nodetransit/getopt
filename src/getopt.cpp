#include "getopt.hpp"

using super = std::map<std::string, std::string>;

getopt::getopt(int argc, const char **argv) :
      super()
{
    // reconstruct vector
    std::vector<std::string> args(argc, std::string());
    for (int                 i = 0; i < argc; ++i) {
        args[i] = argv[i];
    }
    // create key=value and key= args as well
    for (auto                &it : args) {
        std::vector<std::string> tokens;
        auto                     size = getopt_utils::split(tokens, it, "=");

        if (size == 3 && tokens[1] == "=")
            (*this)[tokens[0]] = tokens[2];
        else if (size == 2 && tokens[1] == "=")
            (*this)[tokens[0]] = true;
        else if (size == 1 && tokens[0] != argv[0])
            (*this)[tokens[0]] = true;
    }
    // recreate args
    while (argc--) {
        (*this)[std::to_string(argc)] = std::string(argv[argc]);
    }
}

getopt::getopt(const std::vector<std::string> &args) :
      super()
{
    std::vector<const char *> argv;
    for (auto                 &it : args) {
        argv.push_back(it.c_str());
    }
    *this = getopt(argv.size(), argv.data());
}

size_t
getopt::
size() const
{
    unsigned i = 0;
    while (has(std::to_string(i))) ++i;
    return i;
}

bool
getopt::has(const std::string &op) const
{
    return this->find(op) != this->end();
}

std::string
getopt::str() const
{
    std::stringstream ss;
    std::string       sep;
    for (auto         &it : *this) {
        ss << sep << it.first << "=" << it.second;
        sep = ',';
    }
    return ss.str();
}

std::string
getopt::cmdline() const
{
    std::stringstream cmd;
    std::string       sep;
    // concatenate args
    for (auto         end = size(), arg = end - end; arg < end; ++arg) {
        cmd << sep << this->find(std::to_string(arg))->second;
        sep = ' ';
    }
    return cmd.str();
}
