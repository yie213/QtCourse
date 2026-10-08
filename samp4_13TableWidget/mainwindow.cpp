#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QDate>
#include    <QTableWidgetItem>
#include    <QRandomGenerator>


//为一行的单元格创建 Items
void MainWindow::createItemsARow(int rowNo,QString name,QString sex,QDate birth,QString nation,bool isPM,int score)
{
    uint studID=202105000;  //学号基数
    //姓名
    QTableWidgetItem *item=new  QTableWidgetItem(name, MainWindow::ctName);   //数据项类型为MainWindow::ctName
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    studID  +=rowNo;        //学号 =基数 + 行号
    item->setData(Qt::UserRole,QVariant(studID));           //设置studID为用户数据
    ui->tableInfo->setItem(rowNo,MainWindow::colName,item);

    //性别
    QIcon   icon;
    if (sex=="男")
        icon.addFile(":/images/icons/boy.ico");
    else
        icon.addFile(":/images/icons/girl.ico");

    item=new  QTableWidgetItem(sex,MainWindow::ctSex);      //type为MainWindow::ctSex
    item->setIcon(icon);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    Qt::ItemFlags flags=Qt::ItemIsSelectable |Qt::ItemIsEnabled;    //不允许编辑
    item->setFlags(flags);
    ui->tableInfo->setItem(rowNo,MainWindow::colSex,item);  //为单元格设置Item

    //出生日期
    QString str=birth.toString("yyyy-MM-dd");   //日期转换为字符串
    item=new  QTableWidgetItem(str,MainWindow::ctBirth);        //type为MainWindow::ctBirth
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);   //文本对齐格式
    ui->tableInfo->setItem(rowNo,MainWindow::colBirth,item);

    //民族
    item=new  QTableWidgetItem(nation,MainWindow::ctNation);        //type为MainWindow::ctNation
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colNation,item);

    //是否党员
    item=new  QTableWidgetItem("党员",MainWindow::ctPartyM);      //type为 MainWindow::ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    flags= Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |Qt::ItemIsEnabled;   //不允许编辑，但可以更改复选状态
    item->setFlags(flags);
    if (isPM)
        item->setCheckState(Qt::Checked);
    else
        item->setCheckState(Qt::Unchecked);
    item->setBackground(QBrush(Qt::yellow));   //设置背景颜色
    ui->tableInfo->setItem(rowNo,MainWindow::colPartyM,item);

    //分数
    str.setNum(score);
    item=new  QTableWidgetItem(str,MainWindow::ctScore);    //type为MainWindow::ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colScore,item);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    setCentralWidget(ui->splitterMain);

    // 允许左侧按钮区被折叠（彻底拉短）
    ui->splitterMain->setCollapsible(0, true);
    // 允许右侧表格区被折叠
    ui->splitterMain->setCollapsible(1, true);

    //状态栏初始化创建
    labCellIndex = new QLabel("当前单元格坐标：",this);
    labCellIndex->setMinimumWidth(550);

    labCellType=new QLabel("当前单元格类型：",this);
    labCellType->setMinimumWidth(200);

    labStudID=new QLabel("学生ID：",this);
    labStudID->setMinimumWidth(200);

    labStudentOrigin = new QLabel("籍贯：", this);
    labStudentOrigin->setMinimumWidth(200);
    ui->statusBar->addWidget(labStudentOrigin); // 添加到状态栏

    ui->statusBar->addWidget(labCellIndex); //添加到状态栏
    ui->statusBar->addWidget(labCellType);
    ui->statusBar->addWidget(labStudID);

    ui->splitter->setStretchFactor(0, 4); // 表格占4份（占大头）
    ui->splitter->setStretchFactor(1, 1); // 白框占1份（占小头）
}

MainWindow::~MainWindow()
{
    delete ui;
}


//设置水平表头
void MainWindow::on_btnSetHeader_clicked()
{
    QStringList headerText;
    headerText<<"姓名"<<"性别"<<"出生日期"<<"民族"<<"分数"<<"是否党员";
    //    ui->tableInfo->setHorizontalHeaderLabels(headerText); //只设置标题
    ui->tableInfo->setColumnCount(headerText.size());      //设置表格列数
    for (int i=0;i<ui->tableInfo->columnCount();i++)
    {
        QTableWidgetItem *headerItem=new QTableWidgetItem(headerText.at(i));
        QFont font=headerItem->font();   //获取原有字体设置
        font.setBold(true);              //设置为粗体
        font.setPointSize(11);           //字体大小
        headerItem->setForeground(QBrush(Qt::red));  //设置文字颜色
        headerItem->setFont(font);       //设置字体
        ui->tableInfo->setHorizontalHeaderItem(i,headerItem);    //设置表头单元格的item
    }
}

