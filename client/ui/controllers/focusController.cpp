#include "focusController.h"

FocusController::FocusController(QQmlApplicationEngine* engine, QObject *parent)
    : m_engine{engine}
    , m_focus_chain{}
    , m_current_index{0}
{
}

FocusController::~FocusController()
{

}

QQuickItem *FocusController::nextKeyTabItem()
{
}

QQuickItem *FocusController::previousKeyTabItem()
{
    return {};
}

QQuickItem *FocusController::nextKeyUpItem()
{
    return {};
}

QQuickItem *FocusController::nextKeyDownItem()
{
    return {};
}

QQuickItem *FocusController::nextKeyLeftItem()
{
    return {};
}

QQuickItem *FocusController::nextKeyRightItem()
{
    return {};
}

QQuickItem* FocusController::currentItem() const
{
    return {};
}

void FocusController::reload()
{
    
}
