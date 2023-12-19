#include "pch.h"
#include "FileUtils.h"

FileUtils::FileUtils()
{
}

FileUtils::~FileUtils()
{
	if (handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
	}
}

void FileUtils::Open(const std::wstring& filePath, FileMode mode)
{
	if (mode == FileMode::Write)
	{
		handle = ::CreateFile(
			filePath.c_str(),
			GENERIC_WRITE,
			0,
			nullptr,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
	}
	else
	{
		handle = ::CreateFile
		(
			filePath.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
	}
	if(handle == INVALID_HANDLE_VALUE)
		Utils::ShowErrorMessage(L"File Open Failed");
}

void FileUtils::Write(void* data, unsigned __int32 dataSize)
{
	unsigned __int32 numOfBytes = 0;

	auto t = ::WriteFile(handle, data, dataSize, reinterpret_cast<LPDWORD>(&numOfBytes), nullptr);
	assert(t);
}

void FileUtils::Write(const std::string& data)
{
	UINT32 size = (UINT32)data.size();
	Write(size);

	if (data.size() == 0)
		return;

	Write((void*)data.data(), size);
}

void FileUtils::Read(void** data, unsigned __int32 dataSize)
{
	unsigned __int32 numOfBytes = 0;
	
	auto t = ::ReadFile(handle, *data, dataSize, reinterpret_cast<LPDWORD>(&numOfBytes), nullptr);
	assert(t);
}

void FileUtils::Read(OUT std::string& data)
{
	unsigned __int32 size = Read<unsigned __int32>();

	if (size == 0)
		return;

	char* temp = new char[size + 1];
	temp[size] = 0;
	Read((void**)&temp, size);
	data = temp;
	delete[] temp;
}