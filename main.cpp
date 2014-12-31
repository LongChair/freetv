#include <QApplication>

#include "MainView.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  MainView view;
  view.show();

  return a.exec();
}
