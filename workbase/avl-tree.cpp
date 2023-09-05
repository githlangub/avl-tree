#include "avl-tree.hpp"	/* AvlTree */
#include "type.hpp"	/* UInt8, Int8 */

#include <algorithm>	/* std::max */
#include <functional>	/* std::function */


AvlTree::AvlTree(void)
	: pRoot(nullptr)
{
}


AvlTree::AvlTree(AvlTree const& obj)
	: pRoot(this->copy(obj.pRoot))
{
}


AvlTree::AvlTree(AvlTree&& obj)
	: pRoot(obj.pRoot)
{
	obj.pRoot = nullptr;
}


AvlTree::~AvlTree()
{
	this->destroy(this->pRoot);
	this->pRoot = nullptr;
}


void AvlTree::put(KeyType const& key, NullableValueType const& value)
{
	auto&& route = this->retrieve(key);	

	if(*route.top() != nullptr)
	{
		if(value.has_value())
		{
			this->update(*value, route);
		}
		else
		{
			this->de1ete(route);
		}
	}
	else
	{
		if(value.has_value())
		{
			this->insert(key, *value, route);
		}
	}
}


typename AvlTree::NullableValueType AvlTree::get(KeyType const& key)
{
	auto&& route = this->retrieve(key);
	
	if(*route.top() == nullptr)
	{
		return {};
	}
		
	return (*route.top())->entry.value;
}


void AvlTree::destroy(NodePtr p)
{
	if(p == nullptr)
	{
		return;
	}

	this->destroy(p->pLeft);
	this->destroy(p->pRight);
	delete p;
}


typename AvlTree::NodePtr AvlTree::copy(NodePtr p) const
{
	return	p != nullptr ?
		new Node(
			p->entry,
			p->height,
			this->copy(p->pLeft),
			this->copy(p->pRight)
		) :
		nullptr
	;
}


void AvlTree::insert(
	KeyType const& key,
	ValueType const& value,
	RouteType& route
)
{
	//std::assert(!s.empty() && s.top() != nullptr && *(s.top()) == nullptr);

	// 路径栈的栈顶表示插入位置（要修改的那个节点指针），
	// 该插入位置一定为空，
	// 在该位置新建节点并插入键值对。
	*(route.top()) = new Node(
		Entry(key, value),
		0,
		nullptr,
		nullptr
	);

	// 重新平衡AVL树。
	this->rebalance(route);
}


void AvlTree::de1ete(RouteType& route)
{
	//std::assert(!route.empty() && route.top() != nullptr && *(route.top()) != nullptr);

	// 若待删除节点的左子树存在，
	// 则使用其中序遍历的左邻节点值补位，
	// 并替代删除其左邻节点；
	// 若待删除节点的右子树存在，
	// 则使用其中序遍历的右邻节点值补位，
	// 并替代删除其右邻节点；
	// 若待删除节点为叶子节点，
	// 则直接删除该节点。
	auto pDelete = *route.top();
	if(pDelete->pLeft != nullptr)
	{
		// 待删除节点的左邻节点为其左子树的最右节点。
		for(	route.push(&(pDelete->pLeft));
			(*route.top())->pRight != nullptr;
			route.push(&((*route.top())->pRight))
		)
		{
		};
		auto pLeftSide = *route.top();

		// 使用替代节点值覆盖待删除节点值
		pDelete->entry = pLeftSide->entry;

		// 若替代节点的左子节点存在，
		// 则使用其左子节点值覆盖替代节点值，
		// 并删除其左子节点；
		// 否则直接删除替代节点。
		if(pLeftSide->pLeft != nullptr)
		{
			pLeftSide->entry = pLeftSide->pLeft->entry;

			route.push(&(pLeftSide->pLeft));
		}
	}
	else if(pDelete->pRight != nullptr)
	{
		// 待删除节点的右邻节点为其右子树的最左节点。
		for(	route.push(&(pDelete->pRight));
			(*route.top())->pLeft != nullptr;
			route.push(&((*route.top())->pLeft))
		)
		{
		};
		auto pRightSide = *route.top();

		// 使用替代节点值覆盖待删除节点值
		pDelete->entry = pRightSide->entry;

		// 若替代节点的右子节点存在，
		// 则使用其右子节点值覆盖替代节点值，
		// 并删除其右子节点；
		// 否则直接删除替代节点。
		if(pRightSide->pRight != nullptr)
		{
			pRightSide->entry = pRightSide->pRight->entry;

			route.push(&(pRightSide->pRight));
		}
	}

	// 删除路径栈顶指向的节点并置空。
	delete *route.top();
	*route.top() = nullptr;

	// 重新平衡AVL树。
	this->rebalance(route);
}


void AvlTree::update(ValueType const& value, RouteType& route)
{
	//std::assert(!route.empty() && route.top() != nullptr && *(route.top()) != nullptr);

	(*route.top())->entry.value = value;
}


