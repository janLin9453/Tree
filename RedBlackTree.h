#include <queue>
#include <string>

#define RED 0
#define BLACK 1

//template <class T> class RBTfriend<T>;

template<typename T>
struct nodeData { 
	T _key;
	std::string _data; 

    //Initial red_black_tree node as NIL node;
	nodeData(T key = 0, std::string data = "NIL"): _key(key), _data(data){};

    //nodeData(nodeData<T> dict): _key(dict._key), _data(dict._datadata){};
    void assign(nodeData dst);
}; 

template<typename T>
void nodeData<T>::assign(nodeData dst)
{
	this->_key = dst._key ;
	this->_data = dst._data;
}

template <class T>
class treeNode
{
private:
	struct nodeData<T> _nodeData;
	treeNode<T>* _parent;
	treeNode<T>* _leftChild;
	treeNode<T>* _rightChild;
	bool _color; // default:0 is red, 1 is black

public:

	treeNode():  _nodeData(0), _parent(0), _leftChild(0), _rightChild(0){};
	treeNode(nodeData<T> data,treeNode<T>* parrent = 0, treeNode<T>* nil = 0,  bool color = RED):  _nodeData(data), _parent(parrent), _leftChild(nil), _rightChild(nil), _color(color){};
	
	template <class T> friend bool operator==(const treeNode<T> &in1, const treeNode<T> &in2);
	template <class T> friend std::ostream& operator<<(std::ostream& os, const treeNode<T>& dt);
	template <class T> friend class RedBlackTree;
};

template <class T>
class RedBlackTree
{
private:
	treeNode<T>* _root;
	treeNode<T>* _NIL_node;

	treeNode<T>* _search(T key);
	treeNode<T>*& _leftMostNode(treeNode<T>* &node);
	treeNode<T>*& _inorder_successor(treeNode<T>*& node);
	void _left_rotation(treeNode<T>* axis);
	void _right_rotation(treeNode<T>* axis);
	void _insert_correction(treeNode<T>* node);
	void _remove_correction(treeNode<T>* node);

public:
	RedBlackTree(): _root(0){
		_NIL_node = new treeNode<T>();
		_NIL_node->_color = BLACK;
	};

	//treeNode<T>* _search(T key);
	void level_order_constructor(char *key, char *data, char *color);
	//std::string level_order_structure();
	std::vector<treeNode<T>*> level_order_structure();
	void insert(nodeData<T> dict);
	void remove(T key);

	void RBT_print();

	template <class T>
	friend class RBTfriend;
};

template <class T>
void RedBlackTree<T>::insert(nodeData<T> dict)
{
	treeNode<T>* parent = _root;
	treeNode<T>* current = _root;

	while (current != NULL && current != _NIL_node){ //find proper child position which is NIL
		parent = current;
		if (dict._key < current->_nodeData._key){ // insert data by inorder way(LVR) from small to large
			current = current->_leftChild;
		}
		else{
			current = current->_rightChild;
		}
	}

	treeNode<T>* newNode = new treeNode<T>(dict, _NIL_node, _NIL_node);
	//tree is empty
	if (parent == NULL){
		newNode->_color = BLACK; // default is red
		_root = newNode;
		return;
	}
	// newNode's key is smaller than parent's
	else if (dict._key < parent->_nodeData._key){
		parent->_leftChild = newNode;
		newNode->_parent = parent;
	}
	// newNode's key is larger than parent's
	else{	
		parent->_rightChild = newNode;
		newNode->_parent = parent;
	}

	// If color of newNode and it's parent are both red, which violate against RBT rules. 
	_insert_correction(newNode);
	
	//delete current; different from BST: RBT don't have NULL anymore, we shouldn't delete current node NIL
}

