/*
 * This file is generated from the following input
 *
 * using python version 3.5.1 |Anaconda 4.0.0 (x86_64)| (default, Dec  7 2015, 11:24:55) 
[GCC 4.2.1 (Apple Inc. build 5577)]
 *  QString|pdlName|||RN
 *  QString|lastSync|||RN
 *  bool|homeEnabled|true||RN
 *  int|serverId|||RN
 *  QString|serverName|||RN
 *  QString|airline|||RWN
 *  QString|ACType|||RWN
 *  QString|tailsign|||RWN
 *  QString|ataChapter|||RWN
 *  QString|ataSection|||RWN
 *  QString|software|||RWN
 *  QString|softwareTitle|||RWN
 *  QString|softwareCoC|||RWN
 *  QString|USBButtonText|||RN
 *  QString|status|||RN
 *  QString|statusColor|||RN
 *  QString|buildInfo|||RN
 *  QString|startTime|||RN
 *  QString|remainTime|||RN
 *  QString|elapsedTime|||RN
 *  QString|connectionType|||RN
 *  QString|progress|||RN
 *  bool|demoPDLMode|false||RN
 *  QString|updateStatus|||RN
 *  QString|updateProgress|||RN
 *  QString|updateStartTime|||RN
 *  QString|uploadStatus|||RN
 *  bool|installRunning|false||RN
 *  bool|showLogWindow|false||RN
 *  bool|showUSBAbort|false||RN
 *  QString|logText|||RN
 *  bool|showAdministration|false||RN
 *  QString|li_Disk|||RN
 *  QString|li_Disks|||RN
 *  QString|li_File|||RN
 *  QString|li_Block|||RN
 *  QString|li_Blocks|||RN
 *  bool|FLSDeskChecked|false||RWN
 *  bool|EESMChecked|false||RWN
 *  bool|a6153load|false||RN
 *  bool|a615aload|false||RN
 *  bool|usbload|false||RN
 *  QString|syncButtonText|"Start Synchronisation"||RWN
 *  bool|syncButtonEnabled|true||RWN
 *  bool|syncRunning|false||RWN
 *
*/

#ifndef BACKENDBASE_H
#define BACKENDBASE_H

#include <QObject>

class BackendBase : public QObject
{
    Q_OBJECT
    friend class Backend;


