#pragma once

#include <string>

namespace ArticlesLinkedList
{

	struct Article {
		int yearOfEvent;
		std::string title;
		std::string articleText;
		bool isDraft;
		Article* prev;
		Article* next;

		void publish() const;

		void selectArticle();
	};

}