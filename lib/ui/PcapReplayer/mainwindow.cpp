#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "PcapReplayCtrl.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	// init resource
	Q_INIT_RESOURCE(resource);
	// connection from mainWindow.ui
	connect(ui->actionOpen_File, SIGNAL(triggered()), this, SLOT(onOpen_File()));
	connect(ui->actionOpen_Folder, SIGNAL(triggered()), this, SLOT(onOpen_Folder()));
	connect(ui->actionScheduler, SIGNAL(triggered()), this, SLOT(onScheduler()));
	connect(ui->actionSave_Config, SIGNAL(triggered()), this, SLOT(onSave_Config()));
	connect(ui->actionLoad_Config, SIGNAL(triggered()), this, SLOT(onLoad_Config()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(onClose()));
	connect(ui->actionConvert_Pcap, SIGNAL(triggered()), this, SLOT(ConvertPcapFile()));
	connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(onListWidget_Actived(int)));
	connect(ui->BtnPlay, SIGNAL(pressed()), this, SLOT(onPlay()));
	connect(ui->BtnPause, SIGNAL(pressed()), this, SLOT(onPause()));
	connect(ui->BtnStop, SIGNAL(pressed()), this, SLOT(onStop()));
	connect(ui->BtnRemove, SIGNAL(pressed()), this, SLOT(onRemove()));
	connect(ui->BtnRemoveAll, SIGNAL(pressed()), this, SLOT(onRemoveAll()));
	//connect(ui->BtnClose, SIGNAL(pressed()), this, SLOT(onClose()));
	connect(ui->BtnRegularPlay, SIGNAL(pressed()), this, SLOT(onRegularReplay()));
	connect(ui->BtnAddSrcMap, SIGNAL(pressed()), this, SLOT(onAddSrcMap()));
	connect(ui->BtnAddDstMap, SIGNAL(pressed()), this, SLOT(onAddDstMap()));
	connect(ui->BtnRemoveScrIPMap, SIGNAL(pressed()), this, SLOT(onRemoveScrMapIP()));
	connect(ui->BtnRemoveDstIPMap, SIGNAL(pressed()), this, SLOT(onRemoveDstMapIP()));
	// connection from PcapReplayCtrl
	connect(this, SIGNAL(SetCurProcess(const int)), this, SLOT(UpdateCurProcess(const int)));
	connect(this, SIGNAL(Signal_ToUIState_Stop()), this, SLOT(ToUIState_Stop()));
	connect(this, SIGNAL(SetCurSendingSpeed(int)), this, SLOT(UpdateCurSpeed(int)));
	connect(this, SIGNAL(SetCurSendingPktTime(QString)), this, SLOT(UpdateCurSendingPktTime(QString)));
	connect(this, SIGNAL(SetNextSendingPktTime(QString)), this, SLOT(UpdateNextSendingPktTime(QString)));
	connect(this, SIGNAL(ShowNextSendingPktTime(bool)), this, SLOT(UpdateShowNextSendingPktTime(bool)));
	connect(this, SIGNAL(SetNextPktNo(unsigned int)), this, SLOT(UpdateNextPktNo(unsigned int)));
	connect(this, SIGNAL(StartPlayFromAutoDriver()), this, SLOT(onPlay()));
	connect(this, SIGNAL(StartInitialFirstScreen()), this, SLOT(Init_FirstScreen()));
	connect(this, SIGNAL(ReadNextPcapFile()), this, SLOT(SelectNextPcapFile()));
	connect(this, SIGNAL(SetLoopCount(quint32)), this, SLOT(SetLoopCountSlot(quint32)));
	// connection from schedular dialog
	
	// init app status
	m_curAppPath = QDir::currentPath();
	// init qstring
	m_LatestFilePath = "";
	// init int
	m_nAdapterIdx = 0;
	m_nCurPcapFileIdx = 0;
	// init App status
	setWindowTitle(QString::fromStdString(g_AppName));
	setWindowIcon(QIcon(":/app_icon.png"));
	// init dialog
	m_pDialogScheduler = 0;
	m_pDialogScheduler = new DialogScheduler(this);
	// init bool
	m_bRegularReplayEnable = false;
	// init string list
	m_strLtsNetMapDstHeader.clear();
	m_strLtsNetMapDstHeader.push_back("Original Dst IP");
	m_strLtsNetMapDstHeader.push_back("Converted Dst IP");
	m_strLtsNetMapSrcHeader.clear();
	m_strLtsNetMapSrcHeader.push_back("Original Src IP");
	m_strLtsNetMapSrcHeader.push_back("Converted Src IP");
	// init file drop
	setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
	if (m_pDialogScheduler != 0)
		delete m_pDialogScheduler;
    delete ui;
}

