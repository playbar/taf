#ifndef _JCE2CPP_H
#define _JCE2CPP_H

#include "parse/parse.h"
//#include "servant/BaseF.h"

#include <cassert>
#include <string>

using namespace taf;

/**
 * 根据jce生成c++文件
 * 包括结构的编解码以及生成Proxy和Servant
 */
class Jce2Cpp
{
public:
	Jce2Cpp();

    /**
     * 生成
     * @param file
     */
    void createFile(const string &file, const vector<string> &vsCoder);


    /**
    * 设置生成文件的目录
    *
    */
    void setBaseDir(const std::string & sPath) { m_sBaseDir = sPath; }

	/**
    * 设置是否抛出异常
    *
    */
	void setPDU(bool bPDU) { m_bPDU = bPDU; }

	/**
	 * 设置是否需要打包默认值
	 */
	void setCheckDefault(bool bCheck) { m_bCheckDefault = bCheck; }

    //下面是编解码的源码生成
protected:

    /**
     * 生成某类型的解码源码
     * @param pPtr
     *
     * @return string
     */
    string writeTo(const TypeIdPtr &pPtr) const;

    /**
     * 生成某类型的编码源码
     * @param pPtr
     *
     * @return string
     */
    string readFrom(const TypeIdPtr &pPtr, bool bIsRequire = true) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string display(const TypeIdPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string displaySimple(const TypeIdPtr &pPtr, bool bSep) const;

    //下面是类型描述的源码生成
protected:
    /**
     * 生成某类型的字符串描述源码
     * @param pPtr
     *
     * @return string
     */
    string tostr(const TypePtr &pPtr) const;

    /**
     * 生成内建类型的字符串源码
     * @param pPtr
     *
     * @return string
     */
    string tostrBuiltin(const BuiltinPtr &pPtr) const;
    /**
     * 生成vector的字符串描述
     * @param pPtr
     *
     * @return string
     */
    string tostrVector(const VectorPtr &pPtr) const;

    /**
     * 生成map的字符串描述
     * @param pPtr
     *
     * @return string
     */
    string tostrMap(const MapPtr &pPtr) const;

    /**
     * 生成某种结构的符串描述
     * @param pPtr
     *
     * @return string
     */
    string tostrStruct(const StructPtr &pPtr) const;

    /**
     * 生成某种枚举的符串描述
     * @param pPtr
     *
     * @return string
     */
    string tostrEnum(const EnumPtr &pPtr) const;

    /**
     * 获取定长数组的坐标
     * @param pPtr
     *
     * @return string
     */
    
    string toStrSuffix(const TypeIdPtr &pPtr) const;

    /**
     * 获取定长数组的坐标
     * @param pPtr
     *
     * @return int
     */
    int getSuffix(const TypeIdPtr &pPtr) const;

    /**
     * 生成unsigned转换为signed的符串描述
     * @param pPtr
     *
     * @return string
     */
    //string unsigned2signedStr(const BuiltinPtr &pPtr) const;

    /**
     * 生成类型变量的解码源码
     * @param pPtr
     *
     * @return string
     */
    string decode(const TypeIdPtr &pPtr) const;

    /**
     * 生成类型变量的编码源码
     * @param pPtr
     *
     * @return string
     */
    string encode(const TypeIdPtr &pPtr) const;

    //以下是h和cpp文件的具体生成
protected:
    /**
     * 结构的md5
     * @param pPtr
     *
     * @return string
     */
    string MD5(const StructPtr &pPtr) const;

    /**
     * 生成结构的头文件内容
     * @param pPtr
     *
     * @return string
     */
    string generateH(const StructPtr &pPtr, const string& namespaceId) const;

    /**
     * 生成结构的cpp源码，用在互联网特殊
     * @param pPtr
     *
     * @return string
     */
	string generateCpp(const StructPtr &pPtr, const string& namespaceId) const;

    /**
     * 生成容器的头文件源码
     * @param pPtr
     *
     * @return string
     */
    string generateH(const ContainerPtr &pPtr) const;

    /**
     * 生成容器的cpp源码
     * @param pPtr
     *
     * @return string
     */
    string generateCpp(const ContainerPtr &pPtr) const;

    /**
     * 生成参数声明的头文件内容
     * @param pPtr
     *
     * @return string
     */
    string generateH(const ParamDeclPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return string
     */
    string generateOutH(const ParamDeclPtr &pPtr) const;

    /**
     *
     * @param pPtr
     * @param cn
     *
     * @return string
     */
    string generateAsyncResponseCpp(const OperationPtr &pPtr, const string &cn) const;

    /**
     * 生成参数声明的cpp文件内容
     * @param pPtr
     *
     * @return string
     */
    string generateCpp(const ParamDeclPtr &pPtr) const;

    /**
     * 生成作操作的proxy的cpp文件内容
     * @param pPtr
     * @param cn
     *
     * @return string
     */
    string generateCpp(const OperationPtr &pPtr, const string &cn) const;

    /**
     * 生成作操作的proxy的cpp文件内容
     * @param pPtr
     * @param cn
     *
     * @return string
     */
    string generateCppAsync(const OperationPtr &pPtr, const string &cn) const;

    /**
     * 生成操作cpp文件函数调用分发的源码
     * @param pPtr
     * @param cn
     *
     * @return string
     */
    string generateDispatchCpp(const OperationPtr &pPtr, const string &cn) const;
    /**
     * 生成操作cpp文件函数调用分发的源码
     * @param pPtr
     * @param cn
     *
     * @return string
     */
    string generateDispatchAsyncCpp(const OperationPtr &pPtr, const string &cn) const;
    /**
     * 生成操作的servant的头文件源码
     * @param pPtr
     * @param bVirtual
     *
     * @return string
     */
    string generateHAsync(const OperationPtr &pPtr) const;
    /**
     * 生成操作的servant的头文件源码
     * @param pPtr
     * @param bVirtual
     *
     * @return string
     */
    string generateHAsync(const OperationPtr &pPtr, const string& interfaceId) const;
    /**
     * 生成操作的servant的头文件源码
     * @param pPtr
     * @param bVirtual
     *
     * @return string
     */
    string generateH(const OperationPtr &pPtr, bool bVirtual, const string& interfaceId) const;

    /**
     * 生成接口的头文件源码
     * @param pPtr
     *
     * @return string
     */
    string generateH(const InterfacePtr &pPtr) const;

    /**
     * 生的接口的cpp文件的源码
     * @param pPtr
     *
     * @return string
     */
    string generateCpp(const InterfacePtr &pPtr, const NamespacePtr &nPtr) const;

    /**
     * 生成枚举的头文件源码
     * @param pPtr
     *
     * @return string
     */
    string generateH(const EnumPtr &pPtr) const;

    /**
     * 生成常量头文件源码
     * @param pPtr
     *
     * @return string
     */
    string generateH(const ConstPtr &pPtr) const;

    /**
     * 生成名字空间的头文件源码
     * @param pPtr
     *
     * @return string
     */
    string generateH(const NamespacePtr &pPtr) const;

    /**
     * 生成名字空间cpp文件源码
     * @param pPtr
     *
     * @return string
     */
    string generateCpp(const NamespacePtr &pPtr) const;

    /**
     * 生成名字空间的头文件源码
     * @param pPtr
     *
     * @return string
     */
    void generateH(const ContextPtr &pPtr) const;

    /**
     * 生成名字空间cpp文件源码
     * @param pPtr
     *
     * @return string
     */
    void generateCpp(const ContextPtr &pPtr) const;

    /**
     *  
     * 根据名字查找结构 
     * @param pPtr 
     * @param id 
     * 
     * @return StructPtr 
     */
    StructPtr findStruct(const ContextPtr &pPtr,const string &id);

    /**
     * 
     * 生成接口编解码代码
     * @param pPtr 
     * @param interface 
     */
    void generateCoder(const ContextPtr &pPtr,const string &interface) const;

    string generateCoder(const NamespacePtr &pPtr,const string & sInterface) const;

    string generateCoder(const InterfacePtr &pPtr) const;

    string generateCoder(const OperationPtr &pPtr) const;

private:
    std::string m_sBaseDir;

	bool m_bPDU;
	bool m_bCheckDefault;
};

#endif


