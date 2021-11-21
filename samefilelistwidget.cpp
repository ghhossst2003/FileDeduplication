#include "samefilelistwidget.h"
#include "ui_samefilelistwidget.h"
#include <QFile>
#include <QDir>
#include <QtDebug>
#include <QMessageBox>
#include <QFileDialog>
#include "repeatfileitem.h"
#include <QFileInfo>

SameFileListWidget::SameFileListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SameFileListWidget)
{
    ui->setupUi(this);
    m_hash_files = new QMap<QString, QStringList*>();
    m_hash_thread = new HashThread(m_hash_files);

    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    connect(m_hash_thread, SIGNAL(StartCalculateFile()), this, SLOT(StartCalculateFile()));
    connect(m_hash_thread, SIGNAL(StartHashFile(int)), this, SLOT(StartHashFile(int)));
    connect(m_hash_thread, SIGNAL(HashFileSuccess(int, QString)),
            this, SLOT(HashFileSuccess(int, QString)));
    connect(m_hash_thread, SIGNAL(HashFileError(QString)), this, SLOT(HashFileError(QString)));
    connect(m_hash_thread, SIGNAL(HashFinish()), this, SLOT(HashFinish()));
    connect(ui->startPushButton, SIGNAL(clicked()), this, SLOT(Start()));
    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(Cancel()));
    connect(ui->deletePushButton, SIGNAL(clicked()), this, SLOT(Delete()));
}


//先删除界面上的内容显示元素，避免删除数据的时候界面上错误
//再删除内存数据
SameFileListWidget::~SameFileListWidget()
{
    //增加释放界面
    ClearUi();
    ClearData();//释放内存数据
    delete ui;
}

/*
选择目录
然后遍历目录下所有的文件
开始后台hash线程
*/
void SameFileListWidget::Start()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("open directory"), "/",
                                                    QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty()){
        return;
    }
    ClearData();
    ClearUi();
    qDebug() << "SameFileListWidget::Start()" << dir;
    m_hash_thread->SetHashDir(dir);
    m_hash_thread->start();
    ui->startPushButton->setEnabled(false);
}

void SameFileListWidget::Cancel()
{
    m_hash_thread->StopThread();
}

void SameFileListWidget::StartCalculateFile()
{
    ui->promptLabel->setText(tr("正在计算文件数量..."));
    ui->progressBar->setValue(0);
}

void SameFileListWidget::StartHashFile(int file_size)
{
    ui->promptLabel->setText(QString("文件数量统计完成,文件总数%1，准备对比文件...").arg(file_size));
}

void SameFileListWidget::HashFileSuccess(int progress, QString file)
{
    ui->progressBar->setValue(progress);
    ui->promptLabel->setText(QString("hash文件%1完成").arg(file));
}

void SameFileListWidget::HashFileError(QString file)
{
    ui->promptLabel->setText(QString("hash文件%1出错，暂停处理").arg(file));
    m_hash_thread->StopThread();
}

void SameFileListWidget::HashFinish()
{
    ui->promptLabel->setText(QString("文件hash完成，准备显示"));
    ShowSameFileList();
    ui->startPushButton->setEnabled(true);
}

void SameFileListWidget::ShowSameFileList()
{
    QStringList all_hask = m_hash_files->keys();
    if(all_hask.size() <= 0) {
        ui->promptLabel->setText("程序运行完成，没有内容重复的文件！");
    }
    foreach(QString hash, all_hask) {
        QStringList * hash_files = m_hash_files->value(hash);
        if(hash_files->size() > 1) {
            QFileInfo file_info(hash_files->at(0));
            QTreeWidgetItem * tree_widget_item = new QTreeWidgetItem();
            tree_widget_item->setText(0, file_info.fileName());
            foreach(QString file, *hash_files){
                QTreeWidgetItem * child_tree_widget_item = new QTreeWidgetItem();
                child_tree_widget_item->setCheckState(0, Qt::CheckState::Unchecked);
                child_tree_widget_item->setText(0, file);
                child_tree_widget_item->setData(0, Qt::UserRole, file);
                tree_widget_item->addChild(child_tree_widget_item);
            }
            ui->treeWidget->addTopLevelItem(tree_widget_item);
        }
    }
    ui->promptLabel->setText("程序运行完成，请选择要删除的重复文件进行操作。");
}

void SameFileListWidget::ClearData()
{
    QStringList all_hask = m_hash_files->keys();
    foreach(QString hash, all_hask){
        delete m_hash_files->value(hash);
    }
    m_hash_files->clear();
}

void SameFileListWidget::ClearUi()
{
    int number_of_top_item = ui->treeWidget->topLevelItemCount();
    for(int i = number_of_top_item-1; i>=0; i--){
        QTreeWidgetItem * top_item = ui->treeWidget->topLevelItem(i);
        int number_of_child_item = top_item->childCount();
        for(int j=number_of_child_item-1; j>=0; j--){
            QTreeWidgetItem * child_item = top_item->child(j);
            top_item->removeChild(child_item);
            delete child_item;
        }
        ui->treeWidget->removeItemWidget(top_item, 0);
        delete top_item;
    }
}

void SameFileListWidget::Delete()
{
    int number_of_top_item = ui->treeWidget->topLevelItemCount();
    for(int i = 0; i<number_of_top_item; i++){
        QTreeWidgetItem * top_item = ui->treeWidget->topLevelItem(i);
        int number_of_child_item = top_item->childCount();
        for(int j=number_of_child_item-1; j>=0; j--){
            QTreeWidgetItem * child_item = top_item->child(j);
            if(child_item->checkState(0) == Qt::CheckState::Checked){
                QString data = child_item->data(0, Qt::UserRole).toString();
                QFile file(data);
                file.remove();
                top_item->removeChild(child_item);
                delete child_item;
            }
        }
    }
}
