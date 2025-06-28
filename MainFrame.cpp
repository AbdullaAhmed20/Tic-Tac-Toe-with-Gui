#include "MainFrame.h"
#include <wx/colour.h>
#include <random>


MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title), currentPlayer(true), gameEnded(false),
   
    player1Name("Player X"), player2Name("Player O") {
    SetSize(600, 550);
    
    // Initialize board
    for (int i = 0; i < 9; i++) {
        board[i] = 0;
    }

    // Set background color
    SetBackgroundColour(wxColour(50, 50, 70));

    CreateStartPanel();
    CreateGamePanel();

    ShowStartPanel();
}

void MainFrame::CreateStartPanel() {
    startPanel = new wxPanel(this, wxID_ANY);
    startPanel->SetBackgroundColour(wxColour(70, 70, 90));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* title = new wxStaticText(startPanel, wxID_ANY, "Tic-Tac-Toe");
    title->SetForegroundColour(wxColour(255, 255, 255));
    title->SetFont(wxFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    vsComputerButton = new wxButton(startPanel, wxID_ANY, "Play vs Computer");
    vsPlayerButton = new wxButton(startPanel, wxID_ANY, "Play vs Player");

    vsComputerButton->SetBackgroundColour(wxColour(100, 150, 200));
    vsComputerButton->SetForegroundColour(wxColour(255, 255, 255));
    vsPlayerButton->SetBackgroundColour(wxColour(200, 150, 100));
    vsPlayerButton->SetForegroundColour(wxColour(255, 255, 255));

    vsComputerButton->Bind(wxEVT_BUTTON, &MainFrame::OnVsComputerClicked, this);
    vsPlayerButton->Bind(wxEVT_BUTTON, &MainFrame::OnVsPlayerClicked, this);

    sizer->Add(title, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
    sizer->Add(vsComputerButton, 0, wxALIGN_CENTER | wxALL, 20);
    sizer->Add(vsPlayerButton, 0, wxALIGN_CENTER | wxALL, 20);

    startPanel->SetSizerAndFit(sizer);
}

void MainFrame::CreateGamePanel() {
    gamePanel = new wxPanel(this, wxID_ANY);
    gamePanel->SetBackgroundColour(wxColour(60, 60, 80));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    statusText = new wxStaticText(gamePanel, wxID_ANY, player1Name + "'s Turn");
    statusText->SetForegroundColour(wxColour(255, 255, 255));
    statusText->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    wxGridSizer* gridSizer = new wxGridSizer(3, 3, 5, 5);

    for (int i = 0; i < 9; i++) {
        buttons[i] = new wxButton(gamePanel, wxID_ANY, "", wxDefaultPosition, wxSize(100, 100));
        buttons[i]->SetFont(wxFont(36, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        buttons[i]->Bind(wxEVT_BUTTON, &MainFrame::OnGameButtonClicked, this);
        buttons[i]->SetBackgroundColour(wxColour(80, 80, 100));
        buttons[i]->SetForegroundColour(wxColour(255, 255, 255));
        gridSizer->Add(buttons[i], 0, wxEXPAND);
    }

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    startButton = new wxButton(gamePanel, wxID_ANY, "Back to Menu");
    startButton->SetBackgroundColour(wxColour(120, 120, 140));
    startButton->SetForegroundColour(wxColour(255, 255, 255));
    startButton->Bind(wxEVT_BUTTON, &MainFrame::OnStartButtonClicked, this);

    resetButton = new wxButton(gamePanel, wxID_ANY, "Reset Game");
    resetButton->SetBackgroundColour(wxColour(140, 120, 120));
    resetButton->SetForegroundColour(wxColour(255, 255, 255));
    resetButton->Bind(wxEVT_BUTTON, &MainFrame::OnResetButtonClicked, this);

    buttonSizer->Add(startButton, 0, wxALL, 10);
    buttonSizer->Add(resetButton, 0, wxALL, 10);

    mainSizer->Add(statusText, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 20);
    mainSizer->Add(gridSizer, 1, wxALIGN_CENTER | wxALL, 10);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    gamePanel->SetSizerAndFit(mainSizer);
}

void MainFrame::OnVsComputerClicked(wxCommandEvent& event) {
    wxTextEntryDialog dialog(this, "Enter your name:", "Player Name", "Player X");
    if (dialog.ShowModal() == wxID_OK) {
        player1Name = dialog.GetValue();
        if (player1Name.empty()) player1Name = "Player X";
        player2Name = "Computer";
        gameAgainstComputer = true;
        ResetGame();
        ShowGamePanel();
    }
}

void MainFrame::OnVsPlayerClicked(wxCommandEvent& event) {
    wxTextEntryDialog dialog1(this, "Enter first player name:", "Player Name", "Player X");
    wxTextEntryDialog dialog2(this, "Enter second player name:", "Player Name", "Player O");

    if (dialog1.ShowModal() == wxID_OK && dialog2.ShowModal() == wxID_OK) {
        player1Name = dialog1.GetValue();
        if (player1Name.empty()) player1Name = "Player X";
        player2Name = dialog2.GetValue();
        if (player2Name.empty()) player2Name = "Player O";
        gameAgainstComputer = false;
        ResetGame();
        ShowGamePanel();
    }
}

void MainFrame::OnStartButtonClicked(wxCommandEvent& event) {
    ShowStartPanel();
}

void MainFrame::OnResetButtonClicked(wxCommandEvent& event) {
    ResetGame();
}

void MainFrame::OnGameButtonClicked(wxCommandEvent& event) {
    if (gameEnded) return;

    wxButton* clickedButton = dynamic_cast<wxButton*>(event.GetEventObject());
    if (!clickedButton) return;

    for (int i = 0; i < 9; i++) {
        if (buttons[i] == clickedButton && board[i] == 0) {
            if (currentPlayer) {
                buttons[i]->SetLabel("X");
                buttons[i]->SetForegroundColour(wxColour(100, 200, 255));
                board[i] = 1;
                statusText->SetLabel(player2Name + "'s Turn");
            }
            else {
                buttons[i]->SetLabel("O");
                buttons[i]->SetForegroundColour(wxColour(255, 150, 100));
                board[i] = 2;
                statusText->SetLabel(player1Name + "'s Turn");
            }

            currentPlayer = !currentPlayer;
            CheckWinner();

            if (!gameEnded && gameAgainstComputer && !currentPlayer) {
                ComputerMove();
            }

            break;
        }
    }
}

void MainFrame::ComputerMove() {
    std::vector<int> emptyCells;
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) {
            emptyCells.push_back(i);
        }
    }

    if (!emptyCells.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, emptyCells.size() - 1);
        int move = emptyCells[dist(gen)];

        wxCommandEvent event;
        event.SetEventObject(buttons[move]);
        OnGameButtonClicked(event);
    }
}

void MainFrame::CheckWinner() {
    // Check rows
    for (int i = 0; i < 9; i += 3) {
        if (board[i] != 0 && board[i] == board[i + 1] && board[i] == board[i + 2]) {
            AnnounceWinner(board[i]);
            return;
        }
    }

    // Check columns
    for (int i = 0; i < 3; i++) {
        if (board[i] != 0 && board[i] == board[i + 3] && board[i] == board[i + 6]) {
            AnnounceWinner(board[i]);
            return;
        }
    }

    // Check diagonals
    if (board[0] != 0 && board[0] == board[4] && board[0] == board[8]) {
        AnnounceWinner(board[0]);
        return;
    }

    if (board[2] != 0 && board[2] == board[4] && board[2] == board[6]) {
        AnnounceWinner(board[2]);
        return;
    }

    // Check for draw
    bool draw = true;
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) {
            draw = false;
            break;
        }
    }

    if (draw) {
        statusText->SetLabel("It's a Draw!");
        gameEnded = true;
    }
}

