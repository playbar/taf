#include "util/tc_singleton.h"
#include "util/tc_timeprovider.h"
#include <iostream>

using namespace std;
using namespace taf;

class B : public TC_Singleton<B, CreateStatic,  PhoneixLifetime>
{
public:
    B(){cout << "B" << endl;}
    ~B(){cout << "~B" << endl;}

    void test(){cout << "test B" << endl;}             
};

class A : public TC_Singleton<A, CreateStatic,  PhoneixLifetime>
{
public:
    A(){cout << "A" << endl;}
    ~A()
    {
        cout << "~A" << endl;
        B::getInstance()->test();
    }

    void test(){cout << "test A" << endl;}             
};



int main(int argc, char *argv[])
{
    try
    {
//        A::getInstance()->test();
//        B::getInstance()->test();

        cout << TC_TimeProvider::getInstance()->getNow() << endl;
        sleep(1);
	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}

	return 0;
}

 
