//
// Created by shakhov on 4/5/20.
//

#ifndef CONCURRENCY_MAP_MERGING_H
#define CONCURRENCY_MAP_MERGING_H
#include <iostream>
#include <string>
#include <map>
#include <atomic>
#include "../synch_queue/include/synch_queue.h"
#include <vector>
#include <thread>
typedef std::map<std::string, size_t> WordMap;
typedef std::pair<WordMap, WordMap> pair_map;

void map_merging(WordMap& m1, WordMap& m2);

void infinite_merging(SynchQueue<WordMap>& map_queue, size_t maximum,
                      SynchQueue<pair_map>& pair_queue);

void map_queue_handler(SynchQueue<WordMap>& map_queue, SynchQueue<pair_map>& pair_queue);

//WordMap parallel_merging(SynchQueue<WordMap>& map_queue, size_t thread_number);

#endif //CONCURRENCY_MAP_MERGING_H
