#include "hessian/HessianWrappers.h"
#include "hessian/HessianProxy.h"
#include "util/tc_encoder.h"

#include <sstream>
#include <cassert>

namespace taf
{

HObjectPtr HObject::buildHObject(HessianStringStreamPtr &hssPtr, int tag)
{
    HObjectPtr oPtr;
	switch (tag)
    {
	case 'b':
	case 'B':
        oPtr = new HBinary();
        break;
    case 'T':
        oPtr = new HBoolean(true);
        break;
	case 'F':
        oPtr = new HBoolean(false);
        break;
	case 'd':
        oPtr = new HDate();
        break;
	case 'D':
        oPtr = new HDouble();
        break;
	case 'f':
        oPtr = new HFault();
        break;
	case 'I':
        oPtr = new HInteger();
        break;
	case 'V':
        oPtr = new HList();
        break;
	case 'L':
        oPtr = new HLong();
        break;
	case 'M':
        oPtr = new HMap();
        break;
	case 'N':
        oPtr = new HNull();
        break;
	case 'R':
        oPtr = new HRef();
        break;
	case 'S':
        oPtr = new HString();
        break;
	case 'X':
        oPtr = new HXml();
        break;
    default:
        ostringstream os;
        os << "HessianProtocolException::tag '" << hex << (int)tag << "' cannot be handled";
		throw HessianProtocolException(os.str());
	}

    oPtr->read(hssPtr, tag);
    return oPtr;
}

string HObject::readString(HessianStringStreamPtr &hssPtr, int tag)
{
    HStringPtr sPtr = new HString();
    sPtr->read(hssPtr, tag);

    return sPtr->value();
}

bool HObject::operator==(const HObject&l) const
{
    return this == &l;
}

bool operator!=(const HObject&l, const HObject&r)
{
    return !(l == r);
}

bool operator<(const HObject&l, const HObject&r)
{
    return &l < &r;
}

bool operator<=(const HObject&l, const HObject&r)
{
    return l < r || l == r;
}

bool operator>(const HObject&l, const HObject&r)
{
    return !(l < r) && !(l == r);
}

bool operator>=(const HObject&l, const HObject&r)
{
    return !(l < r);
}

/////////////////////////////////////////////////////////////////////////
string& HNull::write(string &call)
{
    call.append(1, 'N');
    return call;
}

void HRef::read(HessianStringStreamPtr &hssPtr, int tag)
{
	int b32 = hssPtr->read() & 0xFF;
	int b24 = hssPtr->read() & 0xFF;
	int b16 = hssPtr->read() & 0xFF;
	int b8  = hssPtr->read() & 0xFF;

	_value = ((b32 << 24) + (b24 << 16) + (b16 << 8) + b8) & 0x00000000FFFFFFFF;
}

string& HRef::write(string &call)
{
    assert(false);
    call.append(1, 'N');
    return call;
}

string& HRemote::write(string &call)
{
    assert(false);
    call.append(1, 'N');
    return call;
}

void HLong::read(HessianStringStreamPtr &hssPtr, int tag)
{
	unsigned long long b64 = hssPtr->read() & 0xFFULL;
	unsigned long long b56 = hssPtr->read() & 0xFF;
	unsigned long long b48 = hssPtr->read() & 0xFF;
	unsigned long long b40 = hssPtr->read() & 0xFF;
	unsigned long long b32 = hssPtr->read() & 0xFF;
	unsigned long long b24 = hssPtr->read() & 0xFF;
	unsigned long long b16 = hssPtr->read() & 0xFF;
	unsigned long long b8  = hssPtr->read() & 0xFF;
	long long value = (b64 << 56) + (b56 << 48) + (b48 << 40) + (b40 << 32) + (b32 << 24) + (b24 << 16) + (b16 << 8) + b8;
	_value = value;
}

string& HLong::write(string &call)
{
    call.append(1, _c);

    long long b64 = (_value >> 56) & 0x00000000000000FF;
	long long b56 = (_value >> 48) & 0x00000000000000FF;
	long long b48 = (_value >> 40) & 0x00000000000000FF;
	long long b40 = (_value >> 32) & 0x00000000000000FF;
	long long b32 = (_value >> 24) & 0x00000000000000FF;
	long long b24 = (_value >> 16) & 0x00000000000000FF;
	long long b16 = (_value >> 8) & 0x00000000000000FF;
	long long b8  = _value & 0x00000000000000FF;

	call.append(1, (char)b64);
	call.append(1, (char)b56);
	call.append(1, (char)b48);
	call.append(1, (char)b40);
	call.append(1, (char)b32);
	call.append(1, (char)b24);
	call.append(1, (char)b16);
	call.append(1, (char)b8);
	return call;
}

void HDate::read(HessianStringStreamPtr &hssPtr, int tag)
{
    HLongPtr lPtr = new HLong();
    lPtr->read(hssPtr, tag);
    _value = lPtr->value();
}

string& HDate::write(string &call)
{
    HLongPtr lPtr = new HLong(_value, 'd');
    return lPtr->write(call);
}

void HInteger::read(HessianStringStreamPtr &hssPtr, int tag)
{
	int b32 = hssPtr->read() & 0xFF;
	int b24 = hssPtr->read() & 0xFF;
	int b16 = hssPtr->read() & 0xFF;
	int b8  = hssPtr->read() & 0xFF;

	_value = ((b32 << 24) + (b24 << 16) + (b16 << 8) + b8) & 0x00000000FFFFFFFF;
}

string& HInteger::write(string &call)
{
	call.append(1, 'I');
	int value = _value;

	int b32 = value >> 24;
	int b24 = (value >> 16) & 0x000000FF;
	int b16 = (value >> 8) & 0x000000FF;
	int b8  = value & 0x000000FF;

	call.append(1, (char)b32);
	call.append(1, (char)b24);
	call.append(1, (char)b16);
	call.append(1, (char)b8);
	return call;
}

string& HBoolean::write(string &call)
{
    call.append(1, _value ? 'T' : 'F');
    return call;
}

void HBinary::read(HessianStringStreamPtr &hssPtr, int tag)
{
	if (tag == 'N')
    {
		_value = "";
    }

	while (tag == 'b')
    {
		readByteChunk(hssPtr, _value);
		tag = hssPtr->read();
	}

  	if (tag != 'B')
  		throw HessianProtocolException("HessianProtocolException::expect tag 'B'");

	readByteChunk(hssPtr, _value);
}

string& HBinary::write(string &call)
{
    const string& l_bytes = _value;
    size_t l_bytesLen     = l_bytes.size();
    size_t l_pos          = 0;

    while(l_bytesLen > 0x8000)
    {
        size_t l_sublen = 0x8000;

        writeByte(call, l_bytes.substr(l_pos, l_sublen), 'b');

        l_bytesLen -= l_sublen;
        l_pos += l_sublen;
    }

    return writeByte(call, l_bytes.substr(l_pos, l_bytesLen), 'B');
}

string& HBinary::writeByte(string &call, const string &value, int tag)
{
    call.append(1, ((char)tag));

	unsigned short slen = (unsigned short)value.length();
	unsigned char b16 = slen >> 8;
	unsigned char b8  = slen & 0x00FF;
	call.append(1, b16);
	call.append(1, b8);
	call.append(value);
    return call;
}

void HBinary::readByteChunk(HessianStringStreamPtr &hssPtr, string& bytes)
{
	int b16 = hssPtr->read() & 0xFF;
	int b8  = hssPtr->read() & 0xFF;
	int len = (b16 << 8) + b8;
	for (int i = 0; i < len; i++)
    {
		bytes.push_back( (char)hssPtr->read() );
    }
}

////////////////////////////////////////////////////////////////////////////////
void HString::read(HessianStringStreamPtr &hssPtr, int tag)
{
	if (tag == 'N')
    {
		_value = "";
    }
    else
    {
    	int b16 = hssPtr->read();
    	int b8  = hssPtr->read();
    	int len = (b16 << 8) + b8;

        _value = readStringImpl(hssPtr, len);
    }

    _value = utf8To(_value);
}

string& HString::write(string &call)
{
    call.append(1, _c);

    _value = toUtf8(_value);

    size_t len = 0;
    for(size_t i = 0; i < _value.length(); i++)
    {
        unsigned int ch = _value[i];
		if (ch < 0x80)
        {
            //单字节的utf8
        }
		else if ((ch & 0xe0) == 0xc0)
        {
            //双字节的utf8
            i++;
		}
		else if ((ch & 0xf0) == 0xe0)
        {
            //三字节的utf8
            i++;i++;
		}   
        len++;
    }

	unsigned short slen = len;
	unsigned char b16 = slen >> 8;
	unsigned char b8 = slen & 0x00FF;
	call.append(1, b16);
	call.append(1, b8);
	call.append(_value);

	return call;
}

string HString::readStringImpl(HessianStringStreamPtr &hssPtr, int length)
{
	string sb;

	for (int i = 0; i < length; i++)
    {
		unsigned int ch = (char)hssPtr->read();

		if (ch < 0x80)
        {
            //单字节的utf8
			sb.append(1, (char) ch);
        }
		else if ((ch & 0xe0) == 0xc0)
        {
            //双字节的utf8
			int ch1 = hssPtr->read();
            sb.append(1, (char) ch);
            sb.append(1, (char) ch1);
		}
		else if ((ch & 0xf0) == 0xe0)
        {
            //三字节的utf8
			int ch1 = hssPtr->read();
			int ch2 = hssPtr->read();
            sb.append(1, (char) ch);
            sb.append(1, (char) ch1);
            sb.append(1, (char) ch2);
		}
		else
			throw HessianProtocolException("HessianProtocolException::bad utf-8 encoding");
	}

	return sb;
}


string HString::toUtf8(const string &sIn)
{
    if(sIn.length() == 0)
    {
        return "";
    }

    if(HessianProxy::isUtf8())
        return sIn;

    assert(HessianProxy::isGbk());
    return TC_Encoder::gbk2utf8(sIn);
}

string HString::utf8To(const string &sIn)
{
    if(sIn.length() == 0)
    {
        return "";
    }

    if(HessianProxy::isUtf8())
        return sIn;

    assert(HessianProxy::isGbk());
    return TC_Encoder::utf82gbk(sIn);
}

////////////////////////////////////////////////////////////////////////////////
// 
void HXml::read(HessianStringStreamPtr &hssPtr, int tag)
{
    HStringPtr sPtr = new HString();
    sPtr->read(hssPtr, tag);
}

string& HXml::write(string &call)
{
    HStringPtr sPtr = new HString(_value, 'X');
    return sPtr->write(call);
}

void HDouble::read(HessianStringStreamPtr &hssPtr, int tag)
{
    assert(tag == 'D');

    HLongPtr lPtr = new HLong();
    lPtr->read(hssPtr, tag);
    memcpy(&_value, &lPtr->value(), sizeof(long long));
}

string& HDouble::write(string &call)
{
    HLongPtr sPtr = new HLong(0, 'D');
    memcpy(&sPtr->value(), &_value, sizeof(long long));
    return sPtr->write(call);
}

void HList::read(HessianStringStreamPtr &hssPtr, int tag)
{
    if (tag == 'N')
    {
        return;
    }

    assert(tag == 'V');

    int list_length = 0;
    int meta        = hssPtr->read();

    // parse optional (meta) type and/or length
    while (meta == 't' || meta == 'l')
    {
        if (meta == 't')
        {
            string list_type = readString(hssPtr, 'S');
        }
        if (meta == 'l')
        {
            // length
            int b32 = hssPtr->read();
            int b24 = hssPtr->read();
            int b16 = hssPtr->read();
            int b8  = hssPtr->read();
            list_length = (b32 << 24) + (b24 << 16) + (b16 << 8) + b8;
        }
        meta = hssPtr->read();
    }

    // read in the list data
    while (meta != 'z')
    {
        // list ends with 'z'
        // read object
        HObjectPtr obj = HObject::buildHObject(hssPtr, meta);
        // add to list
        _value.push_back(obj);
        // next please
        meta = hssPtr->read();
    }
}

string& HList::write(string &call)
{
	call.append(1, 'V');
    call.append(1, 'l');

	int value = _value.size();

	int b32 = value >> 24;
	int b24 = (value >> 16) & 0x000000FF;
	int b16 = (value >> 8) & 0x000000FF;
	int b8  = value & 0x000000FF;

	call.append(1, (char)b32);
	call.append(1, (char)b24);
	call.append(1, (char)b16);
	call.append(1, (char)b8);

	for (list<HObjectPtr>::iterator i = _value.begin(); i != _value.end(); i++)
    {
        (*i)->write(call);
    }
	call.append(1, 'z');
	return call;
}

/////////////////////////////////////////////////////////////////
void HMap::read(HessianStringStreamPtr &hssPtr, int tag)
{
    if (tag == 'N')
    {
        return;
    }

    assert(tag == 'M');

    int meta = hssPtr->read();

    // parse optional (meta) type
    while (meta == 't')
    {
        if (meta == 't')
        {
            string map_type = readString(hssPtr, 'S');
        }
        meta = hssPtr->read();
    }

    // read in the map data, map ends with 'z'
    while (meta != 'z')
    {
        // read key object

        HObjectPtr key = HObject::buildHObject(hssPtr, meta);

        // read value object
        meta = hssPtr->read();

        HObjectPtr val = HObject::buildHObject(hssPtr, meta);

        _value.insert(std::make_pair(key, val));

        // next please
        meta = hssPtr->read();
    }
}

string& HMap::write(string &call)
{
	call.append(1, 'M');

    for (std::map<HObjectPtr, HObjectPtr>::iterator i = _value.begin(); i != _value.end(); i++)
    {
        i->first->write(call);
        i->second->write(call);
	}
	call.append(1, 'z');
	return call;
}

////////////////////////////////////////////////////////////////////
void HFault::read(HessianStringStreamPtr &hssPtr, int tag)
{
	assert(tag == 'f');

	//读取'code'
    tag = hssPtr->read();
	readString(hssPtr, tag);

	//读取code对应的值
    tag = hssPtr->read();
	_code = readString(hssPtr, tag);

	//读取'message'
    tag = hssPtr->read();
	readString(hssPtr, tag);

	//读取message对应的值
    tag = hssPtr->read();
	_message = readString(hssPtr, tag);

    tag = hssPtr->read();
    if(tag == 'z') 
    {
        return; 
    }

	//读取'detail'
	readString(hssPtr, tag);

	//读取详细的异常
	tag = hssPtr->read();
    HMap m;
    m.read(hssPtr, tag);

	string mesg = _message;
	unsigned long exception_type_start = mesg.find(']');
	if (exception_type_start != string::npos)
    {
		_code       = mesg.substr(0, exception_type_start);
		_message    = mesg.substr(exception_type_start + 1);
	}
}


void HFault::throwException()
{
    if(_code == "ProtocolException")
    {
        throw HessianProtocolException("HessianProtocolException::" + _message);
    }
    else if(_code == "NoSuchHObjectException")
    {
        throw HessianNoSuchHObjectException("HessianNoSuchHObjectException::" + _message);
    }    
    else if(_code == "NoSuchMethodException")
    {
        throw HessianNoSuchMethodException("HessianNoSuchMethodException::" + _message);
    }    
    else if(_code == "RequireHeaderException")
    {
        throw HessianRequireHeaderException("HessianRequireHeaderException::" + _message);
    }
    else if(_code == "ServiceException")
    {
        throw HessianServiceException("HessianServiceException::" + _message);
    }
    else
    {
        throw HessianUnknownException("HessianUnknownException::" + _code + ":" + _message);
    }
}

}



