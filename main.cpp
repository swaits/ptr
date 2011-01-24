#include <vector>
#include <list>
#include <map>

#include "UnitTest++/src/UnitTest++.h"

#include "ptr.h"

// a simple class that reference counts itself
class RefCounter
{
	public:
		RefCounter() { s_instances++; }
		virtual ~RefCounter() { s_instances--; }

		virtual signed Get(signed x) { return x; }

		static signed s_instances;
};

signed RefCounter::s_instances = 0;

// and a derived class off of RefCounter (for testing list of base pointers)
class RefCounterDerived: public RefCounter
{
	public:
		virtual signed Get(signed x) { return x*2; }
};


// a test fixture we need for setup/teardown of each test
struct InstanceFixture
{
	InstanceFixture()
	{
		RefCounter::s_instances = 0;
	}
	~InstanceFixture()
	{
		// empty
	}
};

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,AutoDeletion)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	{
		ptr<RefCounter> a(new RefCounter);
		CHECK_EQUAL(1,RefCounter::s_instances);
	}
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,AssignZeroForDeletion)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	ptr<RefCounter> a = new RefCounter;
	CHECK_EQUAL(1,RefCounter::s_instances);
	a = 0;
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayAutoDeletion)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	{
		array_ptr<RefCounter> a(new RefCounter[20]);
		CHECK_EQUAL(20,RefCounter::s_instances);
	}
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayAssignZeroForDeletion)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	array_ptr<RefCounter> a = new RefCounter[20];
	CHECK_EQUAL(20,RefCounter::s_instances);
	a = 0;
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,AssignNewPointerForDeletion)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	ptr<RefCounter> a = new RefCounter;
	CHECK_EQUAL(1,RefCounter::s_instances);
	a = new RefCounter;
	CHECK_EQUAL(1,RefCounter::s_instances);
	a = 0;
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayAssignNewPointerForDeletion)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	array_ptr<RefCounter> a = new RefCounter[37];
	CHECK_EQUAL(37,RefCounter::s_instances);
	a = new RefCounter[10];
	CHECK_EQUAL(10,RefCounter::s_instances);
	a = 0;
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,CopyToAnotherSmartPointer)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	ptr<RefCounter> a = new RefCounter;
	CHECK_EQUAL(1,RefCounter::s_instances);
	ptr<RefCounter> b(a);
	CHECK_EQUAL(1,RefCounter::s_instances);
	a = 0;
	CHECK_EQUAL(1,RefCounter::s_instances);
	b = 0;
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayCopyToAnotherSmartPointer)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	array_ptr<RefCounter> a = new RefCounter[3];
	CHECK_EQUAL(3,RefCounter::s_instances);
	array_ptr<RefCounter> b(a);
	CHECK_EQUAL(3,RefCounter::s_instances);
	a = 0;
	CHECK_EQUAL(3,RefCounter::s_instances);
	b = 0;
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,StlVector)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	
	std::vector< ptr<RefCounter> > va,vb;
	{
		{
			std::vector< ptr<RefCounter> > vc;
			vc.push_back(new RefCounter);
			CHECK_EQUAL(1,RefCounter::s_instances);
			vc.push_back(new RefCounter);
			CHECK_EQUAL(2,RefCounter::s_instances);
			vc.push_back(new RefCounter);
			CHECK_EQUAL(3,RefCounter::s_instances);
			vb = vc;
			CHECK_EQUAL(3,RefCounter::s_instances);
		}
		CHECK_EQUAL(3,RefCounter::s_instances);
		va.push_back(vb[0]);
		CHECK_EQUAL(3,RefCounter::s_instances);
		vb.clear();
		CHECK_EQUAL(1,RefCounter::s_instances);
	}
	CHECK_EQUAL(1,RefCounter::s_instances);
	va.clear();
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayStlVector)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	
	std::vector< array_ptr<RefCounter> > va,vb;
	{
		{
			std::vector< array_ptr<RefCounter> > vc;
			vc.push_back(new RefCounter[5]);
			CHECK_EQUAL(5,RefCounter::s_instances);
			vc.push_back(new RefCounter[3]);
			CHECK_EQUAL(8,RefCounter::s_instances);
			vc.push_back(new RefCounter[7]);
			CHECK_EQUAL(15,RefCounter::s_instances);
			vb = vc;
			CHECK_EQUAL(15,RefCounter::s_instances);
		}
		CHECK_EQUAL(15,RefCounter::s_instances);
		va.push_back(vb[0]);
		CHECK_EQUAL(15,RefCounter::s_instances);
		vb.clear();
		CHECK_EQUAL(5,RefCounter::s_instances);
	}
	CHECK_EQUAL(5,RefCounter::s_instances);
	va.clear();
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,StlList)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	
	std::list< ptr<RefCounter> > la,lb;
	{
		{
			std::list< ptr<RefCounter> > lc;
			lc.push_back(new RefCounter);
			CHECK_EQUAL(1,RefCounter::s_instances);
			lc.push_back(new RefCounter);
			CHECK_EQUAL(2,RefCounter::s_instances);
			lc.push_back(new RefCounter);
			CHECK_EQUAL(3,RefCounter::s_instances);
			lb = lc;
			CHECK_EQUAL(3,RefCounter::s_instances);
		}
		CHECK_EQUAL(3,RefCounter::s_instances);
		la.push_back(lb.front());
		CHECK_EQUAL(3,RefCounter::s_instances);
		lb.clear();
		CHECK_EQUAL(1,RefCounter::s_instances);
	}
	CHECK_EQUAL(1,RefCounter::s_instances);
	la.clear();
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayStlList)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	
	std::list< array_ptr<RefCounter> > la,lb;
	{
		{
			std::list< array_ptr<RefCounter> > lc;
			lc.push_back(new RefCounter[7]);
			CHECK_EQUAL(7,RefCounter::s_instances);
			lc.push_back(new RefCounter[8]);
			CHECK_EQUAL(15,RefCounter::s_instances);
			lc.push_back(new RefCounter[80]);
			CHECK_EQUAL(95,RefCounter::s_instances);
			lb = lc;
			CHECK_EQUAL(95,RefCounter::s_instances);
		}
		CHECK_EQUAL(95,RefCounter::s_instances);
		la.push_back(lb.front());
		CHECK_EQUAL(95,RefCounter::s_instances);
		lb.clear();
		CHECK_EQUAL(7,RefCounter::s_instances);
	}
	CHECK_EQUAL(7,RefCounter::s_instances);
	la.clear();
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,EqualityOperators)
{
	CHECK_EQUAL(0,RefCounter::s_instances);

	RefCounter* xa = new RefCounter;
	RefCounter* xb = new RefCounter;
	ptr<RefCounter> pa,pb;
	if ( xa < xb )
	{
		pa = xa;
		pb = xb;
	}
	else
	{
		pa = xb;
		pb = xa;
	}
	CHECK_EQUAL(2,RefCounter::s_instances);

	CHECK(pa<pb);
	CHECK(pb>pa);
	CHECK(pa<=pa);
	CHECK(pb>=pb);
	CHECK(pa<=pa);
	CHECK(pb>=pb);
	CHECK(pa==pa);
	CHECK(pb==pb);
	CHECK(pa!=pb);

	pa = 0;
	CHECK_EQUAL(1,RefCounter::s_instances);
	pb = 0;
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayEqualityOperators)
{
	CHECK_EQUAL(0,RefCounter::s_instances);

	RefCounter* xa = new RefCounter[4];
	RefCounter* xb = new RefCounter[6];
	array_ptr<RefCounter> pa,pb;
	if ( xa < xb )
	{
		pa = xa;
		pb = xb;
	}
	else
	{
		pa = xb;
		pb = xa;
	}
	CHECK_EQUAL(10,RefCounter::s_instances);

	CHECK(pa<pb);
	CHECK(pb>pa);
	CHECK(pa<=pa);
	CHECK(pb>=pb);
	CHECK(pa<=pa);
	CHECK(pb>=pb);
	CHECK(pa==pa);
	CHECK(pb==pb);
	CHECK(pa!=pb);

	pa = 0;
	CHECK_EQUAL(6,RefCounter::s_instances);
	pb = 0;
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,AccessPointer)
{
	CHECK_EQUAL(0,RefCounter::s_instances);

	ptr<RefCounter> a = new RefCounter;
	CHECK_EQUAL(1,RefCounter::s_instances);

	CHECK_EQUAL(5,(*a).Get(5));
	CHECK_EQUAL(5,a->Get(5));

	a = 0;

	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayAccessPointer)
{
	CHECK_EQUAL(0,RefCounter::s_instances);

	array_ptr<RefCounter> a = new RefCounter[6];
	CHECK_EQUAL(6,RefCounter::s_instances);

	CHECK_EQUAL(5,(*a).Get(5));
	CHECK_EQUAL(5,a->Get(5));
	CHECK_EQUAL(5,a[0].Get(5));

	a = 0;

	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,BasicInheritance)
{
	CHECK_EQUAL(0,RefCounter::s_instances);

	ptr<RefCounter> a = new RefCounterDerived;
	CHECK_EQUAL(1,RefCounter::s_instances);

	CHECK_EQUAL(10,(*a).Get(5));
	CHECK_EQUAL(10,a->Get(5));

	a = 0;

	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayBasicInheritance)
{
	CHECK_EQUAL(0,RefCounter::s_instances);

	array_ptr<RefCounter> a = new RefCounterDerived[2];
	CHECK_EQUAL(2,RefCounter::s_instances);

	CHECK_EQUAL(10,(*a).Get(5));
	CHECK_EQUAL(10,a->Get(5));

	a = 0;

	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ListOfBasePointers)
{
	CHECK_EQUAL(0,RefCounter::s_instances);

	std::vector< ptr<RefCounter> > v;

	v.push_back(new RefCounter);
	CHECK_EQUAL(1,RefCounter::s_instances);

	v.push_back(new RefCounterDerived);
	CHECK_EQUAL(2,RefCounter::s_instances);

	v.clear();

	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayListOfBasePointers)
{
	CHECK_EQUAL(0,RefCounter::s_instances);

	std::vector< array_ptr<RefCounter> > v;

	v.push_back(new RefCounter[5]);
	CHECK_EQUAL(5,RefCounter::s_instances);

	v.push_back(new RefCounterDerived[2]);
	CHECK_EQUAL(7,RefCounter::s_instances);

	v.clear();

	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,CopyInvalidPointers)
{
	CHECK_EQUAL(0,RefCounter::s_instances);

	ptr<RefCounter> a;
	CHECK_EQUAL(0,RefCounter::s_instances);
	CHECK(!a.valid());
	ptr<RefCounter> b(a);
	CHECK(!a.valid());
	CHECK(!b.valid());
	CHECK_EQUAL(0,RefCounter::s_instances);
	b = new RefCounter;
	CHECK(!a.valid());
	CHECK(b.valid());
	CHECK_EQUAL(1,RefCounter::s_instances);
	b = a;
	CHECK(!a.valid());
	CHECK(!b.valid());
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayCopyInvalidPointers)
{
	CHECK_EQUAL(0,RefCounter::s_instances);

	array_ptr<RefCounter> a;
	CHECK_EQUAL(0,RefCounter::s_instances);
	CHECK(!a.valid());
	array_ptr<RefCounter> b(a);
	CHECK(!a.valid());
	CHECK(!b.valid());
	CHECK_EQUAL(0,RefCounter::s_instances);
	b = new RefCounter[3];
	CHECK(!a.valid());
	CHECK(b.valid());
	CHECK_EQUAL(3,RefCounter::s_instances);
	b = a;
	CHECK(!a.valid());
	CHECK(!b.valid());
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,BoolValidityCheck)
{
	CHECK_EQUAL(0,RefCounter::s_instances);

	{
		ptr<RefCounter> a, b(new RefCounter), c;
		CHECK_EQUAL(1,RefCounter::s_instances);
		CHECK(!a);
		CHECK(!a.valid());
		CHECK(!c);
		CHECK(!c.valid());
		CHECK(b);
		CHECK(b.valid());
		CHECK(a || b || c);
		a = b;
		CHECK_EQUAL(1,RefCounter::s_instances);
		CHECK(a);
		CHECK(a.valid());
		CHECK(b);
		CHECK(b.valid());
		CHECK(a || b);
		CHECK(a && b);
		b = 0;
		CHECK_EQUAL(1,RefCounter::s_instances);
		CHECK(a);
		CHECK(a.valid());
		CHECK(!b);
		CHECK(!b.valid());

		c = new RefCounterDerived;
		CHECK_EQUAL(2,RefCounter::s_instances);
		CHECK(c);
		CHECK(c.valid());
	}

	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayBoolValidityCheck)
{
	CHECK_EQUAL(0,RefCounter::s_instances);

	{
		array_ptr<RefCounter> a, b(new RefCounter[1]), c;
		CHECK_EQUAL(1,RefCounter::s_instances);
		CHECK(!a);
		CHECK(!a.valid());
		CHECK(!c);
		CHECK(!c.valid());
		CHECK(b);
		CHECK(b.valid());
		CHECK(a || b || c);
		a = b;
		CHECK_EQUAL(1,RefCounter::s_instances);
		CHECK(a);
		CHECK(a.valid());
		CHECK(b);
		CHECK(b.valid());
		CHECK(a || b);
		CHECK(a && b);
		b = 0;
		CHECK_EQUAL(1,RefCounter::s_instances);
		CHECK(a);
		CHECK(a.valid());
		CHECK(!b);
		CHECK(!b.valid());

		c = new RefCounterDerived[1];
		CHECK_EQUAL(2,RefCounter::s_instances);
		CHECK(c);
		CHECK(c.valid());
	}

	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,MapMadness)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	{
		std::map< ptr<RefCounter>, ptr<RefCounter> > m,m1,m2,m3,m4;
		CHECK_EQUAL(0,RefCounter::s_instances);

		for (int i=0;i<1000;++i)
		{
			m[ new RefCounter ] = new RefCounterDerived;
			CHECK_EQUAL((i+1)*2,RefCounter::s_instances);
		}

		m1 = m2 = m3 = m4 = m;
		CHECK_EQUAL(2000,RefCounter::s_instances);
		m.clear();
		CHECK_EQUAL(2000,RefCounter::s_instances);
		m2.clear();
		CHECK_EQUAL(2000,RefCounter::s_instances);
		m3.clear();
		CHECK_EQUAL(2000,RefCounter::s_instances);
		m4.clear();
		CHECK_EQUAL(2000,RefCounter::s_instances);
	}
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayMapMadness)
{
	CHECK_EQUAL(0,RefCounter::s_instances);
	{
		std::map< array_ptr<RefCounter>, array_ptr<RefCounter> > m,m1,m2,m3,m4;
		CHECK_EQUAL(0,RefCounter::s_instances);

		for (int i=0;i<1000;++i)
		{
			m[ new RefCounter[2] ] = new RefCounterDerived[2];
			CHECK_EQUAL((i+1)*4,RefCounter::s_instances);
		}

		m1 = m2 = m3 = m4 = m;
		CHECK_EQUAL(4000,RefCounter::s_instances);
		m.clear();
		CHECK_EQUAL(4000,RefCounter::s_instances);
		m2.clear();
		CHECK_EQUAL(4000,RefCounter::s_instances);
		m3.clear();
		CHECK_EQUAL(4000,RefCounter::s_instances);
		m4.clear();
		CHECK_EQUAL(4000,RefCounter::s_instances);
	}
	CHECK_EQUAL(0,RefCounter::s_instances);
}

///////////////////////////////////

TEST_FIXTURE(InstanceFixture,ArrayIndexing)
{
	int* p = new int[10];
	for (int i=0;i<10;++i)
	{
		p[i] = i;
	}
	array_ptr<int> q = p;
	for (int i=0;i<10;++i)
	{
		CHECK_EQUAL(i,q[i]);
		CHECK_EQUAL(&(p[i]),&(q[i]));
	}
}

///////////////////////////////////


int main(int argc, char** argv)
{
	return UnitTest::RunAllTests();
}

