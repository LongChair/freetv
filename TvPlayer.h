#ifndef TVPLAYER_H
#define TVPLAYER_H

#include <QObject>
#include <QDebug>
#include <QMap>
#include <QVector>
#include <QList>
#include "Channel.h"

class TVPlayer : public QObject
{
    Q_OBJECT
private:
    QList<Channel*> m_channels;

public:
    Q_INVOKABLE void play(int id);
    Q_INVOKABLE int canal(int id);
    QList<Channel*> &getChannels() { return m_channels; }
};

#endif // TVPLAYER_H
