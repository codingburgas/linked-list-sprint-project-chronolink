#pragma once

#include <filesystem>
#include "Classes.h"
#include "ArticlesLinkedList.h"

namespace fs = std::filesystem;

namespace Globals
{

	extern Classes::Console console; // declare the classes as an extern variable as they're shared across multiple files

	extern Classes::Keyboard keyboard;

	extern ArticlesLinkedList::Article* head;

}