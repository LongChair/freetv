#include "MainView.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDeclarativeContext>

///////////////////////////////////////////////////////////////////////////////////////////////////
MainView::MainView(QWidget *parent) : QDeclarativeView(parent)
{
  // start grabbing the channel list
  QUrl url("http://mafreebox.freebox.fr/freeboxtv/playlist.m3u");
  QNetworkRequest request(url);

  m_manager.get(request);

  connect(&m_manager, &QNetworkAccessManager::finished, this,
              &MainView::channelListRetrieved);

#ifdef TARGET_RPI
  m_image = new DispmanxImage(1920, 1080);
  m_image->create();
#else
  channelListRetrieved(NULL);
#endif

  setStyleSheet("background:transparent;");
  setAttribute(Qt::WA_TranslucentBackground);
  setWindowFlags(Qt::FramelessWindowHint);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
MainView::~MainView()
{
#ifdef TARGET_RPI
  if (m_image)
  {
    delete m_image;
    m_image = NULL;
  }
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainView::channelListRetrieved(QNetworkReply* reply)
{

  // check if we have an error
  if ((!reply) || (reply->error()))
  {
    qDebug() << "Failed to retrieve channel lists.";

    for (int i=0; i < 40; i++)
    {
        qDebug() << "Adding item " << m_tvplayer.getChannels().size();
        m_tvplayer.getChannels().append(new Channel(i+1, QString("Channel %1").arg(i), QString("url %1").arg(i)));
    }
  }
  else
  {
      // check if we had a redirection
      QVariant redirecturl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
      if (!redirecturl.toString().isEmpty())
      {
        qDebug() << "redirected to :" << redirecturl.toString();
        m_manager.get(QNetworkRequest(redirecturl.toUrl()));
        return;
      }

      // othewise we have to parse the channel list
      QTextStream stream(reply->readAll());

      QString line;
      int channelID;
      QString channelName;

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
            m_tvplayer.getChannels().append(new Channel(channelID, channelName, line));
          }
        }

      } while (!line.isEmpty());
  }

  qDebug() << "We found" << m_tvplayer.getChannels().size() << "channels.";

  // Now build the Channel list for qml
  QStringList *list = new QStringList();
  foreach (Channel *C, m_tvplayer.getChannels())
  {
    list->append(QString("%1 - %2").arg(C->m_id, 5).arg(C->m_name));
  }

  rootContext()->setContextProperty("channelList", QVariant::fromValue(*list));


  // set the tv player object
  rootContext()->setContextProperty("tvplayer", &m_tvplayer);

  setSource(QUrl("qrc:/view.qml"));
  //showFullScreen();

}

