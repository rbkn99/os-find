#ifndef OS_FIND_FILE_WALKER_H
#define OS_FIND_FILE_WALKER_H

#include <vector>
#include "params_holder.h"

class file_walker {
public:
    explicit file_walker(params_holder _ph);

    void walk(string const& dir_name);

    vector<string> get_file_paths() {
        return file_paths;
    }

private:
    params_holder ph;
    vector<string> file_paths;

    static void log_error(string const& msg) {
        cerr << msg << endl;
        cerr << strerror(errno) << endl;
        cerr.flush();
    }

    static void exec(string const& exec_path, vector<char*> const& args);
};


#endif //OS_FIND_FILE_WALKER_H
