#include <QApplication>

#include "MainView.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  MainView view;
  view.setSource(QUrl("qrc:/view.qml"));
  view.show();

  return a.exec();
}
