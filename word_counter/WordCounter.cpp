//
// Created by shakhov on 4/4/20.
//

#include "WordCounter.h"
#include "map_merging.h"

void text_indexing(const std::string& split_text, size_t thread_number,
        const bl::generator& gen, SynchQueue<WordMap>& map_q) {
    std::vector<std::thread> thread_vector;

//    std::string split_text = text;
//    boost::split(split_text, text, boost::is_any_of(" "));
////    ba::ssegment_index map(boost::locale::boundary::word, text.begin(),
////                           text.end(), gen("en_US.UTF-8"));
////    map.rule(boost::locale::boundary::word_any);
////    for (ss_it p = map.begin(); p != map.end(); ++p) {
////        split_text.push_back(*p);
//////        wm[bl::fold_case(bl::normalize(std::string(*p)))]++;
////    }
//
//    std::cout << split_text.size() << std::endl;
    size_t part_size = split_text.size() / thread_number +
                       static_cast<size_t>(split_text.size() % thread_number != 0);
    for (auto i = split_text.cbegin(); i < split_text.cend(); ) {
        auto border = i + part_size;
        border = border < split_text.end() ? border : split_text.cend();
        border = std::find(border, split_text.cend(), ' ');
        thread_vector.emplace_back(block_indexing, i, border,
                                   ref(gen), ref(map_q));
        i = border;
    }

    for (auto& x: thread_vector) {
        x.join();
    }
}


void block_indexing(str_iter start, str_iter end, const bl::generator& gen,
                    SynchQueue<WordMap>& map_q) {
    WordMap wm;
    ba::ssegment_index map(boost::locale::boundary::word, start,
                           end, gen("en_US.UTF-8"));
    map.rule(boost::locale::boundary::word_letter);
    for (ss_it p = map.begin(); p != map.end(); ++p) {
        wm[bl::fold_case(bl::normalize(std::string(*p)))]++;
    }
//    for (auto i = start; i != end; ++i) {
////        ba::ssegment_index map(boost::locale::boundary::word, i->begin(),
////                               i->end(), gen("en_US.UTF-8"));
////        map.rule(boost::locale::boundary::word_any);
////        for (ss_it p = map.begin(); p != map.end(); ++p) {
////            wm[bl::fold_case(bl::normalize(std::string(*p)))]++;
////        }
////        std::string norm = bl::fold_case(bl::normalize(*i));
////        std::string result;
////        std::remove_copy_if(norm.begin(), norm.end(), std::back_inserter(result), //Store output
////                            std::ptr_fun<int, int>(&std::ispunct)  );
//        wm[bl::fold_case(bl::normalize(*i))]++;
//    }
    map_q.push(std::move(wm));
}

void block_indexing_by_queue (SynchQueue<it_pair>& it_q, SynchQueue<WordMap>& q,
                              const bl::generator& gen) {
    WordMap wm;

    while (true) {
        it_pair borders = it_q.pop();
        if (borders.first == borders.second)
        {
            it_q.push(std::move(borders));
            if (!wm.empty())
            {
                q.increase_count();
               // std::cout << "dekoode" << std::endl;
                q.push(std::move(wm));
            }

            break;
        }
        ba::ssegment_index map(boost::locale::boundary::word, borders.first,
                               borders.second, gen("en_US.UTF-8"));
        map.rule(boost::locale::boundary::word_letter);
        for (ss_it p = map.begin(); p != map.end(); ++p) {
            wm[bl::fold_case(bl::normalize(std::string(*p)))]++;
        }
//        for (auto i = borders.first; i != borders.second; ++i) {
//            ba::ssegment_index map(boost::locale::boundary::word, i->begin(),
//                                   i->end(), gen("en_US.UTF-8"));
//            map.rule(boost::locale::boundary::word_any);
//            for (const auto& word: map) {
//                wm[bl::fold_case(bl::normalize(std::string(word)))]++;
//
//            }
        }
}


WordMap text_indexing_by_queue(const std::string& split_text, size_t thread_number,
        size_t part_number, const bl::generator& gen) {
    SynchQueue<WordMap> map_queue;
    SynchQueue<it_pair> it_q;
    std::vector<std::thread> thread_vector;
//    std::vector<std::string> split_text;
//    boost::split(split_text, text, boost::is_any_of(" "));
    size_t part_size = split_text.size() / part_number +
            static_cast<size_t>(split_text.size() % part_number != 0);

    map_queue.set_mal(part_number - 1);
    for (size_t i = 0; i < thread_number; ++i) {
        thread_vector.emplace_back(block_indexing_by_queue, ref(it_q),
                                   ref(map_queue), cref(gen));
    }

    std::vector<std::thread> map_thread_vector;
   // map_queue.set_mal(part_number - 1);
    size_t max_size = part_number - 1;
    SynchQueue<pair_map> pair_queue;
    std::thread map_handler(map_queue_handler, ref(map_queue), ref(pair_queue));
    for (size_t i = 0; i < thread_number; ++i) {
        map_thread_vector.emplace_back(infinite_merging, std::ref(map_queue),
                                       max_size, ref(pair_queue));
    }

    for (auto i = split_text.cbegin(); i < split_text.cend(); ) {
        auto border = i + part_size;
        border = border < split_text.cend() ? border : split_text.cend();
        border = std::find(border, split_text.cend(), ' ');
        it_q.push(it_pair(i, border));
       // std::cout << i - split_text.begin() << std::endl;
        if (border == split_text.cend()) {
           // std::cout << i - split_text.begin() << std::endl;
            it_q.push(it_pair(split_text.cend(), split_text.cend()));
            break;
        }
        i = border;
    }

    for(auto& x: thread_vector) {
        x.join();
    }

    map_handler.join();
  //  std::cout << "Success" << std::endl;
    for (auto& x: map_thread_vector) {
        x.join();
    }

    return map_queue.pop();
}

WordMap text_indexing_sequential(const std::string& text, const bl::generator& gen) {
    WordMap wm;
    ba::ssegment_index map(boost::locale::boundary::word, text.begin(),
                           text.end(), gen("en_US.UTF-8"));
    map.rule(boost::locale::boundary::word_letter);
    for (ss_it p = map.begin(); p != map.end(); ++p) {
        wm[bl::fold_case(bl::normalize(std::string(*p)))]++;
    }

    return wm;
}