#include "stdafx.h"
#include "CppUnitTest.h"

#include "../Point3d.h"
#include "../DList.h"
#include "../DynArray.h"

#include <time.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameDev_UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		// ----------------------------
		// Tests for Point3d Class
		// ----------------------------

		TEST_METHOD(Point3dSum)
		{
			Point3d<float> p1, p2, p3;
			p1.x = p2.x = 1.f;
			p1.y = p2.y = 2.f;
			p1.z = p2.z = 4.f;

			p3 = p1 + p2;

			Assert::IsTrue(p3.x == 2.f && p3.y == 4.f && p3.z == 8.f);
		}
		TEST_METHOD(Point3dDiff)
		{
			Point3d<float> p1, p2, p3;
			p1.x = p1.y = 1.f;
			p2.x = p2.y = 2.f;
			p1.z = p2.z = 4.f;

			p3 = p1 - p2;
			Assert::IsTrue(p3.x == -1.f && p3.y == -1.f && p3.z == 0.f);
			p3 = p2 - p1;
			Assert::IsTrue(p3.x == 1.f && p3.y == 1.f && p3.z == 0.f);
		}
		TEST_METHOD(Point3dSumAssig)
		{
			Point3d<float> p1, p2;
			p1.x = p2.x = 1.f;
			p1.y = p2.y = 2.f;
			p1.z = p2.z = 4.f;

			p1 += p2;
			Assert::IsTrue(p1.x == 2.f && p1.y == 4.f && p1.z == 8.f);
			p2 += p1;
			Assert::IsTrue(p2.x == 3.f && p2.y == 6.f && p2.z == 12.f);
		}
		TEST_METHOD(Point3dDiffAssig)
		{
			Point3d<float> p1, p2;
			p1.x = p2.x = 1.f;
			p1.y = p2.y = 2.f;
			p1.z = p2.z = 4.f;

			p1 -= p2;
			Assert::IsTrue(p1.x == 0.f && p1.y == 0.f && p1.z == 0.f);
			p2 -= p1;
			Assert::IsTrue(p2.x == 1.f && p2.y == 2.f && p2.z == 4.f);
		}
		TEST_METHOD(Point3dEquality)
		{
			Point3d<float> p1, p2, p3;
			p1.x = p2.x = p3.y = 2.0f;
			p1.y = p2.y = p3.x = 3.5f;
			p1.z = p2.z = p3.z = -1.56f;

			Assert::IsTrue(p1 == p2);
			Assert::IsFalse(p1 == p3);
		}
		TEST_METHOD(Point3dInequality)
		{
			Point3d<float> p1, p2, p3;
			p1.x = p2.x = p3.y = 2.0f;
			p1.y = p2.y = p3.x = 3.5f;
			p1.z = p2.z = p3.z = -1.56f;

			Assert::IsFalse(p1 != p2);
			Assert::IsTrue(p1 != p3);
		}
		TEST_METHOD(Point3dSetZero)
		{
			Point3d<float> p1;
			p1.setZero();

			Assert::IsTrue(p1.x == 0 && p1.y == 0 && p1.z == 0);
			Assert::IsFalse(p1.x != 0 && p1.y != 0 && p1.z != 0);
		}
		TEST_METHOD(Point3dIsZero)
		{
			Point3d<float> p1;
			p1.setZero();

			Assert::IsTrue(p1.isZero());
			p1.x = 2.0f;
			Assert::IsFalse(p1.isZero());
		}
		TEST_METHOD(Point3dDistanceTo)
		{
			Point3d<float> p1, p2;
			p1.setZero();
			p2.x = 3.0f;
			p2.y = -4.0f;
			p2.z = 2.0f;

			Assert::IsTrue(p1.distanceTo(p2) < sqrt(30.0f));
		}

		// ---------------------------------------------------
		// --------------------- DList -----------------------
		// ---------------------------------------------------

		TEST_METHOD(DListConstructor)
		{
			DList<int> dl1;

			Assert::IsTrue(dl1.count() == 0);
			Assert::IsFalse(dl1.del(dl1.getNodeAtPos(0)) == true);
			Assert::IsNull(dl1.getNodeAtPos(3));

			DList<float> dl2;

			Assert::IsTrue(dl2.count() == 0);
			Assert::IsFalse(dl2.del(dl2.getNodeAtPos(0)) == true);
			Assert::IsNull(dl2.getNodeAtPos(3));
		}
		TEST_METHOD(DListConcatenateOperator)
		{
			DList<int> dl1, dl2;

			dl1.add(1);
			dl1.add(2);
			dl2.add(3);
			dl2.add(4);

			dl1 += dl2;

			Assert::IsTrue(dl1.count() == 4);
			Assert::IsTrue(dl1.getNodeAtPos(0)->data == 1);
			Assert::IsTrue(dl1.getNodeAtPos(1)->data == 2);
			Assert::IsTrue(dl1.getNodeAtPos(2)->data == 3);
			Assert::IsTrue(dl1.getNodeAtPos(3)->data == 4);

		}
		TEST_METHOD(DListCount)
		{
			DList<int> dl1;

			dl1.add(45);
			dl1.add(2);

			Assert::IsTrue(dl1.count() == 2);
			Assert::IsTrue(dl1.del(dl1.getNodeAtPos(0)) == true);
		}
		TEST_METHOD(DListGetNodeAtPos)
		{
			DList<int> dl1;

			dl1.add(45);

			Assert::IsTrue(dl1.del(dl1.getNodeAtPos(0)) == true);
			Assert::IsTrue(dl1.count() == 0);
		}
		TEST_METHOD(DListDel)
		{
			DList<float> dl1;

			dl1.add(45.3f);

			Assert::IsTrue(dl1.del(dl1.getNodeAtPos(0)) == true);
			Assert::IsTrue(dl1.count() == 0);

			dl1.add(5.0f);
			dl1.add(-36.87f);
			dl1.add(0.0f);

			Assert::IsTrue(dl1.del(dl1.getNodeAtPos(-1)) == false);
			Assert::IsTrue(dl1.del(dl1.getNodeAtPos(3)) == false);
			Assert::IsTrue(dl1.del(dl1.getNodeAtPos(2)) == true);
			Assert::IsTrue(dl1.count() == 2);
			Assert::IsTrue(dl1.del(dl1.getNodeAtPos(1)) == true);
			Assert::IsTrue(dl1.del(dl1.getNodeAtPos(0)) == true);
			Assert::IsTrue(dl1.count() == 0);

		}
		TEST_METHOD(DListDelAll)
		{
			DList<float> dl1;

			dl1.add(45.3f);
			dl1.add(5.0f);
			dl1.add(-36.87f);
			dl1.add(0.0f);

			Assert::IsTrue(dl1.count() == 4);
			dl1.clear();
			Assert::IsTrue(dl1.count() == 0);
		}
		TEST_METHOD(DListIsOnList)
		{
			DList<float> dl1;

			dl1.add(45.3f);
			dl1.add(5.0f);
			dl1.add(-36.87f);
			dl1.add(0.0f);

			Assert::IsTrue(dl1.isOnList(dl1.getNodeAtPos(3)));
			Assert::IsTrue(dl1.isOnList(dl1.getNodeAtPos(0)));
			Assert::IsFalse(dl1.isOnList(dl1.getNodeAtPos(5)));

			DList<float> dl2;

			dl1.add(999.8f);

			Assert::IsFalse(dl1.isOnList(dl2.getNodeAtPos(0)));
		}
		TEST_METHOD(DListInsertList)
		{
			DList<float> dl1, dl2;

			dl1.add(45.3f);
			dl1.add(5.0f);
			dl1.add(-36.87f);
			dl1.add(0.0f);

			dl2.add(999.9f);
			dl2.add(888.8f);
			dl2.add(777.7f);

			dl1.insert(dl2, 3);

			Assert::AreEqual(dl1.getNodeAtPos(0)->data, 45.3f);
			Assert::AreEqual(dl1.getNodeAtPos(1)->data, 5.0f);
			Assert::AreEqual(dl1.getNodeAtPos(2)->data, -36.87f);
			Assert::AreEqual(dl1.getNodeAtPos(3)->data, 999.9f);
			Assert::AreEqual(dl1.getNodeAtPos(4)->data, 888.8f);
			Assert::AreEqual(dl1.getNodeAtPos(5)->data, 777.7f);
			Assert::AreEqual(dl1.getNodeAtPos(6)->data, 0.0f);
		}
		TEST_METHOD(DListGetFirst)
		{
			DList<float> dl1;
			Assert::IsNull(dl1.getFirst());
			Assert::IsTrue(dl1.getNodeAtPos(0) == dl1.getFirst());

			dl1.add(2.0f);
			dl1.add(3.0f);

			Assert::IsNotNull(dl1.getFirst());
			Assert::IsTrue(dl1.getNodeAtPos(0) == dl1.getFirst());
		}
		TEST_METHOD(DListGetLast)
		{
			DList<float> dl1;
			Assert::IsNull(dl1.getLast());
			Assert::IsTrue(dl1.getNodeAtPos(0) == dl1.getLast());

			dl1.add(2.0f);
			dl1.add(3.0f);

			Assert::IsNotNull(dl1.getLast());
			Assert::IsTrue(dl1.getNodeAtPos(1) == dl1.getLast());
		}
		TEST_METHOD(DListOperatorSquareBrackets)
		{
			DList<float> dl1;
			dl1.add(2.0f);
			dl1.add(3.0f);

			Assert::AreEqual(dl1[0], 2.0f);
			Assert::AreEqual(dl1[1], 3.0f);
		}

		TEST_METHOD(DListSortingCopy)
		{
			DList<int> d;

			d.add(5);
			d.add(4);
			d.add(10);
			d.add(8);
			d.add(3);
			d.add(3);
			d.add(-63);
			d.add(22);
			d.add(0);
			d.add(16);
			d.add(-9);
			d.add(-6);
			d.add(15);
			d.add(1);
			d.add(0);

			d.sort_copy();

			for (unsigned int i = 0; i < d.count() - 1; i++)
				Assert::IsTrue(d[i] <= d[i + 1]);
		}

		/*TEST_METHOD(DListSortingReference)
		{
		DList<int> d;

		d.add(5);
		d.add(4);
		d.add(10);
		d.add(8);
		d.add(3);
		d.add(3);
		d.add(-63);
		d.add(22);
		d.add(0);
		d.add(16);
		d.add(-9);
		d.add(-6);
		d.add(15);
		d.add(1);
		d.add(0);

		d.sort_reference();

		for (unsigned int i = 0; i < d.count() - 1; i++)
		Assert::IsTrue(d[i] <= d[i + 1]);
		}*/

		// ---------------------------------------------
		// ---------------- DynArray--------------------
		// ---------------------------------------------
		TEST_METHOD(DynArrayConstruct1)
		{
			DynArray<int> dyn1;

			Assert::IsTrue(dyn1.getMemory() == 16);
			Assert::IsTrue(dyn1.getNumElements() == 0);
		}
		TEST_METHOD(DynArrayConstruct2)
		{
			DynArray<int> dyn1(5);

			Assert::IsTrue(dyn1.getMemory() == 5);
			Assert::IsTrue(dyn1.getNumElements() == 0);
		}
		TEST_METHOD(DynArrayConcatenateOperator)
		{
			DynArray<int> dyn1(3), dyn2;

			dyn1.pushBack(1);
			dyn1.pushBack(2);

			dyn2.pushBack(991);
			dyn2.pushBack(992);
			dyn2.pushBack(993);

			dyn1 += dyn2;

			Assert::IsTrue(dyn1.getNumElements() == 5);
			Assert::AreEqual(dyn1[0], 1);
			Assert::AreEqual(dyn1[1], 2);
			Assert::AreEqual(dyn1[2], 991);
			Assert::AreEqual(dyn1[3], 992);
			Assert::AreEqual(dyn1[4], 993);
		}
		TEST_METHOD(DynArrayPushBack)
		{
			DynArray<int> dyn1;
			dyn1.pushBack(13);
			Assert::IsTrue(dyn1.getMemory() == 16);
			Assert::IsTrue(dyn1.getNumElements() == 1);

			DynArray<double> dyn2(1);
			dyn2.pushBack(13.99823);
			dyn2.pushBack(13.29234);
			Assert::IsTrue(dyn2.getMemory() == 2);
			Assert::IsTrue(dyn2.getNumElements() == 2);

			DynArray<double> dyn3(4);
			dyn3.pushBack(13.99823);
			dyn3.pushBack(13.29234);
			Assert::IsTrue(dyn3.getMemory() == 4);
			Assert::IsTrue(dyn3.getNumElements() == 2);
		}
		TEST_METHOD(DynArrayPop)
		{
			int a;
			DynArray<int> dyn1;
			dyn1.pushBack(13);
			dyn1.pop(a);
			Assert::IsTrue(dyn1.getMemory() == 16);
			Assert::IsTrue(dyn1.getNumElements() == 0);
			dyn1.pop(a);
			Assert::IsTrue(dyn1.getMemory() == 16);
			Assert::IsTrue(dyn1.getNumElements() == 0);

			float b;
			DynArray<float> dyn2;
			Assert::IsFalse(dyn2.pop(b));
			Assert::IsTrue(dyn2.getMemory() == 16);
			Assert::IsTrue(dyn2.getNumElements() == 0);

			DynArray<float> dyn3(6);
			dyn3.pushBack(3);
			dyn3.pushBack(4);
			dyn3.pushBack(5);
			Assert::IsTrue(dyn3.pop(b));
			Assert::IsTrue(dyn3.getMemory() == 6);
			Assert::IsTrue(dyn3.getNumElements() == 2);
		}
		TEST_METHOD(DynArrayInsert)
		{
			DynArray<int> dyn1;
			Assert::IsTrue(dyn1.insert(13, 1) == false);
			Assert::IsTrue(dyn1.getMemory() == 16);
			Assert::IsTrue(dyn1.getNumElements() == 0);
			Assert::IsTrue(dyn1.insert(12, 0) == true);
			Assert::IsTrue(dyn1.getMemory() == 16);
			Assert::IsTrue(dyn1.getNumElements() == 1);
			dyn1.pushBack(11);
			dyn1.pushBack(12);
			dyn1.pushBack(13);
			Assert::IsTrue(dyn1.insert(12, 0) == true);
			Assert::IsTrue(dyn1.insert(12, 2) == true);
			Assert::IsTrue(dyn1.getMemory() == 16);
			Assert::IsTrue(dyn1.getNumElements() == 6);
		}

		TEST_METHOD(DynArrayInsertAray)
		{
			DynArray<int> dyn1(5);

			dyn1.pushBack(1);
			dyn1.pushBack(2);
			dyn1.pushBack(3);
			dyn1.pushBack(4);
			dyn1.pushBack(5);

			DynArray<int> dyn2(10);

			dyn2.pushBack(991);
			dyn2.pushBack(992);
			dyn2.pushBack(993);
			dyn2.pushBack(994);
			dyn2.pushBack(995);

			DynArray<int> dyn3(3);

			dyn3.pushBack(666);
			dyn3.pushBack(777);
			dyn3.pushBack(888);

			dyn1.insert(dyn2, 2);
			dyn1.insert(dyn3, 6);

			Assert::AreEqual((int)dyn1[0], 1);
			Assert::AreEqual((int)dyn1[1], 2);
			Assert::AreEqual((int)dyn1[2], 991);
			Assert::AreEqual((int)dyn1[3], 992);
			Assert::AreEqual((int)dyn1[4], 993);
			Assert::AreEqual((int)dyn1[5], 994);
			Assert::AreEqual((int)dyn1[6], 666);
			Assert::AreEqual((int)dyn1[7], 777);
			Assert::AreEqual((int)dyn1[8], 888);
			Assert::AreEqual((int)dyn1[9], 995);
			Assert::AreEqual((int)dyn1[10], 3);
			Assert::AreEqual((int)dyn1[11], 4);
			Assert::AreEqual((int)dyn1[12], 5);
		}
		TEST_METHOD(DynArrayOperatorClaudator)
		{
			DynArray<int> dyn1;
			dyn1.pushBack(8);
			Assert::IsTrue(dyn1[0] == 8);
		}

		TEST_METHOD(DynArrayFlip)
		{
			DynArray<unsigned int> dyn1;
			dyn1.pushBack(1);
			dyn1.pushBack(2);
			dyn1.pushBack(3);
			dyn1.pushBack(4);
			dyn1.pushBack(5);
			dyn1.pushBack(6);
			dyn1.pushBack(7);

			dyn1.flip();

			for (unsigned int i = 0, j = 7; i < dyn1.getNumElements(); i++, j--)
				Assert::AreEqual(dyn1[i], j);
		}

		TEST_METHOD(DynArrayBubbleSort)
		{
			DynArray<int> test(5);
			DynArray<int> solution(5);

			test.pushBack(5);
			test.pushBack(4);
			test.pushBack(10);
			test.pushBack(8);
			test.pushBack(3);

			solution.pushBack(3);
			solution.pushBack(4);
			solution.pushBack(5);
			solution.pushBack(8);
			solution.pushBack(10);

			test.bubble_sort();

			for (unsigned int i = 0; i < test.getNumElements(); i++)
				Assert::AreEqual(test[i], solution[i]);

		}

		TEST_METHOD(DynArrayBubbleSort2)
		{

			DynArray<int> d;

			d.pushBack(5);
			d.pushBack(4);
			d.pushBack(10);
			d.pushBack(8);
			d.pushBack(3);
			d.pushBack(3);
			d.pushBack(-63);
			d.pushBack(22);
			d.pushBack(0);
			d.pushBack(16);
			d.pushBack(-9);
			d.pushBack(-6);
			d.pushBack(15);
			d.pushBack(1);
			d.pushBack(0);

			d.bubble_sort2();

			for (unsigned int i = 0; i < d.getNumElements() - 1; i++)
				Assert::IsTrue(d[i] <= d[i + 1]);
		}
		TEST_METHOD(DynArrayCombsort)
		{
			DynArray<int> d_opt;
			srand(time(NULL));

			for (unsigned int i = 0; i < 10000; i++)
				d_opt.pushBack(rand());

			d_opt.combsort();

			for (unsigned int i = 0; i < d_opt.getNumElements() - 1; i++)
				Assert::IsTrue(d_opt[i] <= d_opt[i + 1]);
		}
		TEST_METHOD(DynArrayCocktailSort)
		{
			DynArray<int> d_opt;
			srand(time(NULL));

			for (unsigned int i = 0; i < 10000; i++)
				d_opt.pushBack(rand());

			d_opt.cocktailSort();

			for (unsigned int i = 0; i < d_opt.getNumElements() - 1; i++)
				Assert::IsTrue(d_opt[i] <= d_opt[i + 1]);

			for (unsigned int i = 0; i < 10000; i++)
				d_opt.pushBack(rand());

			d_opt.cocktailSort();

			for (unsigned int i = 0; i < d_opt.getNumElements() - 1; i++)
				Assert::IsTrue(d_opt[i] <= d_opt[i + 1]);
		}

	};
}