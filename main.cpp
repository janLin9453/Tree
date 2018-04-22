#include <iostream>
#include <gtest/gtest.h>

//#include "RedBlackTree.h"
//#include "gtest_tree.cpp"
using namespace std;

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	cin.get();
	return 0;
}