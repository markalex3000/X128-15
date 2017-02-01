// find first X prime numbers using the Sieve of Eratosthenes

// Start standsard opening

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

// End standard opening

// Declarations

struct item {				// used to keep track of the numbers and whether they are prime and if they have been harvested
	int the_number;
	bool is_prime;
	bool harvested;
};

//functions
int sieve_init(vector<item>& s, int sieve_s, int sieve_c);  //intitiates the sieve - size and defualt values
int get_next_pot_prime(vector<item>& s, int index);			//finds the next number that is possibly prime from the sieve
int is_multiple_of_prime(int number, vector<int>& p);		//returns 0 if prime, otherwise returns first prime factor found
int mark_multiples(vector<item>& s, int start, int multiple);    //Changes is_prime to false for all multiples
int harvest_primes(vector<item>& s, vector<int>& p);			// captures all non-harvested primes from sieve
int print_sieve(vector<item>& s);								// prints the sieve

int main() {
	int num_primes{ 0 };					// number of primes sought
	int sieve_max{ 100 };					// the number for elements of the sieve
	int sieve_count{ 0 };					// the number of sieves used - first sieve is sieve 0
	bool sieve_stop_sieving{ false };		// flag for whether to continue to process the sieve
	int jump{ 0 };							// the disance to jump whem marking multiples
	int gp_return_value{ -1 };				// general-purpose return value
	int current_sieve_index{ 0 };			// the index of the number currently being processed

	vector<int> primes;						// the vector to store the primes found
	vector<item> sieve;						// the sieve vector


	cout << "Now many primes to find? ";
	cin >> num_primes;
	if (num_primes < 1) simple_error("Not looking for any primes - exiting.\n");

	cout << "\nHow large a sieve? ";
	cin >> sieve_max;

	while (primes.size() < num_primes) {

		// initialize the sieve
		cout << "Sieve Count = " << sieve_count << "\n";
		cout << "Number of primes found = " << primes.size() << "\n";
		gp_return_value = sieve_init(sieve, sieve_max, sieve_count);   //passing sieve by reference - will modify

		// do the sieving
		// Stop sieveing when found a number whose smallest prime factor squared is bigger then last number in Sieve

		sieve_stop_sieving = false;

		while(sieve_stop_sieving == false) {

			current_sieve_index = get_next_pot_prime(sieve, current_sieve_index);
			if(current_sieve_index == -1) break;		// happens when gnpp has reached the end of the sieve

			// check to see if mulitple of primes already found
			jump = is_multiple_of_prime(sieve[current_sieve_index].the_number, primes);

			//  Check to see if we've raeched the highest number in the sieve we need to check
			if (jump * jump > sieve.back().the_number) sieve_stop_sieving = true;

			if (jump != 0) {		//the number is not prime
				gp_return_value = mark_multiples(sieve, current_sieve_index, jump);   //mark multiples
			}
			else {					//the number is prime
				primes.push_back(sieve[current_sieve_index].the_number);  // save it to the primes list
				sieve[current_sieve_index].harvested = true;				// mark it as being harvested
				gp_return_value = mark_multiples(sieve, current_sieve_index, sieve[current_sieve_index].the_number);   //mark multiples
				current_sieve_index += 1;									// get ready to go on to the next number

			}
		}   // drop out of the while loop when we have finished processing a sieve

	//harvest the primes
		gp_return_value = harvest_primes(sieve, primes);
		
		//get ready for the next sieve
		sieve_count += 1;
		current_sieve_index = 0;
	}   // drop out of the while loop when we have found enough primes.

	cout << "Found the first " << num_primes << ": \n";
	for (int i = 0;i < primes.size();i++) {
		if (i % 10 == 0) cout << "\n";
		cout << primes[i] << "\t";
	}
	cout << "all done\n";
	keep_window_open();
}

int sieve_init(vector<item>& s, int sieve_s, int sieve_c) {

	item temp_item{ 0, true, false};	//default values for intiating sieve

	int intitial_number = (sieve_c * sieve_s) + 1;	// calculation of the the_number for the 1st elt of seive
													// sieve_s is size, sieve_c is the number of the sieve

	int sieve_upper_limit = sieve_s * (sieve_c + 1);// calculate the_number for the last elt of the sieve

	if (s.size() != 0) s.clear();	// s.size() only equals zero if it has not been initialized for the first time
									// clears the previous sieve's values

	for (int i = intitial_number; i <= sieve_upper_limit; ++i) {
		temp_item.the_number = i;
		temp_item.is_prime = true;
		temp_item.harvested = false;
		if (i == 1) temp_item.is_prime = false;     //taking care of the number 1 defined as not prime
		s.push_back(temp_item);
	}
	return(1);
}

int get_next_pot_prime(vector<item>& s, int index) {
	for (int i = index; i < s.size(); ++i) {
		if (s[i].is_prime == true) {
			return i;
		}
		else continue;
	}
	return -1;  // indicating we've reached the end of the current sieve
}

int is_multiple_of_prime(int number, vector<int>& p) {
	if (p.size() == 0) return 0;
	for (int i = 0;i < p.size();++i) {
		if (number % p[i] == 0) return p[i];
	}
	return 0;
}

int mark_multiples(vector<item>& s, int start, int multiple) {
	int upper_limit = s.size()-1;
	for (int i = start; i <= upper_limit; i += multiple) {
		if(s[i].harvested != true) s[i].is_prime = false;
	}
	return 0;
}

int harvest_primes(vector<item>& s, vector<int>& p) {
	int count{ 0 };

	for (int i = 0;i < s.size();i++) {
		if (s[i].is_prime == true && s[i].harvested == false) {
			p.push_back(s[i].the_number);
			cout << s[i].the_number << "\n";
			count += 1;
		}
	}
	return count;
}

int print_sieve(vector<item>& s) {
	for (int i = 0;i < s.size();i++) {
		cout << s[i].the_number << "\t" << s[i].is_prime << "\t" << s[i].harvested << "\n";
	}
	return 0;
}

