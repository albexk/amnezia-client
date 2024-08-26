#include "focusController.h"
#include <QQmlContext>
#include <algorithm>
#include <QQuickWindow>


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

QRectF getCoordsOnScene(QQuickItem* item)
{
    if (!item) return {};
    return item->mapRectToScene(item->childrenRect());
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

bool isOnTheScene(QQuickItem* item)
{
    if (!item) {
        return false;
    }
    QRectF itemRect = item->mapRectToScene(item->childrenRect());
    QQuickWindow* window = item->window();
    QRectF windowRect = window->contentItem()->childrenRect();
    const auto res = windowRect.contains(itemRect);
    qDebug() << "itemRect: " << itemRect << "; windowRect: " << windowRect;
    return res;
}

template<typename T>
void printItems(const T& items)
{
    for(const auto& item : items) {
        QQuickItem* i = qobject_cast<QQuickItem*>(item);
        QPointF coords {getCenterPointOnScene(i)};
        qDebug() << "=> Item: " << i/* << "; coords: " << coords << ">>> "*/ << getCoordsOnScene(i);
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
        if (child && isFocusable(child) && qobject_cast<QQuickItem*>(child) && isOnTheScene(qobject_cast<QQuickItem*>(child))) {
            res.append(child);
        }
        res.append(getChain(child));
    }
    return res;
}

FocusController::FocusController(QQmlApplicationEngine* engine, QObject *parent)
    : m_engine{engine}
    , m_focus_chain{}
    , m_current_focused_item{nullptr}
    , m_current_index{-1}
{
    qDebug() << "FocusController ctor" << "triggered";
}

FocusController::~FocusController()
{
    qDebug() << "FocusController dtor" << "triggered";
}

QObject *FocusController::nextKeyTabItem()
{
    qDebug() << "nextKeyTabItem" << "triggered";

    if (m_focus_chain.empty()) {
        m_current_index = -1;
        return nullptr;
    }

    if (m_current_index == (m_focus_chain.size() - 1)) {
        m_current_index = 0;
    } else {
        m_current_index++;
    }
    return m_focus_chain.at(m_current_index);
}

QObject *FocusController::previousKeyTabItem()
{
    qDebug() << "previousKeyTabItem" << "triggered";

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
    qDebug() << "nextKeyUpItem" << "triggered";

    return {};
}

QObject *FocusController::nextKeyDownItem()
{
    qDebug() << "nextKeyDownItem" << "triggered";

    return {};
}

QObject *FocusController::nextKeyLeftItem()
{
    qDebug() << "nextKeyLeftItem" << "triggered";

    return {};
}

QObject *FocusController::nextKeyRightItem()
{
    qDebug() << "nextKeyRightItem" << "triggered";

    return {};
}

QQuickItem* FocusController::currentFocusedItem() const
{
    qDebug() << "currentFocusedItem" << "triggered";

    if(m_focus_chain.empty()) return {};
    qDebug() << "-> " << m_focus_chain;
    return qobject_cast<QQuickItem*>(m_focus_chain.at(m_current_index));
}

void FocusController::reload()
{
    qDebug() << "reload" << "triggered";

    m_current_index = -1;
    m_focus_chain.clear();

    const auto rootObjects = m_engine->rootObjects();
    for(const auto object : rootObjects) {
        m_focus_chain.append(getChain(object));
    }
    std::sort(m_focus_chain.begin(), m_focus_chain.end(), isLess);

    qDebug() << "===>> Focus Chain:";
    printItems(m_focus_chain);
}

// qsizetype FocusController::indexOfFocusedItem(QObject* item) const
// {
//     return m_focus_chain.indexOf(item);
// }
/*
    reload();
    if(m_focus_chain.empty()) {
        return {};
    }

    for (int i = 0; i < m_focus_chain.size(); i++) {
        if (m_current_focused_item == m_focus_chain[i]) {
            m_current_index = i;
            break;
        }
    }
    if(m_current_focused_item)
*/