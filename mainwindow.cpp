#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    openDB();
    createTableDB();
    ui->setupUi(this);
    ui->tableWidget->columnWidth(800);
    last_item = 0;
    ui->statusbar->showMessage("Tip!: Try to add an image and then press the 'Analize' button");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_toolButton_3_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                       tr("Open File"), QDir::currentPath(), tr("Imagenes: (*.png *.jpg *.jpeg *.bmp *.gif *.tif)"));
    if (!fileName.isEmpty()) {
        QImage imagen;
        if(imagen.load(fileName)){
            ui->label_3->setPixmap(QPixmap::fromImage(imagen));
            ui->label_3->setToolTip(fileName);
            ui->tabWidget->setEnabled(true);
            ui->toolButton_2->setEnabled(true);
            ui->lineEdit->setText(fileName);
            nombre_fichero_unitario = fileName;
            ui->statusbar->showMessage("File: '"+ nombre_fichero_unitario +"' Selected");
        }
    }
}


void MainWindow::on_toolButton_4_clicked()
{
    //assume the directory exists and contains some files and you want all jpg and JPG files
    QDir directory = QFileDialog::getExistingDirectory(this, tr("select directory"));
    ruta_Folder = directory.absolutePath();
    QStringList images = directory.entryList(QStringList() << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.gif" << "*.tif",QDir::Files);
    imagen_list.resize(0);
    foreach(QString filename, images) {
        imagen_list.push_back(filename);
    }
    ui->lineEdit_2->setText(ruta_Folder);
}


void MainWindow::on_toolButton_5_clicked()
{
    if(nombre_fichero_unitario != nullptr) {
        ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
        ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1,
                                 0,
                                 new QTableWidgetItem(nombre_fichero_unitario));
        ui->toolButton->setEnabled(true);
        ui->tableWidget->setEnabled(true);
        ui->toolButton_2->setEnabled(true);
        image_To_Analize.push_back(nombre_fichero_unitario);
        ui->statusbar->showMessage("File: '"+ nombre_fichero_unitario +"' added succesfully | Before You analize it, try to name it");
    }
    if(imagen_list.size() != 0) {
        for (auto imagen: imagen_list) {
            ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
            ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1,
                                     0,
                                     new QTableWidgetItem(ruta_Folder + "/" + imagen));
            image_To_Analize.push_back(ruta_Folder + "/" + imagen);
        }
        ui->toolButton->setEnabled(true);
        ui->tableWidget->setEnabled(true);
        ui->toolButton_2->setEnabled(true);
        ui->statusbar->showMessage("Folder: '"+ ruta_Folder +"' added succesfully | Before You analize it, try to name it");
    }
}


void MainWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    if(!item->text().isEmpty()) {
        QImage imagen;
        if(imagen.load(item->text())){
            item_seleccionado = item;
            ui->label_3->setPixmap(QPixmap::fromImage(imagen));
            ui->label_3->setToolTip(item->text());
            imagen_seleccionada = item->text();
        }
    }
}

int MainWindow::callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int MainWindow::callbackSelect(void *veryUsed, int argc, char **argv, char **azColName)
{
    std::vector<QString> aux;
    for(int i = 0; i<argc; i++) {
       aux[i] = argv[i];
    }
    qDebug() << aux;
    return 0;
}

void MainWindow::on_toolButton_4_triggered(QAction *arg1)
{

}