void MainWindow::Init_FirstScreen()
{
	// we have already read default configuration before go into this function

	// init combo box
	ui->comboBox->clear();
	for (qint32 i = 0; i < m_pMainCtrl->m_vecInterfaceInfo.size(); i++)
	{
		ui->comboBox->addItem(QString::fromStdString(m_pMainCtrl->m_vecInterfaceInfo[i]));
	}
	ui->comboBox->setCurrentIndex(m_nAdapterIdx);
	// init label
	ui->LabelSchedulerStatus->clear();
	ui->LabelStatusReport->clear();
	ui->LabelNextPack->hide();
	ui->LabelSpeedMon->setText("Speed monitor");
	ui->LabelDateMon->setText("Date monitor");
	ui->LabelNextPackageMon->setText("Next Package Time monitor");
	HandleLabelSchedularStatus();
	// init process bar
	ui->progressBar->setValue(0);
	// init line edit
	ui->lineEdit_SpeedFactor->setText("1");
	ui->lineEdit_SpeedLimit->setText("0");
	ui->lineEdit_Offset->setText("0");
	// init check box
	ui->checkBox_OffsetEnable->setCheckState(Qt::Unchecked);
	ui->checkBox_FixSpeed->setCheckState(Qt::Unchecked);
	// init button box
	ui->BtnPlay->setEnabled(true);
	ui->BtnPause->setEnabled(false);
	ui->BtnStop->setEnabled(false);
	ui->BtnRemove->setEnabled(false);
	ui->BtnRemoveDstIPMap->setEnabled(true);
	ui->BtnRemoveScrIPMap->setEnabled(true);
	ui->BtnAddDstMap->setEnabled(true);
	ui->BtnAddSrcMap->setEnabled(true);
	//ui->BtnClose->setEnabled(true);
	ui->BtnPause->setText("Pause");
	ui->BtnRemoveAll->setEnabled(false);
	// init list widget
	ui->listWidget->clear();
	Fillup_ListWidget();
	// init table widget
	ui->tableWidgetNetMapDst->setEnabled(true);
	ui->tableWidgetNetMapSrc->setEnabled(true);
	InitTableWidgetNetMapping(ui->tableWidgetNetMapDst, m_qmapDstIP, m_strLtsNetMapDstHeader);
	InitTableWidgetNetMapping(ui->tableWidgetNetMapSrc, m_qmapScrIP, m_strLtsNetMapSrcHeader);
}

void MainWindow::HandleLabelSchedularStatus()
{
	if (g_Config.m_bSchularEnable)
	{
		QString strSchedularStatus;
		if (g_Config.m_bOneTimeOnly)
		{
			char veccDateTime[128];
			memset(veccDateTime, 0, sizeof(veccDateTime));
			sprintf(veccDateTime, "One Time Schedular On : %d-%02d-%02d %02d:%02d:%02d.000",
				g_Config.m_schedularDateTime.tm_year + 1900, g_Config.m_schedularDateTime.tm_mon + 1, g_Config.m_schedularDateTime.tm_mday,
				g_Config.m_schedularDateTime.tm_hour, g_Config.m_schedularDateTime.tm_min, g_Config.m_schedularDateTime.tm_sec);
			strSchedularStatus = veccDateTime;
		}
		else
		{
			strSchedularStatus += "Weekly Schedular On : ";
			for (int i = 0; i < g_Config.m_vecWeekDaysChecked.size(); i++)
			{
				if (g_Config.m_vecWeekDaysChecked[i])
				{
					switch (i)
					{
					case 0: // Monday
						strSchedularStatus += "Monday ";
						break;
					case 1: // Tuesday
						strSchedularStatus += "Tuesday ";
						break;
					case 2: // Wednesday
						strSchedularStatus += "Wednesday ";
						break;
					case 3: // Thrusday
						strSchedularStatus += "Thrusday ";
						break;
					case 4: // Friday
						strSchedularStatus += "Friday ";
						break;
					case 5: // Saturday
						strSchedularStatus += "Saturday ";
						break;
					case 6: // Sunday
						strSchedularStatus += "Sunday ";
						break;
					default:
						break;
					}
				}
			}
			char veccTime[64];
			memset(veccTime, 0, sizeof(veccTime));
			sprintf(veccTime, "%02d:%02d:%02d.000",
				g_Config.m_schedularDateTime.tm_hour, g_Config.m_schedularDateTime.tm_min, g_Config.m_schedularDateTime.tm_sec);
			strSchedularStatus += veccTime;
		}
		ui->LabelSchedulerStatus->setText(strSchedularStatus);
		ui->LabelSchedulerStatus->setStyleSheet("QLabel { background-color : green; color : yellow; }");
	}
	else
	{
		ui->LabelSchedulerStatus->setText("Scheduler Disabled");
		ui->LabelSchedulerStatus->setStyleSheet("QLabel { background-color : red; color : blue; }");
	}
}

