#ifndef MICETITLE_H
#define MICETITLE_H

#include <QHash>

class MiceTitle : public QHash<int, QString>
{
public:
    explicit MiceTitle();
    QString getTitle(int n);
};

#endif // MICETITLE_H
