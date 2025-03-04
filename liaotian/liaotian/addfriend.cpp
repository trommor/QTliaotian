#include "addfriend.h"
#include "ui_addfriend.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

addfriend::addfriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addfriend)
{
    ui->setupUi(this);

    // 连接按钮的点击信号到槽函数
    connect(ui->pushButton, &QPushButton::clicked, this, &addfriend::on_addButton_clicked);
}

addfriend::~addfriend()
{
    delete ui;
}

void addfriend::on_addButton_clicked()
{
    // 获取输入的账号
    QString account = ui->lineEdit->text().trimmed();

    if (account.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "账号不能为空！");
        return;
    }

    // 查询数据库
    QSqlQuery query;
    query.prepare("SELECT nickname FROM users WHERE account = :account");
    query.bindValue(":account", account);

    if (!query.exec()) {
        QMessageBox::warning(this, "数据库错误", "查询失败：" + query.lastError().text());
        return;
    }

    if (query.next()) {
        QString nickname = query.value(0).toString();
        QMessageBox::information(this, "添加成功", "已添加好友：" + nickname);

        // 发送信号，通知 MainWindow 添加好友
        emit friendAdded(nickname);

        // 关闭添加好友窗口
        this->close();
    } else {
        QMessageBox::warning(this, "添加失败", "未找到该账号！");
    }
}
