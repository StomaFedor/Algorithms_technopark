//Постройте B - дерево минимального порядка t и выведите его по слоям.
//В качестве ключа используются числа, лежащие в диапазоне 0..232 - 1
//B - дерево должно быть реализовано в виде шаблонного класса.
//Решение должно поддерживать передачу функции сравнения снаружи.


#include <iostream>
#include <vector>
#include <queue>

template<class T>
class IsLessCompareDefault
{
public:
	bool operator()(const T& left, const T& right) const { return left < right; };
};

template<class T, class Compare = IsLessCompareDefault<T>>
class BTree {
public:
	BTree(size_t t, Compare cmp = IsLessCompareDefault<T>()) : _root(nullptr), _t(t), _cmp(cmp) {}

	BTree(const BTree&) = delete;

	~BTree() { delete _root; }

	void Insert(const T& key);

	void LevelTraversal(void (*visit)(const T& key)) const;

private:
	struct Node {
		bool IsLeaf;

		std::vector<T> Keys;

		std::vector<Node*> Children;

		Node(bool isLeaf) : IsLeaf(isLeaf) {}

		~Node() { for (Node* child : Children) { delete child; } }
	};

	Node* _root;

	size_t _t;

	Compare _cmp;

	bool isNodeFull(Node* node) const { return node->Keys.size() == 2 * _t - 1; }

	void insertNonFull(Node* node, const T& key);

	void splitChild(Node* node, size_t pos);
};

template<class T, class Compare>
void BTree<T, Compare>::LevelTraversal(void (*visit)(const T& key)) const
{
	Node* node;
	std::queue<Node*> queue;
	queue.push(_root);
	while (!queue.empty()) {
		std::queue<Node*> newQueue;
		while (!queue.empty()) {
			node = queue.front();
			queue.pop();
			for (size_t i = 0; i < node->Keys.size(); ++i)
				visit(node->Keys[i]);
			for (size_t i = 0; i < node->Children.size(); ++i)
				newQueue.push(node->Children[i]);
		}
		std::cout << std::endl;
		queue = newQueue;
	}
}

template<class T, class Compare>
void BTree<T, Compare>::Insert(const T& key)
{
	if (_root == nullptr) {
		_root = new Node(true);
		_root->Keys.push_back(key);
		return;
	}

	if (isNodeFull(_root)) {
		Node* newRoot = new Node(false);
		newRoot->Children.push_back(_root);
		_root = newRoot;
		splitChild(_root, 0);
	}

	insertNonFull(_root, key);
}

template<class T, class Compare>
void BTree<T, Compare>::insertNonFull(Node* node, const T& key)
{
	if (node->IsLeaf) {
		node->Keys.resize(node->Keys.size() + 1);
		size_t pos = node->Keys.size() - 2;
		while (pos >= 0 && _cmp(key, node->Keys[pos])) {
			node->Keys[pos + 1] = node->Keys[pos];
			--pos;
		}
		node->Keys[pos + 1] = key;
	}
	else {
		size_t pos = node->Keys.size() - 1;
		while (pos >= 0 && _cmp(key, node->Keys[pos])) {
			--pos;
		}
		if (isNodeFull(node->Children[pos + 1])) {
			splitChild(node, pos + 1);
			if (key > node->Keys[pos + 1]) {
				++pos;
			}
		}
		insertNonFull(node->Children[pos + 1], key);
	}
}

template<class T, class Compare>
void BTree<T, Compare>::splitChild(Node* node, size_t pos)
{
	Node* childNode = node->Children[pos];
	Node* newNode = new Node(childNode->IsLeaf);
	T key = childNode->Keys[_t - 1];
	for (size_t i = _t; i < childNode->Keys.size(); ++i)
		newNode->Keys.push_back(childNode->Keys[i]);
	childNode->Keys.resize(_t - 1);
	if (!childNode->IsLeaf) {
		for (size_t i = _t; i < childNode->Children.size(); ++i)
			newNode->Children.push_back(childNode->Children[i]);
		childNode->Children.resize(_t);
	}
	node->Keys.insert(node->Keys.cbegin() + pos, key);
	node->Children.insert(node->Children.cbegin() + pos + 1, newNode);
}

int main()
{
	int t;
	std::cin >> t;
	BTree<int> tree(t);
	int tmp;
	while (std::cin >> tmp)
		tree.Insert(tmp);
	tree.LevelTraversal([](const int& key) { std::cout << key << " "; });
	return 0;
}