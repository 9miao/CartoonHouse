/*
 * File Name: DataPref.java 
 * History:
 * Created by Siyang.Miao on 2012-4-20
 */
package com.dmzj.manhua.tools;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

public class JPrefreUtil {

	/** 用于存储配置 */
	public static final String CARTTON="cartoon";
	
	
	/** 用于推送的channel id */
	public static final String PUSH_CHANNELID="channelid";
	/** 用于推送的 user id */
	public static final String PUSH_USERID="userid";
	
	
    // ==========================================================================
    // Fields
    // ==========================================================================
    private static JPrefreUtil sInstance;
    private SharedPreferences mPref;
    private Editor mEditor;
    private Context mContext;

    // ==========================================================================
    // Constructors
    // ==========================================================================
    public synchronized static JPrefreUtil getInstance(Context context) {
        if (null == sInstance) {
            sInstance = new JPrefreUtil(context);
        }
        return sInstance;
    }
    
    private JPrefreUtil(Context context) {
        mContext = context;
    }
    
    
    /**
     * 获取推送的channel id
     * 
     * @return
     */
    public String getChannelId (){
    	return getSharedPreferences().getString(PUSH_CHANNELID, "");
    }
    /**
     * 获取推送的user id
     * 
     * @return
     */
    public String getUserId (){
    	return getSharedPreferences().getString(PUSH_USERID, "");
    }
    
    /**
     * 存储推送的channel id
     * @param ip
     * @return
     */
    public boolean setChannelID(String channlId) {
    	Editor editor = getEditor();
    	editor.putString(PUSH_CHANNELID, channlId);
    	return editor.commit();
    }
    
    /**
     * 存储推送的user id
     * @param ip
     * @return
     */
    public boolean setUserID(String userid) {
    	Editor editor = getEditor();
    	editor.putString(PUSH_USERID, userid);
    	return editor.commit();
    }

  
    private Editor getEditor() {
        if (null == mEditor) {
            mEditor = getSharedPreferences().edit();
        }
        return mEditor;
    }

    private SharedPreferences getSharedPreferences() {
        if (null == mPref) {
            mPref = mContext.getSharedPreferences(CARTTON, Context.MODE_PRIVATE);
        }
        return mPref;
    }


}