//设置行数,设置的行数为数据区的行数，不含表头
void MainWindow::on_btnSetRows_clicked()
{
    ui->tableInfo->setRowCount(ui->spinRowCount->value());//设置数据区行数
    ui->tableInfo->setAlternatingRowColors(ui->chkBoxRowColor->isChecked()); //设置交替行背景颜色
}


//初始化表格数据
void MainWindow::on_btnIniData_clicked()
{
    QDate   birth(2001,4,6);        //初始化一个日期
    ui->tableInfo->clearContents(); //只清除工作区，不清除表头
    for (int i=0; i<ui->tableInfo->rowCount(); i++)
    {
        QString strName=QString("学生%1").arg(i);
        QString strSex= ((i % 2)==0)? "男":"女";
        bool isParty= ((i % 2)==0)? false:true;
        int score= QRandomGenerator::global()->bounded(60,100);   //随机数[60,100)
        createItemsARow(i, strName, strSex, birth,"汉族",isParty,score);  //为某一行创建items
        birth=birth.addDays(20);    //日期加20天
    }
}

void MainWindow::on_chkBoxTabEditable_clicked(bool checked)
{ //设置表格是否可编辑，以及进入编辑模式的方式
    if (checked)
        //双击或获取焦点后单击，进入编辑状态
        ui->tableInfo->setEditTriggers(QAbstractItemView::DoubleClicked
                                       | QAbstractItemView::SelectedClicked);
    else
        ui->tableInfo->setEditTriggers(QAbstractItemView::NoEditTriggers); //不允许编辑
}

void MainWindow::on_chkBoxHeaderH_clicked(bool checked)
{//是否显示水平表头
    ui->tableInfo->horizontalHeader()->setVisible(checked);
}

void MainWindow::on_chkBoxHeaderV_clicked(bool checked)
{//是否显示垂直表头
    ui->tableInfo->verticalHeader()->setVisible(checked);
}

void MainWindow::on_chkBoxRowColor_clicked(bool checked)
{ //行的底色交替采用不同颜色
    ui->tableInfo->setAlternatingRowColors(checked);
}

void MainWindow::on_rBtnSelectItem_clicked()
{//选择方式：单元格选择
    ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectItems);
}

void MainWindow::on_rBtnSelectRow_clicked()
{//选择方式：行选择
    ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
}


//将 QTableWidget的所有行的内容提取字符串，显示在QPlainTextEdit里
//void MainWindow::on_btnReadToEdit_clicked()
//{
//   QString str;
//    QTableWidgetItem    *item;

//    ui->textEdit->clear();  //文本编辑器清空
//    for (int i=0;i<ui->tableInfo->rowCount();i++)   //逐行处理
//    {
//        QString str=QString::asprintf("第 %d 行： ",i+1);
//        for (int j=0;j<ui->tableInfo->columnCount()-1;j++) //逐列处理，但最后一列是check型，单独处理
//        {
//            item=ui->tableInfo->item(i,j);      //获取单元格的item
//            str=str+item->text()+"   ";         //字符串连接
//        }
//        item=ui->tableInfo->item(i,colPartyM);  //最后一列，党员
//        if (item->checkState()==Qt::Checked)    //根据check状态显示文字
//            str=str+"党员";
//        else
//            str=str+"群众";
//        ui->textEdit->appendPlainText(str);     //添加到编辑框作为一行
//    }
//}

//currentCellChanged()信号的槽函数，当前单元格发生变化时的响应
void MainWindow::on_tableInfo_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);


    QTableWidgetItem* item=ui->tableInfo->item(currentRow,currentColumn); //获取单元格的Item
    if  (item==nullptr)
        return;

    // 1. 更新状态栏：当前单元格坐标
    labCellIndex->setText(QString::asprintf("当前单元格坐标：%d 行，%d 列", currentRow, currentColumn));

    // 2. 更新状态栏：当前单元格类型
    int cellType = item->type();
    labCellType->setText(QString::asprintf("当前单元格类型：%d", cellType));

    // 3. 【修改】获取学号（第0列），更新状态栏学生ID
    QTableWidgetItem* idItem = ui->tableInfo->item(currentRow, 0); // 第0列现在是学号列
    if (idItem) {
        // 因为新表格里学号就是纯文本，所以直接用 text() 读取，不再读 UserRole
        labStudID->setText(QString("学生ID：%1").arg(idItem->text()));
    }

    // 4. 【新增】获取籍贯（从姓名列的 UserRole+1 中读取），更新状态栏籍贯
    QTableWidgetItem* nameItem = ui->tableInfo->item(currentRow, 1); // 第1列是姓名列
    if (nameItem) {
        QString origin = nameItem->data(Qt::UserRole + 1).toString();
        labStudentOrigin->setText(QString("籍贯：%1").arg(origin.isEmpty() ? "未知" : origin));
    }
}

