//
// Created by shakhov on 4/4/20.
//

#ifndef CONCURRENCY_WORDCOUNTER_H
#define CONCURRENCY_WORDCOUNTER_H

#include <string>
#include "../synch_queue/include/synch_queue.h"
#include <map>
#include <boost/locale/boundary/index.hpp>
#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>
#include <thread>

typedef std::map<std::string, size_t> WordMap;
typedef std::pair<std::string, size_t> Pair;

namespace bl = boost::locale;
namespace ba = boost::locale::boundary;
typedef ba::ssegment_index::iterator ss_it;
typedef std::string::const_iterator str_iter;
typedef std::pair<str_iter, str_iter> it_pair;
using std::ref, std::cref;




void block_indexing(str_iter start, str_iter end, const bl::generator& gen,
                    SynchQueue<WordMap>& map_q);

void text_indexing(const std::string& text, size_t thread_number, const bl::generator& gen,
                   SynchQueue<WordMap>& map_q);

void block_indexing_by_queue (SynchQueue<it_pair>& it_q, SynchQueue<WordMap>& q,
                              const bl::generator& gen);

WordMap text_indexing_by_queue(const std::string& text, size_t thread_number, size_t part_number,
                            const bl::generator& gen);

WordMap text_indexing_sequential(const std::string& text, const bl::generator& gen);

#endif //CONCURRENCY_WORDCOUNTER_H
