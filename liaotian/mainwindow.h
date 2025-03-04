#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onFunctionItemClicked(QListWidgetItem *item);  // 槽函数：功能项点击事件
    void initializeFriendList();//初始化联系人
    void updateFriendList();// 更新联系人列表
    void onFriendItemClicked(QTableWidgetItem *item);
    void onSendButtonClicked();  // 发送按钮的槽函数
    void checkForEnterKey();
    void loadChatHistory(const QString &friendName);
    void onSearchFriend();//搜索好友功能
    void addFriendToList(const QString &friendName);//添加好友功能
    void onAddFriendButtonClicked();  // 添加好友槽函数跳转
private:
    Ui::MainWindow *ui;
    QTextEdit *teChatEdit;  // 聊天输入框
    QPushButton *pushButton;  // 发送按钮
    QTextEdit *teChatHistory;  // 显示消息的标签
    QSqlDatabase db;  // 数据库对象
    QString currentFriend;  // 当前选中的联系人

};

#endif // MAINWINDOW_H
