#ifndef _HESSIAN_WRAPPERS_H
#define _HESSIAN_WRAPPERS_H

#include <iostream>
#include <string>
#include <list>
#include <map>
#include "util/tc_autoptr.h"
#include "hessian/HessianStringStream.h"

/**
*支持类型:
*
*binary						A binary value
*boolean					The byte 'F' represents false and the byte 'T' represents true
*date						HDate represented by a 64-bits long of milliseconds since the epoch
*double						A 64-bit IEEE floating pointer number
*int						A 32-bit signed integer
*list						An ordered list, like an array
*long						A 64-bit signed integer
*map						Represents serialized objects and HMaps
*null						HNull represents a null pointer
*ref						An integer referring to a previous list or map instance
*remote						A reference to a remote object
*string						A 16-bit unicode character string encoded in UTF-8
*xml						An XML document encoded as a 16-bit unicode character string encoded in UTF-8
*/

namespace taf
{

class HObject;
typedef taf::TC_AutoPtr<HObject> HObjectPtr;
class HNull;
typedef taf::TC_AutoPtr<HNull> HNullPtr;
class HBinary;
typedef taf::TC_AutoPtr<HBinary> HBinaryPtr;
class HBoolean;
typedef taf::TC_AutoPtr<HBoolean> HBooleanPtr;
class HDate;
typedef taf::TC_AutoPtr<HDate> HDatePtr;
class HDouble;
typedef taf::TC_AutoPtr<HDouble> HDoublePtr;
class HInteger;
typedef taf::TC_AutoPtr<HInteger> HIntegerPtr;
class HList;
typedef taf::TC_AutoPtr<HList> HListPtr;
class HLong;
typedef taf::TC_AutoPtr<HLong> HLongPtr;
class HMap;
typedef taf::TC_AutoPtr<HMap> HMapPtr;
class HRef;
typedef taf::TC_AutoPtr<HRef> HRefPtr;
class HRemote;
typedef taf::TC_AutoPtr<HRemote> HRemotePtr;
class HXml;
typedef taf::TC_AutoPtr<HXml> HXmlPtr;
class HString;
typedef taf::TC_AutoPtr<HString> HStringPtr;
class HFault;
typedef taf::TC_AutoPtr<HFault> HFaultPtr;

/**
 * 对象基类
 */
class HObject : public taf::TC_HandleBase
{
public:
    /**
     * 构造函数
     */
	HObject() {}

    /**
     * 创建一个对象
     * @param tag
     *
     * @return HObjectPtr
     */
    static HObjectPtr buildHObject(HessianStringStreamPtr &hssPtr, int tag);

    /**
     * 读取数据
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag){};

    /**
     * 组包数据
     * @param call
     *
     * @return string
     */
    virtual string& write(string &call) { return call; }

    /**
     * ==
     * 
     * @return bool
     */
    virtual bool operator==(const HObject&) const;

protected:
	HObject(const HObject& other);
	HObject operator=(const HObject& right);

    /**
     * 读取一个字符串
     * @param hssPtr
     * @param tag
     * 
     * @return string
     */
    string readString(HessianStringStreamPtr &hssPtr, int tag);
};

bool operator!=(const HObject&, const HObject&);
bool operator<(const HObject&, const HObject&);
bool operator<=(const HObject&, const HObject&);
bool operator>(const HObject&, const HObject&);
bool operator>=(const HObject&, const HObject&);

//////////////////////////////////////////////////////
/**
 * HNull对象
 */
class HNull : public HObject
{
public:
	HNull() { }

    /**
     * 读取
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag){}

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);
};

//////////////////////////////////////////////////////

/**
 * HRef对象
 */
class HRef : public HNull
{
public:

    /**
     * 构造
     */
    HRef()
    {
    }

    /**
     * 构造
     * @param value
     */
	HRef(int value) : _value(value)
    {
    }

    /**
     * 读取
     * @param hssPtr
     * @param tag
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag);

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);

    /**
     * 具体值
     * 
     * @return int&
     */
    int &value() { return _value; }

    /**
     * ==
     * 
     * @return bool
     */
    virtual bool operator==(const HObject& l) const
    {
        const HRef *s = dynamic_cast<const HRef*>(&l);
        return s && _value == s->_value;
    }
protected:
    int _value;
};
//////////////////////////////////////////////////////

class HRemote : public HNull
{
public:
    /**
     * 构造
     */
	HRemote() { }

