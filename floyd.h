#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <omp.h>

using namespace std;

const int MAX = 99999999;

void floyd(const int N, std::vector<std::vector<int>>& cost, const int numThreads);
