// DQT Parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "dqtParser.h" // vår egen custom
#include "Windows.h"
#include "X-Tension.h"
#include "string.h"
#include "wchar.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream> // std::cout
#include <fstream>  // std::ifstream
#include <vector>   // std::vector
#include <iomanip>  // std::setprecision
#include <memory> // smartpointers


// Global variables
std::string AppName = "DQT Parser";
double AppVersion = 0.1;



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
	// lage pointer
	// Først få størrelsen
	INT64 itemSize = XWF_GetItemSize(nItemID);
	// Lage en unik pointer slik at vi ikke mister kontroll
	//std::shared_ptr<BYTE> itemPtr = std::make_shared<BYTE>(itemSize);
	BYTE* item_ptr{ nullptr };
	item_ptr = new BYTE[itemSize];
	// Får hvor stor filen faktisk er
	INT64 actualSize = XWF_Read(hItem, 0, item_ptr, itemSize);

	dqtParser parseFile(item_ptr, actualSize);


	XWF_OutputMessage(L"Nå har vi prosessert et item.", 0);
	delete[] item_ptr;
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

}
