// DQT Parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Windows.h"
#include "X-Tension.h"
#include "TinyEXIF.h"
#include "string.h"
#include "wchar.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream> // std::cout
#include <fstream>  // std::ifstream
#include <vector>   // std::vector
#include <iomanip>  // std::setprecision


// Global variables
std::string AppName = "DQT Parser";
double AppVersion = 0.1;

class EXIFStreamFile : public TinyEXIF::EXIFStream {
public:
	explicit EXIFStreamFile(const char* fileName)
		: file(fileName, std::ifstream::in | std::ifstream::binary) {}
	bool IsValid() const override {
		return file.is_open();
	}
	const uint8_t* GetBuffer(unsigned desiredLength) override {
		buffer.resize(desiredLength);
		if (!file.read((char*)buffer.data(), desiredLength))
			return NULL;
		return buffer.data();
	}
	bool SkipBuffer(unsigned desiredLength) override {
		return (bool)file.seekg(desiredLength, std::ios::cur);
	}
	unsigned StartNewSearch(unsigned desiredPos) override {
		previousPosition = file.tellg();
		file.seekg(desiredPos);
		std::cout << std::hex << "desiredPos: " << desiredPos << " at " << previousPosition << " is now at " << file.tellg() << std::dec << std::endl;
		return previousPosition;
	}
private:
	std::ifstream file;
	std::vector<uint8_t> buffer;
};


DWORD gStartTime;
///////////////////////////////////////////////////////////////////////////////
// XT_Init

