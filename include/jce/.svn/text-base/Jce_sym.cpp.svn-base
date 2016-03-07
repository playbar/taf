#include "Jce_sym.h"



namespace taf
{

/************************************************************************/
/* JString 类实现                                                       */
/************************************************************************/
EXPORT_C JString::JString()
{
    _data.append('\0');
}

EXPORT_C JString::JString(const JString& r)
{
    _data = r._data;
}

//howlpan EXPORT_C JString::JString(const char * s, size_type len)
EXPORT_C JString::JString(const signed char * s, size_type len)
{
    assign(s, len);
}

//howlpan EXPORT_C JString::JString(const char * s)
EXPORT_C JString::JString(const signed char * s)
{
    assign(s, strlen((char*)s));
}

//howlpan EXPORT_C char& JString::operator[](size_type n)  
EXPORT_C signed char& JString::operator[](size_type n)  
{ 
    return _data[n]; 
}

//howlpan EXPORT_C const char& JString::operator[](size_type n) const
EXPORT_C const signed char& JString::operator[](size_type n) const
{ 
    return _data[n]; 
}

EXPORT_C JString& JString::operator=(const JString& r)
{
    _data = r._data;
    return *this;
}

//howlpan EXPORT_C JString& JString::operator=(const char * r)
EXPORT_C JString& JString::operator=(const signed char * r)
{
    assign(r, strlen((char*)r));
    return *this;
}

EXPORT_C bool JString::operator==(const JString& r) const
{
    return _data == r._data;
}

EXPORT_C bool JString::operator<(const JString& r) const
{
    return _data < r._data;
}

//howlpan EXPORT_C void JString::assign(const char * r, size_type len)
EXPORT_C void JString::assign(const signed char * r, size_type len)
{
    _data.assign(r, len);
    _data.append('\0');
}

//howlpan EXPORT_C void JString::assign(const char * f, const char * l)    
EXPORT_C void JString::assign(const signed char * f, const signed char * l)    
{ 
    assign(f, l - f); 
}

EXPORT_C void JString::append(const signed char * r, size_type len)
{
    _data.resize(_data.size() - 1);
    _data.append(r, len);
    _data.append('\0'); 
}

EXPORT_C void JString::append(const JString &s)
{
    append(s.data(), s.size());
}

EXPORT_C JString::size_type JString::length() const  
{ 
    return size(); 
}

EXPORT_C JString::size_type JString::size() const        
{ 
    return _data.size() - 1; 
}

//howlpan EXPORT_C const char* JString::data() const   
EXPORT_C const signed char* JString::data() const   
{ 
    return c_str(); 
}

//howlpan EXPORT_C const char* JString::c_str() const  
EXPORT_C const signed char* JString::c_str() const  
{ 
    return _data.data(); 
}

EXPORT_C void JString::AppendData(unsigned char * r, size_type len) 
{ 
    append((const signed char*)r, len);
}

EXPORT_C void JString::AppendData(const JString &s)
{ 
    append(s);
}

EXPORT_C void JString::ToString( int nNum )
{
    int nBitCount = 0 ;
    bool bPositive = (bool)((nNum >= 0) ? true : false) ;
    int nNumber = (bPositive==true) ? nNum : nNum * (-1) ;
    
    //计算位数
    do {
        nNumber = nNumber/10 ;
        nBitCount ++ ;
    } while( nNumber > 0 );
    
    if( bPositive == false )
        nBitCount ++ ;

    _data.resize(nBitCount);
    
    nNumber = (bPositive==true) ? nNum : (nNum * (-1)) ;
    int nUnit = 0 ;
    do
    {
        nUnit = nNumber % 10 ;
        nNumber = nNumber/10 ;
        
        nBitCount -- ;
        switch(nUnit)
        {
        case 0 : _data[nBitCount] = L'0' ;    break;
        case 1 : _data[nBitCount] = L'1' ;    break;
        case 2 : _data[nBitCount] = L'2' ;    break;
        case 3 : _data[nBitCount] = L'3' ;    break;
        case 4 : _data[nBitCount] = L'4' ;    break;
        case 5 : _data[nBitCount] = L'5' ;    break;
        case 6 : _data[nBitCount] = L'6' ;    break;
        case 7 : _data[nBitCount] = L'7' ;    break;
        case 8 : _data[nBitCount] = L'8' ;    break;
        case 9 : _data[nBitCount] = L'9' ;    break;
        default: break ;
        }
    } while( nNumber > 0 ) ;
    
    if( bPositive == false )
        _data[0] = L'-' ;
    
    _data.append('\0');
}

EXPORT_C bool JString::IsEmpty() 
{
    return _data.size() == 0;
}

EXPORT_C void JString::Empty()   
{
    _data.clear();
}


/************************************************************************/
/*  BufferReader 类实现                                                 */
/************************************************************************/
EXPORT_C BufferReader::BufferReader() : _cur(0)
{
    // No implementation required
}

EXPORT_C BufferReader::~BufferReader()
{
}

EXPORT_C BufferReader* BufferReader::NewLC()
{
    BufferReader* self = new (ELeave)BufferReader();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

EXPORT_C BufferReader* BufferReader::NewL()
{
    BufferReader* self=BufferReader::NewLC();
    CleanupStack::Pop(); // self;
    return self;
}

void BufferReader::ConstructL()
{

}

EXPORT_C void BufferReader::reset()
{
    _cur = 0;
}

EXPORT_C void BufferReader::readBuf(void * buf, unsigned int len)
{
    peekBuf(buf, len);
    _cur += len;
}

EXPORT_C void BufferReader::peekBuf(void * buf, unsigned int len, unsigned int offset)
{
    if(_cur + offset + len > _buf.size())
    {
        char s[64];
        String si;
        si.ToString(_buf.size());
        strncpy(s, "buffer overflow when peekBuf, over ", sizeof(s)-1);
        strncat(s, (const char*)si.data(), sizeof(s)-strlen(s)-1 );
        _sLastError = (signed char*)s;
        User::Leave(-4);
    }
    
    ::memcpy(buf, &_buf[_cur + offset], len);
}

EXPORT_C void BufferReader::skip(unsigned int len)
{
    if (_cur + len > _buf.size())
    {
        String si;
        si.ToString(_buf.size());
        
        char s[64];
        strncpy(s, "buffer overflow when peekBuf, over ", sizeof(s)-1);
        strncat(s, (const char*)si.data(), sizeof(s)-strlen(s)-1 );
        _sLastError = (signed char*)s;
        User::Leave(-4);
    }
    
    _cur += len;
}

EXPORT_C void BufferReader::setBuffer(const signed char * buf, unsigned int len)
{
    _buf.assign(buf, len);
    _cur = 0;
}

EXPORT_C String BufferReader::getLastError() 
{
    return _sLastError; 
}


/************************************************************************/
/* BufferWriter 类实现                                                  */
/************************************************************************/
EXPORT_C BufferWriter::BufferWriter()
{
    // No implementation required
}

EXPORT_C BufferWriter::~BufferWriter()
{
}

EXPORT_C BufferWriter* BufferWriter::NewLC()
{
    BufferWriter* self = new (ELeave)BufferWriter();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

EXPORT_C BufferWriter* BufferWriter::NewL()
{
    BufferWriter* self=BufferWriter::NewLC();
    CleanupStack::Pop(); // self;
    return self;
}

void BufferWriter::ConstructL()
{

}

EXPORT_C void BufferWriter::reset() 
{ 
    _buf.clear();
}

EXPORT_C void BufferWriter::writeBuf(const void * buf, unsigned int len)
{
    //howlpan char * p = (char *) buf;
    signed char * p = (signed char *) buf;
    _buf.append(p, len);
}

//howlpan EXPORT_C const char * BufferWriter::getBuffer() const    
EXPORT_C const signed char * BufferWriter::getBuffer() const    
{ 
    return _buf.data(); 
}

EXPORT_C unsigned int BufferWriter::getLength() const        
{ 
    return _buf.size(); 
}

EXPORT_C String BufferWriter::getLastError()
{ 
    return _sLastError; 
}


/************************************************************************/
/* DataHead 类实现                                                      */
/************************************************************************/
DataHead::DataHead(uint8_t type, uint8_t tag) : _type(type), 
                                                _tag(tag)
{
    // No implementation required
}

EXPORT_C DataHead::~DataHead()
{
}

EXPORT_C DataHead* DataHead::NewLC(uint8_t type, uint8_t tag)
{
    DataHead* self = new (ELeave)DataHead(type, tag);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

EXPORT_C DataHead* DataHead::NewL(uint8_t type, uint8_t tag)
{
    DataHead* self=DataHead::NewLC(type, tag);
    CleanupStack::Pop(); // self;
    return self;
}

void DataHead::ConstructL()
{

}

EXPORT_C uint8_t DataHead::getTag() const        
{ 
    return _tag; 
}

EXPORT_C void DataHead::setTag(uint8_t t)    
{ 
    _tag = t; 
}

EXPORT_C uint8_t DataHead::getType() const       
{ 
    return _type; 
}

EXPORT_C void DataHead::setType(uint8_t t)       
{ 
    _type = t; 
}

EXPORT_C void DataHead::readFrom(JceInputStream& is)
 {
     unsigned int n = peekFrom(is);
     is.skip(n);
 }

EXPORT_C  unsigned int DataHead::peekFrom(JceInputStream& is)
 {
     helper h;
     unsigned int n = 1;
     is.peekBuf(&h, 1);
     _type = h.type;
     if(h.tag == 15){
         is.peekBuf(&_tag, 1, 1);
         n += 1;
     }else{
         _tag = h.tag;
     }
     return n;
 }
 /**
  * 写入数据头信息
  * @param os ： 模板类型参数。
  */
EXPORT_C void DataHead::writeTo(JceOutputStream& os)
 {
     helper h;
     h.type = _type;
     
     if(_tag < 15)
     {
         //标签小于15时，头信息只占1字节的大小。
         h.tag = _tag;
         os.writeBuf(&h, 1);
     }
     else
     {
         //标签大于等于15时，头信息占2字节大小。 第一个字节用于type和tag1占用。
         //第二个字节用于tag2占用，即真正的标签值
         h.tag = 15;
         os.writeBuf(&h, 1);
         os.writeBuf(&_tag, 1);
     }
 }


 /************************************************************************/
 /* JceInputStream 类实现                                                */
 /************************************************************************/
EXPORT_C JceInputStream::JceInputStream()
{
    // No implementation required
}

EXPORT_C JceInputStream::~JceInputStream()
{
}

EXPORT_C JceInputStream* JceInputStream::NewLC()
{
    JceInputStream* self = new (ELeave)JceInputStream();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

EXPORT_C JceInputStream* JceInputStream::NewL()
{
    JceInputStream* self=JceInputStream::NewLC();
    CleanupStack::Pop(); // self;
    return self;
}

void JceInputStream::ConstructL()
{

}

void JceInputStream::error(const String &t1, uint8_t tag, unsigned char type, const String &t2, Int32 i)
{
    char s[128];

    String sTag;    
    sTag.ToString(tag);
    
    String sInfo;    
    sInfo.ToString(i);

    String sTypeH;  
    sTypeH.ToString(type);

    strncpy(s, (const char*)t1.data(), sizeof(s)-1);
    strncat(s, ", tag:", sizeof(s)-1);
    strncat(s, (const char*)sTag.data(), sizeof(s)-strlen(s)-1);
    strncat(s, ", type:", sizeof(s)-strlen(s)-1);
    strncat(s, (const char*)sTypeH.data(), sizeof(s)-strlen(s)-1);
    strncat(s, ", ", sizeof(s)-strlen(s)-1);
    strncat(s, (const char*)t2.data(), sizeof(s)-strlen(s)-1);
    strncat(s, ": ", sizeof(s)-strlen(s)-1);
    strncat(s, (const char*)sInfo.data(), sizeof(s)-strlen(s)-1);

    _sLastError = (signed char*)s;
}

EXPORT_C void JceInputStream::errorRequire(uint8_t tag)
{
    String sTag;   
    sTag.ToString(tag);
    
    char s[64];
    strncpy(s, "require field not exist, tag: ", sizeof(s)-1 );
    strncat(s, (const char*)sTag.data(), sizeof(s)-strlen(s)-1);

    _sLastError = (signed char*)s;

    User::Leave(-4);
}

EXPORT_C bool JceInputStream::skipToTag(uint8_t tag)
{
    DataHead* dataHead = DataHead::NewL();
    CleanupStack::PushL(dataHead);
    
    while(true)
    {
        size_t len = 0;

        TRAPD(error1, len = dataHead->peekFrom(*this); );
        if(error1)
        {
            CleanupStack::PopAndDestroy(dataHead);
            return false;
        }

        if(tag <= dataHead->getTag() || dataHead->getType() == DataHead::eStructEnd)
        {
            uint8_t findTag     = dataHead->getTag();
            uint8_t findType    = dataHead->getType();
            CleanupStack::PopAndDestroy(dataHead);

            return findType == DataHead::eStructEnd?false:(tag == findTag);
        }

        BufferReader::skip(len);

        TRAPD(error2, skipField(dataHead->getType());  );
        if(error2)
        {
            CleanupStack::PopAndDestroy(dataHead);
            return false;
        }
    }

    CleanupStack::PopAndDestroy(dataHead);
    
    return false;
}

EXPORT_C void JceInputStream::skipToStructEnd()
{
    DataHead* dataHead = DataHead::NewL();
    CleanupStack::PushL(dataHead);
    
    do{
        dataHead->readFrom(*this);
        skipField(dataHead->getType());
    }while(dataHead->getType() != DataHead::eStructEnd);
    
    CleanupStack::PopAndDestroy(dataHead);
}

EXPORT_C void JceInputStream::skipField()
{
    DataHead* dataHead = DataHead::NewL();
    CleanupStack::PushL(dataHead);
    
    dataHead->readFrom(*this);
    skipField(dataHead->getType());
    
    CleanupStack::PopAndDestroy(dataHead);
}

EXPORT_C void JceInputStream::skipField(uint8_t type)
{
    switch(type){
    case DataHead::eChar:
        this->skip(sizeof(Char));
        break;
    case DataHead::eShort:
        this->skip(sizeof(Short));
        break;
    case DataHead::eInt32:
        this->skip(sizeof(Int32));
        break;
    case DataHead::eInt64:
        this->skip(sizeof(Int64));
        break;
    case DataHead::eFloat:
        this->skip(sizeof(Float));
        break;
    case DataHead::eDouble:
        this->skip(sizeof(Double));
        break;
    case DataHead::eString1:
        {
            uint8_t n;
            readBuf(&n, sizeof(n));
            size_t len = n;
            this->skip(len);
        }
        break;
    case DataHead::eString4:
        {
            UInt32 n;
            readBuf(&n, sizeof(n));
            size_t len =  jce_ntohl(n);
            this->skip(len);
        }
        break;
    case DataHead::eMap:
        {
            Int32 size;
            read(size, 0);
            for(Int32 i = 0; i < size * 2; ++i)
                skipField();
        }
        break;
    case DataHead::eList:
        {
            Int32 size;
            read(size, 0);
            for(Int32 i = 0; i < size; ++i)
                skipField();
        }
        break;
    case DataHead::eSimpleList:
        {
            DataHead* dataHead = DataHead::NewL();
            CleanupStack::PushL(dataHead);
                
            dataHead->readFrom(*this);
            if(dataHead->getType() != DataHead::eChar)
            {

                error((Char*)"skipField with invalid type", 0, type, (Char*)"type", dataHead->getType());

                CleanupStack::PopAndDestroy(dataHead);
               
                User::Leave(-4);
            }
            Int32 size;
            read(size, 0);
            this->skip(size);
            
            CleanupStack::PopAndDestroy(dataHead);
        }
        break;
    case DataHead::eStructBegin:
        skipToStructEnd();
        break;
    case DataHead::eStructEnd:
    case DataHead::eZeroTag:
        break;
    default:
        {
            error((Char*)"skipField with invalid type", 0, type, (Char*)"type", 0);

            User::Leave(-4);
        }
    }
}

EXPORT_C Char JceInputStream::readChar()
{
    Char n;
    readBuf(&n, sizeof(n));
    return n;
}

EXPORT_C Short JceInputStream::readShort()
{
    Short n;
    readBuf(&n, sizeof(n));
    return n;
}

EXPORT_C Int32 JceInputStream::readInt32()
{
    Int32 n;
    readBuf(&n, sizeof(n));
    return n;
}
//add by edwardsu

EXPORT_C void JceInputStream::read(UInt8& c, uint8_t tag, bool isRequire )
{
    Short i = c;
    read(i,tag,isRequire);
    c = (UInt8)i;
}
    
EXPORT_C void JceInputStream::read(UInt16& n, uint8_t tag, bool isRequire )
{
    Int32 i = n;
    read(i,tag,isRequire);
    n = (UInt16)i;
}
    
EXPORT_C void JceInputStream::read(UInt32& n, uint8_t tag, bool isRequire )
{   
    Int64 i = n;
    read(i,tag,isRequire);
    n = (UInt32)i;
}
//end add

EXPORT_C void JceInputStream::read(Bool& b, uint8_t tag, bool isRequire)
{
    Char c = b;
    read(c, tag, isRequire);
    b = c ? true : false;
}

EXPORT_C void JceInputStream::read(Char& c, uint8_t tag, bool isRequire)
{
    if(skipToTag(tag))
    {
        DataHead* dataHead = DataHead::NewL();
        CleanupStack::PushL(dataHead);
        
        dataHead->readFrom(*this);
        switch(dataHead->getType())
        {
        case DataHead::eZeroTag:
            c = 0;
            break;
        case DataHead::eChar:
            this->readBuf(&c, sizeof(c));
            break;
        default:
            {
                error((Char*)"read 'Char' type mismatch", tag, dataHead->getType(), (Char*)"size", 0);

                CleanupStack::PopAndDestroy(dataHead);
                
                User::Leave(-4);
            }
        }
        CleanupStack::PopAndDestroy(dataHead);
    }
    else if(isRequire)
    {
        errorRequire(tag);
    }
}

EXPORT_C void JceInputStream::read(Short& n, uint8_t tag, bool isRequire)
{
    if(skipToTag(tag))
    {
        DataHead* dataHead = DataHead::NewL();
        CleanupStack::PushL(dataHead);
                
        dataHead->readFrom(*this);
        switch(dataHead->getType())
        {
        case DataHead::eZeroTag:
            n = 0;
            break;
        case DataHead::eChar:
            n = readChar();
            break;
        case DataHead::eShort:
            this->readBuf(&n, sizeof(n));
            n = jce_ntohs(n);
            break;
        default:
            {
                error((Char*)"read 'Short' type mismatch", tag, dataHead->getType(), (Char*)"size", 0);

                CleanupStack::PopAndDestroy(dataHead);
                
                User::Leave(-4);
            }
        }
        
        CleanupStack::PopAndDestroy(dataHead);
    }
    else if(isRequire)
    {
        errorRequire(tag);
    }
}

EXPORT_C void JceInputStream::read(Int32& n, uint8_t tag, bool isRequire)
{
    if(skipToTag(tag))
    {
        DataHead* dataHead = DataHead::NewL();
        CleanupStack::PushL(dataHead);
        
        dataHead->readFrom(*this);
        switch(dataHead->getType())
        {
        case DataHead::eZeroTag:
            n = 0;
            break;
        case DataHead::eChar:
            n = readChar();
            break;
        case DataHead::eShort:
            n = (Short) readShort();
            n = (Short) jce_ntohs(n);
            break;
        case DataHead::eInt32:
            this->readBuf(&n, sizeof(n));
            n = jce_ntohl(n);
            break;
        default:
            {
                error((Char*)"read 'Int32' type mismatch", tag, dataHead->getType(), (Char*)"size", 0);

                CleanupStack::PopAndDestroy(dataHead);
                
                User::Leave(-4);
            }
        }
        CleanupStack::PopAndDestroy(dataHead);
    }
    else if(isRequire)
    {
        errorRequire(tag);
    }
}

EXPORT_C void JceInputStream::read(Int64& n, uint8_t tag, bool isRequire)
{
    if(skipToTag(tag))
    {
        DataHead* dataHead = DataHead::NewL();
        CleanupStack::PushL(dataHead);
                
        dataHead->readFrom(*this);
        switch(dataHead->getType())
        {
        case DataHead::eZeroTag:
            n = 0;
            break;
        case DataHead::eChar:
            n = readChar();
            break;
        case DataHead::eShort:
            n = (Short) readShort();
            n = (Short) jce_ntohs(n);
            break;
        case DataHead::eInt32:
            n = (Int32) readInt32();
            n = (Int32) jce_ntohl(n);
            break;
        case DataHead::eInt64:
            this->readBuf(&n, sizeof(n));
            n = jce_ntohll(n);
            break;
        default:
            {
                error((Char*)"read 'Int64' type mismatch", tag, dataHead->getType(), (Char*)"size", 0);

                CleanupStack::PopAndDestroy(dataHead);
                
                User::Leave(-4);
            }
        }
        CleanupStack::PopAndDestroy(dataHead);
    }
    else if(isRequire)
    {
        errorRequire(tag);
    }
}

EXPORT_C void JceInputStream::read(Float& n, uint8_t tag, bool isRequire)
{
    if(skipToTag(tag))
    {
        DataHead* dataHead = DataHead::NewL();
        CleanupStack::PushL(dataHead);
                
        dataHead->readFrom(*this);
        switch(dataHead->getType())
        {
        case DataHead::eZeroTag:
            n = 0;
            break;
        case DataHead::eFloat:
            this->readBuf(&n, sizeof(n));
            n = jce_ntohf(n);
            break;
        default:
            {
                error((Char*)"read 'Float' type mismatch", tag, dataHead->getType(), (Char*)"size", 0);

                CleanupStack::PopAndDestroy(dataHead);
                
                User::Leave(-4);
            }
        }
        CleanupStack::PopAndDestroy(dataHead);
    }
    else if(isRequire)
    {
        errorRequire(tag);
    }
}

EXPORT_C void JceInputStream::read(Double& n, uint8_t tag, bool isRequire)
{
    if(skipToTag(tag))
    {
        DataHead* dataHead = DataHead::NewL();
        CleanupStack::PushL(dataHead);
        
        dataHead->readFrom(*this);
        switch(dataHead->getType())
        {
        case DataHead::eZeroTag:
            n = 0;
            break;
        case DataHead::eFloat:
            {
                Float n1;
                readBuf(&n1, sizeof(n1));
                n = n1;
                break;
            }
        case DataHead::eDouble:
            this->readBuf(&n, sizeof(n));
            n = jce_ntohd(n);
            break;
        default:
            {
                error((Char*)"read 'Double' type mismatch", tag, dataHead->getType(), (Char*)"size", 0);

                CleanupStack::PopAndDestroy(dataHead);
                
                User::Leave(-4);
            }
        }
        CleanupStack::PopAndDestroy(dataHead);
    }
    else if(isRequire)
    {
        errorRequire(tag);
    }
}

EXPORT_C void JceInputStream::read(String& s, uint8_t tag, bool isRequire)
{
    if(skipToTag(tag))
    {
        DataHead* dataHead = DataHead::NewL();
        CleanupStack::PushL(dataHead);
                
        dataHead->readFrom(*this);
        switch(dataHead->getType())
        {
        case DataHead::eString1:
            {
                uint8_t n;
                this->readBuf(&n, sizeof(n));
                size_t len = n;
                Char ss[256];
                this->readBuf(ss, len);
                s.assign(ss, ss + len);
            }
            break;
        case DataHead::eString4:
            {
                UInt32 len;
                readBuf(&len, sizeof(len));
                len = jce_ntohl(len);
                if(len > JCE_MAX_STRING_LENGTH)
                {

                    error((Char*)"invalid string size", tag, dataHead->getType(), (Char*)"size", len);

                    CleanupStack::PopAndDestroy(dataHead);
                    
                    User::Leave(-4);
                }
                Char *ss = new Char[len];
                TRAPD(error, this->readBuf(ss, len););
                if(error)
                {
                    delete[] ss;
                    User::Leave(-4);
                }
                s.assign(ss, ss + len);
                delete[] ss;
            }
            break;
        default:
            {
                error((Char*)"read 'string' type mismatch", tag, dataHead->getType(), (Char*)"size", 0);

                CleanupStack::PopAndDestroy(dataHead);
                
                User::Leave(-4);
            }
        }
        CleanupStack::PopAndDestroy(dataHead);
    }
    else if(isRequire)
    {
        errorRequire(tag);
    }
}

EXPORT_C void JceInputStream::read(JceStructBase& v, uint8_t tag, bool isRequire)
{
    if(skipToTag(tag))
    {
        DataHead* dataHead = DataHead::NewL();
        CleanupStack::PushL(dataHead);
                    
        dataHead->readFrom(*this);
        if(dataHead->getType() != DataHead::eStructBegin)
        {
            error((Char*)"read 'struct' type mismatch", tag, dataHead->getType(), (Char*)"size", 0);

            CleanupStack::PopAndDestroy(dataHead);
            
            User::Leave(-4);
        }
        
        v.readFrom(*this);
        skipToStructEnd();
        
        CleanupStack::PopAndDestroy(dataHead);
    }
    else if(isRequire)
    {
        errorRequire(tag);
    }
}

EXPORT_C bool JceInputStream::readVectorChar(uint8_t tag, Int32 &size)
{
    DataHead* dataHead = DataHead::NewL();
    CleanupStack::PushL(dataHead);

    dataHead->readFrom(*this);
    switch(dataHead->getType())
    {
    case DataHead::eSimpleList:
        {
            DataHead* dataHead2 = DataHead::NewL();
            CleanupStack::PushL(dataHead2);
        
            dataHead2->readFrom(*this);
            if(dataHead2->getType() != DataHead::eChar)
            {
                error((Char*)"type mismatch", tag, dataHead->getType(), (Char*)"type", dataHead2->getType());

                CleanupStack::PopAndDestroy(dataHead2);
                CleanupStack::PopAndDestroy(dataHead);
            
                User::Leave(-4);
            }
            read(size, 0);
            if(size < 0)
            {
                error((Char*)"invalid vector size", tag, dataHead->getType(), (Char*)"size", size);

                CleanupStack::PopAndDestroy(dataHead2);
                CleanupStack::PopAndDestroy(dataHead);
            
                User::Leave(-4);
            }
                    
            CleanupStack::PopAndDestroy(dataHead2);
            CleanupStack::PopAndDestroy(dataHead);
        }
        return true;   
    default:
        {
            error((Char*)"type mismatch", tag, dataHead->getType(), (Char*)"size", 0);

            CleanupStack::PopAndDestroy(dataHead);
        
            User::Leave(-4);
        }
    }

    CleanupStack::PopAndDestroy(dataHead);
    return false;
}

EXPORT_C bool JceInputStream::readVector(uint8_t tag, Int32 &size)
{
    DataHead* dataHead = DataHead::NewL();
    CleanupStack::PushL(dataHead);
            
    dataHead->readFrom(*this);
    switch(dataHead->getType())
    {
    case DataHead::eList:
        {
            read(size, 0);
            if(size < 0)
            {
                error((Char*)"invalid size", tag, dataHead->getType(), (Char*)"size", size);

                CleanupStack::PopAndDestroy(dataHead);
            
                User::Leave(-4);
            }

            CleanupStack::PopAndDestroy(dataHead);
        }
        return true;
    default:
        {
            error((Char*)"read 'vector' type mismatch", tag, dataHead->getType(), (Char*)"size", 0);

            CleanupStack::PopAndDestroy(dataHead);
        
            User::Leave(-4);
        }
    }

    CleanupStack::PopAndDestroy(dataHead);

    return false;
}

bool JceInputStream::readMap(uint8_t tag, Int32 &size)
{
    DataHead* dataHead = DataHead::NewL();
    CleanupStack::PushL(dataHead);
            
    dataHead->readFrom(*this);
    switch(dataHead->getType())
    {
    case DataHead::eMap:
        {
            read(size, 0);
            if(size < 0)
            {
                error((Char*)"invalid map size", tag, dataHead->getType(), (Char*)"size", size);

                CleanupStack::PopAndDestroy(dataHead);
                
                User::Leave(-4);
            }                   

            CleanupStack::PopAndDestroy(dataHead);
            return true;
        }
        break;
    default:
        {
            error((Char*)"invalid map type", tag, dataHead->getType(), (Char*)"size", 0);

            CleanupStack::PopAndDestroy(dataHead);
            
            User::Leave(-4);
        }
    }
    CleanupStack::PopAndDestroy(dataHead);
    return false;
}

/************************************************************************/
/*  JceOutputStream 类实现                                              */
/************************************************************************/
EXPORT_C JceOutputStream::JceOutputStream()
{
    // No implementation required
}

EXPORT_C JceOutputStream::~JceOutputStream()
{
}

EXPORT_C JceOutputStream* JceOutputStream::NewLC()
{
    JceOutputStream* self = new (ELeave)JceOutputStream();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

EXPORT_C JceOutputStream* JceOutputStream::NewL()
{
    JceOutputStream* self=JceOutputStream::NewLC();
    CleanupStack::Pop(); // self;
    return self;
}

void JceOutputStream::ConstructL()
{
    
}

EXPORT_C void JceOutputStream::write(Bool b, uint8_t tag)
{
    Char c = (Char) (b ? 1 : 0);
    write(c, tag);
}

EXPORT_C void JceOutputStream::write(Char n, uint8_t tag)
{
    DataHead* dataHead = DataHead::NewL(DataHead::eChar, tag);
    CleanupStack::PushL(dataHead);

    if(0 == n)
    {
        //为0情况，没有紧跟数据。只写头。
        dataHead->setType(DataHead::eZeroTag);
        dataHead->writeTo(*this);
    }
    else
    {
        //不为0，写完头，后紧跟数据
        dataHead->writeTo(*this);
        this->writeBuf(&n, sizeof(n));
    }
    
    CleanupStack::PopAndDestroy(dataHead);
}

EXPORT_C void JceOutputStream::write(Short n, uint8_t tag)
{
    //if(n >= (Short)(CHAR_MIN) && n <= (Short)CHAR_MAX)
    if(n >= (-128) && n <= 127)
    {
        write((Char) n, tag);
    }
    else
    {
        DataHead* dataHead = DataHead::NewL(DataHead::eShort, tag);
        CleanupStack::PushL(dataHead);

        dataHead->writeTo(*this);
        n = jce_htons(n);
        this->writeBuf(&n, sizeof(n));
        
        CleanupStack::PopAndDestroy(dataHead);
    }
}

EXPORT_C void JceOutputStream::write(Int32 n, uint8_t tag)
{
    //if(n >= SHRT_MIN && n <= SHRT_MAX)
    if(n >= (-32768) && n <= 32767)
    {
        write((Short) n, tag);
    }
    else
    {
        DataHead* dataHead = DataHead::NewL(DataHead::eInt32, tag);
        CleanupStack::PushL(dataHead);
        
        dataHead->writeTo(*this);
        n = jce_htonl(n);
        this->writeBuf(&n, sizeof(n));
        
        CleanupStack::PopAndDestroy(dataHead);
    }
}

//add by edwardsu
EXPORT_C void JceOutputStream::write(UInt8 n, uint8_t tag)
{    
    write((Short)n,tag);
}
EXPORT_C void JceOutputStream::write(UInt16 n, uint8_t tag)
{
    write((Int32)n,tag);
}
EXPORT_C void JceOutputStream::write(UInt32 n, uint8_t tag)
{
    write((Int64)n,tag);
}
//end add

EXPORT_C void JceOutputStream::write(Int64 n, uint8_t tag)
{
    //if(n >= INT_MIN && n <= INT_MAX)
    if(n >= (-2147483647-1) && n <= 2147483647)
    {
        write((Int32) n, tag);
    }
    else
    {
        DataHead* dataHead = DataHead::NewL(DataHead::eInt64, tag);
        CleanupStack::PushL(dataHead);
                
        dataHead->writeTo(*this);
        n = jce_htonll(n);
        this->writeBuf(&n, sizeof(n));
        
        CleanupStack::PopAndDestroy(dataHead);
    }
}

EXPORT_C void JceOutputStream::write(Float n, uint8_t tag)
{
    DataHead* dataHead = DataHead::NewL(DataHead::eFloat, tag);
    CleanupStack::PushL(dataHead);
            
    dataHead->writeTo(*this);
    n = jce_htonf(n);
    this->writeBuf(&n, sizeof(n));
    
    CleanupStack::PopAndDestroy(dataHead);
}

EXPORT_C void JceOutputStream::write(Double n, uint8_t tag)
{
    DataHead* dataHead = DataHead::NewL(DataHead::eDouble, tag);
    CleanupStack::PushL(dataHead);

    dataHead->writeTo(*this);
    n = jce_htond(n);
    this->writeBuf(&n, sizeof(n));
    
    CleanupStack::PopAndDestroy(dataHead);
}

EXPORT_C void JceOutputStream::write(const String& s, uint8_t tag)
{
    DataHead* dataHead = DataHead::NewL(DataHead::eString1, tag);
    CleanupStack::PushL(dataHead);
        
    if(s.size() > 255)
    {
        if(s.size() > JCE_MAX_STRING_LENGTH)
        {
            String sTag;    
            sTag.ToString(tag);
            
            String sSize;  
            sSize.ToString(s.size());
            
            char ss[128];
            strncpy(ss, "invalid string size, tag:", sizeof(ss)-1 );
            strncat(ss, (const char*)sTag.data(), sizeof(ss)-strlen(ss)-1);
            strncat(ss, ", size: ", sizeof(ss)-strlen(ss)-1);
            strncat(ss, (const char*)sSize.data(), sizeof(s)-strlen(ss)-1);

            CleanupStack::PopAndDestroy(dataHead);
            
            _sLastError = s;
            User::Leave(-4);
        }
 
        dataHead->setType(DataHead::eString4);
        dataHead->writeTo(*this);
        UInt32 n = jce_htonl(s.size());
        this->writeBuf(&n, sizeof(n));
        this->writeBuf(s.data(), s.size());

    }else
    {
        dataHead->writeTo(*this);
        uint8_t n = s.size();
        this->writeBuf(&n, sizeof(n));
        this->writeBuf(s.data(), s.size());
    }
    
    CleanupStack::PopAndDestroy(dataHead);
}

EXPORT_C void JceOutputStream::write(const JceStructBase& v, uint8_t tag)
{
    DataHead* dataHead = DataHead::NewL(DataHead::eStructBegin, tag);
    CleanupStack::PushL(dataHead);
            
    dataHead->writeTo(*this);
    v.writeTo(*this);
    dataHead->setType(DataHead::eStructEnd);
    dataHead->setTag(0);
    dataHead->writeTo(*this);
    
    CleanupStack::PopAndDestroy(dataHead);
}

EXPORT_C void JceOutputStream::writeHead(uint8_t type, uint8_t tag)
{
    DataHead* dataHead = DataHead::NewL(type, tag);

    CleanupStack::PushL(dataHead);

    dataHead->writeTo(*this);

    CleanupStack::PopAndDestroy(dataHead);
}
}