template <class T>
void RedBlackTree<T>::_insert_correction(treeNode<T>* current)
{
	// If color of newNode and it's parent are both red, which violate against RBT rules. 
	// Need to fix it: There are three kind of cases
	while (current->_parent->_color == RED){
		if (current->_parent == current->_parent->_parent->_leftChild){
			// insert node is parent's leftChild node
			treeNode<T>* uncle = current->_parent->_parent->_rightChild;

			// case 1: uncle is red
			if (uncle->_color == RED){ 
				uncle->_color = BLACK;						// uncle change to BLACK
				current->_parent->_color = BLACK;           // parent change to BLACK
				current->_parent->_parent->_color = RED;    // grandparent change to RED
				current = current->_parent->_parent;        // Move curent to grandparent: Becaue of changing grandparent to RED, program need to check if Great-grandparent is also RED 
			}
			else{ // case 2 & 3: uncle is Black
				// case 2: current depart from uncle, convert case 2 to case 3
				//if parent, uncle is grandparent's left child, right child. Depart from uncle means: current is parent's left child
				if (current == current->_parent->_rightChild){ 
					current = current->_parent;
					_left_rotation(current);
				}
				// case 3: //current is near uncle
				current->_parent->_color = BLACK;
				current->_parent->_parent->_color = 0;
				_right_rotation(current->_parent->_parent);
			}
		}
		else{// Similar to former part: only _left* and _right* is reversed
			// insert node is parent's leftChild node
			treeNode<T>* uncle = current->_parent->_leftChild;

			// case 1
			if (uncle->_color == 0){ 
				uncle->_color = 1;
				current->_parent->_color = 1;
				current->_parent->_parent->_color = 0;
				current = current->_parent->_parent;
			}
			// case 2 & 3
			else{
				// case 2
				if (current == current->_parent->_leftChild){ // case 2
					current = current->_parent;
					_right_rotation(current);
				}
				// case 3
				current->_parent->_color = 1;
				current->_parent->_parent->_color = 0;
				_left_rotation(current->_parent->_parent);
			}
		}
	}
	// prevnet program change _root's color, _root always be BLACK
	_root->_color = BLACK;
}	

template <class T>
treeNode<T>* RedBlackTree<T>::_search(T key)
{
	treeNode<T>* current = _root;
	while (current != _NIL_node && current->_nodeData._key != key){
		if (key < current->_nodeData._key){
			current = current->_leftChild;
		}
		else{	
			current = current->_rightChild;
		}
	}

	// This key is not in the tree
	if (current == _root && current->_nodeData._key != key){ return _NIL_node;}
	return current;
}

template <class T>
void RedBlackTree<T>::remove(T key)
{
	treeNode<T>* deleteNode = _search(key);
	treeNode<T>* child = 0;
	if (deleteNode == _NIL_node){ return;}// cna't find the node we want to delete

	// case 1: deleteNode has two child, convert case 1 to case 2 or case 3
	if (deleteNode->_leftChild != _NIL_node && deleteNode->_rightChild != _NIL_node){
		treeNode<T>* successor = _inorder_successor(deleteNode);
		deleteNode->_nodeData.assign(successor->_nodeData);      // assign successor's data to deleteNode
		deleteNode = successor;							         // move deleteNode to successor
	}															 // IMPORTANT: if deleteNode have two child, it's successor can only has 0 or 2 child (inorder)

	// case 2 and 3: deleteNode has zero or one child
	if (deleteNode->_leftChild != _NIL_node){
		child = deleteNode->_leftChild;
	}
	else{
		child = deleteNode->_rightChild;
	}

	//// connect child to deleteNode's parent ////
	if (child != NULL){
		child->_parent = deleteNode->_parent;
	}

	//// connect deleteNode's parent to child ////
	// deleteNode is root
	if (deleteNode == _root){
		_root = child;
	}
	// deleteNode is left child
	else if (deleteNode == deleteNode->_parent->_leftChild){
		deleteNode->_parent->_leftChild = child;
	}
	// deleteNode is right child
	else{	
		deleteNode->_parent->_rightChild = child;
	}

	////Fix it: If deleteNode is Black, which violate against RBT rules(each path has same ammount of black node) ////
	if (deleteNode->_color == BLACK && child->_color == BLACK && child != _root){
		_remove_correction(child);
	}
	else{// child is red OR child is root, simply change child's color to black
		child->_color = BLACK;
	}
	delete deleteNode;
	deleteNode = 0;
}