LONG __stdcall XT_Init(DWORD nVersion, DWORD nFlags, HANDLE hMainWnd,
	void* lpReserved)
{
	XT_RetrieveFunctionPointers();
	
	XWF_OutputMessage(L"DQT Parser initialized. Version 0.1", 0);
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// XT_Done

LONG __stdcall XT_Done(void* lpReserved)
{
	XWF_OutputMessage (L"DQT Parser done", 0);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// XT_About

LONG __stdcall XT_About(HANDLE hParentWnd, void* lpReserved)
{
	XWF_OutputMessage(L"DQT Parser about. God påske alle sammen! Mvh Såvi :) :)", 0);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

void appendDateTimeStr(wchar_t* msg, size_t bufLen)
{
	const int buf2Len = 100;
	wchar_t msg2[buf2Len];
	SYSTEMTIME sysTime;

	GetLocalTime(&sysTime);
	GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, DATE_SHORTDATE, &sysTime, NULL,
		msg2, buf2Len, NULL);
	wcscat_s(msg, bufLen, msg2);
	wcscat_s(msg, bufLen, L", ");
	GetTimeFormatW(LOCALE_USER_DEFAULT, 0, &sysTime, NULL, msg2, buf2Len);
	wcscat_s(msg, bufLen, msg2);
}

///////////////////////////////////////////////////////////////////////////////
// XT_Prepare

LONG __stdcall XT_Prepare(HANDLE hVolume, HANDLE hEvidence, DWORD nOpType,
	void* lpReserved)
{
	//XWF_OutputMessage (L"X-Tension prepare", 0);
	wchar_t msg[100] = L"Refine volume snapshot started on ";
	appendDateTimeStr(msg, 100);
	XWF_OutputMessage(msg, 0);
	gStartTime = GetTickCount();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// XT_Finalize

LONG __stdcall XT_Finalize(HANDLE hVolume, HANDLE hEvidence, DWORD nOpType,
	void* lpReserved)
{
	wchar_t msg[100] = L"Refine volume snapshot ended on ";
	appendDateTimeStr(msg, 100);
	wcscat_s(msg, L", ");

	DWORD stopTime = GetTickCount();
	DWORD delta = stopTime - gStartTime;

	if (delta > 100 * 1000) {
		DWORD seconds = (delta / 1000) % 60;
		DWORD minutes = (delta / (60 * 1000)) % 60;
		DWORD hours = (delta / (60 * 60 * 1000)) % 60;
		if (hours > 0) {
			wchar_t hbuf[10] = L"";
			_itow_s(hours, hbuf, 10, 10);
			wcscat_s(msg, hbuf);
			wcscat_s(msg, L" h ");
		}

		if (minutes > 0) {
			wchar_t mbuf[10] = L"";
			_itow_s(minutes, mbuf, 10, 10);
			wcscat_s(msg, mbuf);
			wcscat_s(msg, L" min ");
		}

		wchar_t sbuf[10] = L"";
		_itow_s(seconds, sbuf, 10, 10);
		wcscat_s(msg, sbuf);
		wcscat_s(msg, L" s ");
	}
	else {
		DWORD deciseconds = delta % 10;
		DWORD seconds = (delta / 1000) % 60;
		wchar_t sbuf[10], dsbuf[10];
		_itow_s(seconds, sbuf, 10, 10);
		_itow_s(deciseconds, dsbuf, 10, 10);
		wcscat_s(msg, sbuf);
		wcscat_s(msg, L".");
		wcscat_s(msg, dsbuf);
		wcscat_s(msg, L" s ");
	}

	XWF_OutputMessage(msg, 0);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// XT_ProcessItem

LONG __stdcall XT_ProcessItem(LONG nItemID, void* lpReserved)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// XT_ProcessItemEx

LONG __stdcall XT_ProcessItemEx(LONG nItemID, HANDLE hItem, void* lpReserved)
{
	wchar_t* testvariable{nullptr};
	long itemSize = XWF_GetItemSize(nItemID);
	testvariable = (wchar_t*)XWF_GetItemSize(nItemID);
	wchar_t msg = (wchar_t)itemSize;
	XWF_OutputMessage(&msg, 0);
	XWF_OutputMessage(L"Nå har vi prosessert et item.", 0);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// XT_ProcessSearchHit

#pragma pack(2)
struct SearchHitInfo {
	LONG iSize;
	LONG nItemID;
	LARGE_INTEGER nRelOfs;
	LARGE_INTEGER nAbsOfs;
	void* lpOptionalHitPtr;
	WORD lpSearchTermID;
	WORD nLength;
	WORD nCodePage;
	WORD nFlags;
};

LONG __stdcall XT_ProcessSearchHit(struct SearchHitInfo* info)
{
	//XWF_OutputMessage (L"X-Tension proc. sh", 0);
	return 0;
}

// MAIN FUNCTION

int main(int argc, const char** argv)
{
	if (argc != 2) {
		std::cout << "Usage: TinyEXIF <image_file>\n";
		return -1;
	}

	// read entire image file
	EXIFStreamFile stream(argv[1]);
	if (!stream.IsValid()) {
		std::cout << "error: can not open '" << argv[1] << "'\n";
		return -2;
	}

	// parse image EXIF and XMP metadata
	TinyEXIF::EXIFInfo imageEXIF(stream);
	if (!imageEXIF.Fields) {
		std::cout << "error: no EXIF or XMP metadata\n";
		return -3;
	}

	// print extracted metadata
	if (imageEXIF.ImageWidth || imageEXIF.ImageHeight)
		std::cout << "ImageResolution " << imageEXIF.ImageWidth << "x" << imageEXIF.ImageHeight << " pixels" << "\n";
	if (imageEXIF.RelatedImageWidth || imageEXIF.RelatedImageHeight)
		std::cout << "RelatedImageResolution " << imageEXIF.RelatedImageWidth << "x" << imageEXIF.RelatedImageHeight << " pixels" << "\n";
	if (!imageEXIF.ImageDescription.empty())
		std::cout << "Description " << imageEXIF.ImageDescription << "\n";
	if (!imageEXIF.Make.empty() || !imageEXIF.Model.empty())
		std::cout << "CameraModel " << imageEXIF.Make << " - " << imageEXIF.Model << "\n";
	if (!imageEXIF.SerialNumber.empty())
		std::cout << "SerialNumber " << imageEXIF.SerialNumber << "\n";
	if (imageEXIF.Orientation)
		std::cout << "Orientation " << imageEXIF.Orientation << "\n";
	if (imageEXIF.XResolution || imageEXIF.YResolution || imageEXIF.ResolutionUnit)
		std::cout << "Resolution " << imageEXIF.XResolution << "x" << imageEXIF.YResolution << " (" << imageEXIF.ResolutionUnit << ")\n";
	if (imageEXIF.BitsPerSample)
		std::cout << "BitsPerSample " << imageEXIF.BitsPerSample << "\n";
	if (!imageEXIF.Software.empty())
		std::cout << "Software " << imageEXIF.Software << "\n";
	if (!imageEXIF.DateTime.empty())
		std::cout << "DateTime " << imageEXIF.DateTime << "\n";
	if (!imageEXIF.DateTimeOriginal.empty())
		std::cout << "DateTimeOriginal " << imageEXIF.DateTimeOriginal << "\n";
	if (!imageEXIF.DateTimeDigitized.empty())
		std::cout << "DateTimeDigitized " << imageEXIF.DateTimeDigitized << "\n";
	if (!imageEXIF.SubSecTimeOriginal.empty())
		std::cout << "SubSecTimeOriginal " << imageEXIF.SubSecTimeOriginal << "\n";
	if (!imageEXIF.Copyright.empty())
		std::cout << "Copyright " << imageEXIF.Copyright << "\n";
	std::cout << "ExposureTime " << std::setprecision(10) << imageEXIF.ExposureTime << " s" << "\n";
	std::cout << "FNumber " << imageEXIF.FNumber << "\n";
	std::cout << "ExposureProgram " << imageEXIF.ExposureProgram << "\n";
	std::cout << "ISOSpeed " << imageEXIF.ISOSpeedRatings << "\n";
	std::cout << "ShutterSpeedValue " << std::setprecision(10) << imageEXIF.ShutterSpeedValue << "\n";
	std::cout << "ApertureValue " << std::setprecision(10) << imageEXIF.ApertureValue << "\n";
	std::cout << "BrightnessValue " << std::setprecision(10) << imageEXIF.BrightnessValue << "\n";
	std::cout << "ExposureBiasValue " << imageEXIF.ExposureBiasValue << "\n";
	std::cout << "SubjectDistance " << imageEXIF.SubjectDistance << "\n";
	std::cout << "FocalLength " << imageEXIF.FocalLength << " mm" << "\n";
	std::cout << "Flash " << imageEXIF.Flash << "\n";
	if (!imageEXIF.SubjectArea.empty()) {
		std::cout << "SubjectArea";
		for (uint16_t val : imageEXIF.SubjectArea)
			std::cout << " " << val;
		std::cout << "\n";
	}
	std::cout << "MeteringMode " << imageEXIF.MeteringMode << "\n";
	std::cout << "LightSource " << imageEXIF.LightSource << "\n";
	std::cout << "ProjectionType " << imageEXIF.ProjectionType << "\n";
	if (imageEXIF.Calibration.FocalLength != 0)
		std::cout << "Calibration.FocalLength " << imageEXIF.Calibration.FocalLength << " pixels" << "\n";
	if (imageEXIF.Calibration.OpticalCenterX != 0)
		std::cout << "Calibration.OpticalCenterX " << imageEXIF.Calibration.OpticalCenterX << " pixels" << "\n";
	if (imageEXIF.Calibration.OpticalCenterY != 0)
		std::cout << "Calibration.OpticalCenterY " << imageEXIF.Calibration.OpticalCenterY << " pixels" << "\n";
	std::cout << "LensInfo.FStopMin " << imageEXIF.LensInfo.FStopMin << "\n";
	std::cout << "LensInfo.FStopMax " << imageEXIF.LensInfo.FStopMax << "\n";
	std::cout << "LensInfo.FocalLengthMin " << imageEXIF.LensInfo.FocalLengthMin << " mm" << "\n";
	std::cout << "LensInfo.FocalLengthMax " << imageEXIF.LensInfo.FocalLengthMax << " mm" << "\n";
	std::cout << "LensInfo.DigitalZoomRatio " << imageEXIF.LensInfo.DigitalZoomRatio << "\n";
	std::cout << "LensInfo.FocalLengthIn35mm " << imageEXIF.LensInfo.FocalLengthIn35mm << "\n";
	std::cout << "LensInfo.FocalPlaneXResolution " << std::setprecision(10) << imageEXIF.LensInfo.FocalPlaneXResolution << "\n";
	std::cout << "LensInfo.FocalPlaneYResolution " << std::setprecision(10) << imageEXIF.LensInfo.FocalPlaneYResolution << "\n";
	std::cout << "LensInfo.FocalPlaneResolutionUnit " << imageEXIF.LensInfo.FocalPlaneResolutionUnit << "\n";
	if (!imageEXIF.LensInfo.Make.empty() || !imageEXIF.LensInfo.Model.empty())
		std::cout << "LensInfo.Model " << imageEXIF.LensInfo.Make << " - " << imageEXIF.LensInfo.Model << "\n";
	if (imageEXIF.GeoLocation.hasLatLon()) {
		std::cout << "GeoLocation.Latitude " << std::setprecision(10) << imageEXIF.GeoLocation.Latitude << "\n";
		std::cout << "GeoLocation.Longitude " << std::setprecision(10) << imageEXIF.GeoLocation.Longitude << "\n";
	}
	if (imageEXIF.GeoLocation.hasAltitude()) {
		std::cout << "GeoLocation.Altitude " << imageEXIF.GeoLocation.Altitude << " m" << "\n";
		std::cout << "GeoLocation.AltitudeRef " << (int)imageEXIF.GeoLocation.AltitudeRef << "\n";
	}
	if (imageEXIF.GeoLocation.hasRelativeAltitude())
		std::cout << "GeoLocation.RelativeAltitude " << imageEXIF.GeoLocation.RelativeAltitude << " m" << "\n";
	if (imageEXIF.GeoLocation.hasOrientation()) {
		std::cout << "GeoLocation.RollDegree " << imageEXIF.GeoLocation.RollDegree << "\n";
		std::cout << "GeoLocation.PitchDegree " << imageEXIF.GeoLocation.PitchDegree << "\n";
		std::cout << "GeoLocation.YawDegree " << imageEXIF.GeoLocation.YawDegree << "\n";
	}
	if (imageEXIF.GeoLocation.hasSpeed()) {
		std::cout << "GeoLocation.SpeedX " << imageEXIF.GeoLocation.SpeedX << " m/s" << "\n";
		std::cout << "GeoLocation.SpeedY " << imageEXIF.GeoLocation.SpeedY << " m/s" << "\n";
		std::cout << "GeoLocation.SpeedZ " << imageEXIF.GeoLocation.SpeedZ << " m/s" << "\n";
	}
	if (imageEXIF.GeoLocation.AccuracyXY > 0 || imageEXIF.GeoLocation.AccuracyZ > 0)
		std::cout << "GeoLocation.GPSAccuracy XY " << imageEXIF.GeoLocation.AccuracyXY << " m" << " Z " << imageEXIF.GeoLocation.AccuracyZ << " m" << "\n";
	std::cout << "GeoLocation.GPSDOP " << imageEXIF.GeoLocation.GPSDOP << "\n";
	std::cout << "GeoLocation.GPSDifferential " << imageEXIF.GeoLocation.GPSDifferential << "\n";
	if (!imageEXIF.GeoLocation.GPSMapDatum.empty())
		std::cout << "GeoLocation.GPSMapDatum " << imageEXIF.GeoLocation.GPSMapDatum << "\n";
	if (!imageEXIF.GeoLocation.GPSTimeStamp.empty())
		std::cout << "GeoLocation.GPSTimeStamp " << imageEXIF.GeoLocation.GPSTimeStamp << "\n";
	if (!imageEXIF.GeoLocation.GPSDateStamp.empty())
		std::cout << "GeoLocation.GPSDateStamp " << imageEXIF.GeoLocation.GPSDateStamp << "\n";
	return EXIT_SUCCESS;
}
