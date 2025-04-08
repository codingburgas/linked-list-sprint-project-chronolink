#include "../headers/ArticlesLinkedList.h"
#include "../headers/InputFunctions.h"
#include <filesystem>
#include "../headers/Globals.h"
#include <fstream>
#include <regex>
#include <iostream>

namespace ArticlesLinkedList
{

    void Article::publish() const {
        std::string safeTitle = std::regex_replace(title, std::regex(R"([\/:*?"<>|])"), "_");
        fs::path articlesDir = "../Articles";

        if (!fs::exists(articlesDir)) {
            fs::create_directories(articlesDir);
        }

        fs::path filePath = articlesDir / (safeTitle + ".txt");

        std::ofstream outFile(filePath);
        if (!outFile.is_open()) {
            std::cerr << "Failed to open file: " << fs::absolute(filePath) << std::endl;
            return;
        }

        outFile << yearOfEvent << "\n";
        outFile << articleText << "\n";

        std::cout << "Article published to: " << fs::absolute(filePath) << std::endl;
    }


    void Article::selectArticle() {
        InputFunctions::textEditor(this);
    }

}