template <class T>
void RedBlackTree<T>::_remove_correction(treeNode<T>* current)
{
	// There is 4 cases
	while (current != _root && current->_color == 1){// current is Black or root
		if (current == current->_parent->_leftChild){
		// current is parent's left child
		treeNode<T>* sibling = current->_parent->_rightChild;
			// case 1: sbiling is red, convert case 1 into case 2,3,4
			if (sibling->_color == RED){
				sibling->_color = BLACK;
				sibling->_parent->_color = RED;
				_left_rotation(sibling->_parent);
				sibling = current->_parent->_rightChild;
			}
			// case 2,3,4 sibling is black, parent's color don't care
			// case 2: both sibling's child are BLACK
			if (sibling->_rightChild->_color == BLACK && sibling->_leftChild->_color == BLACK){
				sibling->_color = RED;
				current = current->_parent;
				// if currenet parent's color is red only need to do correction one time
				if (current->_color == RED){ 
					current->_color = BLACK;
					current = _root;
				}
			}
			// case 3, 4
			else{	
				// case 3: sibling's child which near current child is RED, depart from current is BLACK
				if (sibling->_rightChild->_color == BLACK){
					sibling->_color = RED;
					sibling->_leftChild->_color = BLACK;
					_right_rotation(sibling);
					//sibling = sibling->_parent;
					sibling = current->_parent->_rightChild;
				}
				// after csae 3 must be case 4
				// case 4: sibling's child which depart from current is RED
				sibling->_color = current->_parent->_color;
				sibling->_rightChild->_color = BLACK;
				current->_parent->_color = BLACK;
				_left_rotation(current->_parent);
				current = _root;// break the loop
			}
		}
		else{ // Similar to former part: only _left* and _right* is reversed
			treeNode<T>* sibling = current->_parent->_leftChild;
			//case 1: sbiling is red
			if (sibling->_color == RED){
				sibling->_color = BLACK;
				sibling->_parent->_color = RED;
				_left_rotation(current->_parent);
				sibling = current->_parent->_leftChild;
			}
			//case 2,3,4 sibling is black
			//case 2
			if (sibling->_leftChild->_color == BLACK && sibling->_rightChild->_color == BLACK){
				sibling->_color = RED;
				current = current->_parent;
				if (current->_color == RED){ //currenet -> parent's color is red only need to do correction one time
					current->_color = BLACK;
					current = _root;
				}
			}
			//case 3, 4
			else{	
				//case 3 
				if (sibling->_leftChild->_color == BLACK){
					sibling->_color = RED;
					sibling->_rightChild->_color = BLACK;
					_left_rotation(sibling);
					//sibling = sibling->_parent;
					sibling = current->_parent->_leftChild;
				}

				//case 4
				sibling->_color = current->_parent->_color;
				sibling->_leftChild->_color = BLACK;
				current->_parent->_color = BLACK;
				_right_rotation(current->_parent);
				current = _root;// break the loop
			}
		}
	}
	// prevnet program change _root's color, _root should always be BLACK
	_root->_color = BLACK;
}	

template <class T>
void RedBlackTree<T>::RBT_print()
{
	/*treeNode<T>* current = node;
	if (current != _NIL_node){
		RBT_traversal(current->_leftChild);
		std::cout<<current->_nodeData._data<<"("<<current->_nodeData._key<<") ";
		RBT_traversal(current->_rightChild);
	}*/
	std::cout<<"BST traversal: ";
	if (_root == _NIL_node){
		std::cout<<"tree is empty"<<std::endl;
		return;
	}
	treeNode<T>* current = _leftMostNode(_root);
	while (current != _NIL_node){
		std::string color = "";
		if (current->_color == 0){
			color = "RED";
		}
		else{	
			color = "BLACK";
		}
		std::cout<<current->_nodeData._key<<"("<<color<<") ";
		//std::cout<<current->_nodeData._data<<"("<<current->_nodeData._key<<")-"<<color<<" ";
		current = _inorder_successor(current);
	}
	std::cout<<std::endl;
}

template <class T>
treeNode<T>*& RedBlackTree<T>::_leftMostNode(treeNode<T>*& node)
{ 
	treeNode<T>* current = node;
	while (current->_leftChild != _NIL_node){
		current = current->_leftChild;
	}
	return current;
}

template <class T>
treeNode<T>*& RedBlackTree<T>::_inorder_successor(treeNode<T>*& node)
{
	if (node->_rightChild != _NIL_node){
		return _leftMostNode(node->_rightChild);
	}

	treeNode<T>* current = node;
	// go to LVR's V or R 
	while (current->_parent != _NIL_node && current == current->_parent->_rightChild){
		current = current->_parent;
	}
	return current->_parent;
}

