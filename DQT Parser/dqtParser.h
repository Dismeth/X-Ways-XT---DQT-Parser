#pragma once
#include <cstdint>
#include <vector>
#include <string>
class dqtParser
{
public:
	// constructors
	dqtParser();
	dqtParser(const uint8_t* data, unsigned length);

	// methods
	int parseData();
	const uint8_t* getBuffer(unsigned size);

	bool skipBuffer(unsigned size);

	uint16_t parse16(const uint8_t* buf, bool intel);

	std::string convertToHex(const uint8_t byte);

	// write all of our findings to a csv-file.
	bool writeToFile();

	// Struct to store references of each DQT Table
	struct dqt_table {
		int id;
		int offset;
		int sectionLength;
		int dqt_info;
		const uint8_t* dqt;
	};

private:
	// private variables
	const uint8_t* data;
	const uint8_t* data_end;
	const uint8_t* current_ptr;
	unsigned length;
	unsigned current_offset;
	std::vector<dqt_table> all_dqt_tables;
	int dqt_table_id;
};

