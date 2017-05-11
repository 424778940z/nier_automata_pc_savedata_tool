#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDataStream>
#include <QLocale>
#include <QRegExp>

#define POS_STEAM_ACCOUNT_ID 0x04
#define POS_PLAYER_NAME 0x34
#define SIZE_PLAYER_NAME 0x20

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //basic config
    ui->le_steam_account_id->setReadOnly(true);
    QRegExpValidator *steam_account_id_validator = new QRegExpValidator(QRegExp("[0-9]{0,20}"), this);
    ui->le_steam_account_id->setValidator(steam_account_id_validator);

    ui->le_player_name->setReadOnly(true);
    QRegExpValidator *player_name_validator = new QRegExpValidator(QRegExp(".{0,16}"), this);
    ui->le_player_name->setValidator(player_name_validator);

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
        ui->le_player_name->setReadOnly(false);
    }
    else
    {
        ui->le_steam_account_id->setReadOnly(true);
        ui->le_player_name->setReadOnly(true);
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

            file.seek(POS_STEAM_ACCOUNT_ID);
            qint64 steam_account_id;
            file_ds >> steam_account_id;
            ui->le_steam_account_id->setText(QString::number(steam_account_id));

            file.seek(POS_PLAYER_NAME);
            QString player_name;
            char buffer[2];
            for(int i = 0; i < SIZE_PLAYER_NAME/2; i++)
            {
                file.read(buffer,sizeof(buffer));
                player_name.append(buffer);
            }
            ui->le_player_name->setText(player_name);

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

            file.seek(POS_STEAM_ACCOUNT_ID);
            qint64 steam_account_id;
            steam_account_id = ui->le_steam_account_id->text().toLongLong();
            file_ds << steam_account_id;

            file.seek(POS_PLAYER_NAME);
            QString player_name;
            player_name = ui->le_player_name->text();
            char buffer[32];
            for(int i = 0; i < SIZE_PLAYER_NAME/2; i++)
            {
                buffer[i*2] = player_name.toLatin1().data()[i];
                buffer[i*2+1] = '\0';
            }
            file.write(buffer,SIZE_PLAYER_NAME);

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

void MainWindow::refresh_empty_write_blocker()
{
    if(is_empty_le_steam_account_id || is_empty_le_player_name)
    {
        ui->pb_write->setEnabled(false);
        ui->pb_write->setEnabled(false);
    }
    else
    {
        ui->pb_write->setEnabled(true);
        ui->pb_write->setEnabled(true);
    }
}

void MainWindow::on_le_steam_account_id_textChanged(const QString &arg1)
{
    is_empty_le_steam_account_id = arg1.isEmpty();
    refresh_empty_write_blocker();
}

void MainWindow::on_le_player_name_textChanged(const QString &arg1)
{
    is_empty_le_player_name = arg1.isEmpty();
    refresh_empty_write_blocker();
}
