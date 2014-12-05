
#include "OpenBrowser.h"
#import "EAGLView.h"

void OpenBrowser::openUrl(const std::string&  strUrl)
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:strUrl.c_str()]]];

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:strUrl.c_str()]]];
    
#endif
    
}