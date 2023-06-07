#include "graficadiaform.h"
#include "ui_graficadiaform.h"

GraficaDiaForm::GraficaDiaForm(std::vector<std::vector<std::string> > tablaSelect, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraficaDiaForm)
{
    ui->setupUi(this);
    std::map<std::string, int> mapa;
    this->setWindowTitle(*(new QString((" Graphs of the day: " + tablaSelect[0][0]).c_str())));
    for(auto row: tablaSelect) {
       mapa[row[3]] += 1;
    }
    for (auto const& x : mapa)
    {
        ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
        ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1,
                                 0,
                                 new QTableWidgetItem(*(new QString(x.first.c_str()))));
    }
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    openDB();
    selectByHoraDB(tablaSelect[0][3].c_str());
    for(auto row: callBackSelectTable) {
        ui->tableWidget_2->insertRow ( ui->tableWidget_2->rowCount() );
        ui->tableWidget_2->setItem   ( ui->tableWidget_2->rowCount()-1,
                                     0,
                                     new QTableWidgetItem(*(new QString(row[1].c_str()))));
    }
    barGrapDiaForm* graph1 = new barGrapDiaForm(callBackSelectTable);
    graph1->show();
    graph1->activateWindow();
}

void GraficaDiaForm::selectByHoraDB(std::string hora)
{
    callBackSelectTable.resize(0);
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

void GraficaDiaForm::createCharts()
{
    QPieSeries *series = new QPieSeries();
    QString classWithHighestFrecuency = "NULL";
    int maxInt = 0;
    std::map<std::string, int> podenraciones;
    for(auto row: callBackSelectTable) {
        podenraciones[row[4]] += 1;
    }

    for (auto const& x : podenraciones)
    {
        series->append(*(new QString(x.first.c_str())), x.second);
        classWithHighestFrecuency = (x.second > maxInt) ? x.first.c_str(): classWithHighestFrecuency;
    }

    srand(std::time(NULL));
    for (auto slice: series->slices())
    {
        QColor color = QColor(rand()%255, rand()%255, rand()%255);
        slice->setLabelVisible();
        slice->setPen(QPen(color, 2));
        slice->setBrush(QBrush(color));
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Class Ratio");
    chart->legend()->hide();
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFrame_2);
    QBarSet *set0 = new QBarSet("Diseased");
    QBarSet *set1 = new QBarSet("Healthy");
    int accSaludable = 0;
    int accEnfermo = 0;
    qDebug() << callBackSelectTable.size();
    for(auto row: callBackSelectTable) {
        if (row[2] == "Enfermo") {
            accSaludable++;
        } else {
            accEnfermo++;
        }
    }
    *set0 << accSaludable ;
    *set1 << accEnfermo;
    set0->setColor(qRgb(180,0,0));
    set1->setColor(qRgb(0,180,0));
    QBarSeries *series1 = new QBarSeries();
    series1->append(set0);
    series1->append(set1);
    QChart *chart1 = new QChart();
    chart1->addSeries(series1);
    chart1->setTitle("Diseased vs Healthy");
    chart1->setAnimationOptions(QChart::SeriesAnimations);
    QStringList categories;
    categories << "Diseased / Healthy";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart1->createDefaultAxes();
    chart1->setAxisX(axis, series);

    QChartView *chartView1 = new QChartView(chart1);
    chartView1->setParent(ui->horizontalFrame);
    chart->removeSeries(series);
    chart->addSeries(series);
    chart1->removeSeries(series1);
    chart1->addSeries(series1);

}

void GraficaDiaForm::openDB()
{
    int rc;
    rc = sqlite3_open("D:\\clase\\SI\\Vosego\\Vosego\\results.db", &db);
    if( rc ) {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
       fprintf(stderr, "Opened database successfully\n");
    }
}

void GraficaDiaForm::closeDB()
{
    sqlite3_close(db);
}

GraficaDiaForm::~GraficaDiaForm()
{
    delete ui;
}

void GraficaDiaForm::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    qDebug() << item->text();
    if(!item->text().isEmpty()) {
        selectByHoraDB(item->text().toStdString());
        ui->tableWidget_2->setRowCount(0);
        for(auto row: callBackSelectTable) {
            ui->tableWidget_2->insertRow ( ui->tableWidget_2->rowCount() );
            ui->tableWidget_2->setItem   ( ui->tableWidget_2->rowCount()-1,
                                           0,
                                           new QTableWidgetItem(*(new QString(row[1].c_str()))));
        }
        barGrapDiaForm* graph1 = new barGrapDiaForm(callBackSelectTable);
        graph1->show();
        graph1->activateWindow();
    }


}

