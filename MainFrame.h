#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <vector>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    void CreateStartPanel();
    void CreateGamePanel();
    void CreateNameDialog();
    void ShowStartPanel();
    void ShowGamePanel();
    void ResetGame();
    void ComputerMove();
    void CheckWinner();
    void AnnounceWinner(int player);

    void OnVsComputerClicked(wxCommandEvent& event);
    void OnVsPlayerClicked(wxCommandEvent& event);
    void OnStartButtonClicked(wxCommandEvent& event);
    void OnGameButtonClicked(wxCommandEvent& event);
    void OnResetButtonClicked(wxCommandEvent& event);

    wxPanel* startPanel;
    wxPanel* gamePanel;
    wxButton* vsComputerButton;
    wxButton* vsPlayerButton;
    wxButton* buttons[9];
    wxButton* startButton;
    wxButton* resetButton;
    wxStaticText* statusText;

    int board[9];
    bool currentPlayer;
    bool gameEnded;
    bool gameAgainstComputer;

    wxString player1Name;
    wxString player2Name;

    wxDECLARE_EVENT_TABLE();
};

#endif