void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    callBackSelectTable.resize(0);
    sqlite3_stmt * selectStatement;
    std::string fecha = std::to_string(date.day()) + " " + std::to_string(date.month()) + " " + std::to_string(date.year());
    const char * select_sql = ("SELECT * FROM RESULTADOS WHERE FECHA='"+ fecha +"';").c_str();
    if (sqlite3_prepare_v2(db, select_sql, -1, &selectStatement, NULL) == SQLITE_OK)
    {
     while (sqlite3_step(selectStatement) == SQLITE_ROW)
     {
         std::string fecha((char *) sqlite3_column_text(selectStatement, 0));
         std::string foto((char *) sqlite3_column_text(selectStatement, 1));
         std::string estado((char *) sqlite3_column_text(selectStatement, 2));
         std::string hora((char *) sqlite3_column_text(selectStatement, 3));
         std::string enfermedad((char *) sqlite3_column_text(selectStatement, 4));
         callBackSelectTable.push_back({fecha, foto, estado, hora, enfermedad});
     }
    }

    qDebug() << callBackSelectTable.size();
    if (callBackSelectTable.size() > 1) {

        GraficaDiaForm* graph1 = new GraficaDiaForm(callBackSelectTable);
        graph1->show();
        graph1->activateWindow();
    } else {
        ui->statusbar->showMessage(("WARNING: " + fecha + " is Empty You did not work that day").c_str());
    }


}

