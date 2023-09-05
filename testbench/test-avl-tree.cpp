#include "../workbase/avl-tree.hpp"

#include <iostream>
#include <random>
#include <set>


bool printAvlTreeNode(AvlTree::NodePtr p)
{
	std::cout << "p = " << p << '\n';

	if(p != nullptr)
	{
		std::cout << "*p = AvlTree::Node" << '\n';
		std::cout << "{" << '\n';
		std::cout << "\tentry = AvlTree::Entry" << '\n';
		std::cout << "\t{" << '\n';
		std::cout << "\t\tkey = " << p->entry.key << '\n';
		std::cout << "\t\tvalue = " << p->entry.value << '\n';
		std::cout << "\t}" << '\n';
		std::cout << "\theight = " << p->height << '\n';
		std::cout << "\tpLeft = " << p->pLeft << '\n';
		std::cout << "\tpRight = " << p->pRight << '\n';
		std::cout << "}" << '\n';
	}

	return true;
}


bool printAvlTreeNodeEntryKey(AvlTree::NodePtr p)
{
	//std::assert(p != nullptr);

	std::cout << p->entry.key << ' ';

	return true;
}


void testInsert(std::size_t count)
{
	std::set<int> s;
	AvlTree t;

	std::random_device rd;
	std::default_random_engine e(rd());	
	std::uniform_int_distribution<int> u(1, count * count);

	for(int i = 0; i < count; ++i)
	{
		int n = u(e);
		s.insert(n);
		t.put(n, n * n);
	}

	for(auto& e : s)
	{
		std::cout << e << ' ';
	}
	std::cout << std::endl;

	t.traverse_1(t.pRoot, printAvlTreeNodeEntryKey);
	std::cout << std::endl;
}


int main(int argc, char** argv)
{
	AvlTree t1;

	t1.put(10, 1024);
	for(int i = 1; i <= 1000; ++i)
	{
		auto&& v = t1.get(i);

	       	if(v != std::nullopt)
		{	
			std::cout << i << ", ";
			std::cout << *v << '\n';
		}
	}
	std::cout << std::endl;

	testInsert(100);

	return 0;
}
