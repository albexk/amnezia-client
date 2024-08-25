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

    QObject* nextKeyTabItem();
    QObject* previousKeyTabItem();
    QObject* nextKeyUpItem();
    QObject* nextKeyDownItem();
    QObject* nextKeyLeftItem();
    QObject* nextKeyRightItem();
    Q_INVOKABLE QQuickItem* currentFocusedItem() const;

signals:
    void nextTabItemChanged(QObject* item);
    void previousTabItemChanged(QObject* item);
    void nextKeyUpItemChanged(QObject* item);
    void nextKeyDownItemChanged(QObject* item);
    void nextKeyLeftItemChanged(QObject* item);
    void nextKeyRightItemChanged(QObject* item);

public slots:
    void reload();

private:
    QQmlApplicationEngine* m_engine;
    QList<QObject*> m_focus_chain;
    qsizetype m_current_index;
};

#endif // FOCUSCONTROLLER_H
