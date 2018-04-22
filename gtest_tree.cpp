#include "RedBlackTree.h"
#include <string>
#include <gtest/gtest.h>

template <typename T>
bool operator==(const nodeData<T> &in1, const nodeData<T> &in2) 
{
	return (in1._key == in2._key) && (in1._data == in2._data);
}

template <class T>
std::ostream& operator<<(std::ostream& os, const nodeData<T>& dt)  
{
	os << "(key, data)=>(" << dt._key << "," << dt._data<<")";  
    return os;  
}

template <class T>
bool operator==(const treeNode<T> &in1, const treeNode<T> &in2) 
{
	return (in1._color == in2._color) && (in1._nodeData == in2._nodeData);
}

template <class T>
std::ostream& operator<<(std::ostream& os, const treeNode<T>& dt)  
{
	os << "(Dict, color)=>(" << dt._nodeData << "," << dt._color<<")";  
    return os;  
}

class RBTtest : public testing::Test
{
protected:
	RedBlackTree<int>* tree;
	void SetUp(){
		tree = new RedBlackTree<int>();
		char key[] = "36 16 41 4 22 39 48 3 9 19 27 45 52 1 7 10 24 51 55",
			 data[] = "L G N C I M P B E H K x x O R A x D F x x J x x x Q S",
			 color[] = "1 0 1 1 1 1 0 1 0 1 1 1 1 0 1 1 0 0 0";

		tree->level_order_constructor(key, data, color);
	};
};

TEST_F(RBTtest, testStructure)
{
	std::vector<treeNode<int>*> vec, vecVerify;
	treeNode<int>* node;
	std::string dataStr;

	// verify key data color
	int key[] = {36,16,41,4,22,39,48,3,9,19,27,45,52,1,7,10,24,51,55};
	char data[] = {'L','G','N','C','I','M','P','B','E','H','K','O','R','A','D','F','J','Q','S'};
	bool color[] = {1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,1,0,0,0};
	vec = tree->level_order_structure(); //testing of tree node vector
	for (int i = 0; i < sizeof(key)/sizeof(*key); i++){
		dataStr=data[i];
		node = new treeNode<int>(nodeData<int>(key[i], dataStr),0,0,color[i]);// answer of tree node vector
		vecVerify.push_back(node);
	}

	int cnt=0;
	std::vector<treeNode<int>*>::iterator itrVerify = vecVerify.begin();
	for (std::vector<treeNode<int>*>::iterator itr = vec.begin(); itr != vec.end(); itr++, itrVerify++, cnt++){
		ASSERT_EQ(**itrVerify, **itr)<<"vec["<<++cnt<<"] is wrong";
	}
}

TEST_F(RBTtest, testRemoveCase3to4)
{
	std::vector<treeNode<int>*> vec, vecVerify;
	treeNode<int>* node;
	std::string dataStr;

	// verify key data color
	int key[] = {36,16,41,4,24,39,48,3,9,22,27,45,52,1,7,10,51,55};
	char data[] = {'L','G','N','C','J','M','P','B','E','I','K','O','R','A','D','F','Q','S'};
	bool color[] = {1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,1,0,0};

	tree->remove(19);
	vec = tree->level_order_structure(); //testing of tree node vector
	for (int i = 0; i < sizeof(key)/sizeof(*key); i++){
		dataStr=data[i];
		node = new treeNode<int>(nodeData<int>(key[i], dataStr),0,0,color[i]);// answer of tree node vector
		vecVerify.push_back(node);
	}

	int cnt=0;
	std::vector<treeNode<int>*>::iterator itrVerify = vecVerify.begin();
	for (std::vector<treeNode<int>*>::iterator itr = vec.begin(); itr != vec.end(); itr++, itrVerify++, cnt++){
		ASSERT_EQ(**itrVerify, **itr)<<"vec["<<++cnt<<"] is wrong";
	}
}

TEST_F(RBTtest, testRemoveCase4)
{
	std::vector<treeNode<int>*> vec, vecVerify;
	treeNode<int>* node;
	std::string dataStr;

	// verify key data color
	int key[] = {36,16,41,4,24,39,52,3,9,22,27,48,55,1,7,10,51};
	char data[] = {'L','G','N','C','J','M','R','B','E','I','K','P','S','A','D','F','Q'};
	bool color[] = {1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,1,0};

	tree->remove(19); tree->remove(45);
	vec = tree->level_order_structure(); //testing of tree node vector
	for (int i = 0; i < sizeof(key)/sizeof(*key); i++){
		dataStr=data[i];
		node = new treeNode<int>(nodeData<int>(key[i], dataStr),0,0,color[i]);// answer of tree node vector
		vecVerify.push_back(node);
	}

	int cnt=0;
	std::vector<treeNode<int>*>::iterator itrVerify = vecVerify.begin();
	for (std::vector<treeNode<int>*>::iterator itr = vec.begin(); itr != vec.end(); itr++, itrVerify++, cnt++){
		ASSERT_EQ(**itrVerify, **itr)<<"vec["<<++cnt<<"] is wrong";
	}
}

