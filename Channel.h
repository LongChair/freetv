#ifndef CHANNEL_H
#define CHANNEL_H


///////////////////////////////////////////////////////////////////////////////////////////////////
class Channel
{
public:
  Channel(int id, QString name, QString url) : m_id(id), m_name(name), m_url(url) {}

  int m_id;
  QString m_name;
  QString m_url;
};


#endif // CHANNEL_H

