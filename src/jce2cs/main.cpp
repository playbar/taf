#include "util/tc_option.h"
#include "util/tc_file.h"
#include "jce2cs.h"

void usage()
{
    cout << "Usage : jce2cs [OPTION] jcefile" << endl;
    cout << "  jce2cs support type: bool byte short int long float double vector map" << endl;
    cout << "supported [OPTION]:" << endl;
    cout << "  --help                help,print this(帮助)" << endl;
    cout << "  --dir=DIRECTORY       generate java file to DIRECTORY(生成文件到目录DIRECTORY,默认为当前目录)" << endl;
    cout << "  --base-package=NAME   package prefix, default 'com.qq.'(package前缀，未指定则默认为com.qq.)" << endl;
    cout << "  --with-servant        also generate servant class(一并生成服务端代码，未指定则默认不生成)" << endl;
    cout << endl;
    exit(0);
}

void check(vector<string> &vJce)
{
    bool b = true;

    for(size_t i  = 0; i < vJce.size(); i++)
    {
        string ext  = taf::TC_File::extractFileExt(vJce[i]);
        if(ext == "jce")
        {
            if(!b)
            {
                usage();
            }
            if(!taf::TC_File::isFileExist(vJce[i]))
            {
                cerr << "file '" << vJce[i] << "' not exists" << endl;
                exit(0);
            }
        }
        else
        {
            cerr << "only support jce file." << endl;
            exit(0);
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        usage();
    }

    taf::TC_Option option;
    option.decode(argc, argv);
    vector<string> vJce = option.getSingle();

    check(vJce);

    if(option.hasParam("help"))
    {
        usage();
    }

    Jce2Cs j2cs;

    //是否可以以taf开头
    g_parse->setTaf(option.hasParam("with-taf"));

    //设置生成文件的根目录
    if(option.getValue("dir") != "")
    {
        j2cs.setBaseDir(option.getValue("dir"));
    }
    else
    {
        j2cs.setBaseDir(".");
    }

    //包名前缀
    if(option.hasParam("base-package"))
    {
        j2cs.setBasePackage(option.getValue("base-package"));
    }
    else
    {
        j2cs.setBasePackage("Com.QQ.");
    }

    //是否生成服务端类,默认不生成
    if(option.hasParam("with-servant"))
    {
        j2cs.setWithServant(true);
    }
    else
    {
        j2cs.setWithServant(false);
    }

    for(size_t i = 0; i < vJce.size(); i++)
    {
        g_parse->parse(vJce[i]);
        j2cs.createFile(vJce[i]);
    }

    return 0;
}

