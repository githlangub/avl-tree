#ifndef __AVL_TREE_HPP__
#define __AVL_TREE_HPP__

#include "type.hpp"	/* UInt8, Int8 */

#include <stack>	/* std::stack */
#include <functional>	/* std::function */
#include <optional>	/* std::optional */


class AvlTree
{
public:
	typedef int KeyType;
	typedef int ValueType;

	struct Entry
	{
		KeyType key;
		ValueType value;

		Entry(KeyType const& = KeyType(), ValueType const& = ValueType());
		Entry& operator=(Entry const&);
	};

	struct Node
	{
		Entry entry;

		UInt8 height;
		Node* pLeft;
		Node* pRight;

		Node(Entry const& = Entry(), UInt8 const& = 0, Node* const& = nullptr, Node* const& = nullptr);
	};

	typedef Node* NodePtr;

	typedef std::stack<NodePtr*> RouteType;
		
	typedef std::function<bool(NodePtr)> CallbackType;

	typedef std::optional<ValueType> NullableValueType;

public:
	AvlTree(void);
	AvlTree(AvlTree const&);
	AvlTree(AvlTree&&);
	~AvlTree();

	void put(
		KeyType const&,
		NullableValueType const&
	);

	NullableValueType get(
		KeyType const&
	);

protected:
public:
	void destroy(
		NodePtr
	);

	NodePtr copy(
		NodePtr
	) const;

	void insert(
		KeyType const&,
		ValueType const&,
		RouteType&
	);

	void de1ete(
		RouteType&
	);

	void update(
		ValueType const&,
		RouteType&
	);

	RouteType retrieve(
		KeyType const&
	);

	bool traverse_1(
		NodePtr,
		CallbackType const&
	);

	void rebalance(
		RouteType&
	);

	NodePtr leftRotate(
		NodePtr
	);

	NodePtr rightRotate(
		NodePtr
	);

	UInt8 heightOfNode(
		NodePtr
	) const;

	Int8 balanceFactorOfNode(
		NodePtr
	) const;

protected:
public:
	NodePtr pRoot;
};

#endif