void MainWindow::onOpen_File()
{
	QStringList fileNames;
	if (m_LatestFilePath == "")
		fileNames = QFileDialog::getOpenFileNames(this, tr("Open Pcap"), m_curAppPath, tr("Pcap Files (*.pcap)"));
	else
		fileNames = QFileDialog::getOpenFileNames(this, tr("Open Pcap"), m_LatestFilePath, tr("Pcap Files (*.pcap)"));

	if (fileNames.length() == 0)
		return;
	AddFileToListWidget(fileNames);
}

void MainWindow::onScheduler()
{
	m_pDialogScheduler->setModal(true);
	m_pDialogScheduler->show();
	m_pDialogScheduler->Init_UIScreen();
	//m_pDialogScheduler->exec();
}

void MainWindow::UpdateNextPktNo(unsigned int unNextPkt)
{
	ui->LabelNextPack->show();
	ui->LabelNextPack->setText(tr("Next Packet is: ") + QString::number(unNextPkt));
}

void MainWindow::Fillup_ListWidget(QStringList const * fileNames)
{
	if (fileNames != 0)
		m_fileNames += *fileNames;
	for (qint32 i = 0; i < m_fileNames.length(); i++)
	{
		if (IsFileExists(m_fileNames[i]))
		{
			ui->listWidget->addItem(m_fileNames[i]);
			// update UI
			ui->BtnRemove->setEnabled(true);
			ui->BtnRemoveAll->setEnabled(true);
		}
	}
	ui->listWidget->setCurrentRow(0);
}

bool MainWindow::IsFileExists(QString path) 
{
	QFileInfo check_file(path);
	// check if file exists and if yes: Is it really a file and no directory?
	return (check_file.exists() && check_file.isFile());
	return true;
}

void MainWindow::onSave_Config()
{
	QString strConfigFileName;
	if (m_qstrLatestConfigPath == "")
		strConfigFileName = QFileDialog::getSaveFileName(this, tr("Save Config"), m_curAppPath, tr("Config Files (*.pcapcfg)"));
	else
		strConfigFileName = QFileDialog::getSaveFileName(this, tr("Save Config"), m_qstrLatestConfigPath, tr("Config Files (*.pcapcfg)"));

	if (strConfigFileName.length() == 0)
		return;

	QFileInfo fileInfo(strConfigFileName);
	m_qstrLatestConfigPath = fileInfo.filePath();
	
	// collect data from ui
	CollectDataFromUI();

	Serialize(true, strConfigFileName);
}

void MainWindow::onLoad_Config()
{
	QString strConfigFileName;
	if (m_qstrLatestConfigPath == "")
		strConfigFileName = QFileDialog::getOpenFileName(this, tr("Load Config"), m_curAppPath, tr("Config Files (*.pcapcfg)"));
	else
		strConfigFileName = QFileDialog::getOpenFileName(this, tr("Load Config"), m_qstrLatestConfigPath, tr("Config Files (*.pcapcfg)"));

	if (strConfigFileName.length() == 0)
		return;

	QFileInfo fileInfo(strConfigFileName);
	m_qstrLatestConfigPath = fileInfo.filePath();

	Serialize(false, strConfigFileName);
	Init_FirstScreen();
}

