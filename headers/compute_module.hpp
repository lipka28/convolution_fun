#pragma once
#include <vector>
#include <thread>

#include "globals.hpp"

using namespace std;

class Compute_module
{
private:
    sp_Image original_image;
    sp_Image processed_image;

    static void cpu_edge_detection_workload(sp_Image const &orig,
                                            sp_Image &target,
                                            size_t start_index,
                                            size_t end_index,
                                            const size_t stride);


public:
    Compute_module(sp_Image const &source_image);
    ~Compute_module();
    sp_Image get_processed_image() const;
    error slow_cpu_edge_detection();
    error fast_cpu_edge_detection(int num_of_threads);
};
