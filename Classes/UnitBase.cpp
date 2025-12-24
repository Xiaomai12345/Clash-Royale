#include "UnitBase.h"
#include "TowerBase.h"

USING_NS_CC;

bool UnitBase::init()
{
    if (!Node::init())
        return false;

    _playerId = 0;
    _unitType = 0;
    _level = 1;
    _lane = -1;

    _currentTarget = nullptr;
    _currentTargetTower = nullptr;
    _attackTimer = 0.0f;
    _isAttacking = false;

    _hasMoveTarget = false;
    _isFlying = false;

    return true;
}

void UnitBase::initUnit(int playerId, int level, const Vec2& position)
{
    _playerId = playerId;
    _level = level;

    // 基础属性（子类应该重写这些）
    _maxHealth = 100.0f;
    _currentHealth = _maxHealth;
    _attackDamage = 10.0f;
    _attackRange = 50.0f;
    _attackSpeed = 1.0f; // 每秒攻击一次
    _moveSpeed = 100.0f; // 像素/秒

    setPosition(position);

    // 创建精灵
    _unitSprite = Sprite::create();
    // 子类应该设置具体的精灵图片

    addChild(_unitSprite);

    // 创建血条
    createHealthBar();

    // 设置颜色区分阵营
    if (_playerId == 1)
    {
        _unitSprite->setColor(Color3B::BLUE);
    }
    else
    {
        _unitSprite->setColor(Color3B::RED);
    }

    // 开启更新
    scheduleUpdate();
}

void UnitBase::createHealthBar()
{
    _healthBar = ProgressTimer::create(Sprite::create());
    auto bar = LayerColor::create(Color4B::GREEN, 40, 6);
    _healthBar->addChild(bar);

    _healthBar->setType(ProgressTimer::Type::BAR);
    _healthBar->setBarChangeRate(Vec2(1, 0));
    _healthBar->setMidpoint(Vec2(0, 0.5));
    _healthBar->setPercentage(100);
    _healthBar->setPosition(0, 35);

    addChild(_healthBar, 10);
}

void UnitBase::update(float delta)
{
    Node::update(delta);

    if (!isAlive())
        return;

    _attackTimer += delta;

    // 检查攻击目标
    if (_currentTarget && _currentTarget->isAlive())
    {
        if (isTargetInRange())
        {
            if (!_isAttacking && _attackTimer >= 1.0f / _attackSpeed)
            {
                attack(_currentTarget);
                _attackTimer = 0.0f;
                _isAttacking = true;
            }
        }
        else
        {
            _isAttacking = false;
            // 向目标移动
            moveTo(_currentTarget->getPosition(), delta);
        }
    }
    else if (_currentTargetTower && _currentTargetTower->isAlive())
    {
        if (isTowerInRange())
        {
            if (!_isAttacking && _attackTimer >= 1.0f / _attackSpeed)
            {
                attackTower(_currentTargetTower);
                _attackTimer = 0.0f;
                _isAttacking = true;
            }
        }
        else
        {
            _isAttacking = false;
            // 向塔移动
            moveTo(_currentTargetTower->getPosition(), delta);
        }
    }
    else if (_hasMoveTarget)
    {
        moveTo(_moveTarget, delta);
        _isAttacking = false;
    }
    else
    {
        _isAttacking = false;
    }

    // 更新血条
    updateHealthBar();
}

void UnitBase::setTarget(UnitBase* target)
{
    _currentTarget = target;
    _currentTargetTower = nullptr;

    if (target)
    {
        _hasMoveTarget = false;
        _isAttacking = false;
    }
}

void UnitBase::setTargetTower(TowerBase* tower)
{
    _currentTargetTower = tower;
    _currentTarget = nullptr;

    if (tower)
    {
        _hasMoveTarget = false;
        _isAttacking = false;
    }
}

void UnitBase::clearTarget()
{
    _currentTarget = nullptr;
    _currentTargetTower = nullptr;
    _isAttacking = false;
}

bool UnitBase::canAttack(UnitBase* target) const
{
    if (!target || !target->isAlive())
        return false;

    // 检查是否可以攻击该类型
    if (_isFlying)
    {
        // 飞行单位可以攻击地面和空中
        return true;
    }
    else
    {
        // 地面单位只能攻击地面单位
        return !target->isFlying();
    }
}

bool UnitBase::canAttackTower() const
{
    // 大多数单位可以攻击塔
    return true;
}

