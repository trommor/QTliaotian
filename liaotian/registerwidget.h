#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class RegisterWidget; }
QT_END_NAMESPACE

class RegisterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    ~RegisterWidget();

private slots:
    void on_registerButton_clicked();  // 注册按钮的槽函数
     QString generateRandomAccount(); // 声明生成随机账户的函数
     //bool isUsernameUnique(const QString &account);  // 声明函数
private:
    Ui::RegisterWidget *ui;

};

#endif // REGISTERWIDGET_H
