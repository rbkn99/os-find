#include <stdexcept>
#include <sys/stat.h>
#include "params_holder.h"


params_holder::params_holder(int argc, char **argv) {
    if (!check_params(argc)) {
        throw std::invalid_argument("Invalid number of arguments. See usage.");
    }
    dir_name = argv[1];
    for (int i = 2; i < argc; i += 2) {
        string param = argv[i], value = argv[i + 1];
        if (param == "-inum") {
            set_inode(value);
        } else if (param == "-name") {
            name.set(value);
        } else if (param == "-size") {
            set_size(value);
        } else if (param == "-nlinks") {
            set_nlinks(value);
        } else if (param == "-exec") {
            exec_path.set(value);
        } else {
            throw std::invalid_argument("Invalid value of modifier. See usage");
        }
    }
}

bool params_holder::check_params(int argc) {
    return argc >= 2 || argc % 2 == 0;
}

void params_holder::set_inode(const string &str_inode) {
    try {
        inode.set(stoull(str_inode));
    }
    catch (...) {
        throw std::invalid_argument("Incorrect value of inode.");
    }
}

void params_holder::set_size(const string &str_size) {
    bool has_sign = true;
    if (str_size[0] == '-') {
        size_filter = LESS;
    } else if (str_size[0] == '+') {
        size_filter = GREATER;
    } else {
        size_filter = EQUAL;
        if (str_size[0] != '=') {
            has_sign = false;
        }
    }

    try {
        if (has_sign) {
            size.set(stoll(str_size.substr(1)));
        } else {
            size.set(stoll(str_size));
        }
    } catch (...) {
        throw std::invalid_argument("Incorrect value of size.");
    }
}

void params_holder::set_nlinks(const string &str_nlinks) {
    try {
        nlinks.set(stoull(str_nlinks));
    }
    catch (...) {
        throw std::invalid_argument("Incorrect value of nlinks.");
    }
}

bool params_holder::is_file_acceptable(string const &filename, string const &filepath) {
    struct stat stat_buf{};
    stat(filepath.c_str(), &stat_buf);

    return
            (!inode.has_value() || inode.get() == stat_buf.st_ino) &&
            (!name.has_value() || name.get() == filename) &&
            (!size.has_value() ||
             (size_filter == LESS && size.get() < stat_buf.st_size) ||
             (size_filter == EQUAL && size.get() == stat_buf.st_size) ||
             (size_filter == GREATER && size.get() > stat_buf.st_size)) &&
            (!nlinks.has_value() || nlinks.get() == stat_buf.st_nlink);
}
