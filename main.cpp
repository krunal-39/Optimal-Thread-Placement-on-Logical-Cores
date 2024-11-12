#include <iostream>
#include <stdlib.h>
#include "work.h"
#include <cstdlib> // for strtol
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <fstream>
#include <sstream>


std::unordered_map<uint32_t, uint32_t> thread_core_map;
// implement the functions here.
void start_monitoring_for_thread(uint32_t threadIdx, pid_t tid, pthread_t handle)
{
  std::string command = "perf stat -e cycles,instructions,cache-misses,branches,branch-misses -p " +
                        std::to_string(tid) +
                        " --output=perf_thread_" + std::to_string(threadIdx) + ".txt &";

  // Execute the command
  if (system(command.c_str()) == -1)
    std::cerr << "Failed to start monitoring for thread " << threadIdx << std::endl;
  else
    std::cout << "Starting monitoring for thread " << threadIdx << " with TID: " << tid << std::endl;
  // }
  
}

void stop_monitoring_for_thread(uint32_t threadIdx)
{
    std::string stop_command = "pkill -SIGINT perf";
    system(stop_command.c_str());
}
void load_affinity_mapping(const std::string& filename) {
    std::ifstream infile(filename);
    std::string line;

    // Read each line from the file
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        int32_t thread_index, core_id;
        char separator;

        // Parse line in the format "thread_index:core"
        if (iss >> thread_index >> separator >> core_id && separator == ':') {
            thread_core_map[thread_index] = core_id;
        }
    }
}

int32_t get_thread_affinity(uint32_t threadIdx)
{
   // Check if the thread_core_map is empty and load it if needed
    if (thread_core_map.empty()) {
        load_affinity_mapping("core_optimization.txt");
    }
    int32_t core_id = -1;
    // Find the thread_index in the map and return the corresponding core ID
    if (thread_core_map.find(threadIdx) != thread_core_map.end()) { 
        core_id = thread_core_map[threadIdx];
    } 
    std::cout << "Thread " << threadIdx << " should be assigned to core: " << core_id << std::endl;
    return core_id;
}

int main(int argc, char **argv)
{
  // DO NOT MODIFY THE FOLLOWING BLOCK.
  // =================================================
  if (argc != 2)
  {
    printf("Usage: %s <seed>\n", argv[0]);
    return 1;
  }
  int sr_no = atoi(argv[1]);
  int num_threads = work_init(sr_no);
  // =================================================
  // Modify below this

  std::cout << "The number of threads: " << num_threads << std::endl;

  // Modify above this
  // DO NOT MODIFY THE FOLLOWING BLOCK.
  // =================================================
  work_start_monitoring();
  // =================================================
  // Modify below this


    system("python3 analyze.py");
  // Modify above this
  // DO NOT MODIFY THE FOLLOWING BLOCK.
  work_run();
  // =================================================
  // Modify below this
return 0;
}
