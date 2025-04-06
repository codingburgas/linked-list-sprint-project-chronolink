#include <string>
#include <filesystem>
#include <fstream>
#include <regex>

namespace fs = std::filesystem;

struct Article {
	int yearOfEvent;
	std::string title;
	std::string articleText;
	Article* prev;
	Article* next;

	void publish() {
		std::string safeTitle = std::regex_replace(title, std::regex(R"([\/:*?"<>|])"), "_");
		std::string filename = safeTitle + ".txt";

		fs::current_path("../../Articles");
		std::ofstream outFile(filename);

		outFile << yearOfEvent << "\n";
		outFile << articleText << "\n";

		outFile.close();
	}
};