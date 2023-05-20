#ifndef SHORTCUTWIDGET_H
#define SHORTCUTWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include "myCombobox.h"
#include <QCheckBox>

#include "hotKey.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class ShortcutWidget;
}

class ShortcutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShortcutWidget(QWidget *parent = nullptr);
    ~ShortcutWidget();

signals:
    void updateKeys(QVector<HotKey*>hotkeys);

private slots:
    void okButtonPressed();

private:
    Ui::ShortcutWidget *ui;

    QScrollArea* scrollArea;
    QWidget* contentWidget;
    QVBoxLayout* contentLayout;
    QVector<QHBoxLayout*>innerLayouts;
    QVector<QLabel*>labels;
    QPushButton *button;
    QVBoxLayout* mainLayout;

    QVector<QLineEdit*>lineEdits;
    QVector<MyCombobox*>comboboxes;
    QVector<QCheckBox*>ctrlBoxes;
    QVector<QCheckBox*>altBoxes;
    QVector<HotKey*>hotkeys;
};

#endif // SHORTCUTWIDGET_H
