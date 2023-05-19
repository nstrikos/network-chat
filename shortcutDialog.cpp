#include "shortcutDialog.h"
#include "ui_shortcutDialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

ShortcutDialog::ShortcutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShortcutDialog)
{
    ui->setupUi(this);

    // Create the scroll area
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    // Create a widget to hold the layout
    QWidget* contentWidget = new QWidget(scrollArea);

    // Create the outer layout for the content widget
    QVBoxLayout* outerLayout = new QVBoxLayout(contentWidget);

    // Create and add layouts dynamically
    for (int i = 1; i <= 5; ++i)
    {
        // Create an inner layout for each section
        QHBoxLayout* innerLayout = new QHBoxLayout;

        // Create and add widgets dynamically to each inner layout
        for (int j = 1; j <= 10; ++j)
        {
            QLabel* label = new QLabel(QString("Widget %1-%2").arg(i).arg(j));
            innerLayout->addWidget(label);
        }

        // Add the inner layout to the outer layout
        outerLayout->addLayout(innerLayout);
    }

    // Set the content widget for the scroll area
    scrollArea->setWidget(contentWidget);
}

ShortcutDialog::~ShortcutDialog()
{
    delete ui;
}
