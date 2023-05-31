//Дано число N и N строк.Каждая строка содержащит команду добавления или удаления натуральных чисел, 
//а также запрос на получение k - ой порядковой статистики.Команда добавления числа A задается положительным числом A,
//команда удаления числа A задается отрицательным числом “ - A”.Запрос на получение k - ой порядковой статистики задается числом k.
//Требуемая скорость выполнения запроса - O(log n).

#include <iostream>
#include <stack>
#include <cmath>

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
	AVLTree(Compare cmp = IsLessCompareDefault<T>()): _root(nullptr), _cmp(cmp) { }

	AVLTree(const AVLTree&) = delete;

	AVLTree(AVLTree&&) = delete;

	~AVLTree();

	void Insert(T key);

	void Remove(T key);

	size_t KStatistics(int pos);

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

	size_t kstatistics(Node* node, int pos);

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

	Node* insert(Node* node, T key)
	{
		if (node == nullptr)
			return new Node(key);
		if (_cmp(key, node->Key))
			node->Left = insert(node->Left, key);
		else
			node->Right = insert(node->Right, key);
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

	Node* remove(Node* node, T key)
	{
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
size_t AVLTree<T, Compare>::KStatistics(int pos)
{
	if (_root == nullptr)
		return 0;
	return kstatistics(_root, pos);
}

template<class T, class Compare>
size_t AVLTree<T, Compare>::kstatistics(Node* node, int pos)
{
	int leftNodeCount = Count(node->Left);
	if (leftNodeCount == pos)
		return node->Key;
	else if (leftNodeCount - 1 < pos)
		return kstatistics(node->Right, pos - leftNodeCount - 1);
	else if (leftNodeCount - 1 >= pos)
		return kstatistics(node->Left, pos);
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
void AVLTree<T, Compare>::Insert(T key)
{
	if (_root == nullptr) {
		_root = new Node(key);
		return;
	}
	_root = insert(_root, key);
}

template<class T, class Compare>
void AVLTree<T, Compare>::Remove(T key)
{
	if (_root == nullptr)
		return;
	_root = remove(_root, key);
}

int main()
{
	IsLessCompareDefault<int> cmp;
	AVLTree<int> tree(cmp); 
	size_t n;
	std::cin >> n;
	for (size_t i = 0; i < n; ++i) {
		int number, pos;
		std::cin >> number >> pos;
		if (number >= 0)
			tree.Insert(number);
		else
			tree.Remove(abs(number));
		std::cout << tree.KStatistics(pos) << std::endl;
	}
}