typename AvlTree::RouteType AvlTree::retrieve(KeyType const& key)
{
	RouteType route;

	for(	route.push(&(this->pRoot));
		*(route.top()) != nullptr;
	)
	{
		auto& p = *(route.top());

		if(key < p->entry.key)
		{
			route.push(&(p->pLeft));
		}
		else if(key > p->entry.key)
		{
			route.push(&(p->pRight));
		}
		else
		{
			break;
		}
	}

	return route;
}


bool AvlTree::traverse_1(
	NodePtr p,
	CallbackType const& func
)
{
	// 递归法中序遍历。

	// 空树直接返回。
	if(p == nullptr)
	{
		return true;
	}

	// 遍历左子树。
	if(!this->traverse_1(p->pLeft, func))
	{
		return false;
	}

	// 对当前节点执行操作。
	if(!func(p))
	{
		return false;
	}

	// 遍历右子树。
	if(!this->traverse_1(p->pRight, func))
	{
		return false;
	}

	return true;
}


void AvlTree::rebalance(RouteType& route)
{
	// 沿路径栈从删除位置迭代至根节点，
	// 更新节点的高度，
	// 重新平衡节点所在的AVL树。
	for(; !route.empty(); route.pop())
	{
		// 取栈顶指向的节点为当前节点。
		auto& p = *(route.top());

		// 空树直接跳过。
		if(p == nullptr)
		{
			continue;
		}

		// 更新当前节点的高度。
		p->height = 1 + std::max(
			this->heightOfNode(p->pLeft),
			this->heightOfNode(p->pRight)
		);

		// 获得当前节点及其左右子节点的平衡因子，
		// 根据平衡因子来重新平衡以当前节点为根的AVL树。
		auto bfCurr = this->balanceFactorOfNode(p);
		auto bfLeft = this->balanceFactorOfNode(p->pLeft);
		auto bfRight = this->balanceFactorOfNode(p->pRight);
		//std::assert(bfCurr >= -2 && bfCurr <= 2);
		//std::assert(bfLeft >= -1 && bfLeft <= 1);
		//std::assert(bfRight >= -1 && bfRight <= 1);

		if(bfCurr == 2)
		{
			if(bfLeft == -1)
			{
				p->pLeft = this->leftRotate(p->pLeft);
			}

			p = this->rightRotate(p);
		}
		else if(bfCurr == -2)
		{
			if(bfRight == 1)
			{
				p->pRight = this->rightRotate(p->pRight);
			}
	
			p = this->leftRotate(p);
		}
	}
}


typename AvlTree::NodePtr AvlTree::leftRotate(NodePtr p)
{
	//std::assert(p != nullptr && p->pRight != nullptr);

	// 当前节点与其右子节点对换。
	NodePtr p_ = p->pRight;
	p->pRight = p_->pLeft;
	p_->pLeft = p;

	// 重新计算高度。
	p->height = 1 + std::max(
		this->heightOfNode(p->pLeft),
		this->heightOfNode(p->pRight)
	);

	p_->height = 1 + std::max(
		this->heightOfNode(p_->pLeft),
		this->heightOfNode(p_->pRight)
	);

	// 返回左旋后的树。
	return p_;
}


typename AvlTree::NodePtr AvlTree::rightRotate(NodePtr p)
{
	//std::assert(p != nullptr && p->pLeft != nullptr);

	// 当前节点与其左子节点对换。
	NodePtr p_ = p->pLeft;
	p->pLeft = p_->pRight;
	p_->pRight = p;

	// 重新计算高度。
	p->height = 1 + std::max(
		this->heightOfNode(p->pLeft),
		this->heightOfNode(p->pRight)
	);

	p_->height = 1 + std::max(
		this->heightOfNode(p_->pLeft),
		this->heightOfNode(p_->pRight)
	);

	// 返回右旋后的树。
	return p_;
}


UInt8 AvlTree::heightOfNode(NodePtr p) const
{
	return	p == nullptr ?
		0 :
		p->height
	;
}


Int8 AvlTree::balanceFactorOfNode(NodePtr p) const
{
	return	p == nullptr ?
		0 :
		this->heightOfNode(p->pLeft) - this->heightOfNode(p->pRight)
	;
}


AvlTree::Entry::Entry(
	AvlTree::KeyType const& key,
	AvlTree::ValueType const& value
)
	: key(key)
	, value(value)
{
}


typename AvlTree::Entry& AvlTree::Entry::operator=(Entry const& obj)
{
	this->key = obj.key;
	this->value = obj.value;

	return *this;
}


AvlTree::Node::Node(
	AvlTree::Entry const& entry,
	UInt8 const& height,
	AvlTree::NodePtr const& pLeft,
	AvlTree::NodePtr const& pRight
)
	: entry(entry)
	, height(height)
	, pLeft(pLeft)
	, pRight(pRight)
{
}

