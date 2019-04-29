#ifndef OS_FIND_PARAMS_HOLDER_H
#define OS_FIND_PARAMS_HOLDER_H

#include <string>

using namespace std;

class params_holder {
public:
    template <typename T>
    struct optional {
    public:
        void set(T val) {
            value = val;
            is_set = true;
        }

        // let's assume that we are smart enough to call get only after set
        T get() {
            return value;
        }

        bool has_value() {
            return is_set;
        }
    private:
        T value;
        bool is_set = false;
    };

    enum size_comparators {
        LESS, EQUAL, GREATER
    };

    string dir_name;
    optional<ino_t> inode;
    optional<string> name;

    size_comparators size_filter;
    optional<off_t> size;

    optional<nlink_t> nlinks;
    optional<string> exec_path;

    params_holder(int argc, char* argv[]);
    bool is_file_acceptable(string const& filename, string const& filepath);

private:
    static bool check_params(int argc);
    void set_inode(const string& str_inode);
    void set_size(const string& str_size);
    void set_nlinks(const string &str_nlinks);
};


#endif //OS_FIND_PARAMS_HOLDER_H
