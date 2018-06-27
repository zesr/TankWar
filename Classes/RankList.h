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

	/* tableview ��� */
	// ������tableviewʱ�����÷��� ����Ϊ��ǰ��tableview����
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
    // ��tableview���Ŵ����Сʱ�����÷���  ����Ϊ��ǰtableview����
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
    // ��cell�����ʱ���ø÷��� ����Ϊ��ǰ��tableview�����뱻�����cell����
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    // ����tableview��Cell��С
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    // ��ȡ���Ϊidx��cell
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    // ����tableview��cell�ĸ���
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	/* CCTextFieldTTF��� */
	// �����¼�
	bool onTouchBegan(Touch  *touch, Event  *event);

	// �ύ��ť�ص�����
	void menuSubmitCallback(Ref* pSender);

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

private:
    CCTextFieldTTF* textEdit;
	
	Rank p[max_range+1];

	
};

