#include "shortcutWidget.h"
#include "ui_shortcutWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDebug>

ShortcutWidget::ShortcutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShortcutWidget)
{
    ui->setupUi(this);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    // Create a widget to hold the layout
    QWidget* contentWidget = new QWidget(scrollArea);

    // Create a layout for the content widget
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);

    // Create and add labels dynamically
    for (int i = 1; i <= 100; ++i)
    {
        QHBoxLayout* innerLayout = new QHBoxLayout;

        QLabel* label = new QLabel(QString("Phrase %1").arg(i));
        innerLayout->addWidget(label);

        QLineEdit *lineEdit = new QLineEdit();
        innerLayout->addWidget(lineEdit);
        lineEdits.append(lineEdit);

        MyCombobox *combobox = new MyCombobox();
        innerLayout->addWidget(combobox);
        comboboxes.append(combobox);

        QCheckBox *checkbox1 = new QCheckBox("Ctrl");
        innerLayout->addWidget(checkbox1);
        ctrlBoxes.append(checkbox1);

        QCheckBox *checkbox2 = new QCheckBox("Alt");
        innerLayout->addWidget(checkbox2);
        altBoxes.append(checkbox2);

        contentLayout->addLayout(innerLayout);
    }

    // Set the content widget for the scroll area
    scrollArea->setWidget(contentWidget);

    // Create a button
    QPushButton* button = new QPushButton("OK", this);
    connect(button, &QPushButton::pressed, this, &ShortcutWidget::okButtonPressed);

    // Create a layout for the main window
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scrollArea);
    mainLayout->addWidget(button);
}

ShortcutWidget::~ShortcutWidget()
{
    delete ui;
}

void ShortcutWidget::okButtonPressed()
{
    this->hide();

    for (int i = 0; i < hotkeys.size(); i++) {
        delete hotkeys.at(i);
    }

    hotkeys.clear();



    for (int i = 0; i < 100; i++) {
        if (!lineEdits.at(i)->text().isEmpty() && !comboboxes.at(i)->currentText().isEmpty()) {
            HotKey *key = new HotKey();
            key->phrase = lineEdits.at(i)->text();
            key->setCode(comboboxes.at(i)->currentText());
            key->setCtrl(ctrlBoxes.at(i)->isChecked());
            key->setAlt(altBoxes.at(i)->isChecked());

            hotkeys.append(key);
        }
    }

    emit updateKeys(hotkeys);
}
