#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>

namespace Ui {
class addfriend;
}

class addfriend : public QWidget
{
    Q_OBJECT

public:
    explicit addfriend(QWidget *parent = nullptr);
    ~addfriend();

signals:
    void friendAdded(const QString &friendName);  // 信号：添加好友成功

private slots:
    void on_addButton_clicked();  // 槽函数：点击添加按钮

private:
    Ui::addfriend *ui;
};

#endif // ADDFRIEND_H
