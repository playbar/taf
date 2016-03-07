#include "wup_c.h"
#include "Jce_c.h"

///////////////////////////////////////////////////////////////
//put
static Int32 WUP_putAttribute_V2(UniAttribute *pack, const char * name, const char * type, JceOutputStream * data_value)
{
	Int32 ret = 0;
	JceOutputStream *os_map_first=NULL, *os_map_second=NULL;

	UniAttribute * attr = pack;

	os_map_first = JceOutputStream_new();
	os_map_second= JceOutputStream_new();

	JMapWrapper_clear(attr->m_data);

	if (!os_map_second || !os_map_first)
	{
		ret = JCE_MALLOC_ERROR; 
		goto do_clean;
	}

	JceOutputStream_reset(attr->os_string);
	ret = JceOutputStream_writeStringBuffer(attr->os_string, type, strlen(type), 0);
	if (ret) goto do_clean;

	ret = JceOutputStream_writeVectorCharBuffer(os_map_second, JceOutputStream_getBuffer(data_value), JceOutputStream_getLength(data_value), 1);
	if (ret) goto do_clean;

	ret = JMapWrapper_put(attr->m_data, JceOutputStream_getBuffer(attr->os_string), JceOutputStream_getLength(attr->os_string),
						  JceOutputStream_getBuffer(os_map_second), JceOutputStream_getLength(os_map_second));
	if (ret) goto do_clean;

	ret = JceOutputStream_writeMap(os_map_first, attr->m_data, 1);
	if (ret) goto do_clean;

	JceOutputStream_reset(attr->os_string);
	ret = JceOutputStream_writeStringBuffer(attr->os_string, name, strlen(name), 0);
	if (ret) goto do_clean;

	ret = JMapWrapper_put(attr->_data, JceOutputStream_getBuffer(attr->os_string), JceOutputStream_getLength(attr->os_string),
						  JceOutputStream_getBuffer(os_map_first), JceOutputStream_getLength(os_map_first));
	if (ret) goto do_clean;

do_clean:
	if (os_map_first)	JceOutputStream_del(&os_map_first);
	if (os_map_second)	JceOutputStream_del(&os_map_second);

	return ret;
}

static Int32 WUP_putAttribute_V3(UniAttribute *pack, const char * name, const char * type, JceOutputStream * data_value)
{
	Int32 ret 						= 0;
	UniAttribute * attr 			= pack;
	JceOutputStream * os_map_value 	= JceOutputStream_new();
	if (!os_map_value)
	{
		ret = JCE_MALLOC_ERROR; 
		goto do_clean;
	}


	JceOutputStream_reset(attr->os_string);
	ret = JceOutputStream_writeStringBuffer(attr->os_string, name, strlen(name), 0);
	if (ret) goto do_clean;

	
	ret = JceOutputStream_writeVectorCharBuffer(os_map_value, JceOutputStream_getBuffer(data_value), JceOutputStream_getLength(data_value), 1);
	if (ret) goto do_clean;

	ret = JMapWrapper_put(attr->_data, JceOutputStream_getBuffer(attr->os_string), JceOutputStream_getLength(attr->os_string),
									   JceOutputStream_getBuffer(os_map_value), JceOutputStream_getLength(os_map_value));

do_clean:
	if (os_map_value)	JceOutputStream_del(&os_map_value);

	return ret;
}

static Int32 WUP_putAttribute(UniAttribute *pack, const char * name, const char * type, JceOutputStream * data_value)
{
	return pack->_iVer == 3?WUP_putAttribute_V3(pack, name, type, data_value):WUP_putAttribute_V2(pack, name, type, data_value);
}

void WUP_setVersion(void * pack, Short siVersion)
{
	((UniAttribute *)pack)->_iVer = siVersion == 3?3:2;
}

