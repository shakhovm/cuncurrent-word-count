//
// Created by shakhov on 4/4/20.
//

#ifndef CONCURRENCY_CONF_HANDLER_H
#define CONCURRENCY_CONF_HANDLER_H
#include <fstream>


class ConfHandler {
    std::string content;
    struct ConfParams {
        std::string infile;
        std::string out_by_a;
        std::string out_by_n;
        size_t thread_number;
    } conf_params;
public:
    explicit ConfHandler(const std::string& filename);
    ~ConfHandler() = default;
    std::string file_pattern(const std::string& pattern);
    void conf_file_handler();
    inline const ConfParams& getConfParams() { return conf_params; }
};


#endif //CONCURRENCY_CONF_HANDLER_H
