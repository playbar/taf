<?
require_once('jce.php');

class pdu_unipacket
{
    public $version;
    public $command_id;
    public $verify_no;
    public $serial_no;
    public $colorration;
    public $pkg_flag;
    public $svr_resp;
    public $reserve;
        
    public function _encode(&$struct_in,&$stream_out)
    {
        $stream_out = pack("c", 4);                      
    	$stream_out.= pack("c", $this->version);         
    	$stream_out.= pack("N", $this->command_id);      
    	$stream_out.= pack("nN2",$this->verify_no,$this->serial_no,$this->colorration);// unsigned short У��
    	$stream_out.= pack("c", $this->pkg_flag);              
        $stream_out.= pack("n", $this->svr_resp);              
    	$stream_out.= pack("c", $this->reserve);                   	    	
    	$struct_in->writeTo($temp,0);    	        	    	
        $len= strlen($temp);        
    	$stream_out.= pack("N", $len+25);            	    
        $stream_out.= $temp;                          
    	$stream_out.= pack("c", 5);                  
    }
    
    public function _decode(&$stream_in,&$struct_out)
    {
        if(strlen($stream_in) < 25)
        {
            throw JCEException(__LINE__, LEN_NOT_MATCH);
        }
        $stream_in = substr($stream_in,1);    //cut the 0x04
        // version
        $this->version = substr($stream_in,0,1);        
        $stream_in = substr($stream_in,1);    //cut the version
        
        //command id
        $val = unpack('N',$stream_in);
        $this->command_id = $val[1];
        $stream_in = substr($stream_in,4); 
        
        //verify_no
        $val = unpack('n',$stream_in);
        $this->verify_no = $val[1];
        $stream_in = substr($stream_in,2);    
        
        //serial_no
        $val = unpack('N',$stream_in);
        $this->serial_no = $val[1];
        $stream_in = substr($stream_in,4); 
        
        //colorration
        $val = unpack('N',$stream_in);
        $this->colorration = $val[1];
        $stream_in = substr($stream_in,4);
        
        //pkg_flag
        $this->pkg_flag = $stream_in{0};
        $stream_in = substr($stream_in,1);
        
        //svr_resp
        $val = unpack('n',$stream_in);
        $this->svr_resp = $val[1];
        $stream_in = substr($stream_in,2);    
        
        //reserve
        $this->reserve = $stream_in{0};
        $stream_in = substr($stream_in,1);
        
        //pkg len
        $val = unpack('N',$stream_in);
        $len = $val[1];
        $stream_in = substr($stream_in,4);                
       
        if($len < 25 || $len > 102400)
        {
            throw JCEException(__LINE__, OUTOF_RANGE);
        }
        $pktlen = strlen($stream_in);
        //-1 in order to remove eoh 0x05
       	$temp = substr($stream_in,0,$pktlen-1);
       
        $struct_out->readFrom($temp);     
          
    }
};

?>
