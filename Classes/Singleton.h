#ifndef SINGLETON_H
#define SINGLETON_H

#include <cassert>

template<typename T>
class Singleton
{
public:
    // 获取单例实例
    static T* getInstance()
    {
        if (!_instance)
        {
            _instance = new T();
        }
        return _instance;
    }

    // 销毁单例
    static void destroyInstance()
    {
        if (_instance)
        {
            delete _instance;
            _instance = nullptr;
        }
    }

    // 检查是否存在实例
    static bool hasInstance()
    {
        return _instance != nullptr;
    }

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    static T* _instance;
};

// 静态成员初始化
template<typename T>
T* Singleton<T>::_instance = nullptr;

#endif // SINGLETON_H