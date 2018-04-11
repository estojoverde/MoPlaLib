#ifndef MPL_TESTSUITE_H
#define MPL_TESTSUITE_H


#include <stdio.h>
#include <sys/time.h>

#include <QObject>
#include <QString>
#include <QListWidget>
#include <QMainWindow>


#include "mpl_utils.h"
#include "mpl_position.h"
#include "mpl_baseenvironment.h"
#include "mpl_gridenvironment.h"
#include "mpl_potentialfieldenvironment.h"
#include "mpl_harmonicpotentialfield.h"
#include "mpl_orientedpotentialfield.h"


#define MPL_TEST_LOG_MESSAGE2(output, fmt, ...) fprintf(output, ("\n[LOG][%s:%d( %s )]\n---  " fmt "  ---\n\n"), __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__) //log message

#define MPL_TEST_LOG_MESSAGE(fmt, ...) MPL_TEST_LOG_MESSAGE2(fp_logfile, fmt, ##__VA_ARGS__)

#define MPL_TEST_RESULT_MESSAGE2(output, fmt, ...) fprintf(output, ("\n---  " fmt "  ---\n\n"),  ##__VA_ARGS__) //result message

#define MPL_TEST_RESULT_MESSAGE(fmt, ...) MPL_TEST_RESULT_MESSAGE2(fp_resultsfile, fmt, ##__VA_ARGS__)




class MPL_TestSuite : public QMainWindow
{
    Q_OBJECT
public:
    explicit MPL_TestSuite(QWidget *parent = 0);
    ~MPL_TestSuite();

    void testPosition();
    int testBase(void);
    int testGrid(void);
    int testHPF(char *s_filename);
    int testOPF(char *s_filename);

signals:
    void newLogMessageSignal( QString s );
    void signal_done();

public slots:
    void newLogMessage(QString s){
        QLW_log->addItem(s);
    }


private:

    QListWidget *QLW_log;
};

#endif // MPL_TESTSUITE_H
