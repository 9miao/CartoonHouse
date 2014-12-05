/*
 * Copyright (C) 2010-2013 The SINA WEIBO Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.dmzj.manhua.openapi;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

/**
 * @author Tencent
 */
public class AccessTokenKeeper4Tencent {
	
    private static final String PREFERENCES_NAME = "com_tencent_sdk_android";
   
    public static final String KEY_OPENID= "openid";
    
    public static final String KEY_PFKEY= "pfkey";
    
    public static final String KEY_ACCESS_TOKEN= "access_token";
    
    /**
     * 保存 Token 对象到 SharedPreferences。
     * 
     * @param context 应用程序上下文环境
     * @param token   Token 对象
     */
    public static void writeAccessToken(Context context, Oauth2Access4Tencent token) {
        if (null == context || null == token) {
            return;
        }
        
        SharedPreferences pref = context.getSharedPreferences(PREFERENCES_NAME, Context.MODE_APPEND);
        Editor editor = pref.edit();
        editor.putString(KEY_PFKEY, token.getPfkey());
        editor.putString(KEY_ACCESS_TOKEN, token.getAccess_token());
        editor.putString(KEY_OPENID, token.getOpenid());
        editor.commit();
    }

    /**
     * 从 SharedPreferences 读取 Token 信息。
     * 
     * @param context 应用程序上下文环境
     * 
     * @return 返回 Token 对象
     */
    public static Oauth2Access4Tencent readAccessToken(Context context) {
        if (null == context) {
            return null;
        }
        Oauth2Access4Tencent token = new Oauth2Access4Tencent();
        SharedPreferences pref = context.getSharedPreferences(PREFERENCES_NAME, Context.MODE_APPEND);
        token.setPfkey(pref.getString(KEY_PFKEY, ""));
        token.setAccess_token(pref.getString(KEY_ACCESS_TOKEN, ""));
        token.setOpenid(pref.getString(KEY_OPENID, ""));
        return token;
    }

    /**
     * 清空 SharedPreferences 中 Token信息。
     * 
     * @param context 应用程序上下文环境
     */
    public static void clear(Context context) {
        if (null == context) {
            return;
        }
        SharedPreferences pref = context.getSharedPreferences(PREFERENCES_NAME, Context.MODE_APPEND);
        Editor editor = pref.edit();
        editor.clear();
        editor.commit();
    }
    
    public static class Oauth2Access4Tencent{
    	
    	private String openid ;
    	
    	private String pfkey;
    	
    	private String access_token;

		public String getOpenid() {
			return openid;
		}

		public void setOpenid(String openid) {
			this.openid = openid;
		}

		public String getPfkey() {
			return pfkey;
		}

		public void setPfkey(String pfkey) {
			this.pfkey = pfkey;
		}

		public String getAccess_token() {
			return access_token;
		}

		public void setAccess_token(String access_token) {
			this.access_token = access_token;
		}
    	
    	
    	
    	
    }
    
    
    
}
