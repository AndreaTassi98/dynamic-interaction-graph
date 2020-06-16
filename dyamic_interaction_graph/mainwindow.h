#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QPainter>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct node {
    QPoint point;
    int interations;

    node() {}
    node(const QPoint &point) : point(point), interations(0) {}

    bool operator==(const node &other) const {
        return point == other.point;
    }
};

struct edge {
    QLine line;
    int interations;

    edge() {}
    edge(const QLine &line) : line(line), interations(1) {}

    bool operator==(const edge &other) const {
        return line == other.line || (line.p1() == other.line.p2() && line.p2() == other.line.p1());
    }

    QPoint midpoint() const {
        int xm = (line.p1().x() + line.p2().x()) / 2;
        int ym = (line.p1().y() + line.p2().y()) / 2;
        return QPoint(xm, ym);
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void graph_changed(int);
    void maximum_nodes();

private slots:
    void on_pushButton_start_clicked();
    void updateUI();

private:
    Ui::MainWindow *ui;

    QVector<node> nodes;
    QVector<edge> edges;

    QMap<unsigned int, QColor> color_map;
    void initialize_color_map();

    unsigned int iterations;

    void random_node();
    void random_nodes();

    void random_edge();
    void random_edges();
    void random_edges_increment();

    void paintEvent(QPaintEvent*) override;
};

#endif // MAINWINDOW_H
