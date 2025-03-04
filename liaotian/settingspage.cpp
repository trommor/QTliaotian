#include "settingspage.h"
#include <QComboBox>
#include <QVBoxLayout>

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 主题选择框
    themeComboBox = new QComboBox(this);
    themeComboBox->addItems({"浅色主题", "深色主题"});
    layout->addWidget(themeComboBox);
}
