#ifndef HASHTHREAD_H
#define HASHTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QMap>
#include <QList>

class HashThread : public QThread
{
    Q_OBJECT
public:
    explicit HashThread(QMap<QString, QStringList*> * hash_files, QThread *parent = nullptr);
    void StopThread();
    void SetHashDir(const QString & dir);

private:
    QStringList FindFiles(const QString & dir);
    void HashFiles(const QStringList & file_list);

protected:
    virtual void run();

private:
    QString m_dir;
    QStringList m_filelist;
    QMap<QString, QStringList*> * m_hash_files;
    bool m_stop_flag;
    int m_file_size;
    int m_finish_file_size;

signals:
    void StartCalculateFile();
    void StartHashFile(int file_size);
    void HashFileSuccess(int progress, QString file);
    void HashFileError(QString file);
    void HashFinish();
};

#endif // HASHTHREAD_H
