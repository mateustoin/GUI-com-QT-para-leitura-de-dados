#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;

    w.setFixedSize(420, 103); // Fixando o tamanho da janela
    w.setWindowTitle("Medidor de distâncias 4-30"); // Setando o nome da janela

    w.show();

    return a.exec();
}
