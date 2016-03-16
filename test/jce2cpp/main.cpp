#include "util/tc_common.h"
#include "test.h"
#include "test1.h"
#include "test2.h"
#include <iostream>
#include <map>

using namespace Test;
using namespace TestEx;
using namespace std;
using namespace taf;

int main(int argc, char *argv[])
{
    try
    {
		TestInfo ti;
		ti.b	= true;
		ti.by	= 1;
		ti.si	= 345;
		ti.ii	= 68901;
		ti.li   = 1234567890L;
		ti.f	= 3.14f;
		ti.d	= 4.13;
		ti.s	= "test";
		ti.vs.push_back("test1");
		ti.vs.push_back("test2");
		ti.vs.push_back("test3");
		ti.m["test1"] = "test1";
		ti.m["test2"] = "test2";
		ti.m["test3"] = "test3";
		ti.vm.push_back(ti.m);
		ti.vm.push_back(ti.m);
		ti.vm.push_back(ti.m);
		ti.mv[ti.vs] = ti.vs;
		ti.mv[ti.vs] = ti.vs;
		ti.mv[ti.vs] = ti.vs;

		TestInfo tii;
		{
			JceOutputStream<BufferWriter> os;
			ti.writeTo(os);
			
			JceInputStream<BufferReader> is;
			is.setBuffer(os.getBuffer(), os.getLength());
			tii.readFrom(is);
		}
		
		assert(ti == tii);

        cout << tii.b << endl;
        cout << tii.by << endl;
        cout << tii.si << endl;
        cout << tii.ii << endl;
        cout << tii.li << endl;
        cout << tii.f << endl;
        cout << tii.d << endl;
        cout << tii.s << endl;
        cout << TC_Common::tostr(tii.vs.begin(), tii.vs.end()) << endl;
        cout << TC_Common::tostr(tii.m.begin(), tii.m.end()) << endl;

	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}

	return 0;
}


