//
// Created by shakhov on 4/5/20.
//

#include "map_merging.h"

typedef std::map<std::string, size_t> WordMap;
typedef std::pair<WordMap, WordMap> pair_map;

void map_merging(WordMap& m1, WordMap& m2) {
    for (const auto& x: m2) {
        m1[x.first] += x.second;
    }
}

void infinite_merging(SynchQueue<WordMap>& map_queue, size_t maximum,
                      SynchQueue<pair_map>& pair_queue) {
    while (true) {

//        if (map_queue.check_end())
//        {
//
//            map_queue.push(WordMap());
////            map_queue.push(pm.first);
//            break;
//        }
        pair_map pm = pair_queue.pop();
        if (pm.first.empty()) {
            pair_queue.push(std::pair(WordMap(), WordMap()));
            break;
        }
        map_merging(pm.first, pm.second);

        map_queue.increase_count();
        if (map_queue.check_end())
        {

            map_queue.push(WordMap());
            map_queue.push(pm.first);
            break;
        }
        map_queue.push(pm.first);
    }
}

void map_queue_handler(SynchQueue<WordMap>& map_queue, SynchQueue<pair_map>& pair_queue) {
    while (true) {
        
        WordMap wm1 = map_queue.pop();
      //  std::cout << "wm1" << std::endl;
        if (wm1.empty()) {
           // std::cout << "wm2" << std::endl;
            pair_queue.push(std::pair(WordMap(), WordMap()));
            break;
        }
        WordMap wm2 = map_queue.pop();
//       // std::cout << "wm2" << std::endl;
//        if (wm2.empty()) {
//            pair_queue.push(std::pair(WordMap(), WordMap()));
//            map_queue.push(wm1);
//            break;
//        }
        pair_queue.push(std::pair(wm1, wm2));
    }
}

//WordMap parallel_merging(SynchQueue<WordMap>& map_queue, size_t thread_number) {
//
//    return map_queue.pop();
//}