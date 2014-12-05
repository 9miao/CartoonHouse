#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "CrossApp.h"


#define DESIGN_RESOLUTION_480X320    0
#define DESIGN_RESOLUTION_1024X768   1
#define DESIGN_RESOLUTION_2048X1536  2

/* If you want to switch design resolution, change next line */
#define TARGET_DESIGN_RESOLUTION_SIZE  DESIGN_RESOLUTION_480X320

typedef struct tagResource
{
    CrossApp::CCSize size;
    char directory[100];
}Resource;

static Resource smallResource  =  { CrossApp::CCSizeMake(480, 320),   "iphone" };
static Resource mediumResource =  { CrossApp::CCSizeMake(1024, 768),  "ipad"   };
static Resource largeResource  =  { CrossApp::CCSizeMake(2048, 1536), "ipadhd" };

#if (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_480X320)
static CrossApp::CCSize designResolutionSize = CrossApp::CCSizeMake(480, 320);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_1024X768)
static CrossApp::CCSize designResolutionSize = CrossApp::CCSizeMake(1024, 768);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_2048X1536)
static CrossApp::CCSize designResolutionSize = CrossApp::CCSizeMake(2048, 1536);
#else
#error unknown target design resolution!
#endif

// The font size 24 is designed for small resolution, so we should change it to fit for current design resolution
#define TITLE_FONT_SIZE  (CrossApp::EGLView::sharedOpenGLView()->getDesignResolutionSize().width / smallResource.size.width * 24)

#endif /* __APPMACROS_H__ */
