#include "ConvertToBin.h"

Data::Data(std::string ticket, float period, std::vector<Row>& data)
{
	m_period = period;
	m_ticket = ticket;
	m_data = std::move(data);
}

void Utils::ConverToBinary(std::string fileIn, std::string fileOut)
{
	Data data;
	ReadFromTxt(fileIn, fileOut, data);

	std::ofstream streamFileOut(fileOut, std::ios::binary | std::ios::out);
	streamFileOut << data;
	streamFileOut.close();
};

void Utils::ReadFromTxt(std::string fileIn, std::string fileOut, Data& o_data)
{
	std::ifstream streamFileIn(fileIn);
	std::string line;
	std::getline(streamFileIn, line);//skip headers
	bool headersRead = false;
	uint64_t numberOfRow = 0;
	while (!streamFileIn.eof())
	{
		std::getline(streamFileIn, line);
		std::istringstream ss(line);
		int valueIndex = 0;
		Data::Row row;
		std::string value;
		if (!headersRead)
		{
			std::getline(ss, value, ',');
			o_data.m_ticket = value;
			std::getline(ss, value, ',');
			o_data.m_period = std::atof(value.c_str());
			headersRead = true;
		}
		else
		{
			std::getline(ss, value, ',');
			std::getline(ss, value, ',');
		}

		std::getline(ss, value, ',');
		row.date = std::atoi(value.c_str());

		std::getline(ss, value, ',');
		row.time = std::atoi(value.c_str());

		std::getline(ss, value, ',');
		row.open = std::atof(value.c_str());

		std::getline(ss, value, ',');
		row.high = std::atof(value.c_str());

		std::getline(ss, value, ',');
		row.low = std::atof(value.c_str());

		std::getline(ss, value, ',');
		row.close = std::atof(value.c_str());

		std::getline(ss, value, ',');
		row.vol = std::atoi(value.c_str());

		o_data.m_data.push_back(row);
		numberOfRow++;
	}
	std::cout << "Number of read rows is " << numberOfRow << '\n';
};


#include <ctime>
//Main just for the tests
int main(int argc, char **argv)
{
	std::string fileIn = "input2.txt";
	std::string fileOut = "output.bin";
	Utils::ConverToBinary(fileIn, fileOut);
	
	{
		std::clock_t begin = clock();
		std::ifstream streamFileCheck("output.bin", std::ios::binary | std::ios::in);
		Data dataCheck;
		streamFileCheck >> dataCheck;
		std::clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		std::cout << "Bin read clock time: " << elapsed_secs << '\n';
	}

	{
		std::clock_t begin = clock();
		Data data;
		Utils::ReadFromTxt(fileIn, fileOut, data);
		std::clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		std::cout << "Text clock time: " << elapsed_secs << '\n';
	}
	return 0;

}
