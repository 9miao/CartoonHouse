package com.dmzj.manhua;

import org.CrossApp.lib.Cocos2dxActivity;
import org.CrossApp.lib.Cocos2dxGLSurfaceView;

import com.baidu.android.pushservice.PushConstants;
import com.baidu.android.pushservice.PushManager;
import com.dmzj.manhua.openapi.AccessTokenKeeper;
import com.dmzj.manhua.openapi.OpenApiHelper;
import com.dmzj.manhua.openapi.SinaOpenApi;
import com.dmzj.manhua.openapi.TencentOpenApi;
import com.dmzj.manhua.push.MessagePushTool;
import com.dmzj.manhua.push.Utils;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.widget.ImageView;

import com.component.impl.WebViewCtrl;
import com.jni.PhoneNet;
import com.umeng.AnalyticsHome;


public class HelloCpp extends Cocos2dxActivity{
	
	
	//推送的json数据
	public static final String INTENT_EXTRA_TITLE = "intent_extra_title";
	public static final String INTENT_EXTRA_DESC = "intent_extra_desc";
	public static final String INTENT_EXTRA_PUSHMSG = "intent_extra_pushmsg";
	
	private static final String TAG = "HelloCpp";
	 
	private Handler mDefaultHandler;
	
	private SinaOpenApi sinaAPI;
	private TencentOpenApi tencentAPI;
	
	private OpenApiHelper mOpenApiHelper;
	
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		WebViewCtrl.init(this);
		PhoneNet.getInstance().setContext(this);
		initWithApiKey();
		initEnviroment();
		AnalyticsHome.init(this);
		
		/***
		sinaAPI = new SinaOpenApi(HelloCpp.this);
		sinaAPI.getAccessToken();
		**/
		
		/***
		Handler mHandler = new Handler();
		mHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				tencentAPI = new TencentOpenApi(HelloCpp.this);
				tencentAPI.getAccessToken();
			}
		}, 2000);
		**/
		checkPushInfo();

	}
    
     
    @Override
    protected void onNewIntent(Intent intent) {
    	
    	super.onNewIntent(intent);
    }
    
    
    
    private void initEnviroment(){
    	mDefaultHandler = new Handler(){
    		@Override
    		public void handleMessage(Message msg) {
    			onHandleMessage(msg);
    		}
    	};
    	mOpenApiHelper = new OpenApiHelper(this);
    }
    
    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// HelloCpp should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }
    
    static {
        System.loadLibrary("CrossApp_cpp");
    }     
    
    private void onHandleMessage(Message msg){
    	switch (msg.what) {
//		case OpenApiHelper.MSG_WHAT_PLAT_QQ:
//			tencentAPI = new TencentOpenApi(HelloCpp.this);
//			tencentAPI.getAccessToken();
//			break;
//		case OpenApiHelper.MSG_WHAT_PLAT_SINA:
//			sinaAPI = new SinaOpenApi(HelloCpp.this);
//			sinaAPI.getAccessToken();
//			break;
//		case SinaOpenApi.MSG_WHAT_SINATOKEN:
//			String token = AccessTokenKeeper.readAccessToken(HelloCpp.this).getToken();
//			OpenApiHelper.onTokenReturn(OpenApiHelper.PLAT_SINA, token ,AccessTokenKeeper.readAccessToken(HelloCpp.this).getUid());
//			break;
//		case TencentOpenApi.MSG_WHAT_QQTOKEN:
//			OpenApiHelper.onTokenReturn(OpenApiHelper.PLAT_QQ, tencentAPI.getmTencent().getQQToken().getAccessToken(),tencentAPI.getmTencent().getQQToken().getOpenId());
//			break;
		default:
			break;
		}
    }
    
    private void initWithApiKey() {
		// Push: 无账号初始化，用api key绑定
	if(!Utils.hasBind(getApplicationContext()))
		{
			PushManager.startWork(getApplicationContext(),
					PushConstants.LOGIN_TYPE_API_KEY, 
					Utils.getMetaValue(HelloCpp.this, "api_key"));
		}
	}
    
    
    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent intent) {
    	super.onActivityResult(requestCode, resultCode, intent);
    	
//    	if (sinaAPI!=null) {
//    		sinaAPI.getmSsoHandler().authorizeCallBack(requestCode, resultCode, intent);
//		}
//    	if(tencentAPI!=null){
//    		Log.d(TAG, "onActivityResult");
//    		tencentAPI.onActivityResult(requestCode, resultCode, intent);
//    	}
    }
    

	public Handler getmDefaultHandler() {
		return mDefaultHandler;
	}

	public void setmDefaultHandler(Handler mDefaultHandler) {
		this.mDefaultHandler = mDefaultHandler;
	}
	
	
	private void checkPushInfo(){
		if (getIntent().getStringExtra(INTENT_EXTRA_PUSHMSG)!=null) {
			MessagePushTool.onMessageObtain(HelloCpp.this, getIntent().getStringExtra(INTENT_EXTRA_TITLE), 
					getIntent().getStringExtra(INTENT_EXTRA_DESC),
					getIntent().getStringExtra(INTENT_EXTRA_PUSHMSG));
		}
		
	}
    
    
}
