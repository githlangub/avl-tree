#include "avl-tree.hpp"


AvlTree::AvlTree(void)
	: pRoot(nullptr)
{
}


AvlTree::AvlTree(const AvlTree& obj)
	: pRoot()
{
}


AvlTree::AvlTree(AvlTree&& obj)
	: pRoot(obj.pRoot)
{
}


NodePtr AvlTree::copy(AvlTree::NodePtr pObj)
{
	if(pObj == nullptr)
	{
		return nullptr;
	}

	return;
}


AvlTree::Node::Node(int key, int value, Node* pLeft, Node* pRight)
	: key(key)
	, value(value)
	, pLeft(pLeft)
	, pRight(pRight)
{
}