void MainWindow::onOpen_Folder()
{
	QString strFilePath;
	if (m_LatestFilePath == "")
		strFilePath = QFileDialog::getExistingDirectory(this, "", m_curAppPath, QFileDialog::ShowDirsOnly);
	else
		strFilePath = QFileDialog::getExistingDirectory(this, "", m_LatestFilePath, QFileDialog::ShowDirsOnly);

	if (strFilePath == "")
		return;
	
	m_LatestFilePath = strFilePath;
	//QDirIterator it(m_LatestFilePath, QStringList() << "*.pcap", QDir::Files, QDirIterator::Subdirectories);
	QDirIterator it(m_LatestFilePath, QStringList() << "*.pcap", QDir::Files, QDirIterator::NoIteratorFlags);
	
	QStringList fileNames;
	while (it.hasNext())
	{
		it.next();
		QString line = it.filePath();
		fileNames.push_back(line);
		//m_fileNames.push_back(line);
		//ui->listWidget->addItem(line);
	}

	//ui->listWidget->setCurrentRow(0);

	//// update UI
	//ui->BtnRemove->setEnabled(true);
	AddFileToListWidget(fileNames);
}

void MainWindow::onPlay()
{
	// collect data from ui
	CollectDataFromUI();

	ToUIState_Play();

	m_pMainCtrl->m_MsgQ.push(PcapReplayCtrl::GEN_CONFIG);
	m_pMainCtrl->m_MsgQ.push(PcapReplayCtrl::PLAY);
}

void MainWindow::UpdateCurProcess(int nCurProcess)
{
	ui->progressBar->setValue(nCurProcess);
}

void MainWindow::UpdateCurSpeed(int nCurSpeed)
{
	QString line;
	GetBitPerSec(nCurSpeed, line, 0);
	ui->LabelSpeedMon->setText(line);
}

void MainWindow::UpdateCurSendingPktTime(QString strPktTime)
{
	ui->LabelDateMon->setText(strPktTime);
}

void MainWindow::UpdateNextSendingPktTime(QString strPktTime)
{
	ui->LabelNextPackageMon->setText(strPktTime);
}

void MainWindow::UpdateShowNextSendingPktTime(bool bShow)
{
	ui->LabelNextPackageMon->setVisible(bShow);
}

void MainWindow::GetBitPerSec(double bit, QString& line, int step)
{
	if (step >= 4)
		return;

	line = QString::number(bit);
	switch (step)
	{
	case 0:
		line += " Bit/sec";
		break;
	case 1:
		line += " KBit/sec";
		break;
	case 2:
		line += " MBit/sec";
		break;
	case 3:
		line += " GBit/sec";
		break;
	default:
		break;
	}

	int newBit = bit / 1000;
	if (newBit > 0)
		GetBitPerSec(bit / 1000, line, step + 1);

}

void MainWindow::ToUIState_Play()
{
	// handle action
	ui->actionOpen_File->setEnabled(false);
	ui->actionOpen_Folder->setEnabled(false);
	ui->actionLoad_Config->setEnabled(false);
	ui->actionSave_Config->setEnabled(false);
	ui->actionScheduler->setEnabled(false);
	ui->actionConvert_Pcap->setEnabled(false);
	// handle combo box
	ui->comboBox->setEnabled(false);
	// handle list widget
	ui->listWidget->setEnabled(false);
	//for (int i = 0; i < ui->listWidget->count(); i++)
	//{
	//	QListWidgetItem* item = ui->listWidget->takeItem(i);
	//	item->setFlags(item->flags() & ~Qt::ItemIsUserCheckable);
	//	item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
	//}
	// handle button
	ui->BtnPlay->setEnabled(false);
	ui->BtnPause->setEnabled(true);
	ui->BtnStop->setEnabled(true);
	ui->BtnRemove->setEnabled(false);
	ui->BtnRemoveDstIPMap->setEnabled(false);
	ui->BtnRemoveScrIPMap->setEnabled(false);
	ui->BtnAddDstMap->setEnabled(false);
	ui->BtnAddSrcMap->setEnabled(false);
	//ui->BtnClose->setEnabled(true);
	ui->BtnRegularPlay->setEnabled(false);
	ui->BtnPause->setText("Pause");
	ui->BtnRemoveAll->setEnabled(false);
	// handle line edit
	ui->lineEdit_SpeedFactor->setEnabled(false);
	ui->lineEdit_SpeedLimit->setEnabled(false);
	ui->lineEdit_Offset->setEnabled(false);
	ui->lineEdit_LoopCount->setEnabled(false);
	// handle check box
	ui->checkBox_FixSpeed->setEnabled(false);
	ui->checkBox_OffsetEnable->setEnabled(false);
	ui->checkBox_LoopPlay->setEnabled(false);
	// handle label
	ui->LabelNextPack->hide();
	// handle table widget
	ui->tableWidgetNetMapDst->setEnabled(false);
	ui->tableWidgetNetMapSrc->setEnabled(false);
}

