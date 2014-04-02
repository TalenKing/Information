#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
// BOOST
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

int main()
{
	vector<vector<string>> dat;
	string line;
	ifstream infile("Enroll-list.txt");
	ofstream outfile("mail.txt");
	size_t line_counter = 0;

	while(getline(infile, line)) {
		if(line.empty()) {
			continue;
		}//if

		dat.push_back(vector<string>());
		boost::char_separator<char> sep(", ");
		boost::tokenizer<boost::char_separator<char>> tok(line, sep);
		
		for(auto tok_iter = tok.begin(), tok_end = tok.end();
			tok_iter != tok_end; ++tok_iter)
		{
			//(*(this->upData_))[line_counter].push_back(*tok_iter);
			dat[line_counter].push_back(*tok_iter);
		}//for

		++line_counter;
	}//while

	auto i_end = dat.end();
	i_end = prev(i_end, 8);

	for(auto i = i_end; i != dat.end(); ++i) {
		auto j = i->begin();

		cout << *j << ',';
		outfile << *j << ',';

		cout << "女" << ',';
		outfile << "女" << ',';

		++j;
		cout << *j << ',';
		outfile << *j << ',';
		++j;
		cout << *j << ',';
		outfile << *j << ',';

		cout << endl;
		outfile << endl;
	}//for

	outfile.close();
	infile.close();
	
	return 0;
}//main

