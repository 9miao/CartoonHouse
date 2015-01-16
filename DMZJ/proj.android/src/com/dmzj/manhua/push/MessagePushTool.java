package com.dmzj.manhua.push;

import android.content.Context;

public class MessagePushTool {
	
	public static native void onMessageObtain(Context context, String title, 
			String description, String customContentString);
	
	public static native void onBaiduBind(String appid,String userId, String 
			channelId, String requestId);
	
}
