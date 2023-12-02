#pragma once

enum FileMode : UINT
{
	Read = 0,
	Write = 1,
};

class FileUtils
{
private:
	HANDLE handle = INVALID_HANDLE_VALUE;

public:
	FileUtils();
	~FileUtils();

	__declspec(noinline) void Open(const std::wstring& filePath, FileMode mode);

	template<typename T>
	__declspec(noinline) void Write(const T& data)
	{
		DWORD numOfBytes = 0;                                   

		auto t = ::WriteFile(handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr);
		assert(t);
	}
	template<>
	__declspec(noinline) void Write(const std::string& data)
	{
		return Write(data);
	}
	 
	__declspec(noinline) void Write(const std::string& data);
	__declspec(noinline) void Write(void* data, unsigned __int32 dataSize);


	template<typename T>
	__declspec(noinline) void Read(OUT T& data)
	{
		DWORD numOfBytes = 0;

		auto t = ::ReadFile(handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr);
		assert(t);
	}
	template<typename T>
	__declspec(noinline) T Read()
	{
		T data;
		Read(data);
		return data;
	}

	__declspec(noinline) void Read(void** data, unsigned __int32 dataSize);
	__declspec(noinline) void Read(OUT std::string& data);
};

