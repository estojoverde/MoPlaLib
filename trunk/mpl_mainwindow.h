#ifndef MPL_MAINWINDOW_H
#define MPL_MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QtWidgets>
#include <QtDebug>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>

#include "vis_vectorfieldviewer.h"
#include "mpl_core.h"
#include "vis_scene3d.h"

class MPL_MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MPL_MainWindow(QWidget *parent = 0);

    void setCore(MPL_Core *mplCore, QReadWriteLock *locker);

signals:
    void signal_logMessage( QString s );
    void signal_errorMessage( QString s );
    void signal_closedGUI();

    void signalviewer_rotacoes(int xRot, int yRot);

    void signalcore_setOwnRobot(double d_x, double d_y, double d_z);
    void signalcore_setEpsilon_OPF(double d_epsilon);//only OPF
    void signalcore_setVector_OPF(double d_vx, double d_vy, double d_vz);//only OPF

    void signalcore_setOmega(double d_omega);
    void signalcore_setGridSize(int n_sizeX, int n_sizeY, int n_sizeZ);
    void signalcore_setSolveMode(MPL_Core_SolveMode solveMode);

    void signalcore_solveI(int i_maxIt);
    void signalcore_solveE(double d_maxError);
    void signalcore_solveIE(int i_maxIt, double d_maxError);

    //Objects lists
        //ER
    void signalcore_addEnemyRobot(double d_x, double d_y, double d_z);
    void signalcore_clearListOfEnemyRobot(void);
        //FO
    void signalcore_addFixedObstacle(double d_x, double d_y, double d_z);
    void signalcore_clearListOfFixedObstacle(void);
        //MO
    void signalcore_addMovableObstacle(double d_x, double d_y, double d_z);
    void signalcore_clearListOfMovableObstacle(void);
        //G
    void signalcore_addGoal(double d_x, double d_y, double d_z);
    void signalcore_clearListOfGoal(void);






protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;

//    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void slot_logMessage( QString s );
    void slot_errorMessage( QString s );

private slots:
    void slot_open();
//    bool saveLog();
//    bool saveLogAs();
//    bool saveResults();
//    bool saveResultsAs();
    void slot_about();
    void slot_help();

    //GUI SLOTS

    void slotgui_delayedInit();
    void slotgui_topView();
    void slotgui_bottomView();
    void slotgui_leftView();
    void slotgui_rightView();
    void slotgui_frontView();
    void slotgui_backView();

        //Grid
    void slotgui_OPF_SetGrid();
    void slotgui_OPF_RecycleGrid();
    void slotgui_OPF_SetOmega();
    void slotgui_OPF_modeGS();
    void slotgui_OPF_modeSOR();
    void slotgui_OPF_Solve();

        //OR
    void slotgui_OPF_OR_Set();
    void slotgui_OPF_OR_Recycle();

        //ER
    void slotgui_OPF_ER_Add();
    void slotgui_OPF_ER_ClearList();
    void slotgui_OPF_ER_Recycle();

        //FO
    void slotgui_OPF_FO_Add();
    void slotgui_OPF_FO_ClearList();
    void slotgui_OPF_FO_Recycle();

        //MO
    void slotgui_OPF_MO_Add();
    void slotgui_OPF_MO_ClearList();
    void slotgui_OPF_MO_Recycle();

        //G
    void slotgui_OPF_G_Add();
    void slotgui_OPF_G_ClearList();
    void slotgui_OPF_G_Recycle();

    /*    QPushButton *pushbutton_grid_set_opf__;
    QPushButton *pushbutton_omegasor_set_opf__;
    QPushButton *pushbutton_solve_opf__;
    QPushButton *pushbutton_gridsolve_recycle__;*/

    void slotgui_Verbose();


private:
    void createActions();
    void createMenus();
    void createStatusBar();

    void createPageOPF();
    void createPageOPF_objectsListsGroup();
    void createPageOPF_ownRobotGroup();
    void createPageOPF_gridsolve();


    void createPageHPF();

    void createLog();
    void createCoreControl();

    void setConnections();

    void readSettings();
    void writeSettings();

    void create3dviewsToolbar();


//    void loadFile(const QString &fileName);
//    bool saveLog(const QString &fileName);
//    bool saveLogAs(const QString &fileName);
//    bool saveResults(const QString &fileName);
//    bool saveResultsAs(const QString &fileName);

//    QString strippedName(const QString &fullFileName);

    QTextEdit *textEditLog__;
    QDockWidget *dockWidgetLog__;
