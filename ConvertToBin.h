#pragma once
#include <stdio.h>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <cstdint>
#include <vector>



struct Data
{
	struct Row
	{
		uint32_t date;
		uint32_t time;
		float open;
		float high;
		float low;
		float close;
		uint32_t vol;

	};

	static uint8_t GetVersion() { return 1; }
	static uint8_t GetMagicNumber() { return 0x44a6; }

	Data() :m_period(0.f) {};
	Data(std::string ticket, float period, std::vector<Row>& data);
	std::size_t GetSizeInBytes() const { return m_data.size() * sizeof(Row); }


	std::string m_ticket;
	float m_period;
	std::vector<Row> m_data;
};
namespace Utils
{
	void ConverToBinary(std::string fileIn, std::string fileOut);
	void ReadFromTxt(std::string fileIn, std::string fileOut, Data& o_data);
};



std::ostream& operator<<(std::ostream& os, const Data& obj)
{
	uint8_t mn = Data::GetMagicNumber();
	uint8_t ver = Data::GetVersion();
	size_t size = obj.GetSizeInBytes();

	os.write(reinterpret_cast<const char*>(&mn), sizeof(uint8_t));
	os.write(reinterpret_cast<const char*>(&ver), sizeof(uint8_t));
	size_t strSize = obj.m_ticket.size();
	os.write(reinterpret_cast<const char*>(&strSize), sizeof(size_t));
	os.write(obj.m_ticket.c_str(), strSize);
	os.write(reinterpret_cast<const char*>(&obj.m_period), sizeof(float));
	os.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	os.write(reinterpret_cast<const char*>(&obj.m_data[0]), obj.GetSizeInBytes());

	return os;
}

std::istream& operator >> (std::istream& is, Data& obj)
{
	uint8_t maginNumber;
	uint8_t version;
	is.read(reinterpret_cast<char*>(&maginNumber), sizeof(uint8_t));
	is.read(reinterpret_cast<char*>(&version), sizeof(uint8_t));
	if (maginNumber != Data::GetMagicNumber())
		throw std::exception("Invalid magic number of file");
	if (version != Data::GetVersion())
	{
		std::stringstream ss;
		ss << "Versions is differents " << version << "; " << Data::GetVersion();
		throw std::exception("Versions is differents");
	}
	size_t strSize = 0;
	is.read(reinterpret_cast< char*>(&strSize), sizeof(size_t));
	obj.m_ticket.resize(strSize);
	is.read(&obj.m_ticket[0], strSize);
	is.read(reinterpret_cast<char*>(&obj.m_period), sizeof(float));
	size_t size;
	is.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	obj.m_data.resize(size/sizeof(Data::Row));
	is.read(reinterpret_cast<char*>(&obj.m_data[0]), obj.GetSizeInBytes());

	return is;
}

