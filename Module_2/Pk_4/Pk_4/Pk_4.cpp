

#include <iostream>
#include <stack>

template<class T>
class IsLessCompareDefault
{
public:
	bool operator()(const T& left, const T& right) const { return left < right; };
};

template<class T, class Compare = IsLessCompareDefault<T>>
class BinaryTree {
public:
	BinaryTree(const Compare& cmp = IsLessCompareDefault<T>()) : _root(nullptr), _cmp(cmp) { }

	BinaryTree(const BinaryTree&) = delete;

	BinaryTree(BinaryTree&&) = delete;

	~BinaryTree();

	void Add(const T& key);

	bool Has(const T& key) const;

	int ShortestDistance();

	void DfsPreOrder(void (*visit)(const T& key)) const;

private:
	struct Node {
		Node* Left;

		Node* Right;

		T Key;

		Node(const T& key) : Left(nullptr), Right(nullptr), Key(key) {}
	};

	int shortestDistance(Node* node);

	Node* _root;

	Compare _cmp;
};

template<class T, class Compare>
int BinaryTree<T, Compare>::ShortestDistance()
{
	if (_root == nullptr)
		return 0;
	return shortestDistance(_root);
}

template<class T, class Compare>
int BinaryTree<T, Compare>::shortestDistance(Node* node)
{
	if (node == nullptr)
		return 0;
	return std::min(shortestDistance(node->Left), shortestDistance(node->Right)) + 1;
}

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree()
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
void BinaryTree<T, Compare>::Add(const T& key)
{
	Node* node = _root;
	Node* prevNode = _root;
	if (_root == nullptr) {
		_root = new Node(key);
		return;
	}
	while (node != nullptr) {
		prevNode = node;
		if (_cmp(key, node->Key))
			node = node->Left;
		else
			node = node->Right;
	}
	if (_cmp(key, prevNode->Key))
		prevNode->Left = new Node(key);
	else
		prevNode->Right = new Node(key);
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::Has(const T& key) const
{
	Node* node = _root;
	while (node != nullptr) {
		if (_cmp(key, node->Key))
			node = node->Left;
		else if (_cmp(node->Key, key))
			node = node->Right;
		else
			return true;
	}
	return false;
}

template<class T, class Compare>
void BinaryTree<T, Compare>::DfsPreOrder(void (*visit)(const T& key)) const
{
	Node* node;
	std::stack<Node*> stack;
	stack.push(_root);
	while (!stack.empty()) {
		node = stack.top();
		stack.pop();
		visit(node->Key);
		if (node->Right != nullptr)
			stack.push(node->Right);
		if (node->Left != nullptr)
			stack.push(node->Left);
	}
}

int main()
{
	int key;
	BinaryTree<int> tree;
	while (std::cin >> key)
		tree.Add(key);
	std::cout << tree.ShortestDistance();
}