#include "registerwidget.h"
#include "ui_registerwidget.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>
RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterWidget)
{
    ui->setupUi(this);

    // 连接注册按钮的点击信号到槽函数
    connect(ui->registerButton, &QPushButton::clicked, this, &RegisterWidget::on_registerButton_clicked);
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}

void RegisterWidget::on_registerButton_clicked() {
    // 获取昵称和密码
    QString nickname = ui->nameLineEdit->text();  // 获取昵称
    QString password = ui->passwordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();

    // 验证输入是否为空
    if (nickname.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "昵称和密码不能为空！");
        return;
    }

    // 验证密码和确认密码是否一致
    if (password != confirmPassword) {
        QMessageBox::warning(this, "输入错误", "两次输入的密码不一致！");
        return;
    }

    // 生成随机账户
    QString account = generateRandomAccount();

    // 显示生成的随机账户
    //QMessageBox::information(this, "生成的账户", "您的账户为: " + account);

    // 插入数据到数据库
    QSqlQuery query;
    query.prepare("INSERT INTO users (nickname, account, password) VALUES (:nickname, :account, :password)");

    // 绑定值
    query.bindValue(":nickname", nickname.isEmpty() ? QVariant(QVariant::String) : nickname); // 处理空值
    query.bindValue(":account", account);
    query.bindValue(":password", password);

    if (!query.exec()) {
        if (query.lastError().text().contains("Duplicate entry")) {
            QMessageBox::warning(this, "注册失败", "账户已存在！");
        } else {
            QMessageBox::warning(this, "注册失败", "数据库错误：" + query.lastError().text());
        }
        return;
    }

    // 显示注册成功消息
    QMessageBox::information(this, "注册成功", "账号注册成功！您的账户为: " + account);
    ui->nameLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->confirmPasswordLineEdit->clear();

    // 关闭注册页面
    this->close();
}
QString RegisterWidget::generateRandomAccount() {
    const QString possibleCharacters = "0123456789";
    const int accountLength = 8;  // 随机账户的长度
    QString randomAccount;

    for (int i = 0; i < accountLength; ++i) {
        int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        QChar nextChar = possibleCharacters.at(index);
        randomAccount.append(nextChar);
    }

    return randomAccount;
}
