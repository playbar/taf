#include "util/tc_file_mutex.h"
#include <iostream>

using namespace taf;

void ThreadEntry1(TC_FileMutex *fl)
{
	int i = 1;
	while(i++)
	{
		fl->rlock();
		cout << i << ":" << pthread_self() << " : 1234567890abcdefghijklmnopqrstuvwxyz" << endl;
		sleep(1);
		cout << i << ":" << pthread_self() << " : 1234567890abcdefghijklmnopqrstuvwxyz" << endl;
//		fl->unlock();
		sleep(1);
	}
}

void ThreadEntry2(TC_FileMutex *fl)
{
	int i = 1;
	while(i++)
	{
		fl->wlock();
		cout << i << ":" << pthread_self() << " : 1234567890abcdefghijklmnopqrstuvwxyz" << endl;
		sleep(1);
		cout << i << ":" << pthread_self() << " : 1234567890abcdefghijklmnopqrstuvwxyz" << endl;
//		fl->unlock();
		sleep(1);
	}
}


int main(int argc, char *argv[])
{
    try
    {
		TC_FileMutex sl;
		sl.init("./test.lock");

		pthread_t itid1;
		pthread_t itid2;

		pthread_create(&itid1, NULL, (void *(*)(void *))&ThreadEntry1, (void*)&sl);
		pthread_create(&itid2, NULL, (void *(*)(void *))&ThreadEntry2, (void*)&sl);

		pthread_join(itid1, NULL);
		pthread_join(itid2, NULL);
	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}

	return 0;
}


