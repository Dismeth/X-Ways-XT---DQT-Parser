#include "dqtParser.h"

enum JPEG_MARKERS {
	JM_START = 0xFF,
	JM_SOF0 = 0xC0,
	JM_SOF1 = 0xC1,
	JM_SOF2 = 0xC2,
	JM_SOF3 = 0xC3,
	JM_DHT = 0xC4,
	JM_SOF5 = 0xC5,
	JM_SOF6 = 0xC6,
	JM_SOF7 = 0xC7,
	JM_JPG = 0xC8,
	JM_SOF9 = 0xC9,
	JM_SOF10 = 0xCA,
	JM_SOF11 = 0xCB,
	JM_DAC = 0xCC,
	JM_SOF13 = 0xCD,
	JM_SOF14 = 0xCE,
	JM_SOF15 = 0xCF,
	JM_RST0 = 0xD0,
	JM_RST1 = 0xD1,
	JM_RST2 = 0xD2,
	JM_RST3 = 0xD3,
	JM_RST4 = 0xD4,
	JM_RST5 = 0xD5,
	JM_RST6 = 0xD6,
	JM_RST7 = 0xD7,
	JM_SOI = 0xD8,
	JM_EOI = 0xD9,
	JM_SOS = 0xDA,
	JM_DQT = 0xDB, // This is the one we are interested in
	JM_DNL = 0xDC,
	JM_DRI = 0xDD,
	JM_DHP = 0xDE,
	JM_EXP = 0xDF,
	JM_APP0 = 0xE0,
	JM_APP1 = 0xE1, // EXIF and XMP
	JM_APP2 = 0xE2,
	JM_APP3 = 0xE3,
	JM_APP4 = 0xE4,
	JM_APP5 = 0xE5,
	JM_APP6 = 0xE6,
	JM_APP7 = 0xE7,
	JM_APP8 = 0xE8,
	JM_APP9 = 0xE9,
	JM_APP10 = 0xEA,
	JM_APP11 = 0xEB,
	JM_APP12 = 0xEC,
	JM_APP13 = 0xED, // IPTC
	JM_APP14 = 0xEE,
	JM_APP15 = 0xEF,
	JM_JPG0 = 0xF0,
	JM_JPG1 = 0xF1,
	JM_JPG2 = 0xF2,
	JM_JPG3 = 0xF3,
	JM_JPG4 = 0xF4,
	JM_JPG5 = 0xF5,
	JM_JPG6 = 0xF6,
	JM_JPG7 = 0xF7,
	JM_JPG8 = 0xF8,
	JM_JPG9 = 0xF9,
	JM_JPG10 = 0xFA,
	JM_JPG11 = 0xFB,
	JM_JPG12 = 0xFC,
	JM_JPG13 = 0xFD,
	JM_COM = 0xFE
};

enum dqtParserErrorCodes {
	PARSE_SUCCESS = 0, // Parse success and valid JPEG
	PARSE_INVALID = 1, // Invalid JPEG
	PARSE_FAILED = 2, // Something went wrong
};

dqtParser::dqtParser()
	: data(nullptr), length(0){
}

dqtParser::dqtParser(const uint8_t* data, unsigned length)
	: data(data), length(length), data_end(data+length), current_offset(0), current_ptr(data){
	parseData();
}

const uint8_t* dqtParser::getBuffer(unsigned size = 1) {
	const uint8_t* buffer(current_ptr);
	if (size + current_offset >= length) {
		return NULL;
	}
	current_offset += size;
	current_ptr += size;
	return buffer;
}

uint16_t dqtParser::parse16(const uint8_t* buf, bool intel) {
	if (intel)
		return ((uint16_t)buf[1] << 8) | buf[0];
	return ((uint16_t)buf[0] << 8) | buf[1];
}

bool dqtParser::writeToFile() {
}

int dqtParser::parseData() {
	const uint8_t* buf(getBuffer());
	//check if proper header
	if (buf == NULL || buf[0] != JM_START || buf[1] != JM_SOI)
		return PARSE_INVALID;

	while (buf = getBuffer()) {
		if (buf[0] == JM_START && buf[1] == JM_DQT) {
			// hoppe over en
			getBuffer();
			// hva får vi her?

			int sectionLength = parse16(getBuffer(2), false);
			

		
		}


	}
	
	return PARSE_SUCCESS;
}

