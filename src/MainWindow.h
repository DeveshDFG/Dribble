#pragma once

#include <QMainWindow>
#include <QString>

class QComboBox;
class QLabel;
class QPushButton;
class QStackedWidget;
class QWidget;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QWidget* createAgentConfigPage();
    QWidget* createGitHubLoginPage();
    QWidget* createProjectSetupPage();
    QWidget* createAgentCard(const QString& title, const QString& description, QComboBox*& comboBox);
    void applyTheme();
    void goNext();
    void goBack();
    void updateNavigation();
    void selectProjectFolder();

    QStackedWidget* m_pages = nullptr;
    QPushButton* m_backButton = nullptr;
    QPushButton* m_nextButton = nullptr;
    QComboBox* m_codeAgentCombo = nullptr;
    QComboBox* m_searchAgentCombo = nullptr;
    QComboBox* m_reviewAgentCombo = nullptr;
    QLabel* m_projectPathLabel = nullptr;
    QString m_projectPath;
};
