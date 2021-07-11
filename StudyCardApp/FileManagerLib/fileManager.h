#pragma once
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

namespace FileManager {
	typedef struct
	{
		std::wstring keyword;
		std::wstring definition;
		std::wstring fullName;
	} ItemStruct;

	static std::wstring w_directory = L"card-sets/";
	static std::string directory = "card-sets/";

	static std::wstring w_extension = L".txt";
	static std::string extension = ".txt";

	void CreateCardFile(std::wstring fileName);
	void WriteToCardFile(std::wstring filePath, std::vector<std::wstring>& items);
	void GetListOfCardFiles(std::vector<std::wstring>& items);
	void GetItemsFromCardFile(std::string filePath, std::vector<std::wstring>& items);
	void DeleteCardFile(std::string fileName);

	void GetAndParseItemsFromCardFile(std::string filePath, std::vector<ItemStruct>& items);

	std::wstring String2WideString(const std::string& string);
	std::string WideString2String(std::wstring wString);
}

