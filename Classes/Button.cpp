#include"Button.h"

GameButton* GameButton::create(
	const std::string& normalImg,
	const std::string& selectedImg,
	const std::string& disabledImg,
	const std::string& btnText,
	const std::string& fontPath,
	int fontSize,
	const std::function<void()>& clickCallback
) 
{
	GameButton* btn = new (std::nothrow) GameButton();

	//如果创建成功
	if (btn && btn->init()) 
	{
		// 初始化按钮图片样式（加载正常/按下/禁用态图片）
		btn->initImgStyle(normalImg, selectedImg, disabledImg);

		// 如果传入了按钮文字，初始化文字样式
		if (!btnText.empty()) {
			btn->initTextStyle(btnText, fontPath, fontSize);
		}

		// 绑定点击事件
		btn->bindClickEvent(clickCallback);

		// 设置自动释放，避免内存泄漏
		btn->autorelease();

		return btn;
	}
	// 若初始化失败，安全删除对象
		CC_SAFE_DELETE(btn);
	return nullptr;
}

//设置按钮拉伸
void GameButton::setButtonScale9Enabled(bool enabled, const Rect& capInsets)
{
	// 开启/关闭九宫格拉伸
	this->setScale9Enabled(enabled);
	// 如果开启，设置拉伸区域（capInsets：四周不拉伸的像素）
	if (enabled) {
		this->setCapInsets(capInsets);
	}
}

//设置按钮音效
void  GameButton::setClickEffectPath(const std::string& effectPath)
{
	clickEffect = effectPath;
}

//设置按钮禁用
void  GameButton::setBtnEnabled(bool isEnabled)
{
	// 更新成员变量
	isBtnEnabled = isEnabled;
	// 调用父类方法禁用/启用按钮（禁用后无法点击）
	this->setEnabled(isEnabled);

	// 视觉反馈：禁用态按钮变灰
	if (!isEnabled) {
		this->setColor(Color3B(150, 150, 150)); // 灰色
	}
	else {
		this->setColor(Color3B(255, 255, 255)); // 恢复原色
	}
}
bool  GameButton::init()
{
	if (!cocos2d::ui::Button::init())
	{
		return false;
	}

	// 初始化成员变量默认值
	clickEffect = "music/effect/button_click.wav"; // 默认点击音效
	isBtnEnabled = true; // 默认按钮可用

	return true;
}

	//初始化图片样式
void  GameButton::initImgStyle(const std::string& normalImg, const std::string& selectedImg, const std::string& disabledImg)
{
	// 加载按钮图片：normalImg（必填）、selectedImg（按下态）、disabledImg（禁用态）
	// TextureResType::LOCAL：表示加载本地图片
	this->loadTextures(normalImg, selectedImg, disabledImg, cocos2d::ui::TextureResType::LOCAL);

	// 自动适配按钮尺寸为图片原始尺寸
	Size imgSize = this->getVirtualRendererSize();
	this->setContentSize(imgSize);

	// 调试日志：方便排查图片加载问题
	CCLOG("按钮图片加载完成，尺寸：%.0fx%.0f", imgSize.width, imgSize.height);
}

	//初始化文本
void  GameButton::initTextStyle(const std::string& btnText, const std::string& fontPath, int fontSize)
{
	// 创建TTF文字标签（适配你的字体路径）
	auto btnLabel = Label::createWithTTF(btnText, fontPath, fontSize);
	if (btnLabel == nullptr)
	{
		CCLOG("按钮文字创建失败！字体路径：%s", fontPath.c_str());
		return;
	}

	// 设置文字颜色（默认白色）
	btnLabel->setColor(Color3B::WHITE);
	// 设置文字居中
	btnLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);

	// 将文字绑定到按钮上
	this->setTitleLabel(btnLabel);
	_btnTitleLabel = btnLabel;
}

	//绑定点击事件
void  GameButton::bindClickEvent(const std::function<void()>& clickCallback)
{
	//触摸状态监听（用于切换文字颜色）
	this->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
		// 确保文字标签存在
		if (_btnTitleLabel == nullptr) {
			return;
		}

		// 根据触摸状态切换文字颜色
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			// 按钮被按下：文字设为黄色
			_btnTitleLabel->setColor(Color3B(255, 240, 0));
			break;
		case ui::Widget::TouchEventType::ENDED:
		case ui::Widget::TouchEventType::CANCELED:
			// 按钮抬起/触摸取消：文字恢复白色
			_btnTitleLabel->setColor(Color3B::WHITE);
			break;
		default:
			break;
		}
		});

	// 绑定Cocos UI按钮的点击事件
	this->addClickEventListener([=](Ref* sender) {
		// 如果按钮禁用，直接返回
		if (!isBtnEnabled) {
			CCLOG("按钮已禁用，不执行点击逻辑");
			return;
		}

		// 1. 点击缩放反馈（
		this->runAction(Sequence::create(
			ScaleTo::create(0.1f, 0.95f),
			ScaleTo::create(0.1f, 1.0f),
			nullptr
		));

		// 2. 播放点击音效
		AudioManager::getInstance()->playEffect(clickEffect);

		// 3. 执行外部传入的回调
		if (clickCallback) {
			clickCallback();
		}
		});
}