template <class T>
void RedBlackTree<T>::_left_rotation(treeNode<T>* axis)
{
	treeNode<T>* newAxis = axis->_rightChild;

	// link axis and newAxis's left child
	axis->_rightChild = newAxis->_leftChild;
	if (newAxis->_leftChild != _NIL_node){
		newAxis->_leftChild->_parent = axis;
	}

	// link axis->parent and newAxis 
	newAxis->_parent = axis->_parent;
	if (axis->_parent == _NIL_node){
		_root = newAxis;
	}
	else if (axis == axis->_parent->_leftChild){
		axis->_parent->_leftChild = newAxis;
	}
	else{
		axis->_parent->_rightChild = newAxis;
	}

	// make axis as newAxis's left child
	newAxis->_leftChild = axis;
	axis->_parent = newAxis;
}

template <class T>
void RedBlackTree<T>::_right_rotation(treeNode<T>* axis)
{
	// similer to _left_rotation, smiply swap _left* nad _right*
	treeNode<T>* newAxis = axis->_leftChild;

	axis->_leftChild = newAxis->_rightChild;
	if (newAxis->_rightChild != _NIL_node){
		newAxis->_rightChild->_parent = axis;
	}

	newAxis->_parent = axis->_parent;
	if (axis->_parent == _NIL_node){
		_root = newAxis;
	}
	else if (axis == axis->_parent->_leftChild){
		axis->_parent->_leftChild = newAxis;
	}
	else{
		axis->_parent->_rightChild = newAxis;
	}

	newAxis->_rightChild = axis;
	axis->_parent = newAxis;
}

template <class T>
void RedBlackTree<T>::level_order_constructor(char *key, char *data, char *color)
{
	std::queue<treeNode<T>*> q; 
	std::stringstream ssKey, ssData, ssColor;
	ssKey << key;
	ssData << data;
	ssColor <<color;

	_root = new treeNode<T>();
	_root->_parent = _NIL_node;
	ssKey >> _root->_nodeData._key;
	ssData >> _root->_nodeData._data;
	ssColor >> _root->_color;

	treeNode<T>* current = _root;
	treeNode<T>* newNode;
	nodeData<T> node;
	while (ssData >> node._data){
		if (node._data != "x"){
			ssKey >> node._key;
			newNode = new treeNode<T>(node, current, _NIL_node);
			ssColor >> newNode->_color;			
			current->_leftChild = newNode;
			q.push(newNode);
		}

		if (!(ssData >> node._data)){
			break;
		}

		if (node._data != "x"){
			ssKey >> node._key;
			newNode = new treeNode<T>(node, current, _NIL_node);
			ssColor >> newNode->_color;			
			current->_rightChild = newNode;
			q.push(newNode);
		}
		
		current = q.front();
		q.pop();
	}
}

template <class T>
//std::string RedBlackTree<T>::level_order_structure()
std::vector<treeNode<T>*> RedBlackTree<T>::level_order_structure()
{
	//std::string structure;
	std::queue<treeNode<T>*> q; 
	std::vector<treeNode<T>*> v;
	treeNode<T>* current = _root;

	q.push(current);
	while (!q.empty()){
		current = q.front();
		//structure = structure + current->_nodeData._data + " ";
		v.push_back(current);
		if (current->_leftChild != _NIL_node){
			q.push(current->_leftChild);
		}

		if (current->_rightChild != _NIL_node){
			q.push(current->_rightChild);
		}
		q.pop();
	}
	return v;
}

void RBT_test()
{
	RedBlackTree<int> t;
	// remove node in tree which only has root 
	t.insert(nodeData<int>(513, "a"));
	t.remove(513);

	// remove node in tree which has two node 
	t.insert(nodeData<int>(513, "a"));
	t.insert(nodeData<int>(8, "b"));
	t.remove(513);
	t.RBT_print();

	char key[] = "36 16 41 4 22 39 48 3 9 19 27 45 52 1 7 10 24 51 55",
		 data[] = "L G N C I M P B E H K x x O R A x D F x x J x x x Q S",
		 color[] = "1 0 1 1 1 1 0 1 0 1 1 1 1 0 1 1 0 0 0";

	t.level_order_constructor(key, data, color);
	//std::string s = t.level_order_structure();
	std::vector<treeNode<int>*> vec = t.level_order_structure();
	t.RBT_print();
	/*
	t.remove(19);// case 3 -> case 4
	t.remove(45);// case 4
	//TODO
	t.remove(36);// case 1 -> case 4
	t.remove(7); // case 2
	t.remove(3); // case 0
	t.RBT_print();
	*/
}