void MainWindow::ToUIState_Pause()
{
	if (m_pMainCtrl->Get_CurrentState() == PcapReplayCtrl::PAUSE)
	{
		// handle button
		ui->BtnPause->setText("Pause");
		ui->BtnRemoveDstIPMap->setEnabled(false);
		ui->BtnRemoveScrIPMap->setEnabled(false);
		ui->BtnAddDstMap->setEnabled(false);
		ui->BtnAddSrcMap->setEnabled(false);
		// handle line edit
		ui->lineEdit_SpeedFactor->setEnabled(false);
		ui->lineEdit_SpeedLimit->setEnabled(false);
		ui->lineEdit_Offset->setEnabled(false);
		ui->lineEdit_LoopCount->setEnabled(false);
		// handle check box
		ui->checkBox_FixSpeed->setEnabled(false);
		ui->checkBox_OffsetEnable->setEnabled(false);
		ui->checkBox_LoopPlay->setEnabled(false);
		// handle label
		ui->LabelNextPack->hide();
		// handle table widget
		ui->tableWidgetNetMapDst->setEnabled(false);
		ui->tableWidgetNetMapSrc->setEnabled(false);
	}
	else
	{
		// handle button
		ui->BtnPause->setText("Continue");
		ui->BtnRemoveDstIPMap->setEnabled(true);
		ui->BtnRemoveScrIPMap->setEnabled(true);
		ui->BtnAddDstMap->setEnabled(true);
		ui->BtnAddSrcMap->setEnabled(true);
		// handle line edit
		ui->lineEdit_SpeedFactor->setEnabled(true);
		ui->lineEdit_SpeedLimit->setEnabled(true);
		ui->lineEdit_Offset->setEnabled(true);
		ui->lineEdit_LoopCount->setEnabled(true);
		// handle check box
		ui->checkBox_FixSpeed->setEnabled(true);
		ui->checkBox_OffsetEnable->setEnabled(true);
		ui->checkBox_LoopPlay->setEnabled(true);
		// handle label
		ui->LabelNextPack->show();
		// handle table widget
		ui->tableWidgetNetMapDst->setEnabled(true);
		ui->tableWidgetNetMapSrc->setEnabled(true);
	}
}

void MainWindow::ToUIState_Stop()
{
	// handle action
	ui->actionOpen_File->setEnabled(true);
	ui->actionOpen_Folder->setEnabled(true);
	ui->actionLoad_Config->setEnabled(true);
	ui->actionSave_Config->setEnabled(true);
	ui->actionScheduler->setEnabled(true);
	ui->actionConvert_Pcap->setEnabled(true);
	// handle combo box
	ui->comboBox->setEnabled(true);
	// handle list widget
	ui->listWidget->setEnabled(true);
	ui->listWidget->setCurrentRow(0);
	// handle button
	ui->BtnPlay->setEnabled(true);
	ui->BtnPause->setEnabled(false);
	ui->BtnStop->setEnabled(false);
	ui->BtnRemove->setEnabled(true);
	ui->BtnRemoveDstIPMap->setEnabled(true);
	ui->BtnRemoveScrIPMap->setEnabled(true);
	ui->BtnAddDstMap->setEnabled(true);
	ui->BtnAddSrcMap->setEnabled(true);
	//ui->BtnClose->setEnabled(true);
	ui->BtnRegularPlay->setEnabled(true);
	ui->BtnPause->setText("Pause");
	ui->BtnRemoveAll->setEnabled(true);
	// handle line edit
	ui->lineEdit_SpeedFactor->setEnabled(true);
	ui->lineEdit_SpeedLimit->setEnabled(true);
	ui->lineEdit_Offset->setEnabled(true);
	ui->lineEdit_LoopCount->setEnabled(true);
	// handle check box
	ui->checkBox_FixSpeed->setEnabled(true);
	ui->checkBox_OffsetEnable->setEnabled(true);
	ui->checkBox_LoopPlay->setEnabled(true);
	// handle process bar
	ui->progressBar->setValue(0);
	// init label
	ui->LabelNextPack->hide();
	// handle table widget
	ui->tableWidgetNetMapDst->setEnabled(true);
	ui->tableWidgetNetMapSrc->setEnabled(true);
}

