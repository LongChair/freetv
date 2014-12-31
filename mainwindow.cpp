#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QNetworkRequest>
#include <QTextStream>
#include <QNetworkReply>
#include <QSurfaceFormat>
#include <QQmlEngine>
#include <QQmlContext>

///////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

//  setWindowFlags(Qt::FramelessWindowHint);
//  setAttribute(Qt::WA_TranslucentBackground);
//  setAttribute(Qt::WA_QuitOnClose);
//  setStyleSheet("background: transparent");

  m_view = new QQuickWidget(this);
  setCentralWidget(m_view);

//  m_view->setResizeMode(QQuickWidget::SizeRootObjectToView);
//  m_view->setWindowFlags(Qt::FramelessWindowHint);
//  m_view->setAttribute(Qt::WA_TranslucentBackground);

//  QSurfaceFormat format;
//  format.setAlphaBufferSize(8);
//  m_view->setFormat(format);
//  m_view->setClearColor(QColor(Qt::transparent));

  //m_view->setClearColor(QColor(127,127,127,127));

  // start grabbing the channel list
  QUrl url("http://mafreebox.freebox.fr/freeboxtv/playlist.m3u");
  QNetworkRequest request(url);


  QNetworkReply *reply = m_manager.get(request);

  connect(&m_manager, SIGNAL(finished(QNetworkReply*)), this,
              SLOT(channelListRetrieved(QNetworkReply*)));

}

///////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow() {  }

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::channelListRetrieved(QNetworkReply* reply)
{
  // check if we have an error
  if (reply->error())
  {
    qDebug() << "Failed to retrieve channel lists.";
    return;
  }

  // check if we had a redirection
  QVariant redirecturl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
  if (!redirecturl.toString().isEmpty())
  {
    qDebug() << "redirected to :" << redirecturl.toString();
    QNetworkReply *reply = m_manager.get(QNetworkRequest(redirecturl.toUrl()));
    return;
  }

  // othewise we have to parse the channel list
  QTextStream stream(reply->readAll());

  QString line;
  int channelID;
  QString channelName;
  QString channelUrl;

  do
  {
    line = stream.readLine();
    if (!line.isEmpty())
    {
      if (line.startsWith("#EXTM3U"))
      {
        // do nothing, its the header
      }
      else if (line.startsWith("#EXTINF"))
      {
        // extract the id and channel name
        QStringList list = line.split(",");
        QStringList values = list.at(1).split("-");
        channelID = values.at(0).trimmed().toInt();
        channelName = values.at(1).trimmed();
      }
      else if (!line.startsWith("#"))
      {
        m_channels[channelName] = new Channel(channelID, channelName, line);
      }
    }

  } while (!line.isEmpty());

  qDebug() << "We found" << m_channels.size() << "channels.";
  QStringList *list = new QStringList();
  foreach (Channel *C, m_channels)
  {
    //m_listWidget->addItem(QString("%1 - %2").arg(C->m_id, 5).arg(C->m_name));
    list->append(C->m_name);
  }

  QQmlContext *ctxt = m_view->engine()->rootContext();
  ctxt->setContextProperty("channelModel", QVariant::fromValue(*list));

  m_view->setSource(QUrl("qrc:/view.qml"));

}