std::string MainWindow::getNameOfPath(std::string path){
    std::string result;
    int i = path.length() - 1;
    while(path[i] != '/') {
        result += path[i];
        i--;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

void MainWindow::parsing_result(QString result)
{
    QString regist = "";
    QTime timestamp = QTime::currentTime();
    std::string name = ui->lineEdit_3->text().toStdString();
    std::string timeq = name + "_" + QTime::currentTime().toString().toStdString();
    ui->listWidget->addItem("(" + QDate::currentDate().toString() + " " + timestamp.toString() + ") Vosego>");
    for(unsigned int i = 0; i < result.length(); i++) {
        if(result[i] != '*')
            regist += result[i];
        else {
            insertResultDB(parse_line(regist), timeq);
            ui->listWidget->addItem(regist);
            regist = "";
        }

    }
    callBackSelectTable.resize(0);
    selectByHoraDB(timeq);
    GraficasResultadosForm* graph1 = new GraficasResultadosForm(callBackSelectTable);

    graph1->show();
    graph1->activateWindow();
}

std::vector<QString> MainWindow::parse_line(QString line)
{
    std::vector<QString> info(2);
    QStringList palabra_pivot = line.split(QString(" "));
    info[0] = palabra_pivot[1];
    info[1] = palabra_pivot[4];
    return info;

}

std::vector<std::vector<QString> > MainWindow::parse_Table_ResultadoDB(QString data)
{

}

void MainWindow::openDB()
{
    int rc;
    rc = sqlite3_open("D:\\clase\\SI\\Vosego\\Vosego\\results.db", &db);
    if( rc ) {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
       fprintf(stderr, "Opened database successfully\n");
    }
}

void MainWindow::closeDB()
{
    sqlite3_close(db);
}

void MainWindow::createTableDB()
{
    char *zErrMsg = 0;
    char* sql = "CREATE TABLE RESULTADOS("         \
              "FECHA           TEXT    NOT NULL,"  \
              "FOTO            TEXT     NOT NULL," \
              "ESTADO        TEXT    NOT NULL,"    \
              "HORA        TEXT    NOT NULL,"    \
              "ENFERMEDAD         TEXT    NOT NULL );";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
}

void MainWindow::insertResultDB(std::vector<QString> info, std::string fecha)
{
    char *zErrMsg = 0;
    std::string date = std::to_string(QDate::currentDate().day()) + " " + std::to_string(QDate::currentDate().month()) + " " + std::to_string(QDate::currentDate().year());
    std::string estado = (info[1].contains("healthy")) ? "Saludable": "Enfermo";
    std::string sql = "INSERT INTO RESULTADOS (FECHA,FOTO,ESTADO,HORA,ENFERMEDAD) "  \
                "VALUES ('"+ date +"', '"+ info [0].toStdString() +"', '"+ estado + "', '"+ fecha +"', '"+ info[1].toStdString() +"' ); ";

    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
}

void MainWindow::selectByHoraDB(std::string hora)
{
    sqlite3_stmt * selectStatement;
    const char * select_sql = ("SELECT * FROM RESULTADOS WHERE HORA='"+ hora +"';").c_str();
    if (sqlite3_prepare_v2(db, select_sql, -1, &selectStatement, NULL) == SQLITE_OK)
    {
     while (sqlite3_step(selectStatement) == SQLITE_ROW)
     {
         std::string fecha((char *) sqlite3_column_text(selectStatement, 0));
         std::string foto((char *) sqlite3_column_text(selectStatement, 1));
         std::string estado((char *) sqlite3_column_text(selectStatement, 2));
         std::string hora((char *) sqlite3_column_text(selectStatement, 3));
         std::string enfermedad((char *) sqlite3_column_text(selectStatement, 4));
         callBackSelectTable.push_back({fecha, foto, estado, hora, enfermedad});
     }
    }
}

void MainWindow::refreshTable()
{
    ui->tableWidget->setRowCount(0);
    for (auto imagen: image_To_Analize) {
        ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
        ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1,
                                 0,
                                 new QTableWidgetItem(imagen));
    }
}

void MainWindow::on_toolButton_clicked()
{
    ui->statusbar->showMessage("Analizing... This may take seconds");
    std::vector<std::string> image_To_remove;
    for(auto ruta: image_To_Analize) {
        image_To_remove.push_back("D:/clase/SI/Vosego/Vosego/aux_directory/test/" + getNameOfPath(ruta.toStdString()));
        copyFiles(ruta.toStdString().c_str(), ("D:/clase/SI/Vosego/Vosego/aux_directory/test/" + getNameOfPath(ruta.toStdString())).c_str());
    }

    QString result = execCommand("python.exe D:\\clase\\SI\\Vosego\\Vosego\\torchScript.py " + std::to_string(ui->tableWidget->rowCount()));
    parsing_result(result);
    for(auto ruta: image_To_remove) {
        remove( ruta.c_str() );
    }
    ui->statusbar->showMessage("Finished!");
}

QString MainWindow::execCommand(std::string command) {
    std::array<char, 128> buffer;
    QString result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void MainWindow::copyFiles(std::string source, std::string target)
{
    char a;
    std::ifstream fin {source, std::ios_base::binary};
    std::ofstream fout {target, std::ios_base::binary};
    while (fin.get(a))
        fout.put(a);
}



void MainWindow::on_toolButton_2_clicked()
{
    if(item_seleccionado != nullptr) {
        ui->statusbar->showMessage("File deleted'"+ item_seleccionado->text() +"'succesfully");
        image_To_Analize.erase(image_To_Analize.begin() + item_seleccionado->row());
        item_seleccionado = nullptr;
        refreshTable();
    }

}


void MainWindow::on_toolButton_6_clicked()
{
    if(nombre_fichero_unitario != nullptr) {
        ui->label_3->setPixmap(QPixmap::fromImage(QImage(nullptr)));
        ui->label_3->setToolTip("");
        ui->lineEdit->setText("");
        nombre_fichero_unitario = nullptr;
    }
    if(imagen_list.size() != 0) {
        imagen_list.resize(0);
        ui->lineEdit_2->setText("");
        ruta_Folder = "";
    }
}


void MainWindow::on_toolButton_7_clicked()
{
    QTime timestamp = QTime::currentTime();
    std::string filename("D:\\clase\\SI\\Vosego\\Vosego\\logs\\" + QDate::currentDate().toString().toStdString() + "_" + std::to_string(timestamp.hour()) + std::to_string(timestamp.minute()) + std::to_string(timestamp.second()) + std::to_string(timestamp.msec()) + ".txt");
    std::fstream output_fstream;

    output_fstream.open(filename, std::fstream::out);
    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        output_fstream << item->text().toStdString() << std::endl;

    }
    ui->statusbar->showMessage("Log Saved: '"+ *(new QString(filename.c_str())) +"'succesfully");
    output_fstream.close();
}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index == 1) {
        ui->statusbar->showMessage("Try to click on a day from the calendar and see what happen :)");
    }
}

