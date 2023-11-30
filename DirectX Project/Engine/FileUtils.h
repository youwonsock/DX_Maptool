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

	void Open(const std::wstring& filePath, FileMode mode);

	template<typename T>
	void Write(const T& data)
	{
		DWORD numOfBytes = 0;
		assert(::WriteFile(handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
	}
	template<>
	void Write(const std::string& data)
	{
		return Write(data);
	}
	 
	void Write(const std::string& data);
	void Write(void* data, UINT dataSize);


	template<typename T>
	void Read(OUT T& data)
	{
		DWORD numOfBytes = 0;
		assert(::ReadFile(handle, &data, sizeof(T), (LPDWORD)&numOfBytes, nullptr));
	}
	template<typename T>
	T Read()
	{
		T data;
		Read(data);
		return data;
	}

	void Read(void** data, UINT dataSize);
	void Read(OUT std::string& data);
};

