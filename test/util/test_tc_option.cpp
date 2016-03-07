#include "util/tc_option.h"
#include "util/tc_common.h"
#include <iterator>
#include <iostream>
#include <sys/time.h>

using namespace taf;


int main(int argc, char *argv[])
{
    try
    {
        TC_Option op;
        op.decode(argc, argv);

        map<string, string> mp = op.getMulti();

        cout << "map:" << endl;
        cout << TC_Common::tostr(mp) << endl;

        vector<string> d = op.getSingle();
        cout << "vector:" << endl;
        cout << TC_Common::tostr(d) << endl;

		cout << op.getValue("abc") << endl;
	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}

	return 0;
}


