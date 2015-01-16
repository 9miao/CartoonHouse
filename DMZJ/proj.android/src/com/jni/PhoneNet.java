package com.jni;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.util.Log;

public class PhoneNet extends Activity{

	private static PhoneNet instance = null;

	private static Activity net;
	
    public static PhoneNet getInstance()
    {
        if	(instance == null)
        {
            instance = new PhoneNet();
        }
        return instance;
    }
	
    public void setContext(Activity cons){
    	net = cons;
    }
    
    public static String returnMacString(){
    	return getInstance().getLocalMacAddressFromWifiInfo();
    }
    
    public static int returnType(){
    	return getInstance().getAPNType();
    }
    
	public  String getLocalMacAddressFromWifiInfo(){
		
		
        WifiManager wifi = (WifiManager) net.getSystemService(Context.WIFI_SERVICE);  
        WifiInfo info = wifi.getConnectionInfo();  
        String str = null;  
        str = info.getMacAddress();    
        
        if(str==null)  
        {  
 
        }  

        return str;
    }
	
	 public int getAPNType(){
			
			int netType = -1;  
			 
			        ConnectivityManager connMgr = (ConnectivityManager) net.getSystemService(Context.CONNECTIVITY_SERVICE); 
			
			        NetworkInfo networkInfo = connMgr.getActiveNetworkInfo(); 
			 
			          
			 
			         if(networkInfo==null){ 
			 
			             return netType; 
			 
			         } 
			 
			         int nType = networkInfo.getType(); 
			
			        if(nType==ConnectivityManager.TYPE_MOBILE){ 
			 
			             Log.e("networkInfo.getExtraInfo()", "networkInfo.getExtraInfo() is "+networkInfo.getExtraInfo()); 
			 
			             if(networkInfo.getExtraInfo().toLowerCase().equals("cmnet")){ 
			 
			                 netType = 2; 
			 
			             } 
			 
			             else{ 
			 
			                 netType = 3; 
			 
			             } 
			 
			         } 
			 
			         else if(nType==ConnectivityManager.TYPE_WIFI){ 
			 
			            netType = 1; 
			
			         } 
			        Log.d("com.dmzj.manhua", "DSFDSFDSFDSFDSFDS");
			         return netType; 
			
		}
}
