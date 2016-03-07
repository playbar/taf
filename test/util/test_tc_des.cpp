#include "util/tc_des.h"
#include "util/tc_common.h"

using namespace taf;

#define KEY  "dRa93"
#define KEY3 "dRa938066"

#define TESTKEY "12345678"

void test()
{
    string s = "abcdefghijklmnopqrstu343";
    cout << "encrypt:" << s << ":" << s.length()<< endl;

    string v = taf::TC_Des::encrypt(KEY, s.c_str(), s.length());
    string n = TC_Common::bin2str(v);
    cout << "encrypt:" << n << ":" << n.length()/2 << endl;

    string s1 = taf::TC_Des::decrypt(KEY, v.c_str(), v.length());
    cout << s1 << endl;
}

int main(int argc, char *argv[])
{
    test();
    return 0;

    try
    {
        string n = "abde";
        {
            string s = n;
            cout << "encrypt:" << s << endl;
            string v = taf::TC_Des::encrypt(KEY, s.c_str(), s.length());
            cout << "encrypt:" << TC_Common::bin2str(v) << endl;
            string s1 = taf::TC_Des::decrypt(KEY, v.c_str(), v.length());
            cout << s1 << endl;
        }

        /////////////////////////////////////////////////////////////////////////
        {
            string s = n;
            cout << "encrypt:" << s << endl;
            string v = taf::TC_Des::encrypt3(KEY3, s.c_str(), s.length());
            cout << "encrypt:" << TC_Common::bin2str(v.c_str(), v.length()) << endl;
            string s1 = taf::TC_Des::decrypt3(KEY3, v.c_str(), v.length());
            cout << "decrypt:" << s1 << endl;
        }
	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}

	return 0;
}


