#include "pch.h"
#include "CppUnitTest.h"
#include <deque>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ChronoLinkUnitTest
{
	TEST_CLASS(ChronoLinkUnitTest)
	{
	public:

		TEST_METHOD(CTRLBACKSPACE_Erasing)
		{
			std::deque<char> left = { 'H','e','l','l','o',' ','W','o','r','l','d','!' };
			const std::deque<char> right;

			CTRLBACKSPACE_HandlingEraseTest(left, right);

			std::string result(left.begin(), left.end());
			std::string expected = "Hello";

			Assert::IsTrue(result != expected, L"CTRLBACKSPACE_Handling did not trim the string correctly.");
		}
		TEST_METHOD(redrawScreenTesting)
		{
			std::deque<char> left = { 'H','e','l','l','o',' ','W','o','r','l','d','!' };
			const std::deque<char> right = { '!' };

			std::string before(left.begin(), left.end());
			before.append(left.begin(), left.end());

			std::string after = redrawScreenTest(left, right);

			Assert::IsTrue(before != after, L"redrawScreen did not redraw the screen correctly");
		}
	};
}