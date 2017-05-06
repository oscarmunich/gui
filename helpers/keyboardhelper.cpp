#include "keyboardhelper.h"
#include <QQuickView>
#include <QGraphicsScene>
#include <QKeyEvent>

KeyboardHelper::KeyboardHelper(QQuickView *view, QObject *parent) :
    QObject(parent)
  , m_view(view)
{
}

void KeyboardHelper::pressKey(const QString &character, int key)
{
    QKeyEvent pressEvent(QKeyEvent::KeyPress, key, Qt::NoModifier, character);

    QGraphicsItem *focusItem;
   // TBDKMM QGraphicsItem *focusItem = m_view->scene()->focusItem();

    if (focusItem) {
    // TBDKMM    m_view->scene()->sendEvent(focusItem, &pressEvent);
    }
}
