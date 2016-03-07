#include "util/tc_file.h"
#include <errno.h>
#include <iterator>
#include <sys/time.h>

using namespace taf;

/*
int removeFile(const string &sFullFileName, bool bRecursive)
{
    cout << sFullFileName << endl;
    string path = TC_File::simplifyDirectory(sFullFileName);

    struct stat f_stat;

    if (lstat(path.c_str(), &f_stat) == -1)
    {
        return -1;
    }

    if(S_ISDIR(f_stat.st_mode))
    {
        if(bRecursive)
        {
            vector<string> files;
            TC_File::listDirectory(path, files, true, false);
            for(size_t i = 0; i < files.size(); i++)
            {
                removeFile(files[i], bRecursive);
            }

            if(path != "/")
            {
                if(::rmdir(path.c_str()) == -1)
                {
                    return -1;
                }
                return 0;
            }
        }
        else
        {
            if(::rmdir(path.c_str()) == -1)
            {
                return -1;
            }
        }
    }
    else
    {
        if(::remove(path.c_str()) == -1)
        {
            return -1;
        }
    }

    return 0;
}
*/

struct Out
{
    void operator()(const string &n)
    {
        cout << n << endl;
    }
};

int main(int argc, char *argv[])
{
    try
    {
//    	cout << TC_File::getFileSize("./test_tc_file.cpp") << endl;
//    	cout << TC_File::isFileExist("./test_tc_file.cpp", S_IFDIR) << endl;
//    	cout << TC_File::makeDir("test") << endl;
        cout << TC_File::simplifyDirectory("/.") << endl;
        cout << TC_File::simplifyDirectory("/./ab/tt//t///t//../tt/") << endl;
        TC_File::removeFile("./", true);
//        int ret = removeFile("/home/jarodruan/taf/bin/tmp", true);
        vector<string> v;
        TC_File::listDirectory("/home/base.l", v, true);
        for_each(v.begin(), v.end(), Out());

//        TC_File::removeFile("/home/base.l", false);

	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}

	return 0;
}


