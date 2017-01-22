// find first X prime numbers using the Sieve of Eratosthenes

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

inline void keep_window_open() { char ch; cin >> ch; }

inline void simple_error(string s)	// write ``error: s and exit program
{
	cerr << "error: " << s << '\n';
	keep_window_open();		// for some Windows environments
	exit(1);
}

int main() {
	int num_primes{ 1 };
	int sieve_max{ 100 };
	int sieve_count{ 0 };
	int jump{ 0 };
	int index{ 0 };
	vector<int> primes;

	struct item {
		int the_number;
		bool is_prime;
	};

	item temp_item;

	vector<item> sieve;

	cout << "Now many primes to find? ";
	cin >> num_primes;
	if (num_primes < 1) simple_error("Not looking for any primes - exiting.\n");

	cout << "\nHow large a sieve? ";
	cin >> sieve_max;

	while (primes.size() < num_primes) {
		// initialize the sieve
		cout << "Sieve Count = " << sieve_count << "\n";
		cout << "Number of primes found = " << primes.size() << "\n";
		if (sieve_count == 0) {			//true for the first sieve  (Might mot need this)
			for (int i = 1; i <= sieve_max;++i) {
				temp_item.the_number = i;
				temp_item.is_prime = true;
				sieve.push_back(temp_item);
			}
		}
		else {								//for all subsequent sieves
			for (int i = (sieve_count * sieve_max) + 1; i <= sieve_max * (sieve_count + 1);++i) {
				temp_item.the_number = i;
				temp_item.is_prime = true;
				sieve.push_back(temp_item);
			}
		}

		// do the sieving
		if (sieve[0].the_number == 1) sieve[0].is_prime = false;   //take care of the number 1

																   // mark multiple up to the when the_number saquared > max
		for (int i = 1; sieve[i].the_number * sieve[i].the_number < sieve[sieve_max - 1].the_number;++i) {
			//find the next prime
			for (int i = 1; i < sieve_max; i++) {
				if (sieve[i].is_prime == true) {					//found a potentially prime number
																	// check to see if there are any primes to check it against
					if (primes.size() > 0) {							//there are some primes already found
																		// check if this number is a multiple of one of the primes already fournd
						for (int y = 0;y < primes.size();++y) {
							if (sieve[i].the_number % primes[y] == 0) {
								sieve[i].is_prime = false;
								jump = primes[y];
								break;
							}
						}
					}
					jump = sieve[i].the_number;
				}
				index = i + jump;
				while (index < sieve_max) {
					sieve[index].is_prime = false;
					index += jump;
				}
			}
		}
		//harvest the primes
		for (int i = 0;i < sieve_max;i++) {
			if (sieve[i].is_prime == true) {
				primes.push_back(sieve[i].the_number);
				cout << sieve[i].the_number << "\n";
			}
		}
		sieve_count += 1;
	}
	keep_window_open();
}
