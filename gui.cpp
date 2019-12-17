#include "gui.h"

/************* Fucntion Prototypes ************/

Gui::Gui(QWidget *parent) : QWidget(parent){

    //Allocating objects
    dialogFile = new QFileDialog();
    simulator =new Simulator();
    memoryIO = new QPushButton("Memory - IO");
    IOMemory = new QPushButton("IO - Memory");
    MemtoMem = new QPushButton("Memory - Memory");
    open = new QPushButton("Open");
    save = new QPushButton("Save");
    programDMA = new QPushButton("Program DMA");

    simulate = new QPushButton("Simulate");

    source = new QLineEdit();
    destination = new QLineEdit();
    count = new QLineEdit();

    plainText = new QPlainTextEdit();

    systemGrid = new QGridLayout();

    errorMessage = new QMessageBox();

    //Alocating register objects
    BWRegister = new QLineEdit();
    BWCRegister = new QLineEdit();
    commandRegister = new QLineEdit();
    modeRegister = new QLineEdit();
    maskRegister = new QLineEdit();
    requestRegister = new QLineEdit();

    // Calling proper functions
    drawButtons();
    drawLineEdit();
    drawPlainText();
    connections();

    this->setLayout(this->systemGrid);

}

void Gui::drawButtons()
{

    // Adjusting button sizes

    memoryIO->setMinimumSize(100,100);
    IOMemory->setMinimumSize(100,100);
    MemtoMem->setMinimumSize(100,100);
    open->setMinimumSize(100,100);
    save->setMinimumSize(100,100);
    simulate->setMinimumSize(100,100);
    programDMA->setMinimumSize(100,100);

    memoryIO->setIcon(QIcon("C:/Users/Ayman/Desktop/ButtonIcon.png"));
    IOMemory->setIcon(QIcon("C:/Users/Ayman/Desktop/ButtonIcon.png"));
    MemtoMem->setIcon(QIcon("C:/Users/Ayman/Desktop/ButtonIcon.png"));

    systemGrid->addWidget(open,0,1,1,1);
    systemGrid->addWidget(save,0,2,1,1);

    systemGrid->addWidget(memoryIO,2,4,1,3);
    systemGrid->addWidget(IOMemory,3,4,1,3);
    systemGrid->addWidget(MemtoMem,4,4,1,3);
    systemGrid->addWidget(programDMA,5,4,1,3);
    systemGrid->addWidget(simulate,6,4,1,3);


    //systemGrid->setContentsMargins(7,7,7,7);


}

void Gui::drawLineEdit()
{

    source->setPlaceholderText("Source Field");
    destination->setPlaceholderText("Destination Field");
    count->setPlaceholderText("Count Field");

    BWRegister->setPlaceholderText("Base Word Register");
    BWCRegister->setPlaceholderText("Base Word Count Register");
    commandRegister->setPlaceholderText("Command Register");
    modeRegister->setPlaceholderText("Mode Register");
    maskRegister->setPlaceholderText("Mask Register");
    requestRegister->setPlaceholderText("Request Register");

//    source->setMinimumSize(80,100);
//    destination->setMinimumSize(80,100);
//    count->setMinimumSize(80,100);


    systemGrid->addWidget(source,6,1,1,1);
    systemGrid->addWidget(destination,6,2,1,1);
    systemGrid->addWidget(count,6,3,1,1);

    systemGrid->addWidget(BWRegister,0,4,1,1);
    systemGrid->addWidget(BWCRegister,0,5,1,1);
    systemGrid->addWidget(commandRegister,0,6,1,1);
    systemGrid->addWidget(modeRegister,1,4,1,1);
    systemGrid->addWidget(maskRegister,1,5,1,1);
    systemGrid->addWidget(requestRegister,1,6,1,1);

}

void Gui::drawPlainText()
{

    plainText->setPlaceholderText("Insert Assembly Instructions");
    plainText->setBackgroundVisible(true);
    systemGrid->addWidget(plainText,1,1,5,3);
}

void Gui::connections()
{

    // Connecting signals to proper slots

    // Connecting Memory to I/O slot to a button click signal
    connect(memoryIO,SIGNAL(clicked()),this,SLOT(memory_IO_Slot()));

    // Connecting I/O to memory slot to a button click signal
    connect(IOMemory,SIGNAL(clicked()),this,SLOT(io_Memory_Slot()));

    // Connecting memory to memory slot to a button click signal
    connect(MemtoMem,SIGNAL(clicked()),this,SLOT(memory_Memory_Slot()));

    /* Connecting simulate slot to a button click & returning vector
     * of strings which represents my assembly code
     */
    connect(simulate,SIGNAL(clicked()),this,SLOT(simulate_Slot()));

    /* Connecting open file siganls to their proper slots,
     * when file is selected; a path is returned to
     * file_selected_slot as a const string
     */
    connect(open,SIGNAL(clicked()),this,SLOT(open_Slot()));
    connect(dialogFile,SIGNAL(fileSelected(QString)),this,SLOT(file_Is_Selected(const QString)));

    //
    connect(programDMA,SIGNAL(clicked()),this,SLOT(program_DMA_Slot()));



}

