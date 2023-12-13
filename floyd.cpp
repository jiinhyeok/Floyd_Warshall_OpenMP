#include "floyd.h"

void floyd(const int N, vector<vector<int>>& cost, const int numThreads)
{
	int **cost_row = new int*[N];
	int **cost_col = new int*[N];
	for(int i = 1; i < N; i++) {
		cost_row[i] = new int[i + 32];
		cost_col[i] = new int[i + 32];
	}

	for(int i = 1; i < N; i++)
		for(int j = 0; j < i; j++)
			cost_row[i][j] = cost[i][j];
	for(int i = 0; i < N - 1; i++)
		for(int j = i + 1; j < N; j++)
			cost_col[j][i] = cost[i][j];

	
	int *idx = new int[N];
	for(int i = 0, j = N - 1; i < j; ) {
		idx[i << 1] = i;
		idx[(i << 1) + 1] = j;
		i++; j--;
	}
	idx[N - 1] = N / 2;
	
	omp_set_num_threads(numThreads);

	#pragma omp parallel
	{
		int a, *k_;
		#pragma omp critical
		{
			k_ = new int[N];
		}
		for (int k = 0; k < N; k++) {

			for(int i = 0; i < k; i++) k_[i] = cost_row[k][i];
			k_[k] = 0;
			for(int i = k + 1; i < N; i++) k_[i] = cost_col[i][k];

			#pragma omp for schedule(dynamic, 2)
			for (int i = 1; i < N; i++) {
				a = idx[i];
				if(k_[a] == MAX) continue;
				for (int b = 0; b < a; b++) {
					if (k_[a] + k_[b] < cost_row[a][b]) {
						cost_row[a][b] = cost_col[a][b] = k_[a] + k_[b];
					}
				}
			}
		}
		delete[] k_;
	}

	for(int i = 1; i < N; i++)
		for(int j = 0; j < i; j++)
			cost[i][j] = cost_row[i][j];
	for(int i = 0; i < N - 1; i++)
		for(int j = i + 1; j < N; j++)
			cost[i][j] = cost_col[j][i];

	for(int i = 1; i < N; i++) {
		delete[] cost_row[i];
		delete[] cost_col[i];
	}
	delete[] cost_row;
	delete[] cost_col;
	delete[] idx;
}
