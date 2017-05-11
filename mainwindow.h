#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include <QTranslator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_cb_editable_clicked(bool checked);

    void on_pb_read_clicked();

    void on_pb_write_clicked();

    void on_pb_about_clicked();

    void on_actionlang_eng_triggered();

    void on_actionlang_chs_triggered();

    void on_actionlang_jpn_triggered();

    void on_le_steam_account_id_textChanged(const QString &arg1);

private:
    QLocale lang_locale;
    QTranslator lang_translator;
    void lang_switch(QLocale::Language lang);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
