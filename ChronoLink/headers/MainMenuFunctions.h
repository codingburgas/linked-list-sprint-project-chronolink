# pragma once
#include "ArticlesLinkedList.h"
#include "Globals.h"
#include <filesystem>

namespace MainMenuFunctions
{

	void getPublishedArticles(const fs::path& folderPath);

	void deleteArticles(ArticlesLinkedList::Article* head);

	ArticlesLinkedList::Article* getArticle(ArticlesLinkedList::Article* head, int index);

	void showMenu(ArticlesLinkedList::Article*& head);

	ArticlesLinkedList::Article* createNewDraft(ArticlesLinkedList::Article*& head);

	void handleExit(ArticlesLinkedList::Article*& startingArticle);

	bool handleEnter(int selected, ArticlesLinkedList::Article*& head, ArticlesLinkedList::Article*& startingArticle, int& start, bool& selectedChanged);

	void handleArrowKeys(int& selected, int& start, ArticlesLinkedList::Article*& startingArticle, bool& selectedChanged, ArticlesLinkedList::Article* head);
}