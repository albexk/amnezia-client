#ifndef FOCUSCONTROLLER_H
#define FOCUSCONTROLLER_H

#include <QObject>
#include <QQuickItem>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include <QPoint>


class FocusController : public QObject
{
    Q_OBJECT
public:
    explicit FocusController(QQmlApplicationEngine* engine, QObject *parent = nullptr);
    ~FocusController();

    QQuickItem* nextKeyTabItem();
    QQuickItem* previousKeyTabItem();
    QQuickItem* nextKeyUpItem();
    QQuickItem* nextKeyDownItem();
    QQuickItem* nextKeyLeftItem();
    QQuickItem* nextKeyRightItem();
    QQuickItem* currentItem() const;

signals:
    void nextTabItemChanged(QQuickItem* item);
    void previousTabItemChanged(QQuickItem* item);
    void nextKeyUpItemChanged(QQuickItem* item);
    void nextKeyDownItemChanged(QQuickItem* item);
    void nextKeyLeftItemChanged(QQuickItem* item);
    void nextKeyRightItemChanged(QQuickItem* item);

public slots:
    void reload();

private:
    QQmlApplicationEngine* m_engine;
    QList<QQuickItem*> m_focus_chain;
    qsizetype m_current_index;
};

#endif // FOCUSCONTROLLER_H
