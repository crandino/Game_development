#include <stdio.h>
#include "DynArray.h"

int main(int argc, char** argv)
{
	DynArray<int> dyn1;

	DList<int> lst1;
	for (unsigned int i = 756; i < 760; i++)
	{
		lst1.add(i);
	}

	dyn1.insertList(lst1);
	dyn1.info();
	getchar();

	return 0;
}