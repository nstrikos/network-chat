#include "shortcutWindow.h"
#include "ui_shortcutWindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>

ShortcutWindow::ShortcutWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ShortcutWindow)
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
    for (int i = 1; i <= 25; ++i)
    {
        // Create an inner layout for each section
        QHBoxLayout* innerLayout = new QHBoxLayout;

//        // Create and add widgets dynamically to each inner layout
//        for (int j = 1; j <= 10; ++j)
//        {
            QLabel* label = new QLabel(QString("Widget %1-%2").arg(i));
            innerLayout->addWidget(label);

            QLineEdit *lineEdit = new QLineEdit();
            innerLayout->addWidget(lineEdit);

            QComboBox *comboBox = new QComboBox();
            innerLayout->addWidget(comboBox);

            QCheckBox *checkbox1 = new QCheckBox("Ctrl");
            innerLayout->addWidget(checkbox1);

            QCheckBox *checkbox2 = new QCheckBox("Alt");
            innerLayout->addWidget(checkbox2);
//        }

        // Add the inner layout to the outer layout
        outerLayout->addLayout(innerLayout);
    }

    // Set the content widget for the scroll area
    scrollArea->setWidget(contentWidget);

    //this->setCentralWidget(scrollArea);

    QPushButton* button = new QPushButton("Button");

    // Create a layout for the main window
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scrollArea);
    mainLayout->addWidget(button);
}

ShortcutWindow::~ShortcutWindow()
{
    delete ui;
}
