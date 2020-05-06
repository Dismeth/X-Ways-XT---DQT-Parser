#pragma once
#include <cstdint>
class dqtParser
{
public:
	// constructors
	dqtParser();
	dqtParser(const uint8_t* data, unsigned length);

	// methods
	int parseData();
	const uint8_t* getBuffer(unsigned size);

	uint16_t parse16(const uint8_t* buf, bool intel);

	// write all of our findings to a csv-file.
	bool writeToFile();

private:
	// private variables
	const uint8_t* data;
	const uint8_t* data_end;
	const uint8_t* current_ptr;
	unsigned length;
	unsigned current_offset;
};

