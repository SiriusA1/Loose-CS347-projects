/*******************************************************************************
 * Name          : pythagorean.cpp
 * Author        : Conor McGullam
 * Version       : 1.0
 * Date          : September 20, 2019
 * Last modified : September 20, 2019
 * Description   : Pythagorean Triple Generator
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>

using namespace std;

/**
 * Struct to represent a Pythagorean triple.
 * It is merely a way to group together variables a, b, and c.
 */
struct Triple {
    int a, b, c;

    Triple(int a_, int b_, int c_) : a{a_}, b{b_}, c{c_} { }

    friend ostream& operator<<(ostream &os, const Triple &t) {
        os << t.a << "^2 + " << t.b << "^2 = " << t.c << "^2";
        return os;
    }
};

/**
 * Returns a vector of Pythagorean triples where all values (a, b, c) are
 * less than or equal to the limit.
 */
vector<Triple> find_pyth_triples(const int limit) {
	int a = 3, b = 4, c;
	vector<Triple> pyth_list;
    while(a < limit) {
		while(b < limit) {
			c = floor(sqrt(pow(a, 2) + pow(b, 2)));
			if(pow(c,2) == pow(a, 2) + pow(b, 2) && c <= limit) {
					pyth_list.push_back(Triple(a, b, c));
			}
			++b;
		}
		++a;
		b = a + 1;
	}
	return pyth_list;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <limit>" << endl;
        return 1;
    }
    int limit;
    istringstream iss;

    iss.str(argv[1]);
    if ( !(iss >> limit) || limit < 3 ) {
        cerr << "Error: limit must be an integer >= 3." << endl;
        return 1;
    }
    vector<Triple> triples = find_pyth_triples(limit);
    for (const auto &triple : triples) {
        cout << triple << endl;
    }
    return 0;
}
