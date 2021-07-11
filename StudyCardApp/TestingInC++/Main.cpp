#include <iostream>
#include <file.h>
#include <fileManager.h>

int main()
{
	std::vector<FileManager::ItemStruct> items;

	FileManager::GetAndParseItemsFromCardFile("card-sets/abc.txt", items);

	for (auto item : items)
	{
		std::cout << FileManager::WideString2String(item.keyword) << std::endl;
		std::cout << FileManager::WideString2String(item.definition) << std::endl;
		std::cout << FileManager::WideString2String(item.fullName) << std::endl;
	}

}