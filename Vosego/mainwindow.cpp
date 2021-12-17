#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->columnWidth(800);
    last_item = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_toolButton_3_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                       tr("Open File"), QDir::currentPath(), tr("Imagenes: (*.png *.jpg *.bmp *.gif *.tif)"));
    if (!fileName.isEmpty()) {
        QImage imagen;
        if(imagen.load(fileName)){
            ui->label_3->setPixmap(QPixmap::fromImage(imagen));
            ui->label_3->setToolTip(fileName);
            ui->lineEdit->setText(fileName);
            nombre_fichero_unitario = fileName;
        }
    }
}


void MainWindow::on_toolButton_4_clicked()
{

}


void MainWindow::on_toolButton_5_clicked()
{
    if(nombre_fichero_unitario != nullptr) {
        ui->tableWidget->setItem(last_item, 0, new QTableWidgetItem(nombre_fichero_unitario));
        last_item++;
        ui->progressBar->setEnabled(true);
        ui->toolButton->setEnabled(true);
    }
}


void MainWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    if(!item->text().isEmpty()) {
        QImage imagen;
        if(imagen.load(item->text())){
            ui->label_3->setPixmap(QPixmap::fromImage(imagen));
            ui->label_3->setToolTip(item->text());
        }
    }
}

