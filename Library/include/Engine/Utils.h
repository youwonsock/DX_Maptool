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

class Utils
{
public:
	static void ShowErrorMessage(HRESULT hr);
	static void ShowErrorMessage(ID3DBlob* errorCode);
	static void ShowErrorMessage(std::wstring errorMassage);

	static std::string WStringToString(const std::wstring& wstr);
	static std::wstring StringToWString(const std::string& str);

	static bool StartsWith(const std::wstring& str, const std::wstring& comp);
	static bool StartsWith(const std::string& str, const  std::string comp);

	static void Replace(OUT std::string& str, const std::string& comp, const std::string& rep);
	static void Replace(OUT std::wstring& str, const std::wstring& comp, const std::wstring& rep);
};
