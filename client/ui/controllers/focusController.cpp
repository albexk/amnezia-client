#include "focusController.h"
#include <QQmlContext>
#include <algorithm>
#include <QWindow>


const QList<QObject*> getChildren(QObject* obj) {
    return obj->children();
}

// bool isFocusedOnTab(QObject* item)
// {
//     return item->property("activeFocusOnTab").toBool();
// }

bool isVisible(QObject* item)
{
    return item->property("visible").toBool();
}

bool isFocusable(QObject* item)
{
    return item->property("focusable").toBool();
}

QPointF getCenterPointOnScene(QQuickItem* item)
{
    const auto x0 = item->x() + (item->width() / 2);
    const auto y0 = item->y() + (item->height() / 2);
    return item->parentItem()->mapToScene(QPointF{x0, y0});
}

bool isLess(QObject* item1, QObject* item2)
{
    const auto p1 = getCenterPointOnScene(qobject_cast<QQuickItem*>(item1));
    const auto p2 = getCenterPointOnScene(qobject_cast<QQuickItem*>(item2));
    return (p1.y() == p2.y()) ? (p1.x() < p2.x()) : (p1.y() < p2.y());
}

bool isZeroCoords(QObject& item)
{
    // const auto i = qobject_cast
        return true;
}

// bool isOutsideTheScene(QQuickItem* item)
// {
//     const auto itemRect = item->childrenRect();
//     const auto win = item->window();
//     const auto rootItemRect = win-> // contentItem()->childrenRect();
//     return rootItemRect->intersects(itemRect);
// }

template<typename T>
void printItems(const T& items)
{
    for(const auto& item : items) {
        QQuickItem* i = qobject_cast<QQuickItem*>(item);
        QPointF coords {getCenterPointOnScene(i)};
        qDebug() << "=> Item: " << i << "; coords: " << coords;
    }
}

QList<QObject*> getChain(QObject* item)
{
    QList<QObject*> res;
    if (!item) {
        qDebug() << "null top item";
        return res;
    }
    const auto children = item->children();
    for(const auto child : children) {
        if (child && isFocusable(child) && qobject_cast<QQuickItem*>(child)) {
            res.append(child);
        }
        res.append(getChain(child));
    }
    return res;
}

FocusController::FocusController(QQmlApplicationEngine* engine, QObject *parent)
    : m_engine{engine}
    , m_focus_chain{}
    , m_current_index{0}
{
}

FocusController::~FocusController()
{

}

QObject *FocusController::nextKeyTabItem()
{
    if (m_focus_chain.empty()) return {};
    if (m_current_index == (m_focus_chain.size() - 1)) {
        m_current_index = 0;
    } else {
        m_current_index++;
    }
    return m_focus_chain.at(m_current_index);
}

QObject *FocusController::previousKeyTabItem()
{
    if (m_focus_chain.empty()) return {};
    if (m_current_index == 0) {
        m_current_index = m_focus_chain.size() - 1;
    } else {
        m_current_index--;
    }
    return m_focus_chain.at(m_current_index);
}

QObject *FocusController::nextKeyUpItem()
{
    return {};
}

QObject *FocusController::nextKeyDownItem()
{
    return {};
}

QObject *FocusController::nextKeyLeftItem()
{
    return {};
}

QObject *FocusController::nextKeyRightItem()
{
    return {};
}

QObject* FocusController::currentItem() const
{
    if(m_focus_chain.empty()) return {};
    return m_focus_chain.at(m_current_index);
}

void FocusController::reload()
{
    m_current_index = 0;
    m_focus_chain.clear();

    const auto rootObjects = m_engine->rootObjects();
    for(const auto object : rootObjects) {
        m_focus_chain.append(getChain(object));
    }
    std::sort(m_focus_chain.begin(), m_focus_chain.end(), isLess);
    qDebug() << "Focus Chain";
    printItems(m_focus_chain);
}