quint32 MainWindow::Get_LoopCount()
{
	return ui->lineEdit_LoopCount->text().toUInt();
}

quint32 MainWindow::Get_SpeedFactor()
{
	return ui->lineEdit_SpeedFactor->text().toUInt();
}

double MainWindow::Get_SpeedLimit()
{
	return ui->lineEdit_SpeedLimit->text().toDouble();
}

quint32 MainWindow::Get_OffSet()
{
	return ui->lineEdit_Offset->text().toUInt();
}

bool MainWindow::Get_Check_SpeedLimit()
{
	return ui->checkBox_FixSpeed->checkState() == Qt::Checked ? true : false;
}

bool MainWindow::Get_Check_OffsetEnable()
{
	return ui->checkBox_OffsetEnable->checkState() == Qt::Checked ? true : false;
}

bool MainWindow::Get_Check_LoopPlay()
{
	return ui->checkBox_LoopPlay->checkState() == Qt::Checked ? true : false;
}

bool MainWindow::Get_Check_RegularReplayEnable()
{
	return m_bRegularReplayEnable;
}

void MainWindow::onPause()
{
	ToUIState_Pause();
	if (m_pMainCtrl->Get_CurrentState() == PcapReplayCtrl::PAUSE)
	{
		m_pMainCtrl->m_MsgQ.push(PcapReplayCtrl::GEN_CONFIG);
		m_pMainCtrl->m_MsgQ.push(PcapReplayCtrl::RESUME);
	}
	else
	{
		m_pMainCtrl->m_MsgQ.push(PcapReplayCtrl::PAUSE);
	}
}

void MainWindow::onStop()
{
	m_pMainCtrl->m_MsgQ.push(PcapReplayCtrl::STOP);
	ToUIState_Stop();
}

void MainWindow::onRemove()
{
	int nCurIdx = ui->listWidget->currentRow();
	m_fileNames.removeAt(nCurIdx);
	QListWidgetItem* item = ui->listWidget->takeItem(nCurIdx);
	delete item;
	if (ui->listWidget->count() == 0)
	{
		// update UI
		ui->BtnRemove->setEnabled(false);
		ui->BtnRemoveAll->setEnabled(false);
	}
}

void MainWindow::onClose()
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, QString::fromStdString(g_AppName),
		tr("Are you sure?\n"),
		QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
		QMessageBox::Yes);
	if (resBtn != QMessageBox::Yes)
	{
		
	}
	else
	{
		// collect data from ui
		CollectDataFromUI();

		Serialize(true);
		QApplication::quit();
	}
}

