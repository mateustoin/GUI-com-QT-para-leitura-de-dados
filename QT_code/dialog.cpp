#include "dialog.h"
#include "ui_dialog.h"

//Includes das bibliotecas que serão utilizadas para a comunicação serial
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

//Include relacionado as mensagens que serão exibidas na tela
#include <QtWidgets>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->distance_lcdNumber->display("-----");

    arduino_is_available = false; //Iniciando variável
    arduino_port_name = ""; //Iniciando variável com nada
    arduino = new QSerialPort; //Adicionado de acordo com o vídeo

    serialBuffer = "";


    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_mega_vendor_id){
                if(serialPortInfo.productIdentifier() == arduino_mega_product_id){
                    arduino_port_name = serialPortInfo.portName(); // Se passar por todas as verificações recebe o nome da porta
                    arduino_is_available = true; // Se passar por todas as verificações recebe true na verificação
                }
            }
        }
    }

    //Agora que passou as verificações, vamos ao trabalho
    if (arduino_is_available){
        // Temos que abrir e configurar a porta serial
        /*
         * A SEGUIR ENCONTRAMOS AS CONFIGURAÇÕES RECOMENDADAS
         * LEMBRAR DE FAZÊ-LAS DEPOIS QUE ABRIR O ARDUINO (USAR ARDUINO->OPEN()), POIS FAZER ANTES NÃO FAZ SENTIDO
         * VISTO QUE ELES RETORNAM TRUE OU FALSE E SÓ SERVIRÁ DE ALGO QUANDO O ARDUINO ESTIVER ACESSÍVEL
         *
         * */
        arduino->setPortName(arduino_port_name); // Mudando o nome da porta para o que encontramos do arduino
        arduino->open(QSerialPort::ReadOnly); //Acessando arduino em modo de leitura da placa, pois iremos ler a distância
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8); // Arduino, 8 bits, dãã
        arduino->setParity(QSerialPort::NoParity); // Descobrir depois o que está relacionado a isso!!!! <<<<<<<<<<<
        arduino->setStopBits(QSerialPort::OneStop); // A opção half and stop é só para o windows
        arduino->setFlowControl(QSerialPort::NoFlowControl); // Descobrir pra que diachos isso serve também!!! <<<<<<<

        //PARTICULARIDADE P/ LER DADOS
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial())); //Função será chamada sempre que tiver um valor disponível para leitura
        /*
         * Quando é conectado, a função do SIGNAL "readyRead" é usado quando tem informação na porta serial para ler
         * Mas não tem garantias de que a mensagem inteira está disponível
         * Então é necessário criar um buffer para a entrada
         * */

    }else{
        // Exibe uma mensagem de erro se arduino não estiver disponível
        QMessageBox::warning(this, "Erro na porta", "Não foi possível localizar o Arduino...");
    }
}

Dialog::~Dialog()
{
    delete ui;

    // Configurando destrutor para fechar comunicação serial quando finalizar
    if (arduino->isOpen()){
        arduino->close();
    }
}

//Função para ler a porta serial
void Dialog::readSerial()
{
    //qDebug() << "Porta serial funciona...";
        //serialData = arduino->readAll(); // readALl geralmente utilizado nas situações, mas depende de que tipo de dados estamos trabalhando
    //QString distance = QString::fromStdString(serialData.toStdString());
    //qDebug () << distance;
        //serialBuffer += QString::fromStdString(serialData.toStdString());
    //qDebug () << serialBuffer;

    //Depois de testado e as informações chegarem certas, vamos separar os valores baseado nas vírgulas
    QStringList bufferSplit = serialBuffer.split(",");

    if(bufferSplit.length() < 3){ //Para separar os valores
        serialData = arduino->readAll(); // readALl geralmente utilizado nas situações, mas depende de que tipo de dados estamos trabalhando
        serialBuffer += QString::fromStdString(serialData.toStdString());
    }else{
        //bufferSplit[1] é um valor mais confiável, pois o [0] pode vir com erros
        qDebug() << bufferSplit;
        Dialog::updateLCD(bufferSplit[1]); // Momento em que o valor da distância é enviado para a tela
        serialBuffer = "";
    }

}

// Criando método para fazer update na tela do programa
void Dialog::updateLCD(QString leitura_distancia)
{
    ui->distance_lcdNumber->display(leitura_distancia);
}
