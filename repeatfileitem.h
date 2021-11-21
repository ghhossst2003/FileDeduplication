#ifndef REPEATFILEITEM_H
#define REPEATFILEITEM_H

#include <QWidget>
#include <QStringList>

namespace Ui {
class RepeatFileItem;
}

class RepeatFileItem : public QWidget
{
    Q_OBJECT

public:
    explicit RepeatFileItem(QStringList * files, QWidget *parent = nullptr);
    ~RepeatFileItem();

private:
    Ui::RepeatFileItem *ui;
};

#endif // REPEATFILEITEM_H
