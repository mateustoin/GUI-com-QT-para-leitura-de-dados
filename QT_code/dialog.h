#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort> //Incluindo biblioteca por causa da instância criada na classe


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void readSerial();

    void updateLCD(QString); // Método responsável pela atualização do valor da distância no lcd
private:
    Ui::Dialog *ui;

    QSerialPort *arduino;
    //Depois de identificar no debug o vendor e product ID
    static const quint16 arduino_mega_vendor_id = 9025;
    static const quint16 arduino_mega_product_id = 66;
    QString arduino_port_name; // variável que vai receber o nome da porta que o arduino está conectado
    bool arduino_is_available; // booleano para comparar se o arduino ainda está conectado

    //Inicializando atributos para leitura da porta serial
    QByteArray serialData;
    QString serialBuffer;
};

#endif // DIALOG_H
