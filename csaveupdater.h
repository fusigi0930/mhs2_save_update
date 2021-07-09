#ifndef CSAVEUPDATER_H
#define CSAVEUPDATER_H

#include <QObject>

class CSaveUpdater : public QObject
{
    Q_OBJECT
public:
    explicit CSaveUpdater(QObject *parent = nullptr);
    virtual ~CSaveUpdater();

signals:
    void sigErrorMessage(QString msg);

public slots:
    void save_update(QString filename);
    void save_adjust(QString filename, QString id, QString count);
};

#endif // CSAVEUPDATER_H
