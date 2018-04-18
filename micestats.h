#ifndef MICESTATS_H
#define MICESTATS_H

#include <QMainWindow>
#include <QLabel>
#include <QSettings>
#include <QTranslator>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "micetitle.h"

namespace Ui {
    class MiceStats;
}

class MiceStats : public QMainWindow
{
    Q_OBJECT

public:
    explicit MiceStats(QWidget *parent = 0);
    ~MiceStats();

    QString link(const QString& micename);
    void parse(const QString& data);

public slots:
    void clear();
    void getData();
    void gotData(QNetworkReply* reply);

    void openLink(const QString& link);

    void saveXml();

    void goEn();
    void goPt();

    void showAbout();

private:
    Ui::MiceStats *ui;

    QNetworkAccessManager* manager;
    MiceTitle titles;

    QString last_xml;

    QTranslator* ten;
    QSettings* settings;
};

#endif // MICESTATS_H
