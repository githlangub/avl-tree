#ifndef __AVL_TREE_HPP__
#define __AVL_TREE_HPP__


class AvlTree
{
public:
	struct Node
	{
		Node* pLeft;
		Node* pRight;

		unsigned char BalanceFactor;

		int key;
		int value;

		Node(int, int, Node* = nullptr, Node* = nullptr);
	};

	typedef Node* NodePtr;

public:
	AvlTree(void);
	AvlTree(const AvlTree& );
	AvlTree(AvlTree&& );
	~AvlTree();

	/*void Insert();
	void Delete();
	void Update();
	void Retrieve();*/

private:
	NodePtr copy(NodePtr );

	int Set(int, int* );
	int Get(int, int* ) const;

	int leftRotate(Node* , Node* );
	int rightRotate(Node* ,Node* );

private:
	Node* pRoot;
};

#endif

