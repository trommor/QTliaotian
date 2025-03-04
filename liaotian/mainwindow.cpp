#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QStackedWidget>
#include <QTableWidgetItem>
#include "chatpage.h"  // 聊天页面
#include "contactpage.h"  // 联系人页面
#include "settingspage.h"  // 设置页面
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QMessageBox>  // 用于显示消息框

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置主页面标题
    this->setWindowTitle("聊天主页面");

    // 初始化功能列表
    ui->functionList->addItem("聊天");
    ui->functionList->addItem("联系人");
    ui->functionList->addItem("设置");

    // 初始化 stackedWidget
    ui->stackedWidget->addWidget(new ChatPage());  // 聊天页面
    ui->stackedWidget->addWidget(new ContactPage());  // 联系人页面
    ui->stackedWidget->addWidget(new SettingsPage());  // 设置页面

    // 默认隐藏 stackedWidget 和 tbwFriendList
    ui->stackedWidget->setVisible(false);
    ui->tbwFriendList->setVisible(false);

    // 连接功能列表的点击信号到槽函数
    connect(ui->functionList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onFunctionItemClicked(QListWidgetItem*)));

    // 初始化联系人列表
    initializeFriendList();

    // 连接联系人列表的点击信号到槽函数
    connect(ui->tbwFriendList, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onFriendItemClicked(QTableWidgetItem*)));

    // 初始化控件
    teChatEdit = ui->teChatEdit;  // 假设已经在 UI 文件中添加了 QTextEdit
    pushButton = ui->pushButton;  // 假设已经在 UI 文件中添加了 QPushButton
    teChatHistory = ui->teChatHistory;  // 假设已经在 UI 文件中添加了 QTextEdit（用于显示聊天记录）

    // 连接发送按钮的点击信号到槽函数
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onSendButtonClicked()));

    // 连接回车键信号到槽函数
    connect(ui->teChatEdit, SIGNAL(textChanged()), this, SLOT(checkForEnterKey()));

    // 显示头像照片
    QPixmap pix("E:\\QT\\build-liaotian-Desktop_Qt_5_14_2_MinGW_64_bit-Debug\\1.jpg");
    ui->lbProfilePhoto->setPixmap(pix);

    // 初始化 MySQL 数据库
    db = QSqlDatabase::addDatabase("QMYSQL", "qt-data");  // 指定连接名称
    db.setHostName("localhost");  // MySQL 服务器地址
    db.setPort(3306);  // MySQL 端口（默认 3306）
    db.setDatabaseName("qt-data");  // 数据库名称
    db.setUserName("root");  // MySQL 用户名
    db.setPassword("1804");  // MySQL 密码

    if (!db.open()) {
        qDebug() << "无法连接数据库：" << db.lastError().text();
        return;  // 如果连接失败，直接返回
    } else {
        qDebug() << "数据库连接成功！";
        QSqlQuery query(db);
        if (!query.exec("SET NAMES utf8mb4")) {
            qDebug() << "设置字符集失败：" << query.lastError().text();
        }
    }

    // 创建消息表（如果不存在）
    QSqlQuery query(db);  // 使用指定的数据库连接
    if (!query.exec(
        "CREATE TABLE IF NOT EXISTS messages ("
        "id INT AUTO_INCREMENT PRIMARY KEY, "
        "sender VARCHAR(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL, "
        "message TEXT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL, "
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "friend_name VARCHAR(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL)"
    )) {
        qDebug() << "创建表失败：" << query.lastError().text();
    }

    // 连接搜索框的 returnPressed 信号到槽函数
    connect(ui->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(onSearchFriend()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::checkForEnterKey() {
    if (ui->teChatEdit->toPlainText().endsWith("\n")) {  // 检测回车键
        onSendButtonClicked();
    }
}

void MainWindow::onFunctionItemClicked(QListWidgetItem *item) {
    // 根据点击的功能项切换右侧内容
    QString functionName = item->text();
    if (functionName == "聊天") {
        // 显示联系人列表
        ui->tbwFriendList->setVisible(true);

        // 隐藏 stackedWidget，直到选中联系人
        ui->stackedWidget->setVisible(false);

        // 更新联系人列表
        updateFriendList();
    } else if (functionName == "联系人") {
        // 切换到联系人页面
        ui->stackedWidget->setCurrentIndex(1);
        ui->stackedWidget->setVisible(true);
        ui->tbwFriendList->setVisible(false);  // 隐藏联系人列表
    } else if (functionName == "设置") {
        // 切换到设置页面
        ui->stackedWidget->setCurrentIndex(2);
        ui->stackedWidget->setVisible(true);
        ui->tbwFriendList->setVisible(false);  // 隐藏联系人列表
    }
}

void MainWindow::initializeFriendList() {
    // 初始化 tbwFriendList 控件
    ui->tbwFriendList->setColumnCount(1);  // 设置列数
    ui->tbwFriendList->setHorizontalHeaderLabels(QStringList() << "联系人");  // 设置表头
    ui->tbwFriendList->horizontalHeader()->setStretchLastSection(true);  // 最后一列拉伸
    ui->tbwFriendList->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 禁止编辑
}

void MainWindow::updateFriendList() {
    // 清空现有联系人列表
    ui->tbwFriendList->setRowCount(0);

    // 添加联系人
    QStringList friends = {"联系人一", "联系人二", "联系人三"};
    for (int i = 0; i < friends.size(); ++i) {
        ui->tbwFriendList->insertRow(i);  // 插入新行
        ui->tbwFriendList->setItem(i, 0, new QTableWidgetItem(friends[i]));  // 设置联系人名称
    }
}

void MainWindow::onFriendItemClicked(QTableWidgetItem *item) {
    // 显示 stackedWidget
    ui->stackedWidget->setVisible(true);

    // 切换到聊天页面
    ui->stackedWidget->setCurrentIndex(0);

    // 获取选中的联系人名称
    QString friendName = item->text();
    qDebug() << "选中了联系人：" << friendName;

    // 更新当前联系人
    currentFriend = friendName;

    // 更新聊天窗口标题
    this->setWindowTitle("与 " + friendName + " 聊天");

    // 加载该联系人的聊天记录
    loadChatHistory(friendName);
}

// 发送按钮的槽函数
void MainWindow::onSendButtonClicked()
{
    if (!db.isOpen()) {
        qDebug() << "数据库未连接！";
        return;
    }

    // 获取 teChatEdit 的内容
    QString message = teChatEdit->toPlainText().trimmed();  // 去除首尾空白字符

    if (!message.isEmpty()) {
        // 将消息插入数据库
        QSqlQuery query(db);  // 使用指定的数据库连接
        query.prepare("INSERT INTO messages (sender, message, friend_name) VALUES (:sender, :message, :friend_name)");
        query.bindValue(":sender", "我");  // 发送者名称
        query.bindValue(":message", message);  // 消息内容
        query.bindValue(":friend_name", currentFriend);  // 当前联系人名称

        if (!query.exec()) {
            qDebug() << "插入消息失败：" << query.lastError().text();
        }

        // 更新聊天记录显示
        loadChatHistory(currentFriend);

        // 清空 teChatEdit 的内容
        teChatEdit->clear();

        // 可选：自动滚动到底部
        teChatHistory->ensureCursorVisible();  // 确保光标可见
        teChatHistory->moveCursor(QTextCursor::End);  // 移动光标到底部
    }
}

void MainWindow::loadChatHistory(const QString &friendName) {
    if (!db.isOpen()) {
        qDebug() << "数据库未连接！";
        return;
    }

    // 清空当前聊天记录
    teChatHistory->clear();

    // 从数据库加载聊天记录
    QSqlQuery query(db);  // 使用指定的数据库连接
    query.prepare("SELECT sender, message, timestamp FROM messages WHERE friend_name = :friend_name ORDER BY timestamp ASC");
    query.bindValue(":friend_name", friendName);

    if (!query.exec()) {
        qDebug() << "加载聊天记录失败：" << query.lastError().text();
        return;
    }

    // 显示聊天记录
    while (query.next()) {
        QString sender = query.value(0).toString();
        QString message = query.value(1).toString();
        QString timestamp = query.value(2).toString();

        // 格式化消息
        QString formattedMessage = QString("[%1] %2: %3").arg(timestamp, sender, message);
        teChatHistory->append(formattedMessage);  // 追加到聊天记录中
    }
}

// 搜索好友的槽函数
void MainWindow::onSearchFriend() {
    QString searchText = ui->searchLineEdit->text().trimmed();  // 获取搜索框内容

    if (searchText.isEmpty()) {
        // 如果搜索框为空，显示所有好友
        updateFriendList();
        return;
    }

    // 清空现有联系人列表
    ui->tbwFriendList->setRowCount(0);

    // 过滤联系人
    QStringList friends = {"联系人一", "联系人二", "联系人三"};
    for (int i = 0; i < friends.size(); ++i) {
        if (friends[i].contains(searchText, Qt::CaseInsensitive)) {  // 不区分大小写匹配
            ui->tbwFriendList->insertRow(ui->tbwFriendList->rowCount());  // 插入新行
            ui->tbwFriendList->setItem(ui->tbwFriendList->rowCount() - 1, 0, new QTableWidgetItem(friends[i]));  // 设置联系人名称
        }
    }
}
