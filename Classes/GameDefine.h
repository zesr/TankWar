#ifndef __GAME_DEFINE__ 
#define __GAME_DEFINE__ 

#define max_range 5

#define UD_getInt CCUserDefault::sharedUserDefault()->getFloatForKey 
#define UD_getBool CCUserDefault::sharedUserDefault()->getBoolForKey 
#define UD_getDouble CCUserDefault::sharedUserDefault()->getDoubleForKey 
#define UD_getString CCUserDefault::sharedUserDefault()->getStringForKey 

#define UD_setInt CCUserDefault::sharedUserDefault()->setFloatForKey 
#define UD_setBool CCUserDefault::sharedUserDefault()->setBoolForKey 
#define UD_setDouble CCUserDefault::sharedUserDefault()->setDoubleForKey 
#define UD_setString CCUserDefault::sharedUserDefault()->setStringForKey 


#endif