#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auth = new KeyAuth(1);

    ui->comboBox->addItem("Easy");
    ui->comboBox->addItem("Middle");
    ui->comboBox->addItem("Hard");

    ui->comboBox->setCurrentIndex(1);

    ui->label->setText("Введите Пароль");
    firstChar = false;
    butcounter = 0;
    position = 0;
    user = true;
    checkCounter = 0;
    butcall = false;
    authorizing = true;
    SetTableHeader();
    ui->tableWidget->setRowCount(0);
}

void MainWindow::SetTableHeader()
{
    //445
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setMaximumWidth(477);
    ui->tableWidget->setMinimumWidth(477);

    QString headers[5] = {"№", "Клавишный интервал", "Эталон", "Измеренный", "Дельта, %"};
    for(int i = 0; i < 5; i++)
    {
        QTableWidgetItem *header = new QTableWidgetItem();
        header->setText(headers[i]);
        ui->tableWidget->setHorizontalHeaderItem(i, header);
    }
    ui->tableWidget->setColumnWidth(0, 10);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 75);
    ui->tableWidget->setColumnWidth(3, 100);
    ui->tableWidget->setColumnWidth(4, 110);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if(!butcall)
    {
        clock_t clock = std::clock();
        if(!firstChar)
        {
            start = clock;
            prev = clock;
            firstChar = true;
        }
        else
        {
            double dtime = (clock - prev) / (double)CLOCKS_PER_SEC;
            auth->dtimes.push_back(dtime);
            prev = clock;
        }
        char c = arg1.toUtf8().constData()[position];
        auth->workingPass += c;
        position++;
    }
}

void MainWindow::ProcAuthButton()
{
    if (butcounter == 0)
    {
        if (auth->CheckVocPass(auth->workingPass))
        {
            butcounter++;
            auth->length = auth->workingPass.length();
            auth->MainPass = auth->workingPass;
            auth->workingPass = "";
            auth->authtimes.push_back(auth->dtimes);
            auth->dtimes.clear();
            ui->label->setText( "Введите пароль еще " + QString::number(3 - butcounter) + " раз");
        }
    }
    else if(butcounter <3)
    {
        int authcode = auth->CheckPrimaryPass(auth->workingPass);
        if (authcode == 1)
        {
            butcounter++;
            auth->workingPass = "";
            auth->authtimes.push_back(auth->dtimes);
            auth->dtimes.clear();
            ui->label->setText( "Введите пароль еще " + QString::number(3 - butcounter) + " раз");
        }
        else
        {
            QMessageBox::critical(this, "Ошибка", "Пароль введенный вами не совпадает с введенным ранее");
            auth->workingPass = "";
            auth->dtimes.clear();
        }
    }

    if(butcounter == 3)
    {
        ui->label->setText( "Теперь введите пароль для теста");
        authorizing = false;
        auth->calculateLimits();
    }
}

Odtime MainWindow::CheckPass(bool &validPass)
{
    Odtime od;
    int len;
    if(auth->workingPass.length() == auth->MainPass.length())
    {
        len = auth->workingPass.length();
        for(int i = 0; i < len; i++)
        {
            if(auth->workingPass[i] != auth->MainPass[i])
            {
                validPass = false;
            }
        }
    }
    else
    {
        validPass = false;
    }

    if(auth->dtimes.size() == auth->authtimes.size())
    {
        len = auth->dtimes.size();
    }
    else
    {
        ((auth->dtimes.size() < auth->authtimes.size()) ? len = auth->dtimes.size() : len = auth->authtimes.size());
    }

    for(int i = 0; i < len ; i++)
    {
        double curVal = auth->dtimes[i];
        if(auth->MainPass[i + 1] != auth->workingPass[i + 1])
        {
            od.valid.push_back(false);
        }
        else
        {
            double low = auth->lowLim[i];
            double high = auth->highLim[i];
            if(curVal > low && curVal < high)
            {
                od.valid.push_back(true);
            }
            else
            {
                od.valid.push_back(false);
                validPass = false;
            }
        }
        od.value.push_back(curVal);
    }
    return od;
}

void MainWindow::SetRowTry(Odtime od, bool valid)
{
    Qt::GlobalColor color;
    Qt::GlobalColor defcolor;
    if(valid)
    {
        defcolor = Qt::white;
    }
    else
    {
        defcolor = Qt::gray;
    }
    for(int i = 0; i < od.value.size(); i++)
    {
        color = defcolor;
        if(!od.valid[i])
        {
            color = Qt::red;
        }
        int rows = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rows);
        QTableWidgetItem *itm0 = new QTableWidgetItem(QString::number(checkCounter));
        itm0->setBackgroundColor(color);
        ui->tableWidget->setItem(rows, 0, itm0);

        QTableWidgetItem *itm1 = new QTableWidgetItem(QString::number((int)auth->workingPass[i]));
        itm1->setBackgroundColor(color);
        ui->tableWidget->setItem(rows, 1, itm1);

        QTableWidgetItem *itm2 = new QTableWidgetItem(QString::number(1000 * auth->lowLim[i]) + '-' + QString::number(1000 * auth->highLim[i]));
        itm2->setBackgroundColor(color);
        ui->tableWidget->setItem(rows, 2, itm2);

        QTableWidgetItem *itm3 = new QTableWidgetItem(QString::number(1000 * od.value[i]));
        itm3->setBackgroundColor(color);
        ui->tableWidget->setItem(rows, 3, itm3);

        QTableWidgetItem *itm4 = new QTableWidgetItem(QString::number(((od.value[i] - auth->ref[i])/auth->ref[i])*100));
        itm4->setBackgroundColor(color);
        ui->tableWidget->setItem(rows, 4, itm4);
    }


}

void MainWindow::ProcTestButton()
{
    checkCounter++;
    bool validPass = true;
    Odtime od = CheckPass(validPass);
    //int row = ui->tableWidget->rowCount();
    //ui->tableWidget->insertRow();
    SetRowTry(od, validPass);
    auth->workingPass = "";
    auth->dtimes.clear();

}

void MainWindow::on_pushButton_clicked()
{
    butcall = true;
    ui->lineEdit->clear();
    butcall = false;
    position = 0;
    firstChar = false;
    if(authorizing)
    {
        ProcAuthButton();
    }
    else
    {
        ProcTestButton();
    }
}

void MainWindow::ResetAll()
{
    ui->label->setText("Введите Пароль");
    delete auth;
    int level = ui->comboBox->currentIndex();
    auth = new KeyAuth(level);

    firstChar = false;
    butcounter = 0;
    position = 0;
    user = true;
    checkCounter = 0;
    butcall = false;
    authorizing = true;
    SetTableHeader();
    ui->tableWidget->setRowCount(0);
    ui->lineEdit->clear();
}
void MainWindow::on_pushButton_2_clicked()
{
    ResetAll();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ResetAll();
}
