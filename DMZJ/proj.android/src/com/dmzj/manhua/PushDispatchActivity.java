package com.dmzj.manhua;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Window;
import android.widget.ImageView;

public class PushDispatchActivity extends Activity {
	
	//推送的json数据
	public static final String INTENT_EXTRA_TITLE = "intent_extra_title";
	public static final String INTENT_EXTRA_DESC = "intent_extra_desc";
	public static final String INTENT_EXTRA_PUSHMSG = "intent_extra_pushmsg";
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);  
		ImageView imageView = new ImageView(PushDispatchActivity.this);
		imageView.setBackgroundColor(android.R.color.transparent);
		setContentView(imageView);
		Intent intent = new Intent(PushDispatchActivity.this,HelloCpp.class);
		intent.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
		startActivity(intent);
		this.finish();
	}

	
	
}
