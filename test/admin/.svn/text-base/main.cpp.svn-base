#include <iostream>
#include "servant/Application.h"
#include "servant/AdminF.h"

using namespace std;
using namespace taf;

int main(int argc, char *argv[])
{
    try
    {
        CommunicatorPtr c = new Communicator();

        AdminFPrx adminFPrx = c->stringToProxy<AdminFPrx>("AdminObj@tcp -h 127.0.0.1 -p 55678");
//        adminFPrx->shutdown();
        string s1 = adminFPrx->notify("taf.viewstatus");
        cout << s1 << endl;
//        string s2 = adminFPrx->notify("taf.connection");
//        cout << s2 << endl;
        string s3 = adminFPrx->notify("load");
		cout << s3 << endl;
	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}

	return 0;
}


