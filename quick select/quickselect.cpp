/*******************************************************************************
 * Name          : quickselect.cpp
 * Author        : Conor McGullam
 * Pledge        : I pledge my honor that I have abided by the Stevens Honor System.
 * Date          : 3/22/2020
 * Description   : Implements the quickselect algorithm found on page 160 in
 *                 Algorithms, 3e by Anany Levitin.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

size_t lomuto_partition(int array[], size_t left, size_t right) {
    int pivot = array[left];
	size_t s = left;
	for(size_t i = left+1; i <= right; ++i) {
		if(array[i] < pivot) {
			++s;
			int temp = array[s];
			array[s] = array[i];
			array[i] = temp;
		}
	}
	int temp = array[s];
	array[s] = array[left];
	array[left] = temp;
    return s;
}

int quick_select(int array[], size_t left, size_t right, size_t k) {
    size_t s = lomuto_partition(array, left, right);
	if(s == k - 1) {
		return array[s];
	}else if(s > k-1) {
		return quick_select(array, left, s-1, k);
	}else {
		return quick_select(array, s+1, right, k);
	}
}

int quick_select(int array[], const size_t length, size_t k) {
    return quick_select(array, 0, length - 1, k);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <k>" << endl;
        return 1;
    }

    int k;
    istringstream iss;
    iss.str(argv[1]);
    if ( !(iss >> k) || k <= 0 ) {
        cerr << "Error: Invalid value '" << argv[1] << "' for k." << endl;
        return 1;
    }

    cout << "Enter sequence of integers, each followed by a space: " << flush;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    iss.clear();
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }

    int num_values = values.size();
    if (num_values == 0) {
        cerr << "Error: Sequence of integers not received." << endl;
        return 1;
    }
    if(num_values < k) {
		cerr << "Error: Cannot find smallest element " << k << " with only " << num_values << " value";
		if(num_values == 1) {
			cerr << "." << endl;
		}else {
			cerr << "s." << endl;
		}
		return 1;
	}
	int *array = new int[num_values];
	for(size_t i = 0; i < (size_t)num_values; ++i) {
		array[i] = values[i];
	}
	int ans = quick_select(array, num_values, k);
	cout << "Smallest element " << k << ": " << ans << endl;
	delete [] array;
    return 0;
}