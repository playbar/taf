#include "util/tc_autoptr.h"
#include "util/tc_thread_pool.h"
#include <vector>
#include <cassert>
#include <iostream>


using namespace std;
using namespace taf;

void func(int &i)
{
}

class TestPointer : public TC_HandleBase
{
public:
    void func(int &i)
    {
        int n = 10000;
        while(n)
        {
            i++;
            n--;
        }
    }
};

void test()
{
	int i = 0;

	typedef TC_AutoPtr<TestPointer> TestPointerPtr;
	vector<TestPointerPtr> vtp;
	for(size_t j = 0; j < 10; j++)
	{
		vtp.push_back(new TestPointer());
	}

	cout << i << endl;
	for(size_t j = 0; j < 10; j++)
	{
		vtp[j]->func(i);
	}

	cout << i << endl;
	vector<TestPointerPtr> vtp1 = vtp;
	for(size_t j = 0; j < 10; j++)
	{
		vtp[j]->func(i);
	}

	cout << i << endl;
}

TC_Atomic a;

void testAdd()
{
	int i = 10000000;
	while(i--)
	{
		a.inc();
	}
}

void testDel()
{
	int i = 10000000;
	while(i--)
	{
		a.dec();
	}
}

int main(int argc, char *argv[])
{
    try
    {
		cout << a.get() << endl;

		TC_ThreadPool tpoolA;
		tpoolA.init(10);
		tpoolA.start();

		TC_ThreadPool tpoolB;
		tpoolB.init(10);
		tpoolB.start();

		{
			TC_Functor<void> functor(testAdd);
			TC_Functor<void>::wrapper_type wt(functor);

			for(size_t i = 0; i < tpoolA.getThreadNum(); i++)
			{
				tpoolA.exec(wt);
			}
		}

		{
			TC_Functor<void> functor(testDel);
			TC_Functor<void>::wrapper_type wt(functor);

			for(size_t i = 0; i < tpoolB.getThreadNum(); i++)
			{
				tpoolB.exec(wt);
			}
		}

		tpoolA.waitForAllDone();
		tpoolB.waitForAllDone();

		cout << a.get() << endl;
	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}

	return 0;
}


