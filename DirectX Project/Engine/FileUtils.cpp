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

	assert(handle != INVALID_HANDLE_VALUE);
}

void FileUtils::Write(void* data, UINT dataSize)
{
	UINT numOfBytes = 0;
	assert(::WriteFile(handle, data, dataSize, reinterpret_cast<LPDWORD>(&numOfBytes), nullptr));
}

void FileUtils::Write(const std::string& data)
{
	UINT size = (UINT)data.size();
	Write(size);

	if (data.size() == 0)
		return;

	Write((void*)data.data(), size);
}

void FileUtils::Read(void** data, UINT dataSize)
{
	UINT numOfBytes = 0;
	assert(::ReadFile(handle, *data, dataSize, reinterpret_cast<LPDWORD>(&numOfBytes), nullptr));
}

void FileUtils::Read(OUT std::string& data)
{
	UINT size = Read<UINT>();

	if (size == 0)
		return;

	char* temp = new char[size + 1];
	temp[size] = 0;
	Read((void**)&temp, size);
	data = temp;
	delete[] temp;
}