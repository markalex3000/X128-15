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

struct item {
	int the_number;
	bool is_prime;
	bool harvested;
};

//functions
int sieve_init(vector<item>& s, int sieve_s, int sieve_c);
int get_next_pot_prime(vector<item>& s, int index);
int is_multiple_of_prime(int number, vector<int>& p);  //returns 0 if prime, otherwise returns first multiple found
int mark_multiples(vector<item>& s, int start, int multiple);
int harvest_primes(vector<item>& s, vector<int>& p);
int calc_stop_sieving(vector<item>& s, vector<int> p);
int print_sieve(vector<item>& s);

int main() {
	int num_primes{ 0 };
	int sieve_max{ 100 };
	int sieve_count{ 0 };
	bool sieve_stop_sieving{ false };
	int jump{ 0 };
	int index{ 0 };
	int gp_return_value{ -1 };
	int current_sieve_index{ 0 };

	vector<int> primes;

	item temp_item{ 0,true, false };

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

//		cout << "Ready to initialize sieve. enter a char to proceed...";
//		keep_window_open();
		gp_return_value = sieve_init(sieve, sieve_max, sieve_count);   //passing sieve by reference - will modify
//		cout << "Sieve initialized enter a char to proceed...";
//		gp_return_value = print_sieve(sieve);
//		keep_window_open();

		// do the sieving
		// Stop sieveing when the_number squared in greater than the_number of last element of sieve

		sieve_stop_sieving = false;

		while(sieve_stop_sieving == false) {
//		while(primes.back() * primes.back() < sieve.back().the_number){
			// get the index of the next potentially prime number

//			cout << "-----Current sieve index is - " << current_sieve_index << "\n";

			current_sieve_index = get_next_pot_prime(sieve, current_sieve_index);

			if(current_sieve_index == -1) break;

			// check to see if mulitple of primes already found

			jump = is_multiple_of_prime(sieve[current_sieve_index].the_number, primes);
			if (jump * jump > sieve.back().the_number) sieve_stop_sieving = true;

			if (jump != 0) {		//the number is not prime
				gp_return_value = mark_multiples(sieve, current_sieve_index, jump);   //mark multiples
			}
			else {
				primes.push_back(sieve[current_sieve_index].the_number);
				sieve[current_sieve_index].harvested = true;
				gp_return_value = mark_multiples(sieve, current_sieve_index, sieve[current_sieve_index].the_number);   //mark multiples
				current_sieve_index += 1;

			}
		}
	//harvest the primes
//		print_sieve(sieve);
		gp_return_value = harvest_primes(sieve, primes);
		sieve_count += 1;
		current_sieve_index = 0;
	}
	cout << "Found the first " << num_primes << ": \n";
	for (int i = 0;i < primes.size();i++) {
		if (i % 10 == 0) cout << "\n";
		cout << primes[i] << "\t";
	}
	cout << "all done\n";
	keep_window_open();
}

int sieve_init(vector<item>& s, int sieve_s, int sieve_c) {
	item temp_item{ 0, true, false};

	int intitial_number = (sieve_c * sieve_s) + 1;
	int sieve_upper_limit = sieve_s * (sieve_c + 1);

//	cout << "---Sieve_init\n";
	if (s.size() != 0) s.clear();

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
	
//	cout << "---Get_next_pot_prime -- ";
	for (int i = index; i < s.size(); ++i) {
//		cout << "size is " << s.size() << " i is " << i << "s[i] is " << s[i].the_number <<"\n";
		if (s[i].is_prime == true) {
			return i;
		}
		else continue;
	}
	return -1;  // indicating we've reached the end of the current sieve
}

int is_multiple_of_prime(int number, vector<int>& p) {

//	cout << "---Is_multiple_of_prime\n";
	if (p.size() == 0) return 0;
	for (int i = 0;i < p.size();++i) {
		if (number % p[i] == 0) return p[i];
	}
	return 0;
}

int mark_multiples(vector<item>& s, int start, int multiple) {
	int upper_limit = s.size()-1;
//	cout << "---Mark_multiples\n";
	for (int i = start; i <= upper_limit; i += multiple) {
		if(s[i].harvested != true) s[i].is_prime = false;
	}
	return 0;
}

int harvest_primes(vector<item>& s, vector<int>& p) {
	int count{ 0 };
//	cout << "---Harvest_primes\n";
	for (int i = 0;i < s.size();i++) {
		if (s[i].is_prime == true && s[i].harvested == false) {
			p.push_back(s[i].the_number);
			cout << s[i].the_number << "\n";
			count += 1;
		}
	}
	return count;
}
int calc_stop_sieving(vector<item>& s, vector<int> p) {
//	cout << "---Calc_stop_sieving\n";
	int check{ 0 };

	float sqroot{ 0.0 };

	sqroot = sqrt(s.back().the_number);

	check = trunc(sqroot);

	for (int i = 0; i < s.size(); i++) {
		if (check*i > s.back().the_number) return check * (i - 1);
	}
}

/*	if(p.size() == 0){
		for (int i = 0; i <= s.size();i++) {
			check = s[i].the_number;
			if (check * check > s.back().the_number) return s[i].the_number;
		}
	}
	else {
		for (int i = 0; i <= p.size() - 1; i++) {
			check = p[i];
			cout << "check is " << p[i] << "\n";
			if ((2 * p[i]) >= s.back().the_number) return p[i-1];
		}
	}*/


int print_sieve(vector<item>& s) {
//	cout << "current sieve: \n";
	for (int i = 0;i < s.size();i++) {
		cout << s[i].the_number << "\t" << s[i].is_prime << "\t" << s[i].harvested << "\n";
	}
	return 0;
}

