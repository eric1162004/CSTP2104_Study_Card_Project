#include "fileManager.h"

void FileManager::CreateCardFile(std::wstring fileName)
{
	std::wstring dir = w_directory;
	std::wstring ext = w_extension;
	std::wstring fullFileName = dir.append(fileName).append(ext);

	std::ofstream myFile(fullFileName);
	myFile.close();
}

void FileManager::WriteToCardFile(std::wstring filePath, std::vector<std::wstring>& items)
{
	std::string dir = directory;

	if (!fs::exists(dir))
		fs::create_directory(dir);

	std::ofstream of;
	of.open(WideString2String(filePath));

	std::vector<std::string> saveItems = {};
	for (std::wstring item : items) {
		saveItems.push_back(WideString2String(item));
	}

	for (unsigned i = 0; i < saveItems.size(); i++) {
		of << saveItems[i];
		if (i != saveItems.size() - 1)
			of << std::endl;
	}

	of.close();
}

void FileManager::GetListOfCardFiles(std::vector<std::wstring>& items) {
	std::string dir = directory;

	for (auto& p : fs::directory_iterator(dir)) {
		if (fs::path(p).extension() == extension) {
			items.push_back(String2WideString(p.path().string()));
		}
	}
}

void FileManager::GetItemsFromCardFile(std::string filePath, std::vector<std::wstring>& items) {
	std::string line;
	std::ifstream myfile(filePath);

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			items.push_back(String2WideString(line));
		}
		myfile.close();
	}
	else
		std::cout << "Unable to open file";
};

void FileManager::GetAndParseItemsFromCardFile(std::string filePath, std::vector<ItemStruct>& items)
{
	std::string line;
	std::ifstream myfile(filePath);

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			size_t pos{};
			std::string delimiter = ":";

			std::wstring keyword;
			std::wstring definition;
			std::wstring combined;

			pos = line.find(delimiter);
			keyword = String2WideString(line.substr(0, pos));
			line.erase(0, pos + delimiter.length());
			definition = String2WideString(line);
			combined.append(keyword).append(L":").append(definition);

			items.push_back({ keyword, definition, combined });
		}
		myfile.close();
	}
	else
		std::cout << "Unable to open file";
};

void FileManager::DeleteCardFile(std::string fileName) {
	if (remove(fileName.c_str()) != 0)
		perror("Error deleting file");
	else
		puts("File successfully deleted");
};

std::wstring FileManager::String2WideString(const std::string& multi)
{
	std::wstring wide; wchar_t w; mbstate_t mb{};
	size_t n = 0, len = multi.length() + 1;

	while (auto res = mbrtowc(&w, multi.c_str() + n, len - n, &mb))
	{
		if (res == size_t(-1) || res == size_t(-2))
			throw "invalid encoding";

		n += res;
		wide += w;
	}

	return wide;
}

std::string FileManager::WideString2String(std::wstring wstr)
{
	if (wstr.length() == 0)
		return "";

	std::string str;
	str.assign(wstr.begin(), wstr.end());
	return str;
}




