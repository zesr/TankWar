#pragma once

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "GameDefine.h"
#include "Start.h"

USING_NS_CC;
USING_NS_CC_EXT;

struct Rank
{
	std::string name;
	float score;
};

class RankList : public Layer,TableViewDataSource,TableViewDelegate
{
public:
	virtual bool init();  
    static CCScene* createScene(float input,bool type);  
    CREATE_FUNC(RankList);  

	/* tableview 相关 */
	// 当滑动tableview时触发该方法 参数为当前的tableview对象
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
    // 当tableview被放大或缩小时触发该方法  参数为当前tableview对象
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
    // 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    // 设置tableview的Cell大小
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    // 获取编号为idx的cell
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    // 设置tableview中cell的个数
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	/* CCTextFieldTTF相关 */
	// 触摸事件
	bool onTouchBegan(Touch  *touch, Event  *event);

	// 提交按钮回调函数
	void menuSubmitCallback(Ref* pSender);

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

private:
    CCTextFieldTTF* textEdit;
	
	Rank p[max_range+1];

	
};

