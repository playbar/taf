// **********************************************************************
// WUP_C version 1.0.4 by WSRD Tencent.
// **********************************************************************

#ifndef __WUP_C_H__
#define __WUP_C_H__

#include "Jce_c.h"

typedef struct UniAttribute UniAttribute;
typedef struct UniPacket    UniPacket;
typedef struct UniPacket    TafUniPacket;

//属性封装
struct UniAttribute
{
	JMapWrapper 	*_data;
	JceOutputStream *value_os;
	JceInputStream  *value_is;

	JMapWrapper     *m_data;  	//encode decode helper
	JceOutputStream *os_string;	//encode decode helper

	short			_iVer;		//WUP版本的标示(2:标示普通WUP协议，3:标示精简WUP协议)

	char _sLastError[32];
};

///////////////////////////////////////////////////////////////
//put
void  WUP_setVersion(void * pack, Short iVersion);

Int32 WUP_putBool	(void * pack, const char * name, Bool value);

Int32 WUP_putChar	(void * pack, const char * name, Char value);

Int32 WUP_putUInt8	(void * pack, const char * name, UInt8 value);

Int32 WUP_putShort	(void * pack, const char * name, Short value);

Int32 WUP_putUInt16	(void * pack, const char * name, UInt16 value);

Int32 WUP_putFloat	(void * pack, const char * name, Float value);

Int32 WUP_putDouble	(void * pack, const char * name, Double value);

Int32 WUP_putInt32	(void * pack, const char * name, Int32  value);

Int32 WUP_putUInt32	(void * pack, const char * name, UInt32 value);

Int32 WUP_putInt64	(void * pack, const char * name, Int64  value);

Int32 WUP_putString	(void * pack, const char * name, JString * value);
Int32 WUP_putStringBuffer(void * pack, const char * name, const char* buff, uint32_t len);

Int32 WUP_putVector	(void * pack, const char * name, JArray * value);

Int32 WUP_putVectorChar(void * pack, const char * name,  JString* value);
Int32 WUP_putVectorCharBuffer(void * pack, const char * name,  const char* buff, uint32_t len);

Int32 WUP_putMap	(void * pack, const char * name, JMapWrapper * value);

Int32 WUP_putStruct	(void * pack, const char * name, const void* st);

//get
Short WUP_getVersion(const void * pack);

Int32 WUP_getBool	(const void * pack, const char * name, Bool  * value);

Int32 WUP_getChar	(const void * pack, const char * name, Char  * value);

Int32 WUP_getUInt8	(const void * pack, const char * name, UInt8 * value);

Int32 WUP_getShort 	(const void * pack, const char * name, Short * value);

Int32 WUP_getUInt16	(const void * pack, const char * name, UInt16 * value);

Int32 WUP_getFloat 	(const void * pack, const char * name, Float* value);

Int32 WUP_getDouble	(const void * pack, const char * name, Double* value);

Int32 WUP_getInt32 	(const void * pack, const char * name, Int32*  value);

Int32 WUP_getUInt32	(const void * pack, const char * name, UInt32 * value);

Int32 WUP_getInt64 (const void * pack, const char * name, Int64*  value);

Int32 WUP_getString(const void * pack, const char * name, JString * value);

Int32 WUP_getVector(const void * pack, const char * name, JArray * value);

Int32 WUP_getVectorChar(const void * pack, const char * name, JString * value);

Int32 WUP_getMap   (const void * pack, const char * name, JMapWrapper * value);

Int32 WUP_getStruct(const void * pack, const char * name, void *st);


UniAttribute * UniAttribute_new(void);

Int32 UniAttribute_init(UniAttribute *handle);

void UniAttribute_del(UniAttribute ** handle);

Int32 UniAttribute_encode(const UniAttribute * pack, char** buff, uint32_t *len);

Int32 UniAttribute_decode(UniAttribute * pack, const char* buff, uint32_t len);


///////////////////////////////////////////////////////////////
//请求回应包封装
struct UniPacket
{
	UniAttribute attr;

	Short iVersion;
	Char cPacketType;
	Int32 iMessageType;
	Int32 iRequestId;
	JString *sServantName;
	JString *sFuncName;
	JString *sBuffer;	  //vector<taf::Char> sBuffer;
	Int32 iTimeout;
	JMapWrapper *context;//map<std::string, std::string> context;
	JMapWrapper *status; //map<std::string, std::string> status;
};

void  UniPacket_setVersion(UniPacket * pack, Short siVersion);

Short UniPacket_getVersion(UniPacket * pack);

Int32 UniPacket_encode(const UniPacket * pack, char** buff, uint32_t *len);

Int32 UniPacket_decode(UniPacket * pack, const char* buff, uint32_t len);

void UniPacket_del(UniPacket ** handle);

Int32 UniPacket_init(UniPacket *handle);

UniPacket * UniPacket_new(void);

#endif
