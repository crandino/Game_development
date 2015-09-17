#include "stdafx.h"
#include "CppUnitTest.h"

#include "../Point3d.h"


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
	};
}