void UnitBase::moveTo(const Vec2& position, float delta)
{
    if (!isAlive())
        return;

    _moveTarget = position;
    _hasMoveTarget = true;

    Vec2 currentPos = getPosition();
    Vec2 direction = position - currentPos;

    // 如果已经到达目标位置
    if (direction.length() < 5.0f)
    {
        _hasMoveTarget = false;
        return;
    }

    // 标准化方向并移动
    direction.normalize();
    Vec2 newPos = currentPos + direction * _moveSpeed * delta;

    setPosition(newPos);

    // 更新精灵朝向
    if (direction.x > 0)
    {
        _unitSprite->setFlippedX(false);
    }
    else if (direction.x < 0)
    {
        _unitSprite->setFlippedX(true);
    }
}

bool UnitBase::hasReachedTarget() const
{
    if (!_hasMoveTarget)
        return true;

    Vec2 currentPos = getPosition();
    float distance = currentPos.distance(_moveTarget);

    return distance < 5.0f;
}

void UnitBase::takeDamage(float damage)
{
    if (!isAlive())
        return;

    _currentHealth -= damage;
    if (_currentHealth < 0)
        _currentHealth = 0;

    // 显示伤害效果
    playDamageEffect();

    // 如果死亡
    if (_currentHealth <= 0)
    {
        playDeathEffect();
        unscheduleUpdate();
        removeFromParent(); // 注意：实际应该通过BattleManager处理
    }
}

void UnitBase::attack(UnitBase* target)
{
    if (!target || !target->isAlive())
        return;

    // 造成伤害（实际伤害应该在BattleManager中计算）
    // target->takeDamage(_attackDamage);

    // 播放攻击动画
    playAttackEffect();

    // 通知BattleManager
    // BattleManager::getInstance()->unitAttack(this, target);
}

void UnitBase::attackTower(TowerBase* tower)
{
    if (!tower || !tower->isAlive())
        return;

    // 播放攻击动画
    playAttackEffect();

    // 通知BattleManager
    // BattleManager::getInstance()->unitAttackTower(this, tower);
}

bool UnitBase::isTargetInRange() const
{
    if (!_currentTarget || !_currentTarget->isAlive())
        return false;

    float distance = getPosition().distance(_currentTarget->getPosition());
    return distance <= _attackRange;
}

bool UnitBase::isTowerInRange() const
{
    if (!_currentTargetTower || !_currentTargetTower->isAlive())
        return false;

    float distance = getPosition().distance(_currentTargetTower->getPosition());
    return distance <= _attackRange;
}

void UnitBase::updateHealthBar()
{
    float percentage = (_currentHealth / _maxHealth) * 100.0f;
    _healthBar->setPercentage(percentage);

    // 根据血量改变颜色
    if (percentage > 50.0f)
    {
        _healthBar->setColor(Color3B::GREEN);
    }
    else if (percentage > 25.0f)
    {
        _healthBar->setColor(Color3B::YELLOW);
    }
    else
    {
        _healthBar->setColor(Color3B::RED);
    }
}

void UnitBase::playAttackEffect()
{
    // 攻击动画
    auto scaleUp = ScaleTo::create(0.1f, 1.2f);
    auto scaleDown = ScaleTo::create(0.1f, 1.0f);
    auto sequence = Sequence::create(scaleUp, scaleDown, nullptr);
    _unitSprite->runAction(sequence);
}

void UnitBase::playDamageEffect()
{
    // 受伤闪烁
    auto tintTo = TintTo::create(0.1f, 255, 100, 100);
    auto tintBack = TintTo::create(0.1f, 255, 255, 255);
    auto sequence = Sequence::create(tintTo, tintBack, nullptr);
    _unitSprite->runAction(sequence);

    // 伤害数字
    auto damageLabel = Label::createWithTTF("-", "Fonts/arial.ttf", 18);
    damageLabel->setPosition(0, 50);
    damageLabel->setTextColor(Color4B::RED);
    damageLabel->enableOutline(Color4B::BLACK, 1);

    addChild(damageLabel, 11);

    auto moveUp = MoveBy::create(0.5f, Vec2(0, 20));
    auto fadeOut = FadeOut::create(0.5f);
    auto spawn = Spawn::create(moveUp, fadeOut, nullptr);
    auto remove = CallFunc::create([damageLabel]() {
        damageLabel->removeFromParent();
        });

    damageLabel->runAction(Sequence::create(spawn, remove, nullptr));
}

void UnitBase::playDeathEffect()
{
    // 死亡粒子效果
    auto deathEffect = ParticleSystemQuad::create("Particles/death.plist");
    deathEffect->setPosition(getPosition());
    getParent()->addChild(deathEffect);

    // 单位消失动画
    auto fadeOut = FadeOut::create(0.3f);
    auto scaleDown = ScaleTo::create(0.3f, 0.1f);
    auto spawn = Spawn::create(fadeOut, scaleDown, nullptr);

    _unitSprite->runAction(spawn);
}