package com.dmzj.manhua.openapi;


import android.R.string;

import com.dmzj.manhua.HelloCpp;

public class OpenApiHelper {
	
	private static final String TAG = "OpenApiHelper";
	
	/** QQ平台 */
	public static final int PLAT_QQ = 0x1;
	/** 新浪平台 */
	public static final int PLAT_SINA = 0x2;
	
	public static final int MSG_WHAT_PLAT_QQ = 0x10;
	
	public static final int MSG_WHAT_PLAT_SINA = 0x11;
	
	private static HelloCpp mActivity;
	
	public OpenApiHelper(HelloCpp mActivity) {
		this.mActivity = mActivity;
	}
	
	/**
	 * native 调用此方法发出登陆请求
	 * @param platform
	 */
	public static void onLogin(int platform){
		switch (platform) {
		case PLAT_QQ:
			mActivity.getmDefaultHandler().sendEmptyMessage(MSG_WHAT_PLAT_QQ);
			break;
		case PLAT_SINA:
			mActivity.getmDefaultHandler().sendEmptyMessage(MSG_WHAT_PLAT_SINA);
			break;
		default:
			break;
		}
	}
	
	
	public native static void onTokenReturn(int plat , String token ,String uid);
	
	
}
