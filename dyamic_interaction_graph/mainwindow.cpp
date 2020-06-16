#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize_color_map() {
    color_map.insert(0, QColor("#000000"));
    color_map.insert(1, QColor("#696969"));
    color_map.insert(2, QColor("#8FBC8F"));
    color_map.insert(3, QColor("#BDB76B"));
    color_map.insert(4, QColor("#00FF00"));
    color_map.insert(5, QColor("#006400"));
    color_map.insert(6, QColor("#800000"));
    color_map.insert(7, QColor("FF0000"));
    color_map.insert(8, QColor("#FF8C00"));
    color_map.insert(9, QColor("#FFD700"));
    color_map.insert(10, QColor("#4B0082"));
    color_map.insert(11, QColor("#00e0fc"));
    color_map.insert(12, QColor("#0000FF"));
}

void MainWindow::random_node() {
    unsigned int x = rand()%500;
    unsigned int y = rand()%500;
    node n(QPoint(x, y));
    if(!nodes.contains(n))
        nodes.append(n);
}

void MainWindow::random_nodes() {
    unsigned int n = rand()%(30 - nodes.size());
    for(unsigned int i = 0; i < n; ++i)
        random_node();
}

void MainWindow::random_edge() {
    unsigned int i1 = rand()%(nodes.size());
    unsigned int i2 = rand()%(nodes.size());
    edge e(QLine(nodes[i1].point, nodes[i2].point));
    if(!edges.contains(e)) {
        edges.append(e);
        ++(nodes[nodes.indexOf(edges.last().line.p1())].interations);
        ++(nodes[nodes.indexOf(edges.last().line.p2())].interations);
    }
}

void MainWindow::random_edges() {
    unsigned int n = rand()%5;
    for(unsigned int i = 0; i < n; ++i)
        random_edge();
}

void MainWindow::random_edges_increment() {
    unsigned int n = rand()%5;
    for(unsigned int i = 0; i < n; ++i) {
        unsigned int m = rand()%(edges.size());
        int edge_interations = ++(edges[m].interations);
        if(edge_interations > nodes[nodes.indexOf(edges[m].line.p1())].interations)
            ++(nodes[nodes.indexOf(edges[m].line.p1())].interations);
        if(edge_interations > nodes[nodes.indexOf(edges[m].line.p2())].interations)
            ++(nodes[nodes.indexOf(edges[m].line.p2())].interations);
    }
}

void MainWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    for(int i = 0; i < nodes.size(); ++i) {
        QColor color;
        if(nodes[i].interations < color_map.size())
            color = color_map.take(nodes[i].interations);
        else
            color = color_map.last();
        painter.setPen(color);
        painter.drawEllipse(nodes[i].point, 5, 5);
    }
    for(int i = 0; i < edges.size(); ++i) {
        painter.setPen(QColor("#000000"));
        painter.drawLine(edges[i].line);
        painter.drawText(edges[i].midpoint(), QString::number(edges[i].interations));
    }
}

void MainWindow::on_pushButton_start_clicked()
{
    ui->pushButton_start->setEnabled(false);

    initialize_color_map();

    connect(this, SIGNAL(graph_changed(int)), ui->iterations, SLOT(setNum(int)));
    connect(this, SIGNAL(graph_changed(int)), this, SLOT(update()));

    srand(time(NULL));
    random_nodes();
    iterations = 0;
    emit graph_changed(iterations);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateUI()));
    connect(this, SIGNAL(maximum_nodes()), timer, SLOT(stop()));
    timer->start(1000);
}

void MainWindow::updateUI() {
    random_nodes();
    random_edges();
    random_edges_increment();
    emit graph_changed(++iterations);
    if(nodes.size() >= 30)
        emit maximum_nodes();
}
