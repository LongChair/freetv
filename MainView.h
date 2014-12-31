#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QtDeclarative/QDeclarativeView>
#include <QNetworkAccessManager>
#include <QMap>

#include "MainView.h"
#include "TvPlayer.h"
#include "Channel.h"

#ifdef TARGET_RPI
#include "DispmanxImage.h"
#endif


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


  TVPlayer m_tvplayer;

#ifdef TARGET_RPI
  DispmanxImage *m_image;
#endif
};


#endif // MAINVIEW_H
