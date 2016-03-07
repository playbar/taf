import com.qq.Test.*;
import com.qq.taf.holder.JceStringHolder;
import com.qq.taf.proxy.Communicator;

public class TestHello 
{
	public Communicator c = new Communicator();
	
	public HelloPrx pPrx = null;
	
	public TestHello()
	{
		pPrx = c.stringToProxy("Test.HelloServer.HelloObj@tcp -h 172.25.38.67 -p 45454", HelloPrx.class);
	}

	public void syncCall()
	{
		//发起远程调用
		String s = "syncCall hello world";
		JceStringHolder r = new JceStringHolder();
		
		System.out.println("send sync:" + s);
		int ret = pPrx.testHello(s, r);
		
		System.out.println("recv sync:" + r.value);
	}	
	
	///////////////////////////////////////////////////////////
	class HelloCallback extends HelloPrxCallback
	{
		//回调函数
		public void callback_testHello(int ret, String r)
		{
			System.out.println("callback_testHello:" + ret + ":" + r);
		}

		public void callback_testHello_exception(int ret)
		{
			System.out.println("callback_testHello_exception:" + ret);
		}
	};
	
	public void asyncCall()
	{
		HelloPrxCallback p = new HelloCallback();
		
		//发起远程异步调用
		String s = "asyncCall hello world";
		String r;
		
		System.out.println("send async:" + s);
		
		pPrx.async_testHello(p, s);

		System.out.println("async call ok");
	    
		try {
			Thread.currentThread().sleep(1000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void oneWayCall()
	{
		//发起远程单项调用
		String s = "oneWayCall hello world";
		String r;
		
		System.out.println("send oneway call:" + s);
		
		pPrx.async_testHello(null, s);

		System.out.println("send oneway call ok");
	    
		try {
			Thread.currentThread().sleep(1000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) 
	{
		TestHello th = new TestHello();
		th.syncCall();
		
		th.asyncCall();
		
		th.oneWayCall();
		
	}

}