TEST_F(RBTtest, testRemoveCase1to4)
{
	std::vector<treeNode<int>*> vec, vecVerify;
	treeNode<int>* node;
	std::string dataStr;

	// verify key data color
	int key[] = {36,16,52,4,24,48,55,3,9,22,27,41,51,1,7,10};
	char data[] = {'L','G','R','C','J','P','S','B','E','I','K','N','Q','A','D','F'};
	bool color[] = {1,0,1,1,1,0,1,1,0,1,1,1,1,0,1,1};

	tree->remove(19); tree->remove(45); tree->remove(39);
	vec = tree->level_order_structure(); //testing of tree node vector
	for (int i = 0; i < sizeof(key)/sizeof(*key); i++){
		dataStr=data[i];
		node = new treeNode<int>(nodeData<int>(key[i], dataStr),0,0,color[i]);// answer of tree node vector
		vecVerify.push_back(node);
	}

	int cnt=0;
	std::vector<treeNode<int>*>::iterator itrVerify = vecVerify.begin();
	for (std::vector<treeNode<int>*>::iterator itr = vec.begin(); itr != vec.end(); itr++, itrVerify++, cnt++){
		ASSERT_EQ(**itrVerify, **itr)<<"vec["<<++cnt<<"] is wrong";
	}
}

TEST_F(RBTtest, testRemoveCase2)
{
	std::vector<treeNode<int>*> vec, vecVerify;
	treeNode<int>* node;
	std::string dataStr;

	// verify key data color
	int key[] = {36,16,52,4,24,48,55,3,9,22,27,41,51,1,10};
	char data[] = {'L','G','R','C','J','P','S','B','E','I','K','N','Q','A','F'};
	bool color[] = {1,0,1,1,1,0,1,1,1,1,1,1,1,0,0};

	tree->remove(19); tree->remove(45); tree->remove(39); tree->remove(7);
	vec = tree->level_order_structure(); //testing of tree node vector
	for (int i = 0; i < sizeof(key)/sizeof(*key); i++){
		dataStr=data[i];
		node = new treeNode<int>(nodeData<int>(key[i], dataStr),0,0,color[i]);// answer of tree node vector
		vecVerify.push_back(node);
	}

	int cnt=0;
	std::vector<treeNode<int>*>::iterator itrVerify = vecVerify.begin();
	for (std::vector<treeNode<int>*>::iterator itr = vec.begin(); itr != vec.end(); itr++, itrVerify++, cnt++){
		ASSERT_EQ(**itrVerify, **itr)<<"vec["<<++cnt<<"] is wrong";
	}
}

TEST_F(RBTtest, testRemoveCase0)
{
	std::vector<treeNode<int>*> vec, vecVerify;
	treeNode<int>* node;
	std::string dataStr;

	// verify key data color
	int key[] = {36,16,52,4,24,48,55,1,9,22,27,41,51,10};
	char data[] = {'L','G','R','C','J','P','S','A','E','I','K','N','Q','F'};
	bool color[] = {1,0,1,1,1,0,1,1,1,1,1,1,1,0};

	tree->remove(19); tree->remove(45); tree->remove(39); tree->remove(7); tree->remove(3);
	vec = tree->level_order_structure(); //testing of tree node vector
	for (int i = 0; i < sizeof(key)/sizeof(*key); i++){
		dataStr=data[i];
		node = new treeNode<int>(nodeData<int>(key[i], dataStr),0,0,color[i]);// answer of tree node vector
		vecVerify.push_back(node);
	}

	int cnt=0;
	std::vector<treeNode<int>*>::iterator itrVerify = vecVerify.begin();
	for (std::vector<treeNode<int>*>::iterator itr = vec.begin(); itr != vec.end(); itr++, itrVerify++, cnt++){
		ASSERT_EQ(**itrVerify, **itr)<<"vec["<<++cnt<<"] is wrong";
	}
}
