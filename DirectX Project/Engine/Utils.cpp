#include "pch.h"
#include "Utils.h"

void Utils::ShowErrorMessage(HRESULT hr)
{
	LPCSTR errorText = nullptr;

	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errorText, 0, NULL);

	MessageBoxA(NULL, errorText, "Error", MB_OK | MB_ICONERROR);

	assert(false);
}

void Utils::ShowErrorMessage(ID3DBlob* errorCode)
{
	LPCSTR errorText = "nullptr";

	if (errorCode != nullptr)
		errorText = (LPCSTR)errorCode->GetBufferPointer();

	MessageBoxA(NULL, errorText, "Error", MB_OK | MB_ICONERROR);

	assert(false);
}

void Utils::ShowErrorMessage(std::wstring errorMassage)
{
	LPCSTR errorText = "nullptr";
	errorText = (LPCSTR)errorMassage.c_str();
	MessageBoxA(NULL, errorText, "Error", MB_OK | MB_ICONERROR);

	assert(false);
}

std::string Utils::WStringToString(const std::wstring& wstr)
{
	std::string str(wstr.begin(), wstr.end());
	return str;
}

std::wstring Utils::StringToWString(const std::string& str)
{
	std::wstring wstr(str.begin(), str.end());
	return wstr;
}

bool Utils::StartsWith(const std::wstring& str, const std::wstring& comp)
{
	std::wstring::size_type index = str.find(comp);

	if (index != std::wstring::npos && index == 0)
		return true;

	return false;
}

bool Utils::StartsWith(const std::string& str, const std::string comp)
{
	std::string::size_type index = str.find(comp);
	if (index != std::string::npos && index == 0)
		return true;

	return false;
}

void Utils::Replace(OUT std::string& str, const std::string& comp, const std::string& rep)
{
	std::string temp = str;

	UINT startPos = 0;
	while (true)
	{
		startPos = temp.find(comp, startPos);
		if (startPos == std::string::npos || startPos == -1)
			break;

		temp.replace(startPos, comp.length(), rep);
		startPos += rep.length();
	}

	str = temp;
}

void Utils::Replace(OUT std::wstring& str, const std::wstring& comp, const std::wstring& rep)
{
	std::wstring temp = str;

	UINT startPos = 0;
	while (1)
	{
		startPos = temp.find(comp, startPos);
		if (startPos == std::wstring::npos || startPos == -1)
			break;

		temp.replace(startPos, comp.length(), rep);
		startPos += rep.length();
	}

	str = temp;
}
