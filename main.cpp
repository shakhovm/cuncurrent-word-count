#include <iostream>
#include <chrono>
#include "word_counter/WordCounter.h"
#include "conf_handler/conf_handler.h"
#include "archive_handler/include/archive_handler.h"
#include <fstream>

typedef std::pair<std::string, size_t> word_count;
typedef std::vector<word_count> pair_vector;
template <typename Func, typename ...Args, typename T=WordMap>
void time_estimator(Func f,  T& value, size_t& time_es, const Args&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    value = f(args...);
    auto end = std::chrono::high_resolution_clock::now();
    time_es = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void f(const std::string& conf_file="conf.dat") {
    bl::generator gen;
    std::locale::global(gen("en_US.UTF-8"));
    ConfHandler conf_handler(conf_file);
    auto conf_params = conf_handler.getConfParams();

    auto start_all = std::chrono::high_resolution_clock::now();
    std::string file_data = archive_handler(conf_params.infile);
    auto end_read = std::chrono::high_resolution_clock::now();
    std::cout << "Reading (microseconds) : " <<
        std::chrono::duration_cast<std::chrono::microseconds>(end_read - start_all).count() << std::endl;
    WordMap wm;
    size_t time_s;
    time_estimator(text_indexing_by_queue, wm, time_s, cref(file_data),
            conf_params.thread_number, conf_params.thread_number*2, cref(gen));
  //  time_estimator(text_indexing_sequential, wm, time_s, cref(file_data), cref(gen));
    std::cout << "Indexing (milliseconds)  :"  << time_s << std::endl;
    std::vector<word_count> vw(wm.begin(), wm.end());
    std::sort(vw.begin(), vw.end(),
            [](const word_count& a, const word_count& b){ return a.first < b.first; });
    std::ofstream out_by_a(conf_params.out_by_a);
    for (const auto& x: vw) {
        out_by_a << x.first << " : " << x.second << std::endl;
    }
    out_by_a.close();
    std::sort(vw.begin(), vw.end(),
              [](const word_count& a, const word_count& b){ return a.second > b.second; });
    std::ofstream out_by_n(conf_params.out_by_n);
    for (const auto& x: vw) {
        out_by_n << x.first << " : " << x.second << std::endl;
    }
    auto end_all = std::chrono::high_resolution_clock::now();
    std::cout << "Total (milliseconds) : " <<
              std::chrono::duration_cast<std::chrono::milliseconds>(end_all - start_all).count() << std::endl;
}

int main() {
    f("../files/conf.dat");


    
    
//    size_t thread_number = 4;
//    size_t part_number = thread_number*2;
//    SynchQueue<WordMap> q;
//
//
//    std::ifstream raw_file("data.txt");
//    std::string text = dynamic_cast<std::ostringstream&>(
//            std::ostringstream{} << raw_file.rdbuf()).str();
////    std::string text(10, "hello my darling my wqudiuiwq qwio");
//    std::cout << sizeof(char)*text.size() / 1000000 << std::endl;
//    SynchQueue<WordMap> q2;
//   // text = "hello hello hello hello hello Hello";
////    std::cout << time_estimator(text_indexing, cref(text), thread_number,
////            cref(gen),  ref(q2)) << std::endl;
//    std::cout << time_estimator(text_indexing_by_queue, cref(text), thread_number, part_number,
//                                cref(gen),  ref(q)) << std::endl;
//    //std::cout << time_estimator(text_indexing_sequential, cref(text), cref(gen)) << std::endl;


    return 0;
}
