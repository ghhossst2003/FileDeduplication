#ifndef SAMEFILELISTWIDGET_H
#define SAMEFILELISTWIDGET_H

#include <QWidget>
#include <QMap>
#include <QStringList>
#include "hashthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SameFileListWidget; }
QT_END_NAMESPACE

class SameFileListWidget : public QWidget
{
    Q_OBJECT

public:
    SameFileListWidget(QWidget *parent = nullptr);
    ~SameFileListWidget();

private:
    void ShowSameFileList();
    void ClearData();
    void ClearUi();

private:
    Ui::SameFileListWidget *ui;

    QMap<QString, QStringList*> * m_hash_files; //核心数据，同一个哈希值的文件和文件的List组成一个map
    HashThread * m_hash_thread;

public slots:
    void Start();
    void Cancel();
    void Delete();
    void StartCalculateFile();
    void StartHashFile(int file_size);
    void HashFileSuccess(int progress, QString file);
    void HashFileError(QString file);
    void HashFinish();
};
#endif // SAMEFILELISTWIDGET_H
