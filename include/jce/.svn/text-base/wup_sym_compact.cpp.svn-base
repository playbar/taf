// **********************************************************************
// WUP version 1.0.0 by WSRD Tencent.
// **********************************************************************
#include <stddef.h>
#include "wup_sym_compact.h"

namespace wup
{

// static const taf::Int32 WUP_SUCCESS        =  0;      //成功
// static const taf::Int32 WUP_ATTR_NOT_FOUND = -1;      //属性不存在
// static const taf::Int32 WUP_ENCODE_ERROR   = -2;      //编码错误
// static const taf::Int32 WUP_DECODE_ERROR   = -3;      //解码错误
// static const taf::Int32 WUP_RUNTIME_ERROR  = -4;      //其他运行时错误

EXPORT_C UniAttribute::UniAttribute()
{
	_is = taf::JceInputStream::NewL();
	_os = taf::JceOutputStream::NewL();
}

EXPORT_C UniAttribute::~UniAttribute()
{
	if (_is)
	{
		delete _is;
		_is = NULL;
	}

	if (_os)
	{
		delete _os;
		_os = NULL;
	}
}

EXPORT_C taf::Int32 UniAttribute::putAttr(taf::JceOutputStream &os, const taf::String &key)
{
  //howlpan  JArray<JMapPair<String, JMapWrapper<String, JArray<char> > > > & array1 = _data.getArray();
	JArray<JMapPair<String, JArray<signed char> > > & array1 = _data.getArray();
	for(unsigned int i = 0; i < array1.size(); ++i)
	{
		if(array1[i].first == key)
        {
			array1[i].second = JArray<signed char>(os.getBuffer(), os.getLength());
			return WUP_SUCCESS;
		}
	}
	_data.getArray().append(JMapPair<String, JArray<signed char> >(key, JArray<signed char>(os.getBuffer(), os.getLength())));

    return WUP_SUCCESS;
}

EXPORT_C Int32 UniAttribute::put (const String& name, const Bool &t)    {DO_PUT}
EXPORT_C Int32 UniAttribute::put (const String& name, const Char &t)    {DO_PUT}
EXPORT_C Int32 UniAttribute::put (const String& name, const Short &t)   {DO_PUT}
EXPORT_C Int32 UniAttribute::put (const String& name, const Int32 &t)   {DO_PUT}
EXPORT_C Int32 UniAttribute::put (const String& name, const Int64 &t)   {DO_PUT}
EXPORT_C Int32 UniAttribute::put (const String& name, const Float &t)   {DO_PUT}
EXPORT_C Int32 UniAttribute::put (const String& name, const Double &t)  {DO_PUT}
EXPORT_C Int32 UniAttribute::put (const String& name, const String &t)  {DO_PUT}
EXPORT_C Int32 UniAttribute::put (const String& name, const JceStructBase &t) {DO_PUT}

//add by edwardsu
EXPORT_C Int32 UniAttribute::put (const String& name, const UInt8 &t)    {DO_PUT}
EXPORT_C Int32 UniAttribute::put (const String& name, const UInt16 &t)   {DO_PUT}
EXPORT_C Int32 UniAttribute::put (const String& name, const UInt32 &t)   {DO_PUT}
//end add
EXPORT_C Int32 UniAttribute::getAttr(taf::JceInputStream &is, const String &key)
{
    JArray<JMapPair<String, JArray<signed char> > > & array1 = _data.getArray();
    for(unsigned int i = 0; i < array1.size(); ++i)
    {
         if(array1[i].first == key)
         {
			 is.reset();
			 is.setBuffer(array1[i].second.data(), array1[i].second.size());

			 return WUP_SUCCESS;
         }
    }
    _sLastError = (Char*)"UinRequest not found key:";
    _sLastError.AppendData((unsigned char*)key.data(), key.size());
    return WUP_ATTR_NOT_FOUND;
}

EXPORT_C Int32 UniAttribute::get (const String& name,  Bool &t)    {DO_GET}
EXPORT_C Int32 UniAttribute::get (const String& name,  Char &t)    {DO_GET}
EXPORT_C Int32 UniAttribute::get (const String& name,  Short &t)   {DO_GET}
EXPORT_C Int32 UniAttribute::get (const String& name,  Int32 &t)   {DO_GET}
EXPORT_C Int32 UniAttribute::get (const String& name,  Int64 &t)   {DO_GET}
EXPORT_C Int32 UniAttribute::get (const String& name,  Float &t)   {DO_GET}
EXPORT_C Int32 UniAttribute::get (const String& name,  Double &t)  {DO_GET}
EXPORT_C Int32 UniAttribute::get (const String& name,  String &t)  {DO_GET}
EXPORT_C Int32 UniAttribute::get (const String& name,  JceStructBase &t) {DO_GET}

//add by edwardsu
EXPORT_C Int32 UniAttribute::get (const String& name,  UInt8 &t)    {DO_GET}
EXPORT_C Int32 UniAttribute::get (const String& name,  UInt16 &t)   {DO_GET}
EXPORT_C Int32 UniAttribute::get (const String& name,  UInt32 &t)   {DO_GET}
//end add
EXPORT_C void UniAttribute::clear() 
{ 
    _data.clear(); 
}

EXPORT_C Int32 UniAttribute::encode(String& buff)
{
    _os->reset();

    _os->write(_data, 0);

    //buff.assign(_os->getBuffer(), (const char *)(_os->getBuffer() + _os->getLength()));
	buff.assign(_os->getBuffer(), (const signed char *)(_os->getBuffer() + _os->getLength()));

    return WUP_SUCCESS;
    /*
    catch (JceException &e)
    {
        _sLastError = e.what();

        return WUP_ENCODE_ERROR;
    }
    */
}

//howlpan EXPORT_C Int32 UniAttribute::encode(JArray<char>& buff)
EXPORT_C Int32 UniAttribute::encode(JArray<Char>& buff)
{
    _os->reset();

    _os->write(_data, 0);

    buff.assign(_os->getBuffer(), _os->getLength());
    
    return WUP_SUCCESS;
    /*
    catch (JceException &e)
    {
        _sLastError = e.what();

        return WUP_ENCODE_ERROR;
    }
    */
}

EXPORT_C Int32 UniAttribute::encode(char* buff, size_t & len)
{   
    _os->reset();

    _os->write(_data, 0);

    if(len < _os->getLength())
    {
        _sLastError = (Char*)"encode error, buffer length too short";
        return WUP_ENCODE_ERROR;
    }
    memcpy(buff, _os->getBuffer(), _os->getLength());
    len =  _os->getLength();
    
    return WUP_SUCCESS;
}

//howlpan EXPORT_C Int32 UniAttribute::decode(const char* buff, size_t len)
EXPORT_C Int32 UniAttribute::decode(const Char* buff, size_t len)
{
    _is->reset();

    _is->setBuffer(buff, len);

    _data.clear();

    TRAPD(error, _is->read(_data, 0, true););
    if(error)
    {
        _sLastError = _is->getLastError();
        return WUP_DECODE_ERROR;
    }
    return WUP_SUCCESS;

}

//howlpan EXPORT_C Int32 UniAttribute::decode(const JArray<char>& buff)
EXPORT_C Int32 UniAttribute::decode(const JArray<Char>& buff)
{
    _is->reset();

    _is->setBuffer(buff.data(), buff.size());

    _data.clear();

    TRAPD(error, _is->read(_data, 0, true););
    if(error)
    {
        _sLastError = _is->getLastError();
        return WUP_DECODE_ERROR;
    }
    return WUP_SUCCESS;
}

EXPORT_C const JMapWrapper<String, JArray<Char> >& UniAttribute::getData() const
{
    return _data;
}

EXPORT_C bool UniAttribute::isEmpty()
{
    return _data.empty();
}

EXPORT_C size_t UniAttribute::size()
{
    return _data.size();
}

EXPORT_C String UniAttribute::getLastError()
{
    return _sLastError;
}

////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT_C UniPacket::UniPacket() 
{
    iVersion = 3; cPacketType = 0; 

    iMessageType = 0; iRequestId = 0; 

    sServantName = (Char*)""; sFuncName = (Char*)""; 

    iTimeout = 0; sBuffer.clear(); 

    context.clear(); status.clear(); 

    UniAttribute::_data.clear();
}

EXPORT_C UniPacket* UniPacket::createResponseLC()
{
    UniPacket* self = new (ELeave)UniPacket();
    CleanupStack::PushL(self);
    self->ConstructL();
    
    self->sServantName = sServantName;
    self->sFuncName    = sFuncName;
    self->iRequestId   = iRequestId;
    
    return self;
}

EXPORT_C UniPacket* UniPacket::createResponseL()
{
    UniPacket* self=UniPacket::createResponseLC();
    CleanupStack::Pop(); // self;

    return self;
}

EXPORT_C Int32 UniPacket::encode(String& buff)
{
    _os->reset();

    doEncode(*_os);

    taf::Int32 iHeaderLen = jce_htonl(sizeof(taf::Int32) + _os->getLength());
    buff.assign((const Char*)&iHeaderLen, (const Char*) ((const Char*)&iHeaderLen + sizeof(taf::Int32)));
    
    buff.AppendData((unsigned char*)_os->getBuffer(), _os->getLength());

    return WUP_SUCCESS;
}

EXPORT_C Int32 UniPacket::encode(JArray<char>& buff)
{
    _os->reset();

    doEncode(*_os);

    taf::Int32 iHeaderLen = jce_htonl(sizeof(taf::Int32) + _os->getLength());
    
    buff.resize(sizeof(taf::Int32) + _os->getLength());
    memcpy(&buff[0], &iHeaderLen, sizeof(taf::Int32));
    memcpy(&buff[sizeof(taf::Int32)], _os->getBuffer(), _os->getLength());
    return WUP_SUCCESS;
}

EXPORT_C Int32 UniPacket::encode(char* buff, size_t & len)
{
    _os->reset();

    doEncode(*_os);

    taf::Int32 iHeaderLen = jce_htonl(sizeof(taf::Int32) + _os->getLength());
    if(len < sizeof(taf::Int32) + _os->getLength())
    {
        _sLastError = (Char*)"encode error, buffer length too short";
        return WUP_ENCODE_ERROR;
    }

    memcpy(buff, &iHeaderLen, sizeof(taf::Int32));
    memcpy(buff + sizeof(taf::Int32), _os->getBuffer(), _os->getLength());

    len = sizeof(taf::Int32) + _os->getLength();
    
    return WUP_SUCCESS;
}

EXPORT_C Int32 UniPacket::decode(const signed char* buff, size_t len)
{
    if(len < sizeof(taf::Int32))
    {
        _sLastError = (Char*)"packet length too short";
        return WUP_DECODE_ERROR;
    }
    
    _is->reset();

    TRAPD(error, 
            
        _is->setBuffer(buff + sizeof(taf::Int32), len - sizeof(taf::Int32));

        readFrom(*_is);
		if(iVersion != 3)
		{
			_sLastError = (Char*)"wup version error";
			return WUP_DECODE_ERROR;
		}

        _is->reset();

        _is->setBuffer(sBuffer.data(), sBuffer.size());

        UniAttribute::_data.clear();

        _is->read(UniAttribute::_data, 0, true);
    );
    
    if(error)
    {
        _sLastError = _is->getLastError();
        return WUP_DECODE_ERROR;
    }
    return WUP_SUCCESS;
}

EXPORT_C Int32 UniPacket::getRequestId() const { return iRequestId; }

EXPORT_C void UniPacket::setRequestId(Int32 value) { iRequestId = value; }

EXPORT_C const String& UniPacket::getServantName() const { return sServantName; }

EXPORT_C void UniPacket::setServantName(const String& value) { sServantName = value; }

EXPORT_C const String& UniPacket::getFuncName() const { return sFuncName; }

EXPORT_C void UniPacket::setFuncName(const String& value) { sFuncName = value; }

void UniPacket::doEncode(taf::JceOutputStream & os)
{
    os.reset();

    os.write(UniAttribute::_data, 0);

    sBuffer.assign(os.getBuffer(), os.getLength());

    os.reset();

    writeTo(os);
}


////////////////////////////////////////////////////////////////////////////////////////////////
EXPORT_C TafUniPacket::TafUniPacket() {}
EXPORT_C TafUniPacket::~TafUniPacket() {}


EXPORT_C void TafUniPacket::setTafVersion(Short value) { iVersion = value; }

EXPORT_C void TafUniPacket::setTafPacketType(Char value) { cPacketType = value; }

EXPORT_C void TafUniPacket::setTafMessageType(Int32 value) { iMessageType = value; }

EXPORT_C void TafUniPacket::setTafTimeout(Int32 value) { iTimeout = value; }

EXPORT_C void TafUniPacket::setTafBuffer(const JArray<Char>& value) { sBuffer = value; }

EXPORT_C void TafUniPacket::setTafContext(const JMapWrapper<String, String>& value) { context = value; }

EXPORT_C void TafUniPacket::setTafStatus(const JMapWrapper<String, String>& value) { status = value; }

EXPORT_C Short TafUniPacket::getTafVersion() const { return iVersion; }

EXPORT_C Char TafUniPacket::getTafPacketType() const { return cPacketType; }

EXPORT_C Int32 TafUniPacket::getTafMessageType() const { return iMessageType; }

EXPORT_C Int32 TafUniPacket::getTafTimeout() const { return iTimeout; }

EXPORT_C const JArray<Char>& TafUniPacket::getTafBuffer() const { return sBuffer; }

EXPORT_C const JMapWrapper<String, String>& TafUniPacket::getTafContext() const { return context; }

EXPORT_C const JMapWrapper<String, String>& TafUniPacket::getTafStatus() const { return status; }

EXPORT_C taf::Int32 TafUniPacket::getTafResultCode() const
{
    const JArray<JMapPair<String, String> > & arr = this->status.getArray();
    for(unsigned int i = 0; i < arr.size(); ++i)
    {
        if(arr[i].first == STATUS_RESULT_CODE)
        {
            return atoi((const char*)arr[i].second.c_str());
        }
    }
    return 0;
}

EXPORT_C String TafUniPacket::getTafResultDesc() const
{
    const JArray<JMapPair<String, String> > & arr = this->status.getArray();
    for(unsigned int i = 0; i < arr.size(); ++i)
    {
        if(arr[i].first == STATUS_RESULT_DESC)
        {
            return arr[i].second;
        }
    }
    return (Char*)"";
}

}
    
    
    