void MainWindow::Serialize(bool bStore, QString strConfigFile)
{
	if (bStore)
	{
		QFile* pFile;
		if (strConfigFile == "")
		{
			pFile = new QFile(m_curAppPath + "/DefaultConfig.pcapcfg");
		}
		else
		{
			pFile = new QFile(strConfigFile);
		}

		if (pFile->open(QIODevice::WriteOnly))
		{
			QDataStream out(pFile);   // we will serialize the data into the file
			out << m_LatestFilePath;
			//out << m_nAdapterIdx;
			out << m_qstrLatestConfigPath;
			out << m_fileNames;
			out << m_qmapDstIP;
			out << m_qmapScrIP;
			out << ui->lineEdit_LoopCount->text();
			if (ui->checkBox_LoopPlay->checkState() == Qt::Checked)
				out << true;
			else
				out << false;
			m_pDialogScheduler->Serialize(bStore, out);
			pFile->close();
		}
		delete pFile;
	}
	else
	{
		QFile* pFile;
		if (strConfigFile == "")
		{
			pFile = new QFile(m_curAppPath + "/DefaultConfig.pcapcfg");
		}
		else
		{
			pFile = new QFile(strConfigFile);
		}
		if (pFile->open(QIODevice::ReadOnly))
		{
			QDataStream in(pFile);   // we will serialize the data into the file
			in >> m_LatestFilePath;
			//in >> m_nAdapterIdx;
			in >> m_qstrLatestConfigPath;
			in >> m_fileNames;
			in >> m_qmapDstIP;
			in >> m_qmapScrIP;
			QString strLoopCount;
			in >> strLoopCount;
			ui->lineEdit_LoopCount->setText(strLoopCount);
			bool bLoopPlay;
			in >> bLoopPlay;
			ui->checkBox_LoopPlay->setChecked(bLoopPlay);
			m_pDialogScheduler->Serialize(bStore, in);
			pFile->close();
		}
		delete pFile;
	}
}

void MainWindow::onListWidget_Actived(int index)
{
	m_nAdapterIdx = index;
}

// override
void MainWindow::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, QString::fromStdString(g_AppName),
		tr("Are you sure?\n"),
		QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
		QMessageBox::Yes);
	if (resBtn != QMessageBox::Yes) 
	{
		event->ignore();
	}
	else 
	{
		// collect data from ui
		CollectDataFromUI();

		Serialize(true);
		event->accept();
	}
}

void MainWindow::Scheduler_onConfirm()
{
	m_pMainCtrl->m_MsgQ.push(PcapReplayCtrl::GEN_CONFIG_SCHDIALOG);
	m_pDialogScheduler->close();
}

void MainWindow::onRegularReplay()
{
	m_bRegularReplayEnable = !m_bRegularReplayEnable;
	m_pMainCtrl->m_MsgQ.push(PcapReplayCtrl::GEN_CONFIG);
	m_pMainCtrl->m_MsgQ.push(PcapReplayCtrl::REGULAR_REPLAY);
	ToUIState_Play();
}

void MainWindow::onRemoveAll()
{
	ui->listWidget->clear();
	m_fileNames.clear();
	// update UI
	ui->BtnRemove->setEnabled(false);
	ui->BtnRemoveAll->setEnabled(false);
}

void MainWindow::SelectNextPcapFile()
{
	ui->listWidget->setCurrentRow(ui->listWidget->currentRow() + 1);
}

void MainWindow::onAddSrcMap()
{
	ui->tableWidgetNetMapSrc->insertRow(ui->tableWidgetNetMapSrc->rowCount());
}

void MainWindow::onAddDstMap()
{
	ui->tableWidgetNetMapDst->insertRow(ui->tableWidgetNetMapDst->rowCount());
}

QMap<QString, QString> MainWindow::GetMapFromNetworkTable(QTableWidget const * inTable)
{
	QMap<QString, QString> mapTemp;
	for (int nRow = 0; nRow < inTable->rowCount(); nRow++)
	{
		//QTableWidgetItem * itemOriginal = inTable->itemAt(0, nRow);
		//QTableWidgetItem * itemConverted = inTable->itemAt(1, nRow);
		QTableWidgetItem * itemOriginal = inTable->item(nRow, 0);
		QTableWidgetItem * itemConverted = inTable->item(nRow, 1);
		QString strOriginalIP = itemOriginal->text();
		QString strConvertedIP = itemConverted->text();
		mapTemp.insert(strOriginalIP, strConvertedIP);
	}
	return mapTemp;
}

void MainWindow::ConvertQMap2StdMap(const QMap<QString, QString>& inMap, std::map<std::string, std::string>& outMap)
{
	for (QMap<QString, QString>::const_iterator it = inMap.begin();
		it != inMap.end(); it++)
	{
		std::string strKey = it.key().toUtf8().constData();
		std::string strValue = it.value().toUtf8().constData();
		outMap.insert(std::make_pair(strKey, strValue));
	}
}