	Q_PROPERTY(QString ACType             READ getACType             WRITE setACType             NOTIFY ACTypeChanged            )
	Q_PROPERTY(bool    EESMChecked        READ getEESMChecked        WRITE setEESMChecked        NOTIFY EESMCheckedChanged       )
	Q_PROPERTY(bool    FLSDeskChecked     READ getFLSDeskChecked     WRITE setFLSDeskChecked     NOTIFY FLSDeskCheckedChanged    )
    //Q_PROPERTY(QString USBButtonText      READ getUSBButtonText                                  NOTIFY USBButtonTextChanged     )
	Q_PROPERTY(bool    a6153load          READ getA6153load                                      NOTIFY a6153loadChanged         )
	Q_PROPERTY(bool    a615aload          READ getA615aload                                      NOTIFY a615aloadChanged         )
	Q_PROPERTY(QString airline            READ getAirline            WRITE setAirline            NOTIFY airlineChanged           )
	Q_PROPERTY(QString ataChapter         READ getAtaChapter         WRITE setAtaChapter         NOTIFY ataChapterChanged        )
	Q_PROPERTY(QString ataSection         READ getAtaSection         WRITE setAtaSection         NOTIFY ataSectionChanged        )
	Q_PROPERTY(QString buildInfo          READ getBuildInfo                                      NOTIFY buildInfoChanged         )
	Q_PROPERTY(QString connectionType     READ getConnectionType                                 NOTIFY connectionTypeChanged    )
	Q_PROPERTY(bool    demoPDLMode        READ getDemoPDLMode                                    NOTIFY demoPDLModeChanged       )
	Q_PROPERTY(QString elapsedTime        READ getElapsedTime                                    NOTIFY elapsedTimeChanged       )
	Q_PROPERTY(bool    homeEnabled        READ getHomeEnabled                                    NOTIFY homeEnabledChanged       )
	Q_PROPERTY(bool    installRunning     READ getInstallRunning                                 NOTIFY installRunningChanged    )
	Q_PROPERTY(QString lastSync           READ getLastSync                                       NOTIFY lastSyncChanged          )
	Q_PROPERTY(QString li_Block           READ getBlock                                          NOTIFY BlockChanged             )
	Q_PROPERTY(QString li_Blocks          READ getBlocks                                         NOTIFY BlocksChanged            )
	Q_PROPERTY(QString li_Disk            READ getDisk                                           NOTIFY DiskChanged              )
	Q_PROPERTY(QString li_Disks           READ getDisks                                          NOTIFY DisksChanged             )
	Q_PROPERTY(QString li_File            READ getFile                                           NOTIFY FileChanged              )
	Q_PROPERTY(QString logText            READ getLogText                                        NOTIFY logTextChanged           )
	Q_PROPERTY(QString pdlName            READ getPdlName                                        NOTIFY pdlNameChanged           )
	Q_PROPERTY(QString progress           READ getProgress                                       NOTIFY progressChanged          )
	Q_PROPERTY(QString remainTime         READ getRemainTime                                     NOTIFY remainTimeChanged        )
	Q_PROPERTY(int     serverId           READ getServerId                                       NOTIFY serverIdChanged          )
	Q_PROPERTY(QString serverName         READ getServerName                                     NOTIFY serverNameChanged        )
	Q_PROPERTY(bool    showAdministration READ getShowAdministration                             NOTIFY showAdministrationChanged)
	Q_PROPERTY(bool    showLogWindow      READ getShowLogWindow                                  NOTIFY showLogWindowChanged     )
    //Q_PROPERTY(bool    showUSBAbort       READ getShowUSBAbort                                   NOTIFY showUSBAbortChanged      )
	Q_PROPERTY(QString software           READ getSoftware           WRITE setSoftware           NOTIFY softwareChanged          )
	Q_PROPERTY(QString softwareCoC        READ getSoftwareCoC        WRITE setSoftwareCoC        NOTIFY softwareCoCChanged       )
	Q_PROPERTY(QString softwareTitle      READ getSoftwareTitle      WRITE setSoftwareTitle      NOTIFY softwareTitleChanged     )
	Q_PROPERTY(QString startTime          READ getStartTime                                      NOTIFY startTimeChanged         )
	Q_PROPERTY(QString status             READ getStatus                                         NOTIFY statusChanged            )
	Q_PROPERTY(QString statusColor        READ getStatusColor                                    NOTIFY statusColorChanged       )
	Q_PROPERTY(bool    syncButtonEnabled  READ getSyncButtonEnabled  WRITE setSyncButtonEnabled  NOTIFY syncButtonEnabledChanged )
	Q_PROPERTY(QString syncButtonText     READ getSyncButtonText     WRITE setSyncButtonText     NOTIFY syncButtonTextChanged    )
	Q_PROPERTY(bool    syncRunning        READ getSyncRunning        WRITE setSyncRunning        NOTIFY syncRunningChanged       )
	Q_PROPERTY(QString tailsign           READ getTailsign           WRITE setTailsign           NOTIFY tailsignChanged          )
	Q_PROPERTY(QString updateProgress     READ getUpdateProgress                                 NOTIFY updateProgressChanged    )
	Q_PROPERTY(QString updateStartTime    READ getUpdateStartTime                                NOTIFY updateStartTimeChanged   )
	Q_PROPERTY(QString updateStatus       READ getUpdateStatus                                   NOTIFY updateStatusChanged      )
	Q_PROPERTY(QString uploadStatus       READ getUploadStatus                                   NOTIFY uploadStatusChanged      )
    Q_PROPERTY(bool    usbload            READ getUsbload                                        NOTIFY usbloadChanged           )