Int32 WUP_putBool(void * pack, const char * name, Bool value)
{
	Int32 ret=0;
	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeBool(attr->value_os, value, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return WUP_putAttribute(pack, name, "bool", attr->value_os);
}

Int32 WUP_putChar  (void * pack, const char * name, Char value)
{
	Int32 ret=0;
	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeChar(attr->value_os, value, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return WUP_putAttribute(pack, name, "char", attr->value_os);
}

Int32 WUP_putUInt8  (void * pack, const char * name, UInt8 value)
{
	Int32 ret=0;
	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeUInt8(attr->value_os, value, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return WUP_putAttribute(pack, name, "short", attr->value_os);
}

Int32 WUP_putShort  (void * pack, const char * name, Short value)
{
	Int32 ret=0;
	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeShort(attr->value_os, value, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return WUP_putAttribute(pack, name, "short", attr->value_os);
}

Int32 WUP_putUInt16 (void * pack, const char * name, UInt16 value)
{
	Int32 ret=0;
	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeUInt16(attr->value_os, value, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return WUP_putAttribute(pack, name, "int32", attr->value_os); 
}

Int32 WUP_putFloat  (void * pack, const char * name, Float value)
{
	Int32 ret=0;
	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeFloat(attr->value_os, value, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return WUP_putAttribute(pack, name, "float", attr->value_os);
}

Int32 WUP_putDouble(void * pack, const char * name, Double value)
{
	Int32 ret=0;
	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeDouble(attr->value_os, value, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return WUP_putAttribute(pack, name, "double", attr->value_os);
}

Int32 WUP_putInt32 (void * pack, const char * name, Int32  value)
{
	Int32 ret=0;
	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeInt32(attr->value_os, value, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return WUP_putAttribute(pack, name, "int32", attr->value_os);
}

Int32 WUP_putUInt32	(void * pack, const char * name, UInt32 value)
{
	Int32 ret=0;
	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeUInt32(attr->value_os, value, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return WUP_putAttribute(pack, name, "int64", attr->value_os);
}


Int32 WUP_putInt64 (void * pack, const char * name, Int64  value)
{
	Int32 ret=0;
	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	JceOutputStream_writeInt64(attr->value_os, value, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return WUP_putAttribute(pack, name, "int64", attr->value_os);
}

Int32 WUP_putString(void * pack, const char * name, JString * value)
{
	return WUP_putStringBuffer(pack, name, JString_data(value), JString_size(value));
}

Int32 WUP_putStringBuffer(void * pack, const char * name, const char* buff, uint32_t len)
{
	Int32 ret=0;
	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeStringBuffer(attr->value_os, buff, len, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return WUP_putAttribute(pack, name, "string", attr->value_os);
}

Int32 WUP_putVector(void * pack, const char * name, JArray * value)
{
	Int32 ret=0;
	char sType[64];

	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeVector(attr->value_os, value, 0);
	if (JCE_SUCCESS != ret)	return ret;

	snprintf(sType, sizeof(sType), "list<%s>", value->elem_type_name);

	return  WUP_putAttribute(pack, name, sType , attr->value_os);
}

Int32 WUP_putVectorChar(void * pack, const char * name,  JString* value)
{
	return WUP_putVectorCharBuffer(pack, name, JString_data(value), JString_size(value));
}

Int32 WUP_putVectorCharBuffer(void * pack, const char * name,  const char* buff, uint32_t len)
{
	Int32 ret=0;

	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeVectorCharBuffer(attr->value_os, buff, len, 0);
	if (JCE_SUCCESS != ret)	return ret;

	return  WUP_putAttribute(pack, name, "list<char>", attr->value_os);
}

Int32 WUP_putMap(void * pack, const char * name, JMapWrapper * value)
{
	Int32 ret=0;
	char sType[64];

	UniAttribute * attr = pack;
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeMap(attr->value_os, value, 0);
	if (JCE_SUCCESS != ret)	return ret;

	snprintf(sType, sizeof(sType), "map<%s,%s>", value->first->elem_type_name, value->second->elem_type_name);
	return WUP_putAttribute(pack, name, sType, attr->value_os);
}

Int32 WUP_putStruct(void * pack, const char * name, const void* st)
{
	Int32 ret=0;
	const JStructBase * jst = st; 
	UniAttribute * attr = pack; 
	JceOutputStream_reset(attr->value_os);
	ret = JceOutputStream_writeStruct(attr->value_os, st, 0);
	if (JCE_SUCCESS != ret)	return ret;
	return WUP_putAttribute(pack, name, jst->className, attr->value_os);
}

/************************************************************************************************** 
 **	获取属性接口
 **
 **/
static Int32 WUP_getAttribute_V2(const UniAttribute *pack, const char * name, const char * type, JString ** s)
{
	Int32 ret = 0;
	char * pBuff = NULL;
	uint32_t len = 0;
	JceInputStream *is_map;

	const UniAttribute * attr = pack;

	JMapWrapper_clear(attr->m_data);

	is_map = JceInputStream_new();

	if (!is_map)
	{
		ret = JCE_MALLOC_ERROR;
		goto do_clean;
	}

	JceOutputStream_reset(attr->value_os);

	JceOutputStream_reset(attr->os_string);
	ret = JceOutputStream_writeStringBuffer(attr->os_string, name, strlen(name), 0);
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JMapWrapper_find(attr->_data, JceOutputStream_getBuffer(attr->os_string), JceOutputStream_getLength(attr->os_string), &pBuff, &len);
	if (JCE_SUCCESS != ret)	goto do_clean;

	JceInputStream_setBuffer(is_map, pBuff, len);

	ret = JceInputStream_readMap(is_map, attr->m_data, 1, true);
	if (JCE_SUCCESS != ret)	goto do_clean;

	JceOutputStream_reset(attr->os_string);
	ret = JceOutputStream_writeStringBuffer(attr->os_string, type, strlen(type), 0);
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JMapWrapper_find(attr->m_data, JceOutputStream_getBuffer(attr->os_string), JceOutputStream_getLength(attr->os_string), &pBuff, &len);
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_setBuffer(is_map, pBuff, len);
	if (JCE_SUCCESS != ret)	goto do_clean;

	*s = JString_new();
	if (! (*s))
	{
		ret = JCE_MALLOC_ERROR;
		goto do_clean;
	}

	ret = JceInputStream_readVectorChar(is_map, *s, 1, true);
	if (JCE_SUCCESS != ret)	goto do_clean;

	do_clean:
	if (is_map)			  JceInputStream_del(&is_map);

	return ret;
}

static Int32 WUP_getAttribute_V3(const UniAttribute *pack, const char * name, const char * type, JString ** s)
{
	Int32 ret = 0;
	char * pBuff = NULL;
	uint32_t len = 0;

	const UniAttribute * attr = pack;
	JceInputStream *is_map = JceInputStream_new();
	if (!is_map)
	{
		ret = JCE_MALLOC_ERROR;
		goto do_clean;
	}

	JceOutputStream_reset(attr->os_string);
	ret = JceOutputStream_writeStringBuffer(attr->os_string, name, strlen(name), 0);
	if (JCE_SUCCESS != ret)	return ret;

	ret = JMapWrapper_find(attr->_data, JceOutputStream_getBuffer(attr->os_string), JceOutputStream_getLength(attr->os_string), &pBuff, &len);
	if (JCE_SUCCESS != ret)	return ret;
	
	ret = JceInputStream_setBuffer(is_map, pBuff, len);
	if (JCE_SUCCESS != ret)	goto do_clean;

	*s = JString_new();
	if (! (*s))
	{
		ret = JCE_MALLOC_ERROR;
		goto do_clean;
	}
		
	ret = JceInputStream_readVectorChar(is_map, *s, 1, true);
	if (JCE_SUCCESS != ret)	goto do_clean;

do_clean:
	if (is_map)			  JceInputStream_del(&is_map);

	return ret;
}

static Int32 WUP_getAttribute(const UniAttribute *pack, const char * name, const char * type, JString ** s)
{
	return pack->_iVer == 3?WUP_getAttribute_V3(pack, name, type, s):WUP_getAttribute_V2(pack, name, type, s);
}

Short WUP_getVersion(const void * pack)
{
	return ((const UniAttribute *)pack)->_iVer;
}

//get
Int32 WUP_getBool  (const void * pack, const char * name, Bool* value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;

	//s = JString_new();
	//if(!s) return JCE_MALLOC_ERROR;

	ret = WUP_getAttribute(pack, name, "bool", &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readBool(attr->value_is, value, 0, true);

	do_clean:
	JString_del(&s);

	return ret;
}

Int32 WUP_getChar  (const void * pack, const char * name, Char* value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;

	//s = JString_new();
	//if(!s) return JCE_MALLOC_ERROR;

	ret = WUP_getAttribute(pack, name, "char", &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readChar(attr->value_is, value, 0, true);

	do_clean:
	JString_del(&s);

	return ret;
}

Int32 WUP_getUInt8	(const void * pack, const char * name, UInt8 * value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;

	ret = WUP_getAttribute(pack, name, "short", &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readUInt8(attr->value_is, value, 0, true);

do_clean:
	JString_del(&s);

	return ret;	
}

Int32 WUP_getShort	(const void * pack, const char * name, Short * value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;

	//s = JString_new();
	//if(!s) return JCE_MALLOC_ERROR;

	ret = WUP_getAttribute(pack, name, "short", &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readShort(attr->value_is, value, 0, true);

	do_clean:
	JString_del(&s);

	return ret;
}

Int32 WUP_getUInt16	(const void * pack, const char * name, UInt16 * value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;

	ret = WUP_getAttribute(pack, name, "int32", &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readUInt16(attr->value_is, value, 0, true);

do_clean:
	JString_del(&s);

	return ret;
}

Int32 WUP_getFloat (const void * pack, const char * name, Float* value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;

	//s = JString_new();
	//if(!s) return JCE_MALLOC_ERROR;

	ret = WUP_getAttribute(pack, name, "float", &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readFloat(attr->value_is, value, 0, true);

	do_clean:
	JString_del(&s);

	return ret;
}

Int32 WUP_getDouble(const void * pack, const char * name, Double* value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;

	//s = JString_new();
	//if(!s) return JCE_MALLOC_ERROR;

	ret = WUP_getAttribute(pack, name, "double", &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readDouble(attr->value_is, value, 0, true);

	do_clean:
	JString_del(&s);

	return ret;
}

Int32 WUP_getInt32 (const void * pack, const char * name, Int32*  value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;

	ret = WUP_getAttribute(pack, name, "int32", &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readInt32(attr->value_is, value, 0, true);

	do_clean:
	JString_del(&s);

	return ret;
}

Int32 WUP_getUInt32	(const void * pack, const char * name, UInt32 * value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;

	ret = WUP_getAttribute(pack, name, "int64", &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readUInt32(attr->value_is, value, 0, true);

do_clean:
	JString_del(&s);

	return ret;	
}

Int32 WUP_getInt64 (const void * pack, const char * name, Int64*  value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;

	//s = JString_new();
	//if(!s) return JCE_MALLOC_ERROR;

	ret = WUP_getAttribute(pack, name, "int64", &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readInt64(attr->value_is, value, 0, true);

	do_clean:
	JString_del(&s);

	return ret;
}


Int32 WUP_getString(const void * pack, const char * name, JString * value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;

	//s = JString_new();
	//if(!s) return JCE_MALLOC_ERROR;

	ret = WUP_getAttribute(pack, name, "string", &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readString(attr->value_is, value, 0, true);

	do_clean:
	JString_del(&s);

	return ret;
}

Int32 WUP_getVector(const void * pack, const char * name, JArray * value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	char sType[64];
	Int32 ret;

	//s = JString_new();
	//if(!s) return JCE_MALLOC_ERROR;

	snprintf(sType, sizeof(sType), "list<%s>", value->elem_type_name);

	ret = WUP_getAttribute(pack, name, sType, &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readVector(attr->value_is, value, 0, true);

	do_clean:
	JString_del(&s);

	return ret;
}

Int32 WUP_getVectorChar(const void * pack, const char * name, JString * value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;

	//s = JString_new();
	//if(!s) return JCE_MALLOC_ERROR;

	ret = WUP_getAttribute(pack, name, "list<char>", &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readVectorChar(attr->value_is, value, 0, true);

	do_clean:
	JString_del(&s);

	return ret;
}

Int32 WUP_getMap   (const void * pack, const char * name, JMapWrapper * value)
{
	JString *s = NULL;
	const UniAttribute * attr;
	Int32 ret;
	char sType[64];

	//s = JString_new();
	//if(!s) return JCE_MALLOC_ERROR;

	snprintf(sType, sizeof(sType), "map<%s,%s>", value->first->elem_type_name, value->second->elem_type_name);

	ret = WUP_getAttribute(pack, name, sType, &s);
	if (JCE_SUCCESS != ret)	goto do_clean;

	attr = pack;
	ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JceInputStream_readMap(attr->value_is, value, 0, true);

	do_clean:
	JString_del(&s);

	return ret;
}

Int32 WUP_getStruct(const void * pack, const char * name, void *st)
{
	Int32 ret=0;
	JString * s = NULL;
	const UniAttribute * attr = pack;
	const JStructBase *jst = st;
	do
	{
		ret = WUP_getAttribute(pack, name, jst->className, &s);
		if ( JCE_SUCCESS != ret ) break;
		ret = JceInputStream_setBuffer(attr->value_is, JString_data(s), JString_size(s));
		if (JCE_SUCCESS != ret)	 break;
		ret = JceInputStream_readStruct(attr->value_is, st, 0,true);
	}while (0);
	if (s) JString_del(&s);
	return ret;
}

Int32 UniAttribute_encode(const UniAttribute * pack, char** buff, uint32_t *len)
{
	Int32 ret = 0;
	JceOutputStream *os = JceOutputStream_new();
	if (!os) return JCE_MALLOC_ERROR;

	ret = JceOutputStream_writeMap(os, pack->_data, 0);
	if (JCE_SUCCESS != ret)
	{
		JceOutputStream_del(&os);
		return ret;
	}

	//ret = JString_assign(s, JceOutputStream_getBuffer(os), JceOutputStream_getLength(os));
	*buff = JceMalloc(JceOutputStream_getLength(os));
	if (! *buff) return JCE_MALLOC_ERROR;
	*len = JceOutputStream_getLength(os);
	memcpy(*buff, JceOutputStream_getBuffer(os), *len);

	JceOutputStream_del(&os);

	return ret;
}

Int32 UniAttribute_decode(UniAttribute * pack, const char* buff, uint32_t len)
{
	Int32 ret;
	JceInputStream *is = JceInputStream_new();
	if (!is) return JCE_MALLOC_ERROR;

	ret = JceInputStream_setBuffer(is, buff, len);
	if (JCE_SUCCESS != ret)	return ret;

	ret = JceInputStream_readMap(is, pack->_data, 0, true);

	JceInputStream_del(&is);

	return ret;
}

///////////////////////////////////////////////////////////////
void UniAttribute_del(UniAttribute ** handle)
{
	UniAttribute ** this = (UniAttribute **)handle;

	JMapWrapper_del(&((*handle)->_data));
	JceOutputStream_del(&((*this)->value_os));
	JceInputStream_del(&((*this)->value_is));
	JceOutputStream_del(&((*this)->os_string));
	JMapWrapper_del(&((*this)->m_data));

	JceFree(*this);
	*this = NULL;
}

Int32 UniAttribute_init(UniAttribute *handle)
{
	UniAttribute * this = (UniAttribute*) handle;

	this->_data     = JMapWrapper_new("string", "map<string,list<char>>");
	this->value_os  = JceOutputStream_new();
	this->value_is  = JceInputStream_new();
	this->os_string = JceOutputStream_new();
	this->m_data    = JMapWrapper_new("string", "list<char>");
	this->_iVer		= 2;

	if (!this->_data || !this->value_os || !this->value_is || !this->os_string || !this->m_data)
	{
		if (this->_data)	JMapWrapper_del(&this->_data);
		if (this->value_os)	JceOutputStream_del(&this->value_os);
		if (this->value_is)	JceInputStream_del(&this->value_is);
		if (this->os_string)JceOutputStream_del(&this->os_string);
		if (this->m_data)	JMapWrapper_del(&this->m_data);
		return JCE_MALLOC_ERROR;
	}
	return 0;
}


UniAttribute * UniAttribute_new()
{
	Int32 ret=0;
	UniAttribute *this = (UniAttribute*)JceMalloc(sizeof(UniAttribute));
	if (!this) return NULL;

	ret = UniAttribute_init(this);
	if (ret)
	{
		JceFree(this);
		return NULL;
	}
	return this;
}

///////////////////////////////////////////////////////////////

Int32 UniPacket_encode(const UniPacket * pack, char** buff, uint32_t *len)
{
	Int32 ret=0;
	Int32 iHeaderLen = 0;
	JceOutputStream *os;

	os = JceOutputStream_new();
	if (!os) return JCE_MALLOC_ERROR;

	ret = JceOutputStream_writeMap(os, ((UniAttribute*)pack)->_data, 0);
	if (JCE_SUCCESS != ret)	goto do_clean;

	ret = JString_assign(pack->sBuffer, JceOutputStream_getBuffer(os), JceOutputStream_getLength(os));
	if (JCE_SUCCESS != ret)	goto do_clean;

	JceOutputStream_reset(os);

	ret = JceOutputStream_writeShort(os, pack->iVersion, 1);     if (JCE_SUCCESS != ret) goto do_clean;
	ret = JceOutputStream_writeChar(os, pack->cPacketType, 2);   if (JCE_SUCCESS != ret) goto do_clean;
	ret = JceOutputStream_writeInt32(os, pack->iMessageType, 3); if (JCE_SUCCESS != ret) goto do_clean;
	ret = JceOutputStream_writeInt32(os, pack->iRequestId, 4);   if (JCE_SUCCESS != ret) goto do_clean;
	ret = JceOutputStream_writeString(os, pack->sServantName, 5);if (JCE_SUCCESS != ret) goto do_clean;
	ret = JceOutputStream_writeString(os, pack->sFuncName, 6);   if (JCE_SUCCESS != ret) goto do_clean;
	ret = JceOutputStream_writeVectorChar(os, pack->sBuffer, 7); if (JCE_SUCCESS != ret) goto do_clean;
	ret = JceOutputStream_writeInt32(os, pack->iTimeout, 8);     if (JCE_SUCCESS != ret) goto do_clean;
	ret = JceOutputStream_writeMap(os, pack->context, 9);        if (JCE_SUCCESS != ret) goto do_clean;
	ret = JceOutputStream_writeMap(os, pack->status, 10);        if (JCE_SUCCESS != ret) goto do_clean;

	iHeaderLen = jce_htonl(sizeof(Int32) + JceOutputStream_getLength(os));

	*len = sizeof(Int32) + JceOutputStream_getLength(os);
	*buff = JceMalloc(*len);
	if (*buff == NULL)
	{
		*len = 0;
		return JCE_MALLOC_ERROR;
	}
	/*
	if(*len < sizeof(Int32) + JceOutputStream_getLength(os))
	{
		return JCE_ENCODE_ERROR;
	}
	*/

	memcpy(*buff, &iHeaderLen, sizeof(Int32));
	memcpy(*buff + sizeof(Int32), JceOutputStream_getBuffer(os), JceOutputStream_getLength(os));

	/*
	ret = JString_reserve(s, JceOutputStream_getLength(os) + sizeof(Int32));
	if(JCE_SUCCESS != ret) goto do_clean;

	ret = JString_assign(s, (char*) &iHeaderLen, sizeof(Int32));
	if(JCE_SUCCESS != ret) goto do_clean;

	ret = JString_append(s, JceOutputStream_getBuffer(os), JceOutputStream_getLength(os));
	if(JCE_SUCCESS != ret) goto do_clean;
	*/


	do_clean:
	JceOutputStream_del(&os);

	return ret;
}

Int32 UniPacket_decode(UniPacket * pack, const char* buff, uint32_t len)
{
	JceInputStream  *is;
	Int32 ret;

	if (len < sizeof(Int32))
	{
		return JCE_DECODE_ERROR;
	}

	is = JceInputStream_new();
	if (!is) return JCE_MALLOC_ERROR;

	ret = JceInputStream_setBuffer(is, buff + sizeof(Int32), len - sizeof(Int32));
	if (JCE_SUCCESS != ret)	return ret;

	ret = JceInputStream_readShort(is, &pack->iVersion, 1, true);     if (JCE_SUCCESS != ret) return ret;
	ret = JceInputStream_readChar(is, &pack->cPacketType, 2, false);  if (JCE_SUCCESS != ret) return ret;
	ret = JceInputStream_readInt32(is, &pack->iMessageType, 3, false);if (JCE_SUCCESS != ret) return ret;
	ret = JceInputStream_readInt32(is, &pack->iRequestId, 4, true);   if (JCE_SUCCESS != ret) return ret;
	ret = JceInputStream_readString(is, pack->sServantName, 5, true); if (JCE_SUCCESS != ret) return ret;
	ret = JceInputStream_readString(is, pack->sFuncName, 6, true);    if (JCE_SUCCESS != ret) return ret;
	ret = JceInputStream_readVectorChar (is, pack->sBuffer, 7, true); if (JCE_SUCCESS != ret) return ret;
	ret = JceInputStream_readInt32(is, &pack->iTimeout, 8, false);    if (JCE_SUCCESS != ret) return ret;
	ret = JceInputStream_readMap(is, pack->context, 9, false);        if (JCE_SUCCESS != ret) return ret;
	ret = JceInputStream_readMap(is, pack->status, 10, false);        if (JCE_SUCCESS != ret) return ret;

	JceInputStream_reset(is);

	ret = JceInputStream_setBuffer(is, JString_data(pack->sBuffer), JString_size(pack->sBuffer));
	if (JCE_SUCCESS != ret)	return ret;

	JMapWrapper_clear(((UniAttribute*)pack)->_data);
	ret = JceInputStream_readMap(is, ((UniAttribute*)pack)->_data, 0, true);

	JceInputStream_del(&is);

	return ret;
}

void UniPacket_del(UniPacket ** handle)
{
	UniPacket ** this = (UniPacket**)handle;

	JString_del(&(*this)->sServantName);
	JString_del(&(*this)->sFuncName);
	JString_del(&(*this)->sBuffer);
	JMapWrapper_del(&(*this)->context);
	JMapWrapper_del(&(*this)->status);

	//clean attr
	JMapWrapper_del(&(((UniAttribute*)(*this))->_data));
	JceOutputStream_del(&(((UniAttribute*)(*this))->value_os));
	JceInputStream_del(&(((UniAttribute*)(*this))->value_is));
	JceOutputStream_del(&(((UniAttribute*)(*this))->os_string));
	JMapWrapper_del(&(((UniAttribute*)(*this))->m_data));

	JceFree(*this);
	*this = NULL;
}

Int32 UniPacket_init(UniPacket *handle)
{
	Int32 ret;

	UniPacket * this = (UniPacket*) handle;

	ret = UniAttribute_init((UniAttribute*)this);
	if (ret) return ret;

	this->iVersion		= 2;
	this->cPacketType  	= 0;
	this->iMessageType 	= 0;
	this->iRequestId   	= 0;
	this->sServantName	= JString_new();
	this->sFuncName   	= JString_new();
	this->sBuffer      	= JString_new();
	this->iTimeout     	= 0;
	this->context      	= JMapWrapper_new("string", "string");
	this->status       	= JMapWrapper_new("string", "string");
	this->attr._iVer   	= 2;

	if (!this->sServantName || !this->sFuncName || !this->sBuffer || !this->context || !this->status)
	{
		if (this->sServantName)	JString_del(&this->sServantName);
		if (this->sFuncName)	JString_del(&this->sFuncName);
		if (this->sBuffer)		JString_del(&this->sBuffer);
		if (this->context)		JMapWrapper_del(&this->context);
		if (this->status)		JMapWrapper_del(&this->status);

		return JCE_MALLOC_ERROR;
	}

	return 0;

}

UniPacket * UniPacket_new()
{
	Int32 ret;
	UniPacket *this = (UniPacket*)JceMalloc(sizeof(UniPacket));
	if (!this) return NULL;

	ret = UniPacket_init(this);
	if (JCE_SUCCESS != ret)
	{
		JceFree(this);
		return NULL;
	}
	return this;
}

void UniPacket_setVersion(UniPacket * pack, Short siVersion)
{
	pack->iVersion 		= siVersion == 3?3:2;
	pack->attr._iVer	= siVersion == 3?3:2;
}

Short UniPacket_getVersion(UniPacket * pack)
{
	return pack->iVersion;
}

