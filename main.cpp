#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cerrno>
#include <unistd.h>
#include <memory.h>
#include <wait.h>
#include "params_holder.h"
#include "file_walker.h"

using namespace std;


void print_help() {
    cout << "Usage:\n";
    cout << "\tos-find <absolute_path> <-inum num> <-name name> <-size [-=+]size> <-nlinks num> <-exec absolute_path>\n\n";
    cout.flush();
}

int main(int argc, char *argv[]) {
    print_help();
    vector<string> paths;
    try {
        params_holder holder(argc, argv);
        file_walker walker(holder);
        walker.walk(holder.dir_name);
        paths = walker.get_file_paths();
    }
    catch (std::exception& e) {
        cerr << "Exception occurred during the file walking:" << endl;
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    for (auto& path: paths) {
        cout << path << endl;
    }
}