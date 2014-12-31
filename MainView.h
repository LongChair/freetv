#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QtDeclarative/QDeclarativeView>
#include <QNetworkAccessManager>
#include <QMap>

#include "MainView.h"


#ifdef TARGET_RPI
#include "DispmanxImage.h"
#endif

class Channel;

///////////////////////////////////////////////////////////////////////////////////////////////////
class MainView : public QDeclarativeView
{
public:
  MainView(QWidget *parent = NULL);
  ~MainView();

public slots:
    void channelListRetrieved(QNetworkReply* reply);

private:
  QNetworkAccessManager m_manager;
  QMap<QString, Channel*> m_channels;

#ifdef TARGET_RPI
  DispmanxImage *m_image;
#endif
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class Channel
{
public:
  Channel(int id, QString name, QString url) : m_id(id), m_name(name), m_url(url) {}

  int m_id;
  QString m_name;
  QString m_url;
};


#endif // MAINVIEW_H
