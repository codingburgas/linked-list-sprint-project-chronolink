#include "../headers/Globals.h"
#include "../headers/ArticlesLinkedList.h"

namespace Globals
{

	Classes::Console console;

	Classes::Keyboard keyboard(console);

	ArticlesLinkedList::Article* head = nullptr;

}