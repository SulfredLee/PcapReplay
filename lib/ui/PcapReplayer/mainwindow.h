#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/qdir.h>
#include <QtCore/qdiriterator.h>
#include <qfiledialog.h>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtCore/qfileinfo.h>
#include <QtCore/qmap.h>
#include <QtWidgets/qtablewidget.h>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtCore/QMimeData>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QProcess>

#include "Common.h"
#include "dialogscheduler.h"

#include <string>
#include <map>

namespace Ui {
class MainWindow;
}

class PcapReplayCtrl;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	PcapReplayCtrl* m_pMainCtrl;

	qint32 m_nAdapterIdx;
	QString m_curAppPath;
	QString m_LatestFilePath; // latest pcap file path
	QString m_qstrLatestConfigPath; // latest config path
	QStringList m_fileNames;
	DialogScheduler* m_pDialogScheduler;
	QMap<QString, QString> m_qmapDstIP;
	QMap<QString, QString> m_qmapScrIP;
	std::map<std::string, std::string> m_mapDstIP;
	std::map<std::string, std::string> m_mapScrIP;
	int m_nCurPcapFileIdx;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	
	quint32 Get_SpeedFactor();
	quint32 Get_SpeedLimit();
	quint32 Get_OffSet();
	quint32 Get_LoopCount();
	bool Get_Check_SpeedLimit();
	bool Get_Check_OffsetEnable();
	bool Get_Check_LoopPlay();
	bool Get_Check_RegularReplayEnable();
	bool IsFileExists(QString path);
	// Doc
	void Serialize(bool bStore, QString strConfigFile = "");
private:
    Ui::MainWindow *ui;

protected:
	bool m_bRegularReplayEnable;
	QStringList m_strLtsNetMapDstHeader, m_strLtsNetMapSrcHeader;
protected:
	void Fillup_ListWidget(QStringList const * fileNames = 0);
	void ToUIState_Play();
	void ToUIState_Pause();
	void GetBitPerSec(double bit, QString& line, int step);
	void HandleLabelSchedularStatus();
	QMap<QString, QString> GetMapFromNetworkTable(QTableWidget const * inTable);
	void ConvertQMap2StdMap(const QMap<QString, QString>& inMap, std::map<std::string, std::string>& outMap);
	void InitTableWidgetNetMapping(QTableWidget* inTable, const QMap<QString, QString>& inMap, const QStringList& lstHeaders);
	void CollectDataFromUI();
	void AddFileToListWidget(const QStringList& fileNames);
	// override
	void closeEvent(QCloseEvent *event);
	// override
	void dragEnterEvent(QDragEnterEvent* event);
	// override
	void dragMoveEvent(QDragMoveEvent* event);
	// override
	void dragLeaveEvent(QDragLeaveEvent* event);
	// override
	void dropEvent(QDropEvent* event);
signals:
	void SetCurProcess(int nCurProcess);
	void Signal_ToUIState_Stop();
	void SetCurSendingSpeed(int nCurSpeed);
	void SetCurSendingPktTime(QString strPktTime);
	void SetNextSendingPktTime(QString strPktTime);
	void ShowNextSendingPktTime(bool bShow);
	void SetNextPktNo(unsigned int unNextPkt);
	void StartPlayFromAutoDriver();
	void StartInitialFirstScreen();
	void ReadNextPcapFile();
	void SetLoopCount(quint32 unLoopCount);
private slots:
	void onOpen_File();
	void onOpen_Folder();
	void onListWidget_Actived(int index);
	void onPlay();
	void onPause();
	void onStop();
	void onRemove();
	void onClose();
	void UpdateCurProcess(int nCurProcess);
	void ToUIState_Stop();
	void UpdateCurSpeed(int nCurSpeed);
	void UpdateCurSendingPktTime(QString strPktTime);
	void UpdateNextSendingPktTime(QString strPktTime);
	void UpdateShowNextSendingPktTime(bool bShow);
	void onScheduler();
	void UpdateNextPktNo(unsigned int unNextPkt);
	void onSave_Config();
	void onLoad_Config();
	void Scheduler_onConfirm();
	void onRegularReplay();
	void Init_FirstScreen();
	void onRemoveAll();
	void SelectNextPcapFile();
	void onAddSrcMap();
	void onAddDstMap();
	void onRemoveScrMapIP();
	void onRemoveDstMapIP();
	void ConvertPcapFile();
	void SetLoopCountSlot(quint32 unLoopCount);
};

#endif // MAINWINDOW_H
