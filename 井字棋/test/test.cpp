#include <graphics.h>

char board[3][3] = 
{
    {'-', '-', '-'},
    {'-', '-', '-'},
    {'-', '-', '-'}
};

char currentPlayer = 'X';

bool checkWin(char c)
{
    if (board[0][0] == c && board[0][1] == c && board[0][2] == c)
        return true;
    if (board[1][0] == c && board[1][1] == c && board[1][2] == c)
        return true;
    if (board[2][0] == c && board[2][1] == c && board[2][2] == c)
        return true;
    if (board[0][0] == c && board[1][0] == c && board[2][0] == c)
        return true;
    if (board[0][1] == c && board[1][1] == c && board[2][1] == c)
        return true;
    if (board[0][2] == c && board[1][2] == c && board[2][2] == c)
        return true;
    if (board[0][0] == c && board[1][1] == c && board[2][2] == c)
        return true;
    if (board[0][2] == c && board[1][1] == c && board[2][0] == c)
        return true;

    return false;
}

bool checkDraw()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == '-')
                return false;

    return true;
}

void drawBoard()
{
    line(0, 200, 600, 200);
    line(0, 400, 600, 400);
    line(200, 0, 200, 600);
    line(400, 0, 400, 600);
}

void drawPiece()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            switch (board[i][j])
            {
            case 'X':
                line(200 * j, 200 * i, 200 * (j + 1), 200 * (i + 1));
                line(200 * (j + 1), 200 * i, 200 * j, 200 * (i + 1));
                break;
            case 'O':
                circle(200 * j + 100, 200 * i + 100, 100);
                break;
            case '-':
                break;
            }
        }
    }
}

void drawTips()
{
    static TCHAR tips[64];
    _stprintf_s(tips, _T("Player %c's turn"), currentPlayer);
    settextcolor(YELLOW);
    outtextxy(0, 0, tips);
}

int main()
{
    initgraph(600, 600);

    bool run = true;

    ExMessage msg;

    BeginBatchDraw();

    while (run) 
    {
        DWORD startTime = GetTickCount();

        while (peekmessage(&msg))
        {
            if (msg.message == WM_LBUTTONDOWN)
            {
                int x = msg.x;
                int y = msg.y;

                int index_x = x / 200;
                int index_y = y / 200;

                if (board[index_y][index_x] == '-')
                {
                    board[index_y][index_x] = currentPlayer;

                    if (currentPlayer == 'X')
                        currentPlayer = 'O';
                    else
                        currentPlayer = 'X';
                }
            }
        }
        cleardevice();

        drawBoard();
        drawPiece();
        drawTips();

        FlushBatchDraw();

        if (checkWin('X'))
        {
            MessageBox(GetHWnd(), _T("X wins!"), _T("Game Over"), MB_OK);
            run = false;
        }
        else if (checkWin('O'))
        {
            MessageBox(GetHWnd(), _T("O wins!"), _T("Game Over"), MB_OK);
            run = false;
        }
        else if (checkDraw())
        {
            MessageBox(GetHWnd(), _T("Draw!"), _T("Game Over"), MB_OK);
            run = false;
        }
        DWORD endTime = GetTickCount();
        DWORD deltaTime = endTime - startTime;
        if (deltaTime < 1000 / 60)
        {
            Sleep(1000 / 60 - deltaTime);
        }
    }

    EndBatchDraw();
    closegraph();

    return 0;
}