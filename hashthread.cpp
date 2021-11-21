#include "hashthread.h"
#include <QDir>
#include <QCryptographicHash>

HashThread::HashThread(QMap<QString, QStringList*> * hash_files, QThread *parent) :
    QThread(parent), m_dir(""), m_hash_files(hash_files), m_stop_flag(false)
{
    m_file_size = 0;
    m_finish_file_size = 0;
}

void HashThread::SetHashDir(const QString & dir)
{
    m_dir = dir;
}
/*
找到目标文件夹下所有的文件
对目录下的文件进行hash操作
建立map<hash_value, file_list>
把file_list>1的显示到界面
*/
void HashThread::run()
{
    if(m_dir.isEmpty()) {
        return;
    }
    m_stop_flag = false;
    m_file_size = 0;
    m_finish_file_size = 0;
    emit StartCalculateFile();
    QStringList file_list = FindFiles(m_dir);//遍历文件夹下所有文件
    m_file_size = file_list.size();
    emit StartHashFile(m_file_size);
    HashFiles(file_list);
}

QStringList HashThread::FindFiles(const QString & path)
{
    qDebug() << path;
    QStringList file_list;
    QDir dir(path);
    dir.setFilter(QDir::Dirs|QDir::Files);//除了目录或文件，其他的过滤掉
    dir.setSorting(QDir::DirsFirst);//优先显示目录
    QFileInfoList allfile = dir.entryInfoList();
    qDebug() << allfile;
    foreach(QFileInfo file_info, allfile) {
        if(file_info.fileName() == "."||file_info.fileName() == ".."){
            continue;
        }
        if(file_info.isDir()){
            QStringList files = FindFiles(file_info.absoluteFilePath());
            file_list.append(files);
        }else{
            file_list << file_info.absoluteFilePath();
        }
    }
    return file_list;
}

void HashThread::HashFiles(const QStringList & file_list)
{
    foreach(QString file, file_list){
        if(m_stop_flag == true) {
            break;
        }
        QFile f(file);
        if(f.open(QFile::ReadOnly)){
            QCryptographicHash hash(QCryptographicHash::Sha1);
            hash.addData(&f);
            QString sha1 = hash.result().toHex();
            f.close();
            if(m_hash_files->contains(sha1)){
                m_hash_files->value(sha1)->append(file);
            }else{
                QStringList * same_hash_file_list = new QStringList();
                same_hash_file_list->append(file);
                m_hash_files->insert(sha1, same_hash_file_list);
            }
            qDebug() << "HashThread::HashFiles" << file << " : " << sha1;
            m_finish_file_size++;
            int progress = (m_finish_file_size * 100)/m_file_size;
            emit HashFileSuccess(progress, file);
        }else{
            qDebug() << "HashThread::HashFiles" << file;
            emit HashFileError(file);
        }
    }
    if(m_stop_flag != true){
        emit HashFinish();
    }
}

void HashThread::StopThread()
{
    m_stop_flag = true;
}
