#include <utility>
#include <dirent.h>
#include <stdexcept>
#include <unistd.h>
#include <memory.h>
#include <wait.h>
#include <iostream>
#include "file_walker.h"

file_walker::file_walker(params_holder _ph): ph(std::move(_ph)) {}

void file_walker::walk(string const &dir_name) {
    DIR* dir = opendir(dir_name.c_str());
    if (dir == nullptr) {
        cerr << "Cannot open directory: " + dir_name << endl;
        return;
    }

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string name = entry->d_name;
        string path = dir_name + "/" + name;
        //cout << name << endl;

        switch (entry->d_type) {
            case DT_DIR:
                if (name != "." && name != "..") {
                    walk(path);
                }
                break;
            case DT_REG:
                if (ph.is_file_acceptable(name, path)) {
                    file_paths.push_back(path);
                    if (ph.exec_path.has_value()) {
                        vector<string> arg;
                        arg.push_back(ph.exec_path.get());
                        arg.push_back(path);
                        //cout << arg[0];
                        exec(arg);
                    }
                }
                break;
        }
    }
    closedir(dir);
}

vector<char*> file_walker::to_c_vec(vector<string> &v) {
    vector<char*> c_vec(v.size() + 1);
    for (int i = 0; i < v.size(); i++) {
        c_vec[i] = const_cast<char*>(v[i].data());
    }
    c_vec.back() = nullptr;
    return c_vec;
}

void file_walker::exec(vector<string> &args) {
    auto c_args = to_c_vec(args);
    pid_t pid;
    switch (pid = fork()) {
        case -1: // error
            log_error("fork failed.");
            break;
        case 0: //child subprocess
            //cout << args[0] << endl;
            if (execv(c_args[0], c_args.data()) == -1) {
                log_error("program finished with -1 code.");
                exit(EXIT_FAILURE);
            }
            break;
        default: //parent process
            int child_status;
            if (waitpid(pid, &child_status, 0) == -1) {
                log_error("execution failed.");
            }
            else {
                cout << "execution finished with " << WEXITSTATUS(child_status) << " code." << endl;
            }
            break;
    }
}
