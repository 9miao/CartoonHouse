package com.dmzj.manhua.openapi;

import org.json.JSONObject;
import android.content.Intent;
import android.util.Log;
import com.dmzj.manhua.HelloCpp;
import com.dmzj.manhua.openapi.AccessTokenKeeper4Tencent.Oauth2Access4Tencent;
import com.tencent.connect.UserInfo;
import com.tencent.tauth.IUiListener;
import com.tencent.tauth.Tencent;
import com.tencent.tauth.UiError;

public class TencentOpenApi {
	
	private static final String TAG = "TencentOpenApi";
	
	private Tencent mTencent;
	
	private HelloCpp mContext;
	
	public static final int MSG_WHAT_QQTOKEN = 0x21;
	
	public TencentOpenApi(HelloCpp mContext) {
		this.mContext = mContext;
	}
	
	public void getAccessToken(){
		login();
	}
	
	public void onActivityResult(int requestCode, int resultCode, Intent data){
		mTencent.onActivityResult(requestCode, resultCode, data);
	}
	
	private class BaseUiListener implements IUiListener {
		
		protected void doComplete(Object response) {
			if (null == response) {
                Log.d(TAG,"返回为空");
                return;
            }
            JSONObject jsonResponse = (JSONObject) response;
            if (null != jsonResponse && jsonResponse.length() == 0) {
                Log.d(TAG,"登录失败");
                return;
            }
            
            Oauth2Access4Tencent bean = new AccessTokenKeeper4Tencent.Oauth2Access4Tencent();
            bean.setPfkey(jsonResponse.optString("pfkey"));
            bean.setAccess_token(jsonResponse.optString("openid"));
            bean.setOpenid(jsonResponse.optString("access_token"));
            AccessTokenKeeper4Tencent.writeAccessToken(mContext, bean);
            
            mContext.getmDefaultHandler().sendEmptyMessage(MSG_WHAT_QQTOKEN);

            
//			{
//				"ret":0,
//				"pay_token":"xxxxxxxxxxxxxxxx",
//				"pf":"openmobile_android",
//				"expires_in":"7776000",
//				"openid":"xxxxxxxxxxxxxxxxxxx",
//				"pfkey":"xxxxxxxxxxxxxxxxxxx",
//				"msg":"sucess",
//				"access_token":"xxxxxxxxxxxxxxxxxxxxx"
//			}
		}

		@Override
		public void onError(UiError e) {
			Log.d(TAG,"onError(UiError e)");
		}

		@Override
		public void onCancel() {
			Log.d(TAG," onCancel()");
		}

		@Override
		public void onComplete(Object response) {
			doComplete((JSONObject)response);
		}
	}
	
	public void login()
	{
		BaseUiListener mBaseUiListener = new BaseUiListener();
		mTencent = Tencent.createInstance("101131188", mContext);
		if (!mTencent.isSessionValid())
		{
			mTencent.login(mContext, "all", mBaseUiListener);
		}
	}
	
	
	public void getUserInfo(){
		UserInfo mInfo = new UserInfo(mContext, mTencent.getQQToken());
		mInfo.getUserInfo(new UserInfoUiListener());
	}
	
	
	private class UserInfoUiListener implements IUiListener {
		
		protected void doComplete(Object response) {
			if (null == response) {
                Log.d(TAG,"返回为空");
                return;
            }
            JSONObject jsonResponse = (JSONObject) response;
            if (null != jsonResponse && jsonResponse.length() == 0) {
                Log.d(TAG,"获取失败");
                return;
            }
            
            Log.d(TAG, "用户信息："+jsonResponse.toString());
            
            /**
            {
            	  "is_yellow_year_vip": "0",
            	  "ret": 0,
            	  "figureurl_qq_1": "http://q.qlogo.cn/qqapp/101131188/06C68144C8C23302154EB63C3BFFFBFA/40",
            	  "figureurl_qq_2": "http://q.qlogo.cn/qqapp/101131188/06C68144C8C23302154EB63C3BFFFBFA/100",
            	  "nickname": "动漫之家",
            	  "yellow_vip_level": "0",
            	  "is_lost": 0,
            	  "msg": "",
            	  "city": "朝阳",
            	  "figureurl_1": "http://qzapp.qlogo.cn/qzapp/101131188/06C68144C8C23302154EB63C3BFFFBFA/50",
            	  "vip": "0",
            	  "level": "0",
            	  "figureurl_2": "http://qzapp.qlogo.cn/qzapp/101131188/06C68144C8C23302154EB63C3BFFFBFA/100",
            	  "province": "北京",
            	  "is_yellow_vip": "0",
            	  "gender": "男",
            	  "figureurl": "http://qzapp.qlogo.cn/qzapp/101131188/06C68144C8C23302154EB63C3BFFFBFA/30"
            	}
            **/
		}

		@Override
		public void onError(UiError e) {
			Log.d(TAG,"onError(UiError e)");
		}

		@Override
		public void onCancel() {
			Log.d(TAG," onCancel()");
		}

		@Override
		public void onComplete(Object response) {
			doComplete((JSONObject)response);
		}
	}
	
	

	public Tencent getmTencent() {
		return mTencent;
	}

	public void setmTencent(Tencent mTencent) {
		this.mTencent = mTencent;
	}
	
}
