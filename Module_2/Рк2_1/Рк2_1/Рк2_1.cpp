#include <iostream>
#include <stack>
#include <cmath>
#include <string>

template<class T>
class IsLessCompareDefault
{
public:
	bool operator()(const T& left, const T& right) const { return left < right; };
};

template<class T, class Compare = IsLessCompareDefault<T>>
class AVLTree
{
public:
	AVLTree(Compare cmp = IsLessCompareDefault<T>()) : _root(nullptr), _cmp(cmp) { }

	AVLTree(const AVLTree&) = delete;

	AVLTree(AVLTree&&) = delete;

	~AVLTree();

	void Insert(const T& key);

	void Remove(const T& key);

	bool Exists(const T& key);

	T Next(const T& key);

	T Prev(const T& key);

private:
	struct Node
	{
		T Key;

		size_t Count;

		size_t Height;

		Node* Left;

		Node* Right;

		Node(int k) : Key(k), Left(nullptr), Right(nullptr), Height(1), Count(1) { }
	};

	Node* _root;

	Compare _cmp;

	size_t Height(Node* node);

	size_t Count(Node* node);

	T next(Node* node, const T& key);

	T prev(Node* node, const T& key);

	int BFactor(Node* node);

	void FixHeightAndCount(Node* node);

	Node* RotateRight(Node* node)
	{
		Node* leftNode = node->Left;
		node->Left = leftNode->Right;
		leftNode->Right = node;
		FixHeightAndCount(node);
		FixHeightAndCount(leftNode);
		return leftNode;
	}

	Node* RotateLeft(Node* node)
	{
		Node* rightNode = node->Right;
		node->Right = rightNode->Left;
		rightNode->Left = node;
		FixHeightAndCount(node);
		FixHeightAndCount(rightNode);
		return rightNode;
	}

	Node* Balance(Node* node)
	{
		FixHeightAndCount(node);
		if (BFactor(node) == 2)
		{
			if (BFactor(node->Right) < 0)
				node->Right = RotateRight(node->Right);
			return RotateLeft(node);
		}
		if (BFactor(node) == -2)
		{
			if (BFactor(node->Left) > 0)
				node->Left = RotateLeft(node->Left);
			return RotateRight(node);
		}
		return node;
	}

	bool exists(Node* node, const T& key);

	Node* insert(Node* node, const T& key)
	{
		if (node == nullptr)
			return new Node(key);
		if (_cmp(key, node->Key))
			node->Left = insert(node->Left, key);
		else if (_cmp(node->Key, key))
			node->Right = insert(node->Right, key);
		else return node;
		return Balance(node);
	}

	std::pair<Node*, Node*> FindAndRemoveMin(Node* node)
	{

		if (node->Left == nullptr) {
			return std::pair<Node*, Node*>(node, node->Right);
		}
		std::pair<Node*, Node*> p = FindAndRemoveMin(node->Left);
		node->Left = p.second;
		return std::pair<Node*, Node*>(p.first, Balance(node));
	}

	Node* remove(Node* node, const T& key)
	{
		if (node == nullptr)
			return node;
		if (_cmp(key, node->Key))
			node->Left = remove(node->Left, key);
		else if (_cmp(node->Key, key))
			node->Right = remove(node->Right, key);
		else
		{
			Node* leftNode = node->Left;
			Node* rightNode = node->Right;
			delete node;
			if (rightNode == nullptr)
				return leftNode;
			std::pair<Node*, Node*> p = FindAndRemoveMin(rightNode);
			Node* min = p.first;
			min->Right = p.second;
			min->Left = leftNode;
			return Balance(min);
		}
		return Balance(node);
	}

};

template<class T, class Compare>
T AVLTree<T, Compare>::Next(const T& key)
{
	if (_root == nullptr)
		return key;
	return next(_root, key);
}

template<class T, class Compare>
T AVLTree<T, Compare>::next(Node* node, const T& key)
{
	if (!_cmp(node->Key, key)) {
		if (node->Left == nullptr)
			return _cmp(key, node->Key) ? node->Key : key;
		if (_cmp(node->Left->Key, key))
			return node->Key;
		return next(node->Left, key);
	}
	if (node->Right == nullptr)
		return _cmp(key, node->Key) ? node->Key : key;
	return next(node->Right, key);
}

template<class T, class Compare>
T AVLTree<T, Compare>::Prev(const T& key)
{
	if (_root == nullptr)
		return key;
	return prev(_root, key);
}

template<class T, class Compare>
T AVLTree<T, Compare>::prev(Node* node, const T& key)
{
	if (!_cmp(node->Key, key)) {
		if (node->Left == nullptr)
			return _cmp(node->Key, key) ? node->Key : key;
		return prev(node->Left, key);
	}
	if (node->Right == nullptr)
		return _cmp(node->Key, key) ? node->Key : key;
	if (!_cmp(node->Right->Key, key))
		return node->Key;
	return prev(node->Right, key);
}

template<class T, class Compare>
bool AVLTree<T, Compare>::Exists(const T& key)
{
	return exists(_root, key);
}

template<class T, class Compare>
bool AVLTree<T, Compare>::exists(Node* node, const T& key)
{
	if (node == nullptr)
		return false;
	if (node->Key == key)
		return true;
	if (_cmp(key, node->Key))
		return exists(node->Left, key);
	return exists(node->Right, key);
}

template<class T, class Compare>
AVLTree<T, Compare>::~AVLTree()
{
	Node* node;
	std::stack<Node*> stack;
	stack.push(_root);
	while (!stack.empty()) {
		node = stack.top();
		stack.pop();
		if (node->Right != nullptr)
			stack.push(node->Right);
		if (node->Left != nullptr)
			stack.push(node->Left);
		delete node;
	}
}

template<class T, class Compare>
size_t AVLTree<T, Compare>::Count(Node* node)
{
	return node ? node->Count : 0;
}

template<class T, class Compare>
size_t AVLTree<T, Compare>::Height(Node* node)
{
	return node ? node->Height : 0;
}

template<class T, class Compare>
int AVLTree<T, Compare>::BFactor(Node* node)
{
	return Height(node->Right) - Height(node->Left);
}

template<class T, class Compare>
void AVLTree<T, Compare>::FixHeightAndCount(Node* node)
{
	size_t hl = Height(node->Left);
	size_t hr = Height(node->Right);
	node->Count = Count(node->Left) + Count(node->Right) + 1;
	node->Height = (hl > hr ? hl : hr) + 1;
}

template<class T, class Compare>
void AVLTree<T, Compare>::Insert(const T& key)
{
	if (_root == nullptr) {
		_root = new Node(key);
		return;
	}
	_root = insert(_root, key);
}

template<class T, class Compare>
void AVLTree<T, Compare>::Remove(const T& key)
{
	if (_root == nullptr)
		return;
	_root = remove(_root, key);
}

int main()
{
	IsLessCompareDefault<int> cmp;
	AVLTree<int> tree(cmp);
	std::string command;
	int key;
	while (std::cin >> command >> key) {
		if (command == "insert")
			tree.Insert(key);
		else if (command == "delete")
			tree.Remove(key);
		else if (command == "exists")
			std::cout << (tree.Exists(key) == true ? "true" : "false") << std::endl;
		else if (command == "next") {
			int res = tree.Next(key);
			std::cout << (res == key ? "none" : std::to_string(res)) << std::endl;
		}
		else if (command == "prev") {
			int res = tree.Prev(key);
			std::cout << (res == key ? "none" : std::to_string(res)) << std::endl;
		}
	}
}