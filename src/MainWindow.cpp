#include "MainWindow.h"

#include <QComboBox>
#include <QFileDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("AgenticWorkspace Orchestrator");
    resize(960, 640);

    auto* root = new QWidget(this);
    auto* rootLayout = new QVBoxLayout(root);
    rootLayout->setContentsMargins(32, 28, 32, 24);
    rootLayout->setSpacing(24);

    auto* header = new QWidget(root);
    auto* headerLayout = new QVBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(6);

    auto* title = new QLabel("Setup Wizard", header);
    title->setObjectName("wizardTitle");
    auto* subtitle = new QLabel("Configure the manager, connect services, and choose the workspace.", header);
    subtitle->setObjectName("wizardSubtitle");
    headerLayout->addWidget(title);
    headerLayout->addWidget(subtitle);

    m_pages = new QStackedWidget(root);
    m_pages->addWidget(createAgentConfigPage());
    m_pages->addWidget(createGitHubLoginPage());
    m_pages->addWidget(createProjectSetupPage());

    auto* navigation = new QWidget(root);
    auto* navigationLayout = new QHBoxLayout(navigation);
    navigationLayout->setContentsMargins(0, 0, 0, 0);
    navigationLayout->addStretch();

    m_backButton = new QPushButton("Back", navigation);
    m_nextButton = new QPushButton("Next", navigation);
    m_backButton->setObjectName("secondaryButton");
    m_nextButton->setObjectName("primaryButton");
    navigationLayout->addWidget(m_backButton);
    navigationLayout->addWidget(m_nextButton);

    rootLayout->addWidget(header);
    rootLayout->addWidget(m_pages, 1);
    rootLayout->addWidget(navigation);

    setCentralWidget(root);
    applyTheme();

    connect(m_backButton, &QPushButton::clicked, this, &MainWindow::goBack);
    connect(m_nextButton, &QPushButton::clicked, this, &MainWindow::goNext);
    connect(m_pages, &QStackedWidget::currentChanged, this, &MainWindow::updateNavigation);
    connect(m_codeAgentCombo, &QComboBox::currentIndexChanged, this, &MainWindow::updateNavigation);

    updateNavigation();
}

QWidget* MainWindow::createAgentConfigPage()
{
    auto* page = new QWidget(this);
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(20);

    auto* heading = new QLabel("Agent Config", page);
    heading->setObjectName("pageTitle");
    layout->addWidget(heading);

    auto* cards = new QWidget(page);
    auto* cardsLayout = new QHBoxLayout(cards);
    cardsLayout->setContentsMargins(0, 0, 0, 0);
    cardsLayout->setSpacing(18);

    cardsLayout->addWidget(createAgentCard("Code", "Required worker for file edits, command execution, and implementation.", m_codeAgentCombo));
    cardsLayout->addWidget(createAgentCard("Search", "Optional worker for live documentation and current issue research.", m_searchAgentCombo));
    cardsLayout->addWidget(createAgentCard("Review", "Optional worker for security, performance, and style critique.", m_reviewAgentCombo));

    layout->addWidget(cards);
    layout->addStretch();

    return page;
}

QWidget* MainWindow::createGitHubLoginPage()
{
    auto* page = new QWidget(this);
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(18);

    auto* heading = new QLabel("GitHub Login", page);
    heading->setObjectName("pageTitle");

    auto* panel = new QFrame(page);
    panel->setObjectName("panel");
    auto* panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(28, 28, 28, 28);
    panelLayout->setSpacing(16);

    auto* text = new QLabel("GitHub integration will be connected here for repository access and automation.", panel);
    text->setObjectName("bodyText");
    text->setWordWrap(true);

    auto* loginButton = new QPushButton("Login", panel);
    loginButton->setObjectName("primaryButton");
    loginButton->setFixedWidth(140);

    panelLayout->addWidget(text);
    panelLayout->addWidget(loginButton, 0, Qt::AlignLeft);
    panelLayout->addStretch();

    layout->addWidget(heading);
    layout->addWidget(panel, 1);

    connect(loginButton, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, "GitHub Login", "GitHub OAuth will be implemented in a later step.");
    });

    return page;
}

QWidget* MainWindow::createProjectSetupPage()
{
    auto* page = new QWidget(this);
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(18);

    auto* heading = new QLabel("Project Setup", page);
    heading->setObjectName("pageTitle");

    auto* panel = new QFrame(page);
    panel->setObjectName("panel");
    auto* panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(28, 28, 28, 28);
    panelLayout->setSpacing(16);

    auto* text = new QLabel("Select the codebase folder that the manager and worker agents will share.", panel);
    text->setObjectName("bodyText");
    text->setWordWrap(true);

    auto* chooseButton = new QPushButton("Select Folder", panel);
    chooseButton->setObjectName("primaryButton");
    chooseButton->setFixedWidth(160);

    m_projectPathLabel = new QLabel("No folder selected", panel);
    m_projectPathLabel->setObjectName("pathLabel");
    m_projectPathLabel->setWordWrap(true);

    panelLayout->addWidget(text);
    panelLayout->addWidget(chooseButton, 0, Qt::AlignLeft);
    panelLayout->addWidget(m_projectPathLabel);
    panelLayout->addStretch();

    layout->addWidget(heading);
    layout->addWidget(panel, 1);

    connect(chooseButton, &QPushButton::clicked, this, &MainWindow::selectProjectFolder);

    return page;
}

