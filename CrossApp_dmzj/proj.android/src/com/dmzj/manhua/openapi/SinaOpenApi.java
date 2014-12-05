package com.dmzj.manhua.openapi;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import com.dmzj.manhua.HelloCpp;
import com.sina.weibo.sdk.auth.Oauth2AccessToken;
import com.sina.weibo.sdk.auth.WeiboAuth;
import com.sina.weibo.sdk.auth.WeiboAuthListener;
import com.sina.weibo.sdk.auth.WeiboAuth.AuthInfo;
import com.sina.weibo.sdk.auth.sso.SsoHandler;
import com.sina.weibo.sdk.exception.WeiboException;

public class SinaOpenApi {
	
	private HelloCpp mContext;
	
	public static final int MSG_WHAT_SINATOKEN= 0x31;
	
	public SinaOpenApi(HelloCpp mContext) {
		this.mContext = mContext;
	}
	
	private  SsoHandler mSsoHandler;
	
	private AuthListener mAuthListener;
	
	public void getAccessToken(){
		// 创建授权认证信息
        AuthInfo authInfo = new AuthInfo(mContext, Constants.APP_KEY, Constants.REDIRECT_URL, Constants.SCOPE);
        mAuthListener = new AuthListener();
        WeiboAuth weiboAuth = new WeiboAuth(mContext, authInfo);
        mSsoHandler = new SsoHandler((Activity)mContext, weiboAuth);
        mSsoHandler.authorize(mAuthListener);
	}
	
	/**
     * 登入按钮的监听器，接收授权结果。
     */
    private class AuthListener implements WeiboAuthListener {
        private static final String TAG = "AuthListener";

		@Override
        public void onComplete(Bundle values) {
			Log.d(TAG, "认证成功,还没把token持久化。。。");
            Oauth2AccessToken accessToken = Oauth2AccessToken.parseAccessToken(values);
            if (accessToken != null && accessToken.isSessionValid()) {
                AccessTokenKeeper.writeAccessToken(mContext, accessToken);
            }
            mContext.getmDefaultHandler().sendEmptyMessage(MSG_WHAT_SINATOKEN);
            Log.d(TAG, "认证成功。。。");
        }

        @Override
        public void onWeiboException(WeiboException e) {
        	Log.d(TAG, "认证失败。。。"+ e.toString());
        }
        
        @Override
        public void onCancel() {
        	Log.d(TAG, "认证取消。。。");
        }
    }




	public SsoHandler getmSsoHandler() {
		return mSsoHandler;
	}


	public void setmSsoHandler(SsoHandler mSsoHandler) {
		this.mSsoHandler = mSsoHandler;
	}
	
}
