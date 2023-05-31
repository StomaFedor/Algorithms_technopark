//Дано число N < 106 и последовательность целых чисел из[-231..231] длиной N.
//Требуется построить бинарное дерево, заданное наивным порядком вставки.
//Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, 
//то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
//Требования: Рекурсия запрещена.Решение должно поддерживать передачу функции сравнения снаружи.
//2_2.Выведите элементы в порядке pre - order(сверху вниз).

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

	void DfsPreOrder(void (*visit)(const T& key)) const;

private:
	struct Node {
		Node* Left;

		Node* Right;
		
		T Key;
		
		Node(const T& key) : Left(nullptr), Right(nullptr), Key(key) {}
	};

	Node* _root;

	Compare _cmp;
};

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
	int n;
	std::cin >> n;
	BinaryTree<int> tree;
	for (int i = 0; i < n; ++i) {
		int tmp;
		std::cin >> tmp;
		tree.Add(tmp);
	}
	tree.DfsPreOrder([](const int& key) { std::cout << key << " "; });
}