//插入一行
void MainWindow::on_btnInsertRow_clicked()
{
    int curRow=ui->tableInfo->currentRow();     //当前行号

    ui->tableInfo->insertRow(curRow);           //插入一行，但不会自动为单元格创建item
    createItemsARow(curRow, "新学生", "男",
                    QDate::fromString("2002-10-1","yyyy-M-d"),"苗族",true,80 ); //为某一行创建items
}

//添加一行
void MainWindow::on_btnAppendRow_clicked()
{
    int curRow=ui->tableInfo->rowCount();       //当前行号
    ui->tableInfo->insertRow(curRow);           //在表格尾部添加一行
    createItemsARow(curRow, "新生", "女",
                    QDate::fromString("2002-6-5","yyyy-M-d"),"满族",false,76 ); //为某一行创建items
}

//删除当前行及其items
void MainWindow::on_btnDelCurRow_clicked()
{
    int curRow=ui->tableInfo->currentRow();     //当前行号
    ui->tableInfo->removeRow(curRow);           //删除当前行及其items
}

void MainWindow::on_btnAutoHeght_clicked()
{
    ui->tableInfo->resizeRowsToContents();
}

void MainWindow::on_btnAutoWidth_clicked()
{
    ui->tableInfo->resizeColumnsToContents();
}

void MainWindow::on_actSetStudentList_triggered()
{
    // 1. 设置表头
    QStringList headerText;
    headerText << "学号" << "姓名" << "性别" << "行政班级" << "院(系)/部" << "专业" << "修读性质";
    ui->tableInfo->setColumnCount(headerText.size());
    ui->tableInfo->setHorizontalHeaderLabels(headerText);

    // 2. 设置行数
    int rowCount = 5;
    ui->tableInfo->setRowCount(rowCount);
    ui->tableInfo->clearContents(); // 清空原有内容

    // 3. 准备数据
    struct Student {
        QString id, name, gender, className, department, major, nature, origin;
    };
    QList<Student> students = {
        {"2024414290331", "文世杰", "男", "2024软件3班(基础软件)", "计算机科学与技术学院", "软件工程", "初修", "广东广州"},
        {"2024414290333", "吴湘楠", "女", "2024软件3班(基础软件)", "计算机科学与技术学院", "软件工程", "初修", "广东茂名"},
        {"2024414290334", "谢嘉仪", "女", "2024软件3班(基础软件)", "计算机科学与技术学院", "软件工程", "初修", "广东汕头"}, // 目标行
        {"2024414290337", "许龙奇", "男", "2024软件3班(基础软件)", "计算机科学与技术学院", "软件工程", "初修", "广东深圳"},
        {"2024414290339", "余昊哲", "男", "2024软件3班(基础软件)", "计算机科学与技术学院", "软件工程", "初修", "广东东莞"}
    };

    // 4. 填充表格
    for (int row = 0; row < rowCount; ++row) {
        const Student &s = students[row];
        QStringList rowData = {s.id, s.name, s.gender, s.className, s.department, s.major, s.nature};

        for (int col = 0; col < rowData.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(rowData[col]);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); // 居中对齐


            if (col == 2) { // 0是学号, 1是姓名, 2是性别
                            QIcon icon;
                            if (rowData[col] == "男") {
                                icon.addFile(":/images/icons/boy.ico");
                            } else if (rowData[col] == "女") {
                                icon.addFile(":/images/icons/girl.ico");
                            }
                            item->setIcon(icon); // 设置图标
            }

            // 在“姓名”列（第1列）的item中关联籍贯信息
            if (col == 1) {
                item->setData(Qt::UserRole + 1, s.origin);
            }

            // 谢嘉仪所在行（第2行，索引为2）设置字体为粗体，颜色为红色
            if (row == 2) {
                QFont font;// 新建字体对象
                font.setFamily("Microsoft YaHei"); //设置字体
                font.setPointSize(10);// 设置字号
                font.setBold(true);//
                item->setFont(font);
                item->setForeground(QBrush(Qt::red));
            }

            ui->tableInfo->setItem(row, col, item);
        }
    }

    // 自动调整列宽以适应内容
    ui->tableInfo->resizeColumnsToContents();
}