void MainFrame::AnnounceWinner(int player) {
    gameEnded = true;

    if (player == 1) {
        statusText->SetLabel(player1Name + " Wins!");
    }
    else {
        statusText->SetLabel(player2Name + " Wins!");
    }
}

void MainFrame::ResetGame() {
    currentPlayer = true;
    gameEnded = false;

    for (int i = 0; i < 9; i++) {
        board[i] = 0;
        buttons[i]->SetLabel("");
    }

    statusText->SetLabel(player1Name + "'s Turn");
}

void MainFrame::ShowStartPanel() {
    if (gamePanel->IsShown()) {
        gamePanel->Hide();
    }
    startPanel->Show();
    Layout();
}

void MainFrame::ShowGamePanel() {
    if (startPanel->IsShown()) {
        startPanel->Hide();
    }
    gamePanel->Show();
    Layout();
}

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(wxID_ANY, MainFrame::OnGameButtonClicked)
EVT_BUTTON(wxID_ANY, MainFrame::OnStartButtonClicked)
EVT_BUTTON(wxID_ANY, MainFrame::OnResetButtonClicked)
EVT_BUTTON(wxID_ANY, MainFrame::OnVsComputerClicked)
EVT_BUTTON(wxID_ANY, MainFrame::OnVsPlayerClicked)
wxEND_EVENT_TABLE()