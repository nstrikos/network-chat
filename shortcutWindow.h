#ifndef SHORTCUTWINDOW_H
#define SHORTCUTWINDOW_H

#include <QMainWindow>

namespace Ui {
class ShortcutWindow;
}

class ShortcutWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShortcutWindow(QWidget *parent = nullptr);
    ~ShortcutWindow();

private:
    Ui::ShortcutWindow *ui;
};

#endif // SHORTCUTWINDOW_H
