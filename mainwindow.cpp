#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDataStream>
#include <QLocale>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //basic config
    ui->le_steam_account_id->setReadOnly(true);
    ui->pb_write->setEnabled(false);

    //translation
    lang_switch(lang_locale.language());
}

void MainWindow::lang_switch(QLocale::Language lang)
{
    qApp->removeTranslator(&lang_translator);
    switch(lang)
    {
    case QLocale::English:
        lang_translator.load(":/lang/nier_automata_pc_savedata_tool_en_us.qm");
        qApp->installTranslator(&lang_translator);
        ui->retranslateUi(this);
        break;
    case QLocale::Chinese:
        lang_translator.load(":/lang/nier_automata_pc_savedata_tool_zh_cn.qm");
        qApp->installTranslator(&lang_translator);
        ui->retranslateUi(this);
        break;
    case QLocale::Japanese:
        lang_translator.load(":/lang/nier_automata_pc_savedata_tool_ja_jp.qm");
        qApp->installTranslator(&lang_translator);
        ui->retranslateUi(this);
        break;
    default:
        lang_translator.load(":/lang/nier_automata_pc_savedata_tool_en_us.qm");
        qApp->installTranslator(&lang_translator);
        ui->retranslateUi(this);
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cb_editable_clicked(bool checked)
{
    if(checked)
    {
        ui->le_steam_account_id->setReadOnly(false);
    }
    else
    {
        ui->le_steam_account_id->setReadOnly(true);
    }
}

void MainWindow::on_pb_read_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("sel_savedata_for_read"),
                                                    ".",
                                                    tr("nier_automata_pc_savedata")+" (*.dat)");
    if(!filename.isEmpty())
    {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly))
        {
            QDataStream file_ds(&file);
            file_ds.setByteOrder(QDataStream::LittleEndian);
            file.seek(0x04);

            qint64 steam_account_id;
            file_ds >> steam_account_id;
            ui->le_steam_account_id->setText(QString::number(steam_account_id));

            file.close();
            QMessageBox::information(this,this->windowTitle(),tr("succeed"),QMessageBox::Ok);
        }
        else
        {
            QMessageBox::critical(this,this->windowTitle(),tr("failed"),QMessageBox::Ok);
        }
    }
}

void MainWindow::on_pb_write_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("sel_savedata_for_write"),
                                                    ".",
                                                    tr("nier_automata_pc_savedata")+" (*.dat)");
    if(!filename.isEmpty())
    {
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite))
        {
            QDataStream file_ds(&file);
            file_ds.setByteOrder(QDataStream::LittleEndian);
            file.seek(0x04);

            qint64 steam_account_id;
            steam_account_id = ui->le_steam_account_id->text().toLongLong();
            file_ds << steam_account_id;

            file.close();
            QMessageBox::information(this,this->windowTitle(),tr("succeed"),QMessageBox::Ok);
        }
        else
        {
            QMessageBox::critical(this,this->windowTitle(),tr("failed"),QMessageBox::Ok);
        }
    }
}

void MainWindow::on_pb_about_clicked()
{
    QMessageBox::about(NULL,this->windowTitle(),tr("about_text"));
}

void MainWindow::on_actionlang_eng_triggered()
{
    lang_switch(QLocale::English);
}

void MainWindow::on_actionlang_chs_triggered()
{
    lang_switch(QLocale::Chinese);
}

void MainWindow::on_actionlang_jpn_triggered()
{
    lang_switch(QLocale::Japanese);
}

void MainWindow::on_le_steam_account_id_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->pb_write->setEnabled(false);
    }
    else
    {
        ui->pb_write->setEnabled(true);
    }
}
