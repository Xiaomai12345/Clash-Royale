#ifndef BUTTON_H
#define BUTTON_H

#include"cocos2d.h"
#include"ui/CocosGUI.h"
#include"AudioManager.h"

USING_NS_CC;

class GameButton : public cocos2d::ui::Button{
public:
	//构造接口
	static GameButton* create(
		const std::string& normalImg,//按钮正常状态图片
		const std::string& selectedImg,//按钮按下去图片
		const std::string& disabledImg,//按钮禁用图片
		const std::string& btnText = "",//按钮文本
		const std::string& fontPath = "fonts/Clash_Regular.otf",
		int fontSize = 36,
		const std::function<void()>& clickCallback = nullptr
	);
	//设置按钮拉伸
	void setButtonScale9Enabled(bool enabled, const Rect& capInsets = Rect(10, 10, 10, 10));
	//设置按钮音效
	void setClickEffectPath(const std::string& effectPath);
	//设置按钮禁用
	void setBtnEnabled(bool isEnabled);
protected:
	virtual bool init() override;
	//初始化图片样式
	void initImgStyle(const std::string& normalImg, const std::string& selectedImg, const std::string& disabledImg);
	//初始化文本
	void initTextStyle(const std::string& btnText, const std::string& fontPath , int fontSize);
	//绑定点击事件
	void bindClickEvent(const std::function<void()>& clickCallback);

	//成员变量
	std::string clickEffect;
	bool isBtnEnabled;
	Label* _btnTitleLabel;
};
#endif //BUTTON_H