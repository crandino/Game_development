//=================================
// include guard
#ifndef __DLIST_H__
#define __DLIST_H__
//=================================
// forward declared dependencies
//=================================
// included dependencies
#include <stdio.h>
//=================================
// the actual class

template <class TYPE>
struct doubleNode {
	TYPE data;
	doubleNode<TYPE>* next;
	doubleNode<TYPE>* previous;

	inline doubleNode(const TYPE &_data)
	{
		data = _data;
		next = previous = NULL;
	}

	~doubleNode()
	{ }
};

template <class TYPE>
class DList {

private:

	doubleNode<TYPE>* start;
	doubleNode<TYPE>* end;
	unsigned int size;

public:

	inline DList()
	{
		start = end = NULL;
		size = 0;
	}

	~DList()
	{
		clear();
	}

	unsigned int count() const
	{
		return size;
	}

	unsigned int add(const TYPE &new_data)
	{
		doubleNode<TYPE> *new_node;
		new_node = new doubleNode<TYPE>(new_data);
		new_node->data = new_data;

		if (start != NULL)
		{
			new_node->previous = end;
			end->next = new_node;
			end = new_node;
		}
		else
		{
			start = end = new_node;
		}
			
		return (++size);
	
	}

	doubleNode<TYPE>* getNodeAtPos(unsigned int _pos) const
	{
		// Node 1 is zero, node 2 is one, etc.
		if (start != NULL && _pos < count())
		{
			unsigned int pos_counter = 0;
			doubleNode<TYPE>* tmp = start;
			
			while (pos_counter != _pos)
			{
				tmp = tmp->next;
				pos_counter++;
			}
			return tmp;
		}
		return NULL;
	}

	/**
	* Find by index (by Ricard)
	*/
	bool at(unsigned int index, TYPE &new_data) const
	{
		bool ret = false;
		unsigned int i = 0;
		doubleNode<TYPE>*   searching_node = start;

		for (unsigned int i = 0; i < index - 1 && searching_node != NULL; ++i)
			searching_node = searching_node->next;

		if (searching_node != NULL)
		{
			ret = true;
			new_data = searching_node->data;
		}

		return ret;
	}

	bool del(doubleNode<TYPE> *node_to_delete)
	{
		if (start != NULL)
		{
			if (node_to_delete->previous != NULL)
			{
				node_to_delete->previous->next = node_to_delete->next;
				if (node_to_delete->next != NULL)
					node_to_delete->next->previous = node_to_delete->previous;
				else
					end = node_to_delete->previous;
			}
			else
			{
				if (node_to_delete->next != NULL)
				{
					start = node_to_delete->next;
					node_to_delete->next->previous = NULL;
				}
				else
					start = end = NULL;
			}

			delete node_to_delete;
			--size;
			return true;
		}
		return false;
	}

	void clear()
	{

		doubleNode<TYPE> *item = start;
		doubleNode<TYPE> *item_next;

		while (item != NULL)
		{
			item_next = item->next;
			delete item;
			item = item_next;			
		}

		start = end = NULL;
		size = 0;
	
	}

	doubleNode<TYPE>* getFirst() const
	{
		return start;
	}

	doubleNode<TYPE>* getLast() const
	{
		return end;
	}

	void info() const
	{
		if (start != NULL) {
			doubleNode<TYPE>* tmp = start;
			unsigned int node_num = 1;
			while (tmp != NULL)
			{
				printf("%s %d: %s %d | %s %p | %s %p | %s %p\n",
					    "Number", node_num,
						"Data", tmp->data,
						"Node", tmp,
						"Next", tmp->next,
						"Previous", tmp->previous);
				tmp = tmp->next;
				node_num++;
			}	
			printf("%s\n", "--------------------------------");
		}
		else
			printf("%s\n", "Empty Double Linked List");
	}

};


#endif // !__DLIST_H__
