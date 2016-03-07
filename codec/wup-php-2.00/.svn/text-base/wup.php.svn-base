<?php

require_once('jce.php');


class wup_unipacket
{
    public $iVersion;//协议版本号
    public $cPacketType;//调用类型
    public $iMessageType;//消息类型
    public $iRequestId;//请求序列ID号
    public $sServantName;//对象名称
    public $sFuncName;//处理功能函数名字
    public $sBuffer;//数据缓存，缓存要发送或者接受的数据，编码后或者解码前的内容
    public $iTimeout;//超时值
    public $context;//wup环境
    public $status;//特殊消息的状态值
    public $map_data;//解码的时候用于把buffer复原为可辨识的对象
    
    public function __construct()
    {
        $this->iVersion     = new c_short;
        $this->cPacketType  = new c_char;
        $this->iMessageType = new c_int;
        $this->iRequestId   = new c_int;
        $this->sServantName = new c_string;
        $this->sFuncName    = new c_string;

        $this->sBuffer      = new c_vector(new c_char);
        $this->iTimeout     = new c_int;
        $this->context      = new c_map(new c_string,new c_string);
        $this->status       = new c_map(new c_string,new c_string);  
        $this->map_data     = new c_map(new c_string,new c_map(new c_string,new c_vector(new c_char)));          
        
        $this->iVersion->val    = 2;
        $this->cPacketType->val = 0;
        $this->iMessageType->val = 0;
        $this->iRequestId->val   = 0;
        $this->iTimeout->val = 0;                        
    }
    //设置请求包ID
    public function set_request_id($id)
    {
        $this->iRequestId->val = $id;
    }
    //设置服务名字
    public function set_service_name($name)
    {
        $this->sServantName->val=$name;
    } 
    //设置函数名称
    public function set_func_name($name)
    {
        $this->sFuncName->val=$name;
    }
    
    //传入name和对象struct，把struct以name作为key打包到buffer里去
    public function put($name,$mystruct)
    {
    	//必须要有sServantName 和sFuncName
        if(strlen($this->sServantName->val) == 0)
        {
            throw new JCEException(__LINE__, UNKNOWN_SERVICE);
        }
        if(strlen($this->sFuncName->val) == 0)
        {
            throw new JCEException(__LINE__, UNKNOWN_FUNCTION);
        }
        
        //把struct写到$struct_data里去
        $mystruct->writeTo($struct_data,0);  
        
        //构造存放map的 data，$vector就是用来存放数据value的地方 $struct则加上了类名作为key
        $vector = new c_vector(new c_char);        
        $struct = new c_map(new c_string,$vector);
        $data = new c_map(new c_string,$struct);
        
        //vector加入数据
        $vector->push_back($struct_data);
        //stucte的map，把类名作为key，对应该对象的的vector                
        $struct->push_back($mystruct->get_class_name(),$vector);
        //最外面一层就是map data把name作为key，value是上面的map
        $data->push_back($name,$struct); 
        //写入 
        $data->write($data_stream,0);  

        //放入缓存,等候编码
        $this->sBuffer->push_back($data_stream);
            
    }
    
    //编码过程 返回stream
    public function _encode(&$stream)
    {
    	//把协议的相关变量写入stream
        $this->iVersion->write($stream,1);
        $this->cPacketType->write($stream,2);        
        $this->iMessageType->write($stream,3);        
        $this->iRequestId->write($stream,4);
        $this->sServantName->write($stream,5);
        $this->sFuncName->write($stream,6); 
        //开始写入要传送的struct的数据       
        $this->sBuffer->write($stream,7);
        //继续写入其他变量
        $this->iTimeout->write($stream,8);
        $this->context->write($stream,9);
        $this->status->write($stream,10);  
              
        //在stream的最前面，加上stream的长度（int）。
        $len = strlen($stream)+4;  
        $stream = pack('N',$len).$stream;
    }
    
    public function _decode(&$stream)
    {        
    	//先检查了stream是否为空，这个在请求包不合法的时候，server返回包没有设计body数据的时候会发生
    	if(strlen($stream) == 0 || is_null($stream))
    	{
    		ms_glog::write(LM_ERROR,__FILE__,__LINE__,__METHOD__,'the stream for wup decode ,wup body empty ');	
    		return false;
    	}
    	
    	//WUP的包包头前面有wup包的长度
        $len = @unpack('N',$stream);
        $len = $len[1];
    
        if(intval($len) <= 0 )
        {	
        	ms_glog::write(LM_ERROR,__FILE__,__LINE__,__METHOD__,'the stream for wup decode ,wup body length <0 ');
        	return false;
        }
      
       	//Qzone 协议包头读取出去
        $stream = substr($stream,4);
        $this->iVersion->read($stream,1);
        $this->cPacketType->read($stream,2);        
        $this->iMessageType->read($stream,3);        
        $this->iRequestId->read($stream,4);        
        $this->sServantName->read($stream,5);        
        $this->sFuncName->read($stream,6);
        $this->sBuffer->read($stream,7);
        $this->iTimeout->read($stream,8);
        $this->context->read($stream,9);
        $this->status->read($stream,10);    

        //把buffer作为一个map解包
        $this->map_data->read($this->sBuffer->get_val(),0);
    }          
    
    // 传入name作为key，取得回包struct信息
    public function get($name,&$mystruct)
    {                
    	//主要是解析解包出来的map_data
    	//第一层用name为key
        $struct_map = $this->map_data->get_val($name);
        //第二层用类名作为key
        $struct_vec = $struct_map->get_val($mystruct->get_class_name());        
        //最后从vector里得到
        $mystruct->readFrom($struct_vec->get_val(),0);        
    }
        
};

?>