#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QMap>
#include <QListWidget>
#include <QtQuickWidgets/QQuickWidget>

namespace Ui {
  class MainWindow;
}

class Channel
{
public:
  Channel(int id, QString name, QString url) : m_id(id), m_name(name), m_url(url) {}

  int m_id;
  QString m_name;
  QString m_url;
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
    void channelListRetrieved(QNetworkReply* reply);

private:
  QListWidget *m_listWidget;
  QQuickWidget *m_view;

  QNetworkAccessManager m_manager;

  QMap<QString, Channel*> m_channels;
};

#endif // MAINWINDOW_H