QGridLayout* Gui::getMainGrid()
{
    return systemGrid;
}

void Gui::memory_IO_Slot()
{

    plainText->appendPlainText("INIT_MEM_IO");

}

void Gui::io_Memory_Slot()
{
    plainText->appendPlainText("INIT_IO_MEM");
}

void Gui::memory_Memory_Slot()
{
    // Checking if source, destination & count are specified or not
    if(source->isModified() != 1)
    {
        errorMessage->setInformativeText("Please Specify Source Field");
        errorMessage->show();
    }
    else if(destination->isModified() != 1)
    {
       errorMessage->setInformativeText("Please Specify Destination Field");
       errorMessage->show();
    }

    else if(count->isModified() != 1)
    {
        errorMessage->setInformativeText("Please Specify Count Field");
        errorMessage->show();
    }
    /* Checking if source, destination & count within available transfer range or not
     * Assuming our range from location 64 to 1023 & count ranges from 1 to 512
     */
    else if (source->text().toInt()<MIN_TRANSFER_LOCATION ||
             destination->text().toInt()>MAX_TRANSFER_LOCATION)
    {
        //errorMessage->setInformativeText(source->text());
        errorMessage->setInformativeText("Please Specify Within Allowed Range \nSource :384 \n "
                                         "Destination: 65,536‬ ");
        errorMessage->show();
    }
    else if (count->text().toInt() <= 0 || count->text().toInt() >= MAX_TRANSFER_BYTES)
    {
        errorMessage->setInformativeText("Please Specify A Correct Count Value ");
        errorMessage->show();
    }
    else
    {
        plainText->appendPlainText("INIT_MEM_MEM " + (source->text()) + QString(" ,") + (destination->text())
                                   + QString(" ,")  + (count->text()));
    }
}

void Gui::simulate_Slot()
{
    this->simulator->clear();
    this->assemblyCode.clear();

    QStringList *list = new QStringList();

    QString *temp = new QString(plainText->toPlainText());
    list->append(temp->split("\n"));

    for (int i =0;i<list->size();i++)
    {
        assemblyCode.push_back(list->at(i).toStdString());
    }
    this->simulator->ASSEMBLY(assemblyCode);
    delete list;
    delete temp;
}

void Gui::open_Slot()
{
    dialogFile->setNameFilter("*.txt");
    dialogFile->setAcceptMode(QFileDialog::AcceptOpen);
    dialogFile->setViewMode(QFileDialog::List);
    dialogFile->setFileMode(QFileDialog::ExistingFile);
    dialogFile->show();
}

void Gui::program_DMA_Slot()
{
    if(BWRegister->isModified()!=1 || BWCRegister->isModified()!=1 ||
       commandRegister->isModified()!=1 || maskRegister->isModified()!=1 ||
       modeRegister->isModified()!=1 || requestRegister->isModified()!=1 )
    {
        errorMessage->setInformativeText("Please Specify All Registers");
        errorMessage->show();
    }
    else if (BWRegister->text().toInt() > MAX_REGISTER_VALUE ||
             BWCRegister->text().toInt() > MAX_REGISTER_VALUE ||
             commandRegister->text().toInt() > MAX_REGISTER_VALUE ||
             maskRegister->text().toInt() > MAX_REGISTER_VALUE ||
             modeRegister->text().toInt() > MAX_REGISTER_VALUE ||
             requestRegister->text().toInt() > MAX_REGISTER_VALUE)
    {
        errorMessage->setInformativeText("Please Specify Registers Value Within Range");
        errorMessage->show();
    }
}

void Gui::file_Is_Selected(const QString processFile)
{
    QStringList list;
    QFile assemblyFile(processFile);
    assemblyFile.open(QIODevice::ReadOnly);

    while(!(assemblyFile.atEnd()))
    {
        list.push_back(assemblyFile.readLine());
    }
    short i =0;
    while(i < list.size())
    {
       plainText->insertPlainText(list.at(i));
       i++;
    }
    assemblyFile.close();
}











