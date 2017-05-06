#ifndef KEYBOARDHELPER_H
#define KEYBOARDHELPER_H

#include <QObject>

class QQuickView;

class KeyboardHelper : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardHelper(QQuickView *view, QObject *parent = 0);
    
    Q_INVOKABLE void pressKey(const QString &character, int key = 0);

private:
    QQuickView *m_view;
};

#endif // KEYBOARDHELPER_H
