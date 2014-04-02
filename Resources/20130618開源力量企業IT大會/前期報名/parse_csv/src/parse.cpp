// STL
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <memory>
#include <functional>
// BOOST
#include <boost/tokenizer.hpp>

class CsvParser {
private:
	using upFileType = std::unique_ptr<
		std::fstream,
		std::function<void(std::fstream * p)>>;
	using upDataType = std::unique_ptr<
		std::vector<std::vector<std::string>>>;
public:
	explicit
	CsvParser(const std::string & fileName) :
		upFile_(new std::fstream(fileName, std::ios_base::in),
					[](std::fstream * p) { p->close(); delete p; }),
		upData_(new std::vector<std::vector<std::string>>)
	{}//Constructor

	const std::vector<std::vector<std::string>> &
	parse() const {
		std::size_t line_counter = 0;
		std::string lineString;
		boost::char_separator<char> sep(", ");

		// read in the file
		while(std::getline(*(this->upFile_), lineString)) {
			if(lineString.empty())
				continue;
			this->upData_->push_back(std::vector<std::string>());

			boost::tokenizer<boost::char_separator<char>> tok(
					lineString, sep);
			for(auto tok_iter = tok.begin(), tok_end = tok.end();
				tok_iter != tok_end; ++tok_iter)
			{
				(*(this->upData_))[line_counter].push_back(*tok_iter);
			}//for
			++line_counter;
		}//while

		return *(this->upData_);
	}//parse()

private:
	upFileType upFile_;
	upDataType upData_;
};//class CsvParser

int main(int argc, char * argv[]) {
	if(argc != 2) {
		std::cerr << "Usage: " << argv[0] << " csvfile.csv" << std::endl;
		exit(EXIT_FAILURE);
	}//if

	CsvParser cp(argv[1]);
	std::cout << "Parsing ..." << std::endl;

	const auto & data_ref = cp.parse();
	if(data_ref.size() == 0) {
		std::cout << "No Data Available." << std::endl;
		return 0;
	}//if

	auto row_max = data_ref.size();
	auto col_max = data_ref[0].size();
	std::string line;
	std::vector<std::string> line_param;
	boost::char_separator<char> sep("[], ");

	// INTERACTIVE MODE
	std::cout << "Interactive Mode: Ctrl-D to Exit" << std::endl;
	std::cout << "list all the data: " << std::endl;
	std::cout << ">> list" << std::endl;
	std::cout << "output to file: " << std::endl;
	std::cout << ">> [Row_Begin,Row_End][Col_Begin,Col_End] FileName"
		<< std::endl;
	std::cout << "Get Data from data[Row][Column]: " << std::endl;
	std::cout << ">> Row,Column" << std::endl;
	std::cout << "Row size: " << row_max << " Col size: " << col_max << std::endl;
	while(1) {
		line.clear();
		// OUTPUT Title
		for(auto i : data_ref[0])
			std::cout << i << ' ';
		std::cout << std::endl;
		// GET INPUT
		do {
			std::cout << ">> ";
			std::getline(std::cin, line);
		} while(line.empty() && std::cin);

		// Test Ctrl-D
		if(!std::cin) {
			std::cout << std::endl;
			break;
		}//if

		// PARSE INPUT
		line_param.clear();
		boost::tokenizer<boost::char_separator<char>> tok(line, sep);
		std::copy(tok.begin(), tok.end(), std::back_inserter(line_param));

		try {
			switch(line_param.size()) {
			case 1: {
					if(line_param[0] == "l" || line_param[0] == "list") {
						std::size_t line_counter = 0;
						auto i = data_ref.begin(), i_end = data_ref.end();
						std::advance(i, 1); // skip the title
						for(; i != i_end; ++i) {
							std::cout << ++line_counter << ": ";
							for(auto j : *i) {
								std::cout << j << ',';
							}//for
							std::cout << "\b \b" << std::endl;
						}//for
					}//if
				}
				break;
			case 2: {
					auto row = std::stoi(line_param[0]);
					auto col = std::stoi(line_param[1]);
					if(row >= row_max || col >= col_max) {
						std::cout << "Out of Range!" << std::endl;
					} else {
						std::cout << data_ref[row][col] << std::endl;
					}//if-else
				}
				break;
			case 5: {
					std::ofstream out(line_param[4]);

					for(auto row_iter = std::stoi(line_param[0])-1,
								row_end = std::stoi(line_param[1]);
							row_iter != row_end;
							++row_iter)
					{
						for(auto col_iter = std::stoi(line_param[2])-1,
									col_end = std::stoi(line_param[3]);
								col_iter != col_end;
								++col_iter)
						{
							out << data_ref[row_iter][col_iter] << ',';
						}//for
						out << std::endl;
					}//for

					out.close();
				}//case
				break;
			default:
				std::cout << "Format Error" << std::endl;
			}//switch-case
		} catch(...) {
			std::cerr << "Format Error" << std::endl;
		}//try-catch
	}//while

	return 0;
}//main