    /**
     * 读取
     * @param hssPtr
     * @param tag
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag) { }

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);
};

//////////////////////////////////////////////////////
class HLong : public HObject
{
public:
    /**
     * 构造函数
     * @param value
     */
	HLong(long long value = 0, char c = 'L'): _value(value), _c(c)
    {
    }

    /**
     * 读取
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag);

    /**
     * 具体值
     *
     * @return long long
     */
    long long& value() { return _value; }

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);

    /**
     * ==
     * 
     * @return bool
     */
    virtual bool operator==(const HObject& l) const
    {
        const HLong *s = dynamic_cast<const HLong*>(&l);
        return s && _value == s->_value;
    }

protected:
    long long   _value;
    char        _c;
};

///////////////////////////////////////////
class HDate : public HObject
{
public:
    /**
     * 构造函数
     * @param value
     */
	HDate(long long value = 0): _value(value)
    {
    }

    /**
     * 读取
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag);

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);

    /**
     * 具体值
     * 
     * @return int&
     */
    long long &value() { return _value; }

    /**
     * ==
     * 
     * @return bool
     */
    virtual bool operator==(const HObject& l) const
    {
        const HDate *s = dynamic_cast<const HDate*>(&l);
        return s && _value == s->_value;
    }

protected:
    long long _value;
};

////////////////////////////////////////
class HInteger : public HObject
{
public:
    /**
     * 构造函数
     * @param value
     */
	HInteger(int value = 0): _value(value)
    {
    }

    /**
     * 读取
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag);

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);

    /**
     * 具体值
     * 
     * @return int&
     */
    int &value() { return _value; }

    /**
     * ==
     * 
     * @return bool
     */
    virtual bool operator==(const HObject& l) const
    {
        cout << _value << endl;
        const HInteger *s = dynamic_cast<const HInteger*>(&l);
        cout << _value << ":" << (_value == s->_value) << endl;
        return s && _value == s->_value;
    }
protected:
    int _value;
};

////////////////////////////////////////
class HBoolean : public HObject
{
public:
    /**
     * 构造函数
     * @param value
     */
	HBoolean(bool value = false): _value(value)
    {
    }

    /**
     * 读取
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag) { }

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);

    /**
     * 具体值
     * 
     * @return bool&
     */
    bool &value() { return _value; }

    /**
     * ==
     * 
     * @return bool
     */
    virtual bool operator==(const HObject& l) const
    {
        const HBoolean *s = dynamic_cast<const HBoolean*>(&l);
        return s && _value == s->_value;
    }

protected:
    bool _value;
};

////////////////////////////////////////
class HBinary : public HObject
{
public:
    /**
     * 构造函数
     * @param value
     */
	HBinary(const string &value = ""): _value(value)
    {
    }

    /**
     * 读取
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag);

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);

    /**
     * 具体值
     * 
     * @return string&
     */
    string &value() { return _value; }

    /**
     * ==
     * 
     * @return bool
     */
    virtual bool operator==(const HObject& l) const
    {
        const HBinary *s = dynamic_cast<const HBinary*>(&l);
        return s && _value == s->_value;
    }
protected:

    /**
     * 读取具体字节
     * @param hssPtr
     * @param bytes
     */
    void readByteChunk(HessianStringStreamPtr &hssPtr, string& bytes);

    /**
     * 组包
     * @param call
     * @param value
     * @param tag
     * 
     * @return string&
     */
    string& writeByte(string &call, const string &value, int tag);
protected:
    string _value;
};

//////////////////////////////////////
class HString : public HObject
{
public:
    /**
     * 构造函数
     * @param value
     */
	HString(const string &value = "", char c = 'S'): _value(value), _c(c)
    {
    }

    /**
     * 读取
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag);

    /**
     * 具体值
     *
     * @return string&
     */
    string &value() { return _value; }

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);

    /**
     * ==
     * 
     * @return bool
     */
    virtual bool operator==(const HObject& l) const
    {
        const HString *s = dynamic_cast<const HString*>(&l);
        return s && (_value == s->_value);
    }

protected:
    /**
     * 读具体的字符串
     * @param hssPtr
     * @param length
     * 
     * @return string
     */
    string readStringImpl(HessianStringStreamPtr &hssPtr, int length);

    /**
     * gbk到utf8
     * @param sIn
     * 
     * @return string
     */
    string toUtf8(const string &sIn);

