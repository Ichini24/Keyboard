#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <iostream>
#include <ctime>
#include "keyauth.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    void ProcAuthButton();
    void ProcTestButton();
    Odtime CheckPass(bool &validPass);
    void SetTableHeader();
    void SetRowTry(Odtime od, bool valid);
    void ResetAll();

    Ui::MainWindow *ui;
    KeyAuth *auth;
    int position;

    bool firstChar;
    int butcounter;
    std::clock_t prev;
    std::clock_t start;
    bool butcall;
    bool user;
    int checkCounter;
    bool authorizing;

};

#endif // MAINWINDOW_H