QWidget* MainWindow::createAgentCard(const QString& title, const QString& description, QComboBox*& comboBox)
{
    auto* card = new QFrame(this);
    card->setObjectName("agentCard");
    auto* layout = new QVBoxLayout(card);
    layout->setContentsMargins(22, 22, 22, 22);
    layout->setSpacing(14);

    auto* titleLabel = new QLabel(title, card);
    titleLabel->setObjectName("cardTitle");

    auto* descriptionLabel = new QLabel(description, card);
    descriptionLabel->setObjectName("bodyText");
    descriptionLabel->setWordWrap(true);

    comboBox = new QComboBox(card);
    comboBox->addItem("Not selected");
    comboBox->addItem("Local");
    comboBox->addItem("API");

    layout->addWidget(titleLabel);
    layout->addWidget(descriptionLabel);
    layout->addStretch();
    layout->addWidget(comboBox);

    return card;
}

void MainWindow::applyTheme()
{
    setStyleSheet(R"(
        QMainWindow {
            background: #111318;
            color: #eef1f6;
            font-family: "Segoe UI", Arial, sans-serif;
            font-size: 14px;
        }

        QWidget {
            color: #eef1f6;
            font-family: "Segoe UI", Arial, sans-serif;
            font-size: 14px;
        }

        QLabel {
            background: transparent;
        }

        QLabel#wizardTitle {
            font-size: 30px;
            font-weight: 700;
        }

        QLabel#wizardSubtitle, QLabel#bodyText {
            color: #aeb7c6;
        }

        QLabel#pageTitle {
            font-size: 22px;
            font-weight: 650;
        }

        QLabel#cardTitle {
            font-size: 18px;
            font-weight: 650;
        }

        QLabel#pathLabel {
            color: #d7dde8;
            padding-top: 8px;
        }

        QFrame#agentCard, QFrame#panel {
            background: #1a1f29;
            border: 1px solid #2d3544;
            border-radius: 8px;
        }

        QComboBox {
            background: #0f1218;
            border: 1px solid #3a4658;
            border-radius: 6px;
            color: #eef1f6;
            min-height: 36px;
            padding: 0 12px;
        }

        QComboBox::drop-down {
            border: 0;
            width: 28px;
        }

        QPushButton {
            border-radius: 6px;
            font-weight: 600;
            min-height: 38px;
            padding: 0 18px;
        }

        QPushButton#primaryButton {
            background: #3777ff;
            border: 1px solid #4b86ff;
            color: white;
        }

        QPushButton#primaryButton:hover {
            background: #4b86ff;
        }

        QPushButton#secondaryButton {
            background: #202633;
            border: 1px solid #3a4658;
            color: #eef1f6;
        }

        QPushButton:disabled {
            background: #252a34;
            border-color: #303746;
            color: #697386;
        }
    )");
}

void MainWindow::goNext()
{
    if (m_pages->currentIndex() == 0 && m_codeAgentCombo->currentIndex() == 0) {
        QMessageBox::warning(this, "Code Agent Required", "Select a Code agent before continuing.");
        return;
    }

    if (m_pages->currentIndex() < m_pages->count() - 1) {
        m_pages->setCurrentIndex(m_pages->currentIndex() + 1);
        return;
    }

    QMessageBox::information(this, "Setup Complete", "Initial setup is ready.");
}

void MainWindow::goBack()
{
    if (m_pages->currentIndex() > 0) {
        m_pages->setCurrentIndex(m_pages->currentIndex() - 1);
    }
}

void MainWindow::updateNavigation()
{
    const int pageIndex = m_pages->currentIndex();
    m_backButton->setEnabled(pageIndex > 0);
    m_nextButton->setText(pageIndex == m_pages->count() - 1 ? "Finish" : "Next");
    m_nextButton->setEnabled(pageIndex != 0 || m_codeAgentCombo->currentIndex() > 0);
}

void MainWindow::selectProjectFolder()
{
    const QString folder = QFileDialog::getExistingDirectory(this, "Select Project Folder", m_projectPath);
    if (folder.isEmpty()) {
        return;
    }

    m_projectPath = folder;
    m_projectPathLabel->setText(folder);
}
