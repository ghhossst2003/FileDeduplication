#include "repeatfileitem.h"
#include "ui_repeatfileitem.h"
#include <QCheckBox>
#include <QString>

RepeatFileItem::RepeatFileItem(QStringList * files, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RepeatFileItem)
{
    ui->setupUi(this);
    foreach(QString file, *files) {
        QCheckBox * box = new QCheckBox();
        box->setText(file);
        ui->verticalLayout->addWidget(box);
    }
}

RepeatFileItem::~RepeatFileItem()
{
    delete ui;
}