    Q_PROPERTY(int     repositoryAll      READ getRepositoryAll                                  NOTIFY repositoryAllChanged     )
    Q_PROPERTY(int     repositoryValid    READ getRepositoryValid                                NOTIFY repositoryValidChanged   )
    Q_PROPERTY(int     repositoryExpired  READ getRepositoryExpired                              NOTIFY repositoryExpiredChanged )
    Q_PROPERTY(int     repositoryNotRef   READ getRepositoryNotRef                               NOTIFY repositoryNotRefChanged  )
    Q_PROPERTY(int     repositoryNotDown  READ getRepositoryNotDown                              NOTIFY repositoryNotDownChanged )

public:
    BackendBase() {}
    explicit BackendBase(QObject * /* parent */ = 0) {}

private:
	QString mACType;                                   // 
	bool    mEESMChecked        = false;               // 
	bool    mFLSDeskChecked     = false;               // 
	QString mUSBButtonText;                            // 
	bool    mA6153load          = false;               // 
	bool    mA615aload          = false;               // 
	QString mAirline;                                  // 
	QString mAtaChapter;                               // 
	QString mAtaSection;                               // 
	QString mBuildInfo;                                // 
	QString mConnectionType;                           // 
	bool    mDemoPDLMode        = false;               // 
	QString mElapsedTime;                              // 
	bool    mHomeEnabled        = true;                // 
	bool    mInstallRunning     = false;               // 
	QString mLastSync;                                 // 
	QString mLiBlock;                                  // 
	QString mLiBlocks;                                 // 
	QString mLiDisk;                                   // 
	QString mLiDisks;                                  // 
	QString mLiFile;                                   // 
	QString mLogText;                                  // 
	QString mPdlName;                                  // 
	QString mProgress;                                 // 
	QString mRemainTime;                               // 
	int     mServerId;                                 // 
	QString mServerName;                               // 
	bool    mShowAdministration = false;               // 
	bool    mShowLogWindow      = false;               // 
    //bool    mShowUSBAbort       = false;               //
	QString mSoftware;                                 // 
	QString mSoftwareCoC;                              // 
	QString mSoftwareTitle;                            // 
	QString mStartTime;                                // 
	QString mStatus;                                   // 
	QString mStatusColor;                              // 
	bool    mSyncButtonEnabled  = true;                // 
	QString mSyncButtonText     = "Start Synchronisation";// 
	bool    mSyncRunning        = false;               // 
	QString mTailsign;                                 // 
	QString mUpdateProgress;                           // 
	QString mUpdateStartTime;                          // 
	QString mUpdateStatus;                             // 
	QString mUploadStatus;                             // 
    bool    mUsbload            = false;               //
    int     mRepositoryAll      = 1;
    int     mRepositoryValid    = 2;
    int     mRepositoryExpired  = 3;
    int     mRepositoryNotRef   = 4;
    int     mRepositoryNotDown  = 5;

public:
	inline QString getACType()              { return mACType; } 
	inline bool    getEESMChecked()         { return mEESMChecked; } 
	inline bool    getFLSDeskChecked()      { return mFLSDeskChecked; } 
    //inline QString getUSBButtonText()       { return mUSBButtonText; }
	inline bool    getA6153load()           { return mA6153load; } 
	inline bool    getA615aload()           { return mA615aload; } 
	inline QString getAirline()             { return mAirline; } 
	inline QString getAtaChapter()          { return mAtaChapter; } 
	inline QString getAtaSection()          { return mAtaSection; } 
	inline QString getBuildInfo()           { return mBuildInfo; } 
	inline QString getConnectionType()      { return mConnectionType; } 
	inline bool    getDemoPDLMode()         { return mDemoPDLMode; } 
	inline QString getElapsedTime()         { return mElapsedTime; } 
	inline bool    getHomeEnabled()         { return mHomeEnabled; } 
	inline bool    getInstallRunning()      { return mInstallRunning; } 
	inline QString getLastSync()            { return mLastSync; } 
	inline QString getBlock()               { return mLiBlock; } 
	inline QString getBlocks()              { return mLiBlocks; } 
	inline QString getDisk()                { return mLiDisk; } 
	inline QString getDisks()               { return mLiDisks; } 
	inline QString getFile()                { return mLiFile; } 
	inline QString getLogText()             { return mLogText; } 
	inline QString getPdlName()             { return mPdlName; } 
	inline QString getProgress()            { return mProgress; } 
    inline QString getRemainTime()          { return mRemainTime; }
    inline int     getRepositoryAll()       { return mRepositoryAll; }
    inline int     getRepositoryValid()     { return mRepositoryValid; }
    inline int     getRepositoryExpired()   { return mRepositoryExpired; }
    inline int     getRepositoryNotRef()    { return mRepositoryNotRef; }
    inline int     getRepositoryNotDown()   { return mRepositoryNotDown; }
    inline int     getServerId()            { return mServerId; }
	inline QString getServerName()          { return mServerName; } 
	inline bool    getShowAdministration()  { return mShowAdministration; } 
	inline bool    getShowLogWindow()       { return mShowLogWindow; } 
    //inline bool    getShowUSBAbort()        { return mShowUSBAbort; }
	inline QString getSoftware()            { return mSoftware; } 
	inline QString getSoftwareCoC()         { return mSoftwareCoC; } 
	inline QString getSoftwareTitle()       { return mSoftwareTitle; } 
	inline QString getStartTime()           { return mStartTime; } 
	inline QString getStatus()              { return mStatus; } 
	inline QString getStatusColor()         { return mStatusColor; } 
	inline bool    getSyncButtonEnabled()   { return mSyncButtonEnabled; } 
	inline QString getSyncButtonText()      { return mSyncButtonText; } 
	inline bool    getSyncRunning()         { return mSyncRunning; } 
	inline QString getTailsign()            { return mTailsign; } 
	inline QString getUpdateProgress()      { return mUpdateProgress; } 
	inline QString getUpdateStartTime()     { return mUpdateStartTime; } 
	inline QString getUpdateStatus()        { return mUpdateStatus; } 
	inline QString getUploadStatus()        { return mUploadStatus; } 
	inline bool    getUsbload()             { return mUsbload; } 

public:
	virtual inline void setACType(QString ACType) {
        if (mACType != ACType) {
            mACType  = ACType;
            emit ACTypeChanged();
        }
    }
	virtual inline void setEESMChecked(bool EESMChecked) {
        if (mEESMChecked != EESMChecked) {
            mEESMChecked  = EESMChecked;
            emit EESMCheckedChanged();
        }
    }
	virtual inline void setFLSDeskChecked(bool FLSDeskChecked) {
        if (mFLSDeskChecked != FLSDeskChecked) {
            mFLSDeskChecked  = FLSDeskChecked;
            emit FLSDeskCheckedChanged();
        }
    }
//	virtual inline void setUSBButtonText(QString USBButtonText) {
//        if (mUSBButtonText != USBButtonText) {
//            mUSBButtonText  = USBButtonText;
//            emit USBButtonTextChanged();
//        }
//    }
	virtual inline void setA6153load(bool a6153load) {
        if (mA6153load != a6153load) {
            mA6153load  = a6153load;
            emit a6153loadChanged();
        }
    }
	virtual inline void setA615aload(bool a615aload) {
        if (mA615aload != a615aload) {
            mA615aload  = a615aload;
            emit a615aloadChanged();
        }
    }
	virtual inline void setAirline(QString airline) {
        if (mAirline != airline) {
            mAirline  = airline;
            emit airlineChanged();
        }
    }
	virtual inline void setAtaChapter(QString ataChapter) {
        if (mAtaChapter != ataChapter) {
            mAtaChapter  = ataChapter;
            emit ataChapterChanged();
        }
    }
	virtual inline void setAtaSection(QString ataSection) {
        if (mAtaSection != ataSection) {
            mAtaSection  = ataSection;
            emit ataSectionChanged();
        }
    }
	virtual inline void setBuildInfo(QString buildInfo) {
        if (mBuildInfo != buildInfo) {
            mBuildInfo  = buildInfo;
            emit buildInfoChanged();
        }
    }
	virtual inline void setConnectionType(QString connectionType) {
        if (mConnectionType != connectionType) {
            mConnectionType  = connectionType;
            emit connectionTypeChanged();
        }
    }
	virtual inline void setDemoPDLMode(bool demoPDLMode) {
        if (mDemoPDLMode != demoPDLMode) {
            mDemoPDLMode  = demoPDLMode;
            emit demoPDLModeChanged();
        }
    }
	virtual inline void setElapsedTime(QString elapsedTime) {
        if (mElapsedTime != elapsedTime) {
            mElapsedTime  = elapsedTime;
            emit elapsedTimeChanged();
        }
    }
	virtual inline void setHomeEnabled(bool homeEnabled) {
        if (mHomeEnabled != homeEnabled) {
            mHomeEnabled  = homeEnabled;
            emit homeEnabledChanged();
        }
    }
	virtual inline void setInstallRunning(bool installRunning) {
        if (mInstallRunning != installRunning) {
            mInstallRunning  = installRunning;
            emit installRunningChanged();
        }
    }
//	virtual inline void setLastSync(QString lastSync) {
//        if (mLastSync != lastSync) {
//            mLastSync  = lastSync;
//            emit lastSyncChanged();
//        }
//    }
	virtual inline void setBlock(QString li_Block) {
        if (mLiBlock != li_Block) {
            mLiBlock  = li_Block;
            emit BlockChanged();
        }
    }
	virtual inline void setBlocks(QString li_Blocks) {
        if (mLiBlocks != li_Blocks) {
            mLiBlocks  = li_Blocks;
            emit BlocksChanged();
        }
    }
	virtual inline void setDisk(QString li_Disk) {
        if (mLiDisk != li_Disk) {
            mLiDisk  = li_Disk;
            emit DiskChanged();
        }
    }
	virtual inline void setDisks(QString li_Disks) {
        if (mLiDisks != li_Disks) {
            mLiDisks  = li_Disks;
            emit DisksChanged();
        }
    }
	virtual inline void setFile(QString li_File) {
        if (mLiFile != li_File) {
            mLiFile  = li_File;
            emit FileChanged();
        }
    }
	virtual inline void setLogText(QString logText) {
        if (mLogText != logText) {
            mLogText  = logText;
            emit logTextChanged();
        }
    }
	virtual inline void setPdlName(QString pdlName) {
        if (mPdlName != pdlName) {
            mPdlName  = pdlName;
            emit pdlNameChanged();
        }
    }
	virtual inline void setProgress(QString progress) {
        if (mProgress != progress) {
            mProgress  = progress;
            emit progressChanged();
        }
    }
    virtual inline void setRemainTime(QString remainTime) {
        if (mRemainTime != remainTime) {
            mRemainTime  = remainTime;
            emit remainTimeChanged();
        }
    }
    virtual inline void setRepositoryAll(int repositoryAll) {
        if (mRepositoryAll != repositoryAll) {
            mRepositoryAll  = repositoryAll;
            emit repositoryAllChanged();
        }
    }
    virtual inline void setRepositoryValid(int repositoryValid) {
        if (mRepositoryValid != repositoryValid) {
            mRepositoryValid  = repositoryValid;
            emit repositoryValidChanged();
        }
    }
    virtual inline void setRepositoryExpired(int repositoryExpired) {
        if (mRepositoryExpired != repositoryExpired) {
            mRepositoryExpired  = repositoryExpired;
            emit repositoryExpiredChanged();
        }
    }
    virtual inline void setRepositoryNotDown(int repositoryNotDown) {
        if (mRepositoryNotDown != repositoryNotDown) {
            mRepositoryNotDown  = repositoryNotDown;
            emit repositoryNotDownChanged();
        }
    }
    virtual inline void setRepositoryNotRef(int repositoryNotRef) {
        if (mRepositoryNotRef != repositoryNotRef) {
            mRepositoryNotRef  = repositoryNotRef;
            emit repositoryNotRefChanged();
        }
    }
    virtual inline void setServerId(int serverId) {
        if (mServerId != serverId) {
            mServerId  = serverId;
            emit serverIdChanged();
        }
    }
	virtual inline void setServerName(QString serverName) {
        if (mServerName != serverName) {
            mServerName  = serverName;
            emit serverNameChanged();
        }
    }
	virtual inline void setShowAdministration(bool showAdministration) {
        if (mShowAdministration != showAdministration) {
            mShowAdministration  = showAdministration;
            emit showAdministrationChanged();
        }
    }
	virtual inline void setShowLogWindow(bool showLogWindow) {
        if (mShowLogWindow != showLogWindow) {
            mShowLogWindow  = showLogWindow;
            emit showLogWindowChanged();
        }
    }
//	virtual inline void setShowUSBAbort(bool showUSBAbort) {
//        if (mShowUSBAbort != showUSBAbort) {
//            mShowUSBAbort  = showUSBAbort;
//            emit showUSBAbortChanged();
//        }
//    }
	virtual inline void setSoftware(QString software) {
        if (mSoftware != software) {
            mSoftware  = software;
            emit softwareChanged();
        }
    }
	virtual inline void setSoftwareCoC(QString softwareCoC) {
        if (mSoftwareCoC != softwareCoC) {
            mSoftwareCoC  = softwareCoC;
            emit softwareCoCChanged();
        }
    }
	virtual inline void setSoftwareTitle(QString softwareTitle) {
        if (mSoftwareTitle != softwareTitle) {
            mSoftwareTitle  = softwareTitle;
            emit softwareTitleChanged();
        }
    }
	virtual inline void setStartTime(QString startTime) {
        if (mStartTime != startTime) {
            mStartTime  = startTime;
            emit startTimeChanged();
        }
    }
	virtual inline void setStatus(QString status) {
        if (mStatus != status) {
            mStatus  = status;
            emit statusChanged();
        }
    }
	virtual inline void setStatusColor(QString statusColor) {
        if (mStatusColor != statusColor) {
            mStatusColor  = statusColor;
            emit statusColorChanged();
        }
    }
	virtual inline void setSyncButtonEnabled(bool syncButtonEnabled) {
        if (mSyncButtonEnabled != syncButtonEnabled) {
            mSyncButtonEnabled  = syncButtonEnabled;
            emit syncButtonEnabledChanged();
        }
    }
	virtual inline void setSyncButtonText(QString syncButtonText) {
        if (mSyncButtonText != syncButtonText) {
            mSyncButtonText  = syncButtonText;
            emit syncButtonTextChanged();
        }
    }
	virtual inline void setSyncRunning(bool syncRunning) {
        if (mSyncRunning != syncRunning) {
            mSyncRunning  = syncRunning;
            emit syncRunningChanged();
        }
    }
	virtual inline void setTailsign(QString tailsign) {
        if (mTailsign != tailsign) {
            mTailsign  = tailsign;
            emit tailsignChanged();
        }
    }
	virtual inline void setUpdateProgress(QString updateProgress) {
        if (mUpdateProgress != updateProgress) {
            mUpdateProgress  = updateProgress;
            emit updateProgressChanged();
        }
    }
	virtual inline void setUpdateStartTime(QString updateStartTime) {
        if (mUpdateStartTime != updateStartTime) {
            mUpdateStartTime  = updateStartTime;
            emit updateStartTimeChanged();
        }
    }
	virtual inline void setUpdateStatus(QString updateStatus) {
        if (mUpdateStatus != updateStatus) {
            mUpdateStatus  = updateStatus;
            emit updateStatusChanged();
        }
    }
	virtual inline void setUploadStatus(QString uploadStatus) {
        if (mUploadStatus != uploadStatus) {
            mUploadStatus  = uploadStatus;
            emit uploadStatusChanged();
        }
    }
	virtual inline void setUsbload(bool usbload) {
        if (mUsbload != usbload) {
            mUsbload  = usbload;
            emit usbloadChanged();
        }
    }

signals:
	void ACTypeChanged();
	void EESMCheckedChanged();
	void FLSDeskCheckedChanged();
    //void USBButtonTextChanged();
	void a6153loadChanged();
	void a615aloadChanged();
	void airlineChanged();
	void ataChapterChanged();
	void ataSectionChanged();
	void buildInfoChanged();
	void connectionTypeChanged();
	void demoPDLModeChanged();
	void elapsedTimeChanged();
	void homeEnabledChanged();
	void installRunningChanged();
	void lastSyncChanged();
	void BlockChanged();
	void BlocksChanged();
	void DiskChanged();
	void DisksChanged();
	void FileChanged();
	void logTextChanged();
	void pdlNameChanged();
	void progressChanged();
    void remainTimeChanged();
    void repositoryAllChanged();
    void repositoryValidChanged();
    void repositoryExpiredChanged();
    void repositoryNotRefChanged();
    void repositoryNotDownChanged();
    void serverIdChanged();
	void serverNameChanged();
	void showAdministrationChanged();
	void showLogWindowChanged();
	void softwareChanged();
	void softwareCoCChanged();
	void softwareTitleChanged();
	void startTimeChanged();
	void statusChanged();
	void statusColorChanged();
	void syncButtonEnabledChanged();
	void syncButtonTextChanged();
	void syncRunningChanged();
	void tailsignChanged();
	void updateProgressChanged();
	void updateStartTimeChanged();
	void updateStatusChanged();
	void uploadStatusChanged();
	void usbloadChanged();

};
#endif // BACKENDBASE_H

