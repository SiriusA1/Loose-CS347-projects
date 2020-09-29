/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Conor McGullam
 * Date        : 2/3/2020
 * Description : Sieve of Eratosthenes
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve {
public:
    PrimesSieve(int limit);

    ~PrimesSieve() {
        delete [] is_prime_;
    }

    int num_primes() const {
        return num_primes_;
    }

    void display_primes() const;

private:
    // Instance variables
    bool * const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;

    // Method declarations
    int count_num_primes() const;
    void sieve();
    static int num_digits(int num);
};

PrimesSieve::PrimesSieve(int limit) :
        is_prime_{new bool[limit + 1]}, limit_{limit} {
    sieve();
}

void PrimesSieve::display_primes() const {
	/* num_length used for setw and also to find how many primes can fit on one line
	 * row_space used to find how many primes can fit on one line
	 * primes_per_row is a counter for the while loop that finds how many primes can fit in a row */
	int num_length = num_digits(max_prime_);
	int row_space = 80;
	int primes_per_row = 0;
	/* Uses subtraction and the 3 variables above to find out how many primes can fit on one row.
	 * Also subtracts the spaces after each prime but does not subtract a space after the last number. */
	while(row_space >= num_length) {
		row_space -= num_length;
		primes_per_row++;
		if(row_space > num_length) {
			row_space -= 1;
		}
	}
	//spots_taken is a counter used for comparison with primes_per_row
	int spots_taken = 0;
	//looping through array of primes
    for(int i = 2; i < limit_+1; ++i) {
		if(is_prime_[i]) {
			/* First if statement is finding out if everything can fit on one line.
			 * If it can, then setw formatting isn't needed. */
			if(limit_ < 107) {
				if(i != max_prime_) {
					cout << i << " ";
				}else {
					cout << i << endl;
				}
			}else {
				/* Creates graph format. First two if statements have special formatting for the
				 * last prime in the graph. Last three if statements compare how many numbers
				 * are in the current line to the max that can fit in the line and formats accordingly. */
				if(i == max_prime_ && spots_taken == 0) {
					cout << setw(num_length) << i << endl;
				}else if(i == max_prime_) {
					cout << " " << setw(num_length) << i << endl;
				}else if(spots_taken == 0) {
					cout << setw(num_length) << i;
					spots_taken++;
				}else if(spots_taken < primes_per_row) {
					cout << " " << setw(num_length) << i;
					spots_taken++;
				}else if(spots_taken == primes_per_row) {
					cout << endl;
					cout << setw(num_length) << i;
					spots_taken = 1;
				}
			}
		}
	}
}

int PrimesSieve::count_num_primes() const {
	//Counts number of primes in sieved array by counting the number of 'true's.
	int count = 0;
	for(int i = 2; i < limit_+1; ++i) {
		if(is_prime_[i]) {
			++count;
		}
	}
	return count;
}

void PrimesSieve::sieve() {
	//setting all indexes >=2 to true
    for(int i = 2; i < limit_+1; ++i) {
		is_prime_[i] = true;
	}
	//Outer loop iterates starting at 2, looking for values that are true
	for(int i = 2; i <= floor(sqrt(limit_)); ++i) {
		if(is_prime_[i]) {
			/*Inner loop takes values that are true and sets all of their multiples
			(starting at i squared) to false as they cannot be prime.*/
			for(int j = i; j*i < limit_+1; ++j) {
				is_prime_[i*j] = false;
			}
		}
	}
	//finding max prime
	for(int i = limit_; i > 1; --i) {
		if(is_prime_[i]) {
			max_prime_ = i;
			break;
		}
	}
	//finding number of primes
	num_primes_ = count_num_primes();
}

int PrimesSieve::num_digits(int num) {
	//finds number of digits by dividing by 10
    int count = 0;
	while(num >= 10) {
		num = num / 10;
		count++;
	}
	return count + 1;
}

int main() {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }
	PrimesSieve sieve_test = PrimesSieve(limit);
	//sets new line after the user prompt
	cout << endl;
	//displays number of primes
	cout << "Number of primes found: " << sieve_test.num_primes() << endl;
	//displays all primes in graph format
	cout << "Primes up to " << limit << ":" << endl;
    sieve_test.display_primes();
    return 0;
}
