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
    void save_update(QString filename, QString count);
    void save_adjust(QString filename, QString id, QString count);
    void save_addWeapon(QString filename);
};

#endif // CSAVEUPDATER_H
