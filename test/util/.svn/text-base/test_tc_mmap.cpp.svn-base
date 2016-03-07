#include "util/tc_mmap.h"
#include "util/tc_option.h"
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace taf;

void testCreate(size_t n)
{
    TC_Mmap mmap;
    cout << "create mmap" << endl;
    mmap.mmap("mmap.dat", n);
    mmap.munmap();
    cout << "create mmap OK" << endl;
}

void testWrite(const string &s)
{
    TC_Mmap mmap;
    cout << "write mmap" << endl;
    mmap.mmap("mmap.dat", 1000);
    memcpy(mmap.getPointer(), s.c_str(), s.length());

    sleep(10);

    mmap.munmap();
}

void testRead()
{
    TC_Mmap mmap;
    cout << "read mmap" << endl;
    mmap.mmap("mmap.dat", 1000);

    string s;
    s.assign((char*)mmap.getPointer(), mmap.getSize());
    mmap.munmap();

    cout << s << endl;
}

int main(int argc, char *argv[])
{
    try
    {
        TC_Option option;
        option.decode(argc, argv);

        int pagesize = sysconf(_SC_PAGESIZE);

        cout << "pagesize:" << pagesize << endl;

        if(option.getValue("test") == "create")
        {
            size_t n = 50;
            testCreate(n);
        }
        else if(option.getValue("test") == "write")
        {
            testWrite(option.getValue("c"));
        }
        else if(option.getValue("test") == "read")
        {
            testRead();
        }


        return 0;
	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}

	return 0;
}