    /**
     * utf8到gbk
     * @param sIn
     * 
     * @return string
     */
    string utf8To(const string &sIn);

protected:
    string _value;
    char   _c;
};

//////////////////////////////////////
class HXml : public HObject
{
public:
    /**
     * 构造函数
     * @param value
     */
	HXml(const string &value = ""): _value(value)
    {
    }

    /**
     * 读取
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag);

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);

    /**
     * 具体值
     * 
     * @return string&
     */
    string &value() { return _value; }

    /**
     * ==
     * 
     * @return bool
     */
    virtual bool operator==(const HObject& l) const
    {
        const HXml *s = dynamic_cast<const HXml*>(&l);
        return s && _value == s->_value;
    }
protected:
    string _value;
};

/////////////////////////////////////////
class HDouble : public HObject
{
public:
    /**
     * 构造函数
     * @param value
     */
	HDouble(double value = 0): _value(value)
    {
    }

    /**
     * 读取
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag);

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);

    /**
     * 具体值
     * 
     * @return string&
     */
    double &value() { return _value; }

    /**
     * ==
     * 
     * @return bool
     */
    virtual bool operator==(const HObject& l) const
    {
        const HDouble *s = dynamic_cast<const HDouble*>(&l);
        return s && _value == s->_value;
    }
protected:
    double _value;
};

/////////////////////////////////////////
class HList : public HObject
{
public:

    /**
     * 构造函数
     * @param value
     */
	HList(const std::list<HObjectPtr>& value = std::list<HObjectPtr>()): _value(value)
    {
    }

    /**
     * 读取
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag);

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);

    /**
     * 具体值
     * 
     * @return string&
     */
    std::list<HObjectPtr> &value() { return _value; }

    /**
     * ==
     * 
     * @return bool
     */
    virtual bool operator==(const HObject& l) const
    {
        const HList *s = dynamic_cast<const HList*>(&l);
        return s && _value == s->_value;
    }
protected:
    std::list<HObjectPtr> _value;
};

/////////////////////////////////////////
class HMap : public HObject
{
public:
    /**
     * 构造函数
     */
    HMap(){ }

    /**
     * 构造函数
     * @param value
     */
	HMap(const std::map<HObjectPtr, HObjectPtr>& value): _value(value)
    {
    }

    /**
     * 读取
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag);

    /**
     * 组包
     * @param call
     *
     * @return string&
     */
    virtual string& write(string &call);

    /**
     * 具体值
     * 
     * @return string&
     */
    std::map<HObjectPtr, HObjectPtr> &value() { return _value; }

    /**
     * ==
     * 
     * @return bool
     */
    virtual bool operator==(const HObject& l) const
    {
        const HMap *s = dynamic_cast<const HMap*>(&l);
        return s && _value == s->_value;
    }

    /**
     * 查找元素
     * @param o
     * 
     * @return HObjectPtr
     */
    HObjectPtr& find(const HObjectPtr &o)
    {
        static HObjectPtr to;
        std::map<HObjectPtr, HObjectPtr>::iterator it = _value.begin();
        while(it != _value.end())
        {
            if(it->first == o)
            {
                return it->second;
            }
            ++it;
        }
        return to;
    }

protected:
    std::map<HObjectPtr, HObjectPtr> _value;
};

//////////////////////////////////////////////////////
/**
 * hessian错误类
 */
class HFault : public HObject
{
public:
	typedef std::string basic_type;

    /**
     * 构造函数
     */
    HFault(){};

    /**
     * 构造函数
     * @param code
     * @param message
     */
	HFault(const string& code, const string& message):	_code(code), _message(message)
	{
	}

    /**
     * 拷贝构造
     * @param other
     */
	HFault(const HFault& other) :	_code(other._code), _message(other._message)
	{
	}

    /**
     * 读取
     * @param hssPtr
     */
    virtual void read(HessianStringStreamPtr &hssPtr, int tag);

    /**
     * 错误提示
     * 
     * @return string
     */
	string value() { return string("HessianHFault: {code=").append(_code).append(", message=").append(_message).append("}"); }

    /**
     * 错误
     * 
     * @return string
     */
	string code() const { return _code; }

    /**
     * 错误提示
     * 
     * @return string
     */
	string message() const { return _message; }

    /**
     * 抛出异常
     */
    void throwException();
protected:
	string _code;
	string _message;
};

}

#endif
