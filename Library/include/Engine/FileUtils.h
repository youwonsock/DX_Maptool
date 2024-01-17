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

	__declspec(noinline) bool Open(const std::wstring& filePath, FileMode mode);

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
	__declspec(noinline) bool Read(OUT T& data)
	{
		DWORD numOfBytes = 0;

		auto t = ::ReadFile(handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr);

		if(t && numOfBytes == 0)
			return false;

		return true;
	}
	template<typename T>
	__declspec(noinline) T Read()
	{
		T data;

		if(!Read(data))
			ZeroMemory(&data, sizeof(T));

		return data;
	}

	__declspec(noinline) void Read(void** data, unsigned __int32 dataSize);
	__declspec(noinline) bool Read(OUT std::string& data);
};