//    QString curFile__;

    QMenu *fileMenu__;
    QMenu *helpMenu__;
    QMenu *viewMenu__;


//Gui Control

    QToolBar *toolbar_3dviews;
    QAction *action_mouseMultiplier;
    QAction *action_3DViews[9];
    QAction *toggle3DViewsAct__;


//    QAction *openAct__;
//    QAction *saveLogAct__;
//    QAction *saveResultsAct__;
//    QAction *saveLogActAs__;
//    QAction *saveResultsActAs__;

    QAction *exitAct__;

//    QAction *copyAct__;

    QAction *aboutAct__;
    QAction *helpAct__;

    QAction *toggleLogAct__;
    QAction *toggleCoreModeAct__;

    QAction *snapshotAct__;

    QAction *verboseAct__;
    bool b_verbose__;

    //MPL_VectorFieldViewer *vfViewer__;
    VIS_VectorFieldViewer *vfViewer__;

    MPL_Core *mplCore__;

    bool b_textEdit_logState;


    /*Core state*/
    QDockWidget *dockwidget_coreMode__;
    QLabel *label_coreMode__;
    QComboBox *combobox_coreMode__;
    QStackedLayout *stackedlayout_coreMode__;
    QGridLayout *gridlayout_coreMode__;
    QDoubleValidator *doublevalidator_positive__;
    QTabWidget *tabwidget_coreMore__;

    QLabel *label_x__;
    QLabel *label_y__;
    QLabel *label_z__;

    //hpf
    QWidget *widget_page_hpf__;


    //opf
    QWidget *widget_page_opf__;

    QGridLayout *gridlayout_opf__;


    //gridandsolve
    QGroupBox *groupbox_gridsolve__;
    QGridLayout *gridlayout_opf_gridsolve__;

    QLineEdit *lineedit_grid_opf__[3];
    QLineEdit *lineedit_omegasor_opf__;
    QPushButton *pushbutton_grid_set_opf__;
    QPushButton *pushbutton_omegasor_set_opf__;
    QPushButton *pushbutton_solve_opf__;
    QPushButton *pushbutton_gridsolve_recycle__;
    QGroupBox *groupBox_solveMode__;
    QRadioButton *radioButton__solveMode_gs__;
    QRadioButton *radioButton__solveMode_sor__;
    QVBoxLayout *vboxLayout_solveMode__;
    QGroupBox *groupBox_opf_omegaSor__;
    QHBoxLayout *hboxLayout_omegaSor__;

    QGroupBox *groupBox_opf_solveMode2__;
    QCheckBox *chechBox_opf_solveMode2_I__;
    QCheckBox *chechBox_opf_solveMode2_E__;
    QGridLayout *gridLayout_opf_solveMode2__;
    QLineEdit *lineedit_opf_solveMode_It__;
    QLineEdit *lineedit_opf_solveMode_Er__;


    //objects
    QGroupBox *groupbox_objectLists__;
    QGridLayout *gridlayout_opf_objects__;

        //ER
    QLabel *label_enemyRobot__;
    QLineEdit *lineedit_ER[3];
    QPushButton *pushbutton_ER_add__;
    QPushButton *pushbutton_ER_clear__;
    QPushButton *pushbutton_ER_recycle__;

        //FO
    QLabel *label_fixedObstacle__;
    QLineEdit *lineedit_FO[3];
    QPushButton *pushbutton_FO_add__;
    QPushButton *pushbutton_FO_clear__;
    QPushButton *pushbutton_FO_recycle__;

        //MO
    QLabel *label_movableObstacle__;
    QLineEdit *lineedit_MO[3];
    QPushButton *pushbutton_MO_add__;
    QPushButton *pushbutton_MO_clear__;
    QPushButton *pushbutton_MO_recycle__;

        //G
    QLabel *label_goal__;
    QLineEdit *lineedit_G[3];
    QPushButton *pushbutton_G_add__;
    QPushButton *pushbutton_G_clear__;
    QPushButton *pushbutton_G_recycle__;


        //OR
    QLabel *label_ownRobot__;
    QLineEdit *lineedit_OR[3];
    QLineEdit *lineedit_OR_opf[4];
    QPushButton *pushbutton_OR_set__;
    QPushButton *pushbutton_OR_recycle__;
    QLabel *label_epsilon__;
    QLabel *label_v__;
    QGridLayout *gridlayout_ownRobot__;
    QGroupBox *groupbox_ownRobot__;



    //Locker
    QReadWriteLock *locker__;

};

#endif // MPL_MAINWINDOW_H
