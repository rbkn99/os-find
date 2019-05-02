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

    dirent* entry = readdir(dir);
    while (entry) {
        string name = entry->d_name;
        string path = dir_name + "/" + name;

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
                        vector<char*> arg;
                        arg.push_back(const_cast<char*>(path.c_str()));
                        exec(ph.exec_path.get(), arg);
                    }
                }
                break;
        }
        entry = readdir(dir);
    }
    closedir(dir);
}

void file_walker::exec(string const &exec_path, vector<char *> const &args) {
    pid_t pid;
    switch (pid = fork()) {
        case -1: // error
            log_error("fork failed.");
            break;
        case 0: //child subprocess
            //cout << args[0] << endl;
            if (execv(exec_path.c_str(), args.data()) == -1) {
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
