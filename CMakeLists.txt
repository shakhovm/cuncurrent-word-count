cmake_minimum_required(VERSION 3.15)
project(Concurrency)
set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_MULTITHREAD ON)
find_package(Boost REQUIRED COMPONENTS locale filesystem system)
find_package(LibArchive REQUIRED)

find_package( ICU REQUIRED COMPONENTS uc i18n)
set(SRC synch_queue/src/synch_queue.cpp
        conf_handler/conf_handler.cpp
        conf_handler/conf_handler.cpp word_counter/WordCounter.cpp word_counter/map_merging.cpp)
set(INC synch_queue/include/synch_queue.h
        archive_handler/include/archive_handler.h
        conf_handler/conf_handler.h conf_handler/conf_handler.h word_counter/WordCounter.h
        word_counter/map_merging.h)

set(REQUIRED_LIBRARIES Threads::Threads Boost::locale
        Boost::filesystem Boost::system ICU::uc ICU::i18n
        ${LibArchive_LIBRARIES})
add_executable(Concurrency main.cpp ${SRC} ${INC})
add_executable(k k.cpp synch_queue/src/synch_queue.cpp synch_queue/include/synch_queue.h)
target_link_libraries(k Threads::Threads)
target_link_libraries(Concurrency ${REQUIRED_LIBRARIES})

#add_executable(example k.cpp)
#target_link_libraries(example Threads::Threads)
#add_executable(project_x aaa.cpp)
if(MINGW)
    target_link_libraries(Concurrency ws2_32)
endif()
