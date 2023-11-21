#pragma once

struct PathString
{
	WCHAR Drive[_MAX_DRIVE];
	WCHAR Dir[_MAX_DIR];
	WCHAR Filename[_MAX_FNAME];
	WCHAR Ext[_MAX_EXT];

	PathString(const wchar_t* fullPath)
	{
		_wsplitpath_s(fullPath, Drive, _MAX_DRIVE
			, Dir, _MAX_DIR
			, Filename, _MAX_FNAME
			, Ext, _MAX_EXT);
	}
};

inline void ShowErrorMessage(HRESULT hr)
{
	LPCSTR errorText = nullptr;

	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errorText, 0, NULL);

	MessageBoxA(NULL, errorText, "Error", MB_OK | MB_ICONERROR);
}

inline void ShowErrorMessage(ID3DBlob* errorCode)
{
	LPCSTR errorText = "nullptr";

	if (errorCode != nullptr)
		errorText = (LPCSTR)errorCode->GetBufferPointer();

	MessageBoxA(NULL, errorText, "Error", MB_OK | MB_ICONERROR);
}

inline std::string WStringToString(const std::wstring& wstr)
{
	std::string str(wstr.begin(), wstr.end());
	return str;
}

inline std::wstring StringToWString(const std::string& str)
{
	std::wstring wstr(str.begin(), str.end());
	return wstr;
}

inline bool StartsWith(const std::wstring& str, const std::wstring& comp)
{
	std::wstring::size_type index = str.find(comp);

	if (index != std::wstring::npos && index == 0)
		return true;

	return false;
}

inline bool StartsWith(const std::string& str, const  std::string comp)
{
	std::string::size_type index = str.find(comp);
	if (index != std::string::npos && index == 0)
		return true;

	return false;
}