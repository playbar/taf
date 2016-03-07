#include "util/tc_logger.h"

using namespace taf;

ostream& display(ostream& os)
{
//    cout << &os << endl;
    os << "display" << endl;
    return os;
}

TC_LoggerThreadGroup group;
TC_RollLogger        logger;
TC_DayLogger         dlogger;

#define DT \
        n1 = TC_Common::now2ms();   \
        t = n1 - n;                 \
        n = n1;

#define OT if(t > 2) cout << i << ":" << t << endl;

void test(){ofstream os; os << "abc";};

void RollTest( )
{
    int i = 1000000;
    
    while(i)
    {
/*
        int64_t n = TC_Common::now2ms();
        int64_t n1=TC_Common::now2ms();
        int64_t t =TC_Common::now2ms();
*/
//        t = TC_Common::now2ms() - t;
//        logger.info() << "|" << i << "|" << t << endl;

//        DT;
//        ostringstream os;
//        test();

//          display(logger.debug());
//        logger.info();
        logger.debug() << "|" << i << "|" << endl;
//        OT;
/*
        DT;
        logger.warn() << "|" << i << "|" << t << endl;
        OT;

        DT;
        logger.error() << "|" << i << "|" << t << endl;
        OT;
*/
        --i;
    }

    
}

void RollTestThread()
{
    logger.init("./test", 1024000, 10);
    logger.modFlag(TC_RollLogger::HAS_LEVEL | TC_RollLogger::HAS_PID, true);
    logger.setupThread(&group);

    cout << TC_Common::now2str() << endl;
	TC_ThreadPool tpoolA;
	tpoolA.init(10);
	tpoolA.start();

	{
		TC_Functor<void> functor(RollTest);
		TC_Functor<void>::wrapper_type wtA(functor);

		for(size_t i = 0; i < tpoolA.getThreadNum(); i++)
		{
			tpoolA.exec(wtA);
		}
	}

	tpoolA.waitForAllDone();
    cout << TC_Common::now2str() << endl;
}

void DayTest( )
{
    int i = 10000000;
    while(i)
    {
        dlogger.any() << i << endl;
        --i;
    }
}

void DayTestThread()
{
    dlogger.init("./test", "%Y%m%d%H%M");

    dlogger.setupThread(&group);

    cout << TC_Common::now2str() << endl;
	TC_ThreadPool tpoolA;
	tpoolA.init(5);
	tpoolA.start();

	{
		TC_Functor<void> functor(DayTest);
		TC_Functor<void>::wrapper_type wtA(functor);

		for(size_t i = 0; i < tpoolA.getThreadNum(); i++)
		{
			tpoolA.exec(wtA);
		}
	}

	tpoolA.waitForAllDone();
    cout << TC_Common::now2str() << endl;
}

int main(int argc, char *argv[])
{
    try
    {
        group.start(3);

        RollTestThread();
//		DayTestThread();
	}
	catch(exception &ex)
	{
        cout << ex.what() << endl;
	}

	return 0;
}