void MainWindow::InitTableWidgetNetMapping(QTableWidget* inTable, const QMap<QString, QString>& inMap, const QStringList& lstHeaders)
{
	inTable->clear();
	inTable->setColumnCount(lstHeaders.size());
	for (int i = 0; i < lstHeaders.size(); i++)
	{
		inTable->setHorizontalHeaderItem(i, new QTableWidgetItem(lstHeaders[i]));
	}
	inTable->setRowCount(inMap.size());
	int nRow = 0;
	for (QMap<QString, QString>::const_iterator it = inMap.begin();
		it != inMap.end(); it++)
	{
		//int nRowCount = inTable->rowCount();
		//inTable->insertRow(inTable->rowCount());
		inTable->setItem(nRow, 0, new QTableWidgetItem(it.key()));
		inTable->setItem(nRow, 1, new QTableWidgetItem(it.value()));
		nRow++;
	}
}

void MainWindow::CollectDataFromUI()
{
	m_nCurPcapFileIdx = ui->listWidget->currentRow();
	m_qmapScrIP = GetMapFromNetworkTable(ui->tableWidgetNetMapSrc);
	m_qmapDstIP = GetMapFromNetworkTable(ui->tableWidgetNetMapDst);
	ConvertQMap2StdMap(m_qmapScrIP, m_mapScrIP);
	ConvertQMap2StdMap(m_qmapDstIP, m_mapDstIP);
}

void MainWindow::onRemoveScrMapIP()
{
	int ncurRow = ui->tableWidgetNetMapSrc->currentRow();
	if (ncurRow == -1)
		return;
	ui->tableWidgetNetMapSrc->removeRow(ncurRow);

	m_qmapScrIP = GetMapFromNetworkTable(ui->tableWidgetNetMapSrc);
	ConvertQMap2StdMap(m_qmapScrIP, m_mapScrIP);
}

void MainWindow::onRemoveDstMapIP()
{
	int ncurRow = ui->tableWidgetNetMapDst->currentRow();
	if (ncurRow == -1)
		return;
	ui->tableWidgetNetMapDst->removeRow(ncurRow);

	m_qmapDstIP = GetMapFromNetworkTable(ui->tableWidgetNetMapDst);
	ConvertQMap2StdMap(m_qmapDstIP, m_mapDstIP);
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
	// if some actions should not be usable, like move, this code must be adopted
	event->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent* event)
{
	// if some actions should not be usable, like move, this code must be adopted
	event->acceptProposedAction();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent* event)
{
	event->accept();
}

void MainWindow::dropEvent(QDropEvent* event)
{
	const QMimeData* mimeData = event->mimeData();

	if (mimeData->hasUrls())
	{
		QStringList pathList;
		QList<QUrl> urlList = mimeData->urls();

		for (int i = 0; i < urlList.size(); ++i)
		{
			QString fileName = urlList.at(i).toLocalFile();
			if (fileName.endsWith("pcap", Qt::CaseInsensitive))
				pathList.append(urlList.at(i).toLocalFile());
		}
		AddFileToListWidget(pathList);
		//if (openFiles(pathList))
		//	event->acceptProposedAction();
	}
}

void MainWindow::AddFileToListWidget(const QStringList& fileNames)
{
	QFileInfo fileInfo(fileNames.front());
	m_LatestFilePath = fileInfo.filePath();
	ui->listWidget->clear();
	Fillup_ListWidget(&fileNames);
}

void MainWindow::ConvertPcapFile()
{
	QFile file("convert.bat");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);
	for (int i = 0; i < m_fileNames.size(); i++)
	{
		QFileInfo fi(m_fileNames[i]);
		if (!QDir(fi.absolutePath() + "\\converted").exists())
			QDir().mkdir(fi.absolutePath() + "\\converted");
		out << ".\\bin\\wireshark\\editcap.exe -F pcap " << m_fileNames[i] << " " << fi.absolutePath() << "\\converted\\" << fi.fileName() << "\n";
	}
	file.close();

	QMessageBox WaitingBox;
	WaitingBox.setWindowTitle(QString("Wait for converting!"));
	//WaitingBox.setText("Wait for converting!");
	WaitingBox.show();
	QProcess p;
	p.start(".\\convert.bat");
	p.waitForFinished();
	WaitingBox.close();
}

void MainWindow::SetLoopCountSlot(quint32 unLoopCount)
{
	ui->lineEdit_LoopCount->setText(QString::number(unLoopCount));
}