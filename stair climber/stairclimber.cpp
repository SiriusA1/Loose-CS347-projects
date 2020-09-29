/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Conor McGullam
 * Date        : 2/28/2020
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

void display_ways(const vector< vector<int> > &ways) {
	//max is the largest index (by number of digits) used for setw
	int max = (int)ways.size() - 1;
	//sum finds finds num_stairs since display_ways has no way of knowing
	int sum = (int)ways[0].size();
	if(sum > 1) {
		cout << max+1 << " ways to climb " << sum << " stairs." << endl;
	}else {
		cout << max+1 << " way to climb " << sum << " stair." << endl;
	}
	//count finds number of digits in max for setw
	int count = 0;
	while(max >= 10) {
		max = max / 10;
		count++;
	}
	int max_digits = count + 1;
	/* Iterates through the outer vector, then prints the inner vector backwards since all values were pushed
	*  to the back instead of inserted at the front. */
	for(int i = 0; i < (int)ways.size(); ++i) {
		cout << setw(max_digits) << i+1 << ". [";
		for(int j = (int)ways[i].size() - 1; j > 0; --j) {
			cout << ways[i][j] << ", ";
		}
		cout << ways[i][0] << "]" << endl;
	}
}

vector< vector<int> > get_ways_old(int num_stairs) {
	//old get_ways method
    vector< vector<int> > ways = {};
	if(num_stairs <= 0) {
		ways.push_back({});
	}else {
		for(int i = 1; i < 4; ++i) {
			if(num_stairs >= i) {
				vector< vector<int> > result = get_ways_old(num_stairs - i);
				for(auto j = result.begin(); j != result.end(); ++j) {
					(*j).insert((*j).begin(), i);
				}
				ways.reserve(ways.size() + result.size());
				ways.insert(ways.end(), result.begin(), result.end());
			}
		}
	}
	return ways;
}

vector< vector<int> > get_ways_helper(int num_stairs, vector< vector< vector<int> > > &memo) {
	vector< vector<int> > ways = {};
	//checks to see if current call has already been memoized to save work
	if(memo[num_stairs][0][0] != -1) {
		return memo[num_stairs];
	}else {
		/* The normal loop from the given python code with one change. This code
		*  adds to the back instead of the front. */
		if(num_stairs <= 0) {
			ways.push_back({});
		}else {
			for(int i = 1; i < 4; ++i) {
				if(num_stairs >= i) {
					vector< vector<int> > result = get_ways_helper(num_stairs - i, memo);
					for(auto j = result.begin(); j != result.end(); ++j) {
						(*j).push_back(i);
					}
					ways.insert(ways.end(), result.begin(), result.end());
				}
			}
		}
		//updates memo
		memo[num_stairs] = ways;
		return ways;
	}
}

vector< vector<int> > get_ways(int num_stairs) {
	/* Calls helper function to do most of the work, memo is initialized here and passed by reference 
	*  so that it can be consistent throughout the many recursive calls of the helper. */
	vector< vector< vector<int> > > memo(num_stairs+1, { {-1} });
	return get_ways_helper(num_stairs, memo);
}

int main(int argc, char * const argv[]) {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	if(argc != 2) {
		cout << "Usage: ./stairclimber <number of stairs>" << endl;
	}else {
		istringstream iss(argv[1]);
		int stairs;
		if(!( iss >> stairs) || stairs <= 0) {
			cout << "Error: Number of stairs must be a positive integer." << endl;
		}else {
			vector< vector<int> > test = get_ways(stairs);
			display_ways(test);
		}
	}
}
