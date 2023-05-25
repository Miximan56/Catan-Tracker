#include "widget.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    widget w;
    w.show();
    
    return app.exec();
}

// 5 zile // ~12 ore (prima zi) ~14 ore (a doua zi) ~10 ore (a 3 a zi) ~12 ore (a 4 a zi) ~6 ore (a 5 a zi)

/*int main()
{
    Board board;
    std::vector<Player> players;

    clear();
    std::cout << "Board code: ";
    board.initialize();

    std::cout << "Player Number: ";
    int playerNum; std::cin >> playerNum;
    
    for (int i = 0; i < playerNum; i++)
    {
        std::cout << "Name #" << i + 1 << ' ';
        std::string name; std::cin >> name;
        players.push_back(Player(name));
    }

    int me, currentPlayer;
    std::cout << "Your number: "; std::cin >> me;
    std::cout << "First player: "; std::cin >> currentPlayer;
    currentPlayer--;
    me--;

    clear();

    for (int i = 0; i < players.size(); i++)
    {
        std::cout << players[currentPlayer].name << "'s settlement(n, spaces...): ";
        int n; std::cin >> n;
        for (int j = 0; j < n; j++)
        {
            int m; std::cin >> m;
            players[currentPlayer].spacesOwned[--m]++;
        }

        if (i != players.size() - 1) currentPlayer = (currentPlayer + 1) % players.size();
    }
    for (int i = 0; i < players.size(); i++)
    {
        std::cout << players[currentPlayer].name << "'s settlement(n, spaces...): ";
        int n; std::cin >> n;
        for (int j = 0; j < n; j++)
        {
            int m; std::cin >> m;
            players[currentPlayer].spacesOwned[--m]++;
            players[currentPlayer].getResource(m, 1, board);
        }

        if (i != players.size() - 1)
        {
            currentPlayer--;
            if (currentPlayer < 0) currentPlayer = players.size() - 1;
        }
    }

    updateUndoStack(players, board, currentPlayer);
    while (true)
    {
        bool wantsToUndo = false;
        while (players[currentPlayer].devCards > 0)
        {
            roundInfo(players, currentPlayer, me);
            std::cout << "1. Thief\n";
            std::cout << "2. Dice\n";
            std::cout << "3. Year of plenty\n";
            std::cout << "4. Do monopol\n";
            std::cout << "5. Other dev card\n";
            std::cout << "6. Trade\n";
            std::cout << "R: ";
            int r; std::cin >> r;
            while (r < 1 || r > 6)
            {
                std::cout << "Invalid response! Try again!\n";
                std::cout << "R: "; std::cin >> r;
            }

            if (r == 1)
            {
                roundInfo(players, currentPlayer, me);
                std::cout << "Thief\n";
                players[currentPlayer].devCards--;

                int space;
                std::cout << "Space: "; std::cin >> space;
                while (space < 1 || space > 19)
                {
                    std::cout << "Space does not exist! Try again!\n";
                    std::cout << "Space: "; std::cin >> space;
                }
                space--;

                std::vector<int> playersOnSpace;
                for (int i = 0; i < players.size(); i++)
                    playersOnSpace.push_back((players[i].spacesOwned[space] > 0) ? 1 : 0);

                if (std::accumulate(playersOnSpace.begin(), playersOnSpace.end(), 0) > 1)
                {
                    int s{};
                    for (int i = 0; i < players.size(); i++)
                    {
                        if (i == currentPlayer) continue;
                        if (playersOnSpace[i])
                        {
                            for (int j = 0; j < 5; j++)
                                if (players[i].resourceCombinations.front()[j])
                                {
                                    s++;
                                    break;
                                }
                        }
                    }
                    if (s > 1)
                    {
                        std::cout << "Player who got stolen from: ";
                        int stolenPlayer; std::cin >> stolenPlayer;
                        while (stolenPlayer < 1 || stolenPlayer > players.size() || stolenPlayer - 1 == currentPlayer || !playersOnSpace[stolenPlayer - 1])
                        {
                            std::cout << "Invalid answer! Try again!\n";
                            std::cout << "Player who got stolen from: ";
                            std::cin >> stolenPlayer;
                        }
                        stolenPlayer--;
                        thief(players[currentPlayer], players[stolenPlayer], board, space);
                    }
                    else if (s)
                    {
                        for (int i = 0; i < players.size(); i++)
                        {
                            if (i == currentPlayer) continue;
                            if (playersOnSpace[i])
                            {
                                int s{};
                                for (int j = 0; j < 5; j++)
                                    s += players[i].resourceCombinations.front()[j];
                                if (!s)
                                {
                                    for (auto& var : board.space)
                                        std::get<2>(var) = false;
                                    std::get<2>(board.space[space]) = true;
                                    break;
                                }
                                thief(players[currentPlayer], players[i], board, space);
                                break;
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < 19; i++)
                            std::get<2>(board.space[i]) = false;
                        std::get<2>(board.space[space]) = true;
                    }
                }
                else if (std::accumulate(playersOnSpace.begin(), playersOnSpace.end(), 0))
                {
                    for (int i = 0; i < players.size(); i++)
                    {
                        if (i == currentPlayer) continue;
                        if (playersOnSpace[i])
                        {
                            int s{};
                            for (int j = 0; j < 5; j++)
                                s += players[i].resourceCombinations.front()[j];
                            if (!s)
                            {
                                for (auto& var : board.space)
                                    std::get<2>(var) = false;
                                std::get<2>(board.space[space]) = true;
                                break;
                            }
                            thief(players[currentPlayer], players[i], board, space);
                            break;
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < 19; i++)
                        std::get<2>(board.space[i]) = false;
                    std::get<2>(board.space[space]) = true;
                }
            }
            else if (r == 3)
            {
                yearOfPlenty(players[currentPlayer]);
            }
            else if (r == 4)
            {
                roundInfo(players, currentPlayer, me);
                std::cout << "Monopol\n";
                std::cout << "0. Lumber\n";
                std::cout << "1. Brick\n";
                std::cout << "2. Wool\n";
                std::cout << "3. Grain\n";
                std::cout << "4. Ore\n";
                std::cout << "Resource: ";
                int res; std::cin >> res;
                while (res < 0 || res > 4)
                {
                    std::cout << "Invalid response! Try again!\n";
                    std::cout << "Resource: "; std::cin >> res;
                }

                std::vector<int> amounts;
                for (int i = 0; i < players.size(); i++)
                {
                    if (i == currentPlayer) continue;

                    std::cout << players[i].name << " gave: ";
                    int amount; std::cin >> amount;
                    while (amount < 0)
                    {
                        std::cout << "Invalid answer! Try again!\n";
                        std::cout << players[i].name << " gave: ";
                        std::cin >> amount;
                    }
                    amounts.push_back(amount);
                }

                doMonopol(players, currentPlayer, res, amounts);
            }
            else if (r == 5)
                spendDevOnElse(players[currentPlayer]);
            else if (r == 6)
            {
                if (players[currentPlayer].couldTradeBank())
                {
                    int r;
                    std::cout << "1. Trade player\n";
                    std::cout << "2. Trade bank\n";
                    std::cout << "R: "; std::cin >> r;
                    while (r < 1 || r > 2)
                    {
                        std::cout << "Invalid response! Try again!\n";
                        std::cout << "R: "; std::cin >> r;
                    }
                    if (r == 2)
                    {
                        roundInfo(players, currentPlayer, me);
                        std::cout << "Trade bank\n";
                        std::cout << players[currentPlayer].name << "'s trade (L, B, W, G, O): ";
                        int playerTrade[5], bankTrade[5];
                        for (int i = 0; i < 5; i++)
                            std::cin >> playerTrade[i];
                        while (playerTrade[0] < 0 || playerTrade[1] < 0 || playerTrade[2] < 0 || playerTrade[3] < 0 || playerTrade[4] < 0)
                        {
                            std::cout << "Invalid trade! Try again!\n";
                            std::cout << players[currentPlayer].name << "'s trade (L, B, W, G, O): ";
                            for (int i = 0; i < 5; i++)
                                std::cin >> playerTrade[i];
                        }
                        std::cout << "Bank's trade: ";
                        for (int i = 0; i < 5; i++)
                            std::cin >> bankTrade[i];
                        while (bankTrade[0] < 0 || bankTrade[1] < 0 || bankTrade[2] < 0 || bankTrade[3] < 0 || bankTrade[4] < 0)
                        {
                            std::cout << "Invalid trade! Try again!\n";
                            std::cout << "Bank's trade (L, B, W, G, O): ";
                            for (int i = 0; i < 5; i++)
                                std::cin >> bankTrade[i];
                        }

                        tradeBank(players[currentPlayer], Resources(playerTrade[0], playerTrade[1], playerTrade[2], playerTrade[3], playerTrade[4]),
                            Resources(bankTrade[0], bankTrade[1], bankTrade[2], bankTrade[3], bankTrade[4]));
                    }
                    else goto maiJos;
                }
                else
                {
                maiJos:
                    roundInfo(players, currentPlayer, me);
                    std::cout << "Trade player\n";
                    std::cout << "Player number: ";
                    int playerNum; std::cin >> playerNum;
                    while (playerNum < 1 || playerNum > players.size() || playerNum - 1 == currentPlayer)
                    {
                        std::cout << "Invalid answer! Try again!\n";
                        std::cout << "Player number: "; std::cin >> playerNum;
                    }
                    playerNum--;

                    std::cout << players[currentPlayer].name << "'s trade (L, B, W, G, O): ";
                    int playerTrade1[5], player2Trade[5];
                    for (int i = 0; i < 5; i++)
                        std::cin >> playerTrade1[i];
                    while (playerTrade1[0] < 0 || playerTrade1[1] < 0 || playerTrade1[2] < 0 || playerTrade1[3] < 0 || playerTrade1[4] < 0)
                    {
                        std::cout << "Invalid trade! Try again!\n";
                        std::cout << players[currentPlayer].name << "'s trade (L, B, W, G, O): ";
                        for (int i = 0; i < 5; i++)
                            std::cin >> playerTrade1[i];
                    }

                    std::cout << players[playerNum].name << "'s trade (L, B, W, G, O): ";
                    for (int i = 0; i < 5; i++)
                        std::cin >> player2Trade[i];
                    while (player2Trade[0] < 0 || player2Trade[1] < 0 || player2Trade[2] < 0 || player2Trade[3] < 0 || player2Trade[4] < 0)
                    {
                        std::cout << "Invalid trade! Try again!\n";
                        std::cout << players[playerNum].name << "'s trade (L, B, W, G, O): ";
                        for (int i = 0; i < 5; i++)
                            std::cin >> player2Trade[i];
                    }

                    trade(players[currentPlayer], players[playerNum], Resources(playerTrade1[0], playerTrade1[1], playerTrade1[2], playerTrade1[3], playerTrade1[4]),
                        Resources(player2Trade[0], player2Trade[1], player2Trade[2], player2Trade[3], player2Trade[4]));
                }
            }
            else break;
        }

        roundInfo(players, currentPlayer, me);
        int dice = dices();

        if (dice == 7)
        {
            roundInfo(players, currentPlayer, me);
            std::cout << "Thief\n";

            for (auto& var : players)
            {
                int num{};
                for (int i = 0; i < 5; i++)
                    num += var.resourceCombinations.front()[i];
                if (num > 7)
                {
                    discard(var);
                    roundInfo(players, currentPlayer, me);
                    std::cout << "Theif\n";
                }
            }

            int space;
            std::cout << "Space: "; std::cin >> space;
            while (space < 1 || space > 19)
            {
                std::cout << "Space does not exist! Try again!\n";
                std::cout << "Space: "; std::cin >> space;
            }
            space--;

            std::vector<int> playersOnSpace;
            for (int i = 0; i < players.size(); i++)
                playersOnSpace.push_back((players[i].spacesOwned[space] > 0) ? 1 : 0);

            if (std::accumulate(playersOnSpace.begin(), playersOnSpace.end(), 0) > 1)
            {
                int s{};
                for (int i = 0; i < players.size(); i++)
                {
                    if (i == currentPlayer) continue;
                    if (playersOnSpace[i])
                    {
                        for (int j = 0; j < 5; j++)
                            if (players[i].resourceCombinations.front()[j])
                            {
                                s++;
                                break;
                            }
                    }
                }
                if (s > 1)
                {
                    std::cout << "Player who got stolen from: ";
                    int stolenPlayer; std::cin >> stolenPlayer;
                    while (stolenPlayer < 1 || stolenPlayer > players.size() || stolenPlayer - 1 == currentPlayer || !playersOnSpace[stolenPlayer - 1])
                    {
                        std::cout << "Invalid answer! Try again!\n";
                        std::cout << "Player who got stolen from: ";
                        std::cin >> stolenPlayer;
                    }
                    stolenPlayer--;
                    thief(players[currentPlayer], players[stolenPlayer], board, space);
                }
                else if (s)
                {
                    for (int i = 0; i < players.size(); i++)
                    {
                        if (i == currentPlayer) continue;
                        if (playersOnSpace[i])
                        {
                            int s{};
                            for (int j = 0; j < 5; j++)
                            {
                                s = ((players[i].resourceCombinations.front()[j]) ? 1 : 0);
                                if (s) break;
                            }
                            if (!s)
                            {
                                for (auto& var : board.space)
                                    std::get<2>(var) = false;
                                std::get<2>(board.space[space]) = true;
                                continue;
                            }
                            thief(players[currentPlayer], players[i], board, space);
                            break;
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < 19; i++)
                        std::get<2>(board.space[i]) = false;
                    std::get<2>(board.space[space]) = true;
                }
            }
            else if (std::accumulate(playersOnSpace.begin(), playersOnSpace.end(), 0))
            {
                for (int i = 0; i < players.size(); i++)
                {
                    if (i == currentPlayer) continue;
                    if (playersOnSpace[i])
                    {
                        int s{};
                        for (int j = 0; j < 5; j++)
                            s += players[i].resourceCombinations.front()[j];
                        if (!s)
                        {
                            for (auto& var : board.space)
                                std::get<2>(var) = false;
                            std::get<2>(board.space[space]) = true;
                            break;
                        }
                        thief(players[currentPlayer], players[i], board, space);
                        break;
                    }
                }
            }
            else
            {
                for (int i = 0; i < 19; i++)
                    std::get<2>(board.space[i]) = false;
                std::get<2>(board.space[space]) = true;
            }
        } // La fel ca mai sus
        else
        {
            std::vector<int> spaces;
            for (int i = 0, num = 2; i < 19 && num; i++)
            {
                if (std::get<0>(board.space[i]) == dice)
                {
                    spaces.push_back(i);
                    if (std::get<0>(board.space[i]) == 2 || std::get<0>(board.space[i]) == 12)
                        break;
                    num--;
                }
            }
            for (auto var : spaces)
                for (auto& player : players)
                    player.getResource(var, player.spacesOwned[var], board);
        }

        int optiune = -1;
        while (optiune != 0)
        {
            roundInfo(players, currentPlayer, me);
            std::cout << "0. End of turn\n";
            if (players[currentPlayer].couldMakeRoad()) std::cout << "1. Make road\n";
            if (players[currentPlayer].couldMakeSettlement()) std::cout << "2. Make settlement\n";
            if (players[currentPlayer].couldMakeCity()) std::cout << "3. Make city\n";
            if (players[currentPlayer].couldBuyDevCard()) std::cout << "4. Buy development card\n";
            if (players[currentPlayer].couldDoMonopol()) std::cout << "5. Do monopol\n";
            if (players[currentPlayer].couldYearOfPlenty()) std::cout << "6. Year of plenty\n";
            if (players[currentPlayer].couldThief()) std::cout << "7. Use solider\n";
            if (players[currentPlayer].devCards) std::cout << "8. Other dev card\n";
            if (players[currentPlayer].couldTrade()) std::cout << "9. Trade\n";
            if (undoStack.size() > 1) std::cout << "10. Undo\n";

            std::cout << "Option: "; std::cin >> optiune;
            while (optiune < 0 || optiune > 10 || (optiune == 10 && undoStack.size() < 2))
            {
                std::cout << "Invalid option! Try again!\n";
                std::cout << "Option: "; std::cin >> optiune;
            }

            roundInfo(players, currentPlayer, me);
            switch (optiune)
            {
                case 10:
                {
                    undo(players, board, currentPlayer);
                    wantsToUndo = true;
                    break;
                }
                case 9:
                {
                    if (players[currentPlayer].couldTradeBank())
                    {
                        int r;
                        std::cout << "1. Trade player\n";
                        std::cout << "2. Trade bank\n";
                        std::cout << "R: "; std::cin >> r;
                        while (r < 1 || r > 2)
                        {
                            std::cout << "Invalid response! Try again!\n";
                            std::cout << "R: "; std::cin >> r;
                        }
                        if (r == 2)
                        {
                            roundInfo(players, currentPlayer, me);
                            std::cout << "Trade bank\n";
                            std::cout << players[currentPlayer].name << "'s trade (L, B, W, G, O): ";
                            int playerTrade[5], bankTrade[5];
                            for (int i = 0; i < 5; i++)
                                std::cin >> playerTrade[i];
                            while (playerTrade[0] < 0 || playerTrade[1] < 0 || playerTrade[2] < 0 || playerTrade[3] < 0 || playerTrade[4] < 0)
                            {
                                std::cout << "Invalid trade! Try again!\n";
                                std::cout << players[currentPlayer].name << "'s trade (L, B, W, G, O): ";
                                for (int i = 0; i < 5; i++)
                                    std::cin >> playerTrade[i];
                            }
                            std::cout << "Bank's trade: ";
                            for (int i = 0; i < 5; i++)
                                std::cin >> bankTrade[i];
                            while (bankTrade[0] < 0 || bankTrade[1] < 0 || bankTrade[2] < 0 || bankTrade[3] < 0 || bankTrade[4] < 0)
                            {
                                std::cout << "Invalid trade! Try again!\n";
                                std::cout << "Bank's trade (L, B, W, G, O): ";
                                for (int i = 0; i < 5; i++)
                                    std::cin >> bankTrade[i];
                            }

                            tradeBank(players[currentPlayer], Resources(playerTrade[0], playerTrade[1], playerTrade[2], playerTrade[3], playerTrade[4]),
                                Resources(bankTrade[0], bankTrade[1], bankTrade[2], bankTrade[3], bankTrade[4]));
                        }
                        else goto maiJos2;
                    }
                    else
                    {
                    maiJos2:
                        roundInfo(players, currentPlayer, me);
                        std::cout << "Trade player\n";
                        std::cout << "Player number: ";
                        int playerNum; std::cin >> playerNum;
                        while (playerNum < 1 || playerNum > players.size() || playerNum - 1 == currentPlayer)
                        {
                            std::cout << "Invalid answer! Try again!\n";
                            std::cout << "Player number: "; std::cin >> playerNum;
                        }
                        playerNum--;

                        std::cout << players[currentPlayer].name << "'s trade (L, B, W, G, O): ";
                        int playerTrade1[5], player2Trade[5];
                        for (int i = 0; i < 5; i++)
                            std::cin >> playerTrade1[i];
                        while (playerTrade1[0] < 0 || playerTrade1[1] < 0 || playerTrade1[2] < 0 || playerTrade1[3] < 0 || playerTrade1[4] < 0)
                        {
                            std::cout << "Invalid trade! Try again!\n";
                            std::cout << players[currentPlayer].name << "'s trade (L, B, W, G, O): ";
                            for (int i = 0; i < 5; i++)
                                std::cin >> playerTrade1[i];
                        }

                        std::cout << players[playerNum].name << "'s trade (L, B, W, G, O): ";
                        for (int i = 0; i < 5; i++)
                            std::cin >> player2Trade[i];
                        while (player2Trade[0] < 0 || player2Trade[1] < 0 || player2Trade[2] < 0 || player2Trade[3] < 0 || player2Trade[4] < 0)
                        {
                            std::cout << "Invalid trade! Try again!\n";
                            std::cout << players[playerNum].name << "'s trade (L, B, W, G, O): ";
                            for (int i = 0; i < 5; i++)
                                std::cin >> player2Trade[i];
                        }

                        trade(players[currentPlayer], players[playerNum], Resources(playerTrade1[0], playerTrade1[1], playerTrade1[2], playerTrade1[3], playerTrade1[4]),
                            Resources(player2Trade[0], player2Trade[1], player2Trade[2], player2Trade[3], player2Trade[4]));
                    }
                    break;
                }

                case 8:
                {
                    spendDevOnElse(players[currentPlayer]);
                    break;
                }

                case 7:
                {
                    std::cout << "Thief\n";
                    players[currentPlayer].devCards--;

                    int space;
                    std::cout << "Space: "; std::cin >> space;
                    while (space < 1 || space > 19)
                    {
                        std::cout << "Space does not exist! Try again!\n";
                        std::cout << "Space: "; std::cin >> space;
                    }
                    space--;

                    std::vector<int> playersOnSpace;
                    for (int i = 0; i < players.size(); i++)
                        playersOnSpace.push_back((players[i].spacesOwned[space] > 0) ? 1 : 0);

                    if (std::accumulate(playersOnSpace.begin(), playersOnSpace.end(), 0) > 1)
                    {
                        int s{};
                        for (int i = 0; i < players.size(); i++)
                        {
                            if (i == currentPlayer) continue;
                            if (playersOnSpace[i])
                            {
                                for (int j = 0; j < 5; j++)
                                    if (players[i].resourceCombinations.front()[j])
                                    {
                                        s++;
                                        break;
                                    }
                            }
                        }
                        if (s > 1)
                        {
                            std::cout << "Player who got stolen from: ";
                            int stolenPlayer; std::cin >> stolenPlayer;
                            while (stolenPlayer < 1 || stolenPlayer > players.size() || stolenPlayer - 1 == currentPlayer || !playersOnSpace[stolenPlayer - 1])
                            {
                                std::cout << "Invalid answer! Try again!\n";
                                std::cout << "Player who got stolen from: ";
                                std::cin >> stolenPlayer;
                            }
                            stolenPlayer--;
                            thief(players[currentPlayer], players[stolenPlayer], board, space);
                        }
                        else if (s)
                        {
                            for (int i = 0; i < players.size(); i++)
                            {
                                if (i == currentPlayer) continue;
                                if (playersOnSpace[i])
                                {
                                    int s{};
                                    for (int j = 0; j < 5; j++)
                                        s += players[i].resourceCombinations.front()[j];
                                    if (!s)
                                    {
                                        for (auto& var : board.space)
                                            std::get<2>(var) = false;
                                        std::get<2>(board.space[space]) = true;
                                        break;
                                    }
                                    thief(players[currentPlayer], players[i], board, space);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            for (int i = 0; i < 19; i++)
                                std::get<2>(board.space[i]) = false;
                            std::get<2>(board.space[space]) = true;
                        }
                    }
                    else if (std::accumulate(playersOnSpace.begin(), playersOnSpace.end(), 0))
                    {
                        for (int i = 0; i < players.size(); i++)
                        {
                            if (i == currentPlayer) continue;
                            if (playersOnSpace[i])
                            {
                                int s{};
                                for (int j = 0; j < 5; j++)
                                    s += players[i].resourceCombinations.front()[j];
                                if (!s)
                                {
                                    for (auto& var : board.space)
                                        std::get<2>(var) = false;
                                    std::get<2>(board.space[space]) = true;
                                    break;
                                }
                                thief(players[currentPlayer], players[i], board, space);
                                break;
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < 19; i++)
                            std::get<2>(board.space[i]) = false;
                        std::get<2>(board.space[space]) = true;
                    }
                    break;
                }

                case 6:
                {
                    yearOfPlenty(players[currentPlayer]);
                    break;
                }

                case 5:
                {
                    std::cout << "Monopol\n";
                    std::cout << "0. Lumber\n";
                    std::cout << "1. Brick\n";
                    std::cout << "2. Wool\n";
                    std::cout << "3. Grain\n";
                    std::cout << "4. Ore\n";
                    std::cout << "Resource: ";
                    int res; std::cin >> res;
                    while (res < 0 || res > 4)
                    {
                        std::cout << "Invalid response! Try again!\n";
                        std::cout << "Resource: "; std::cin >> res;
                    }

                    std::vector<int> amounts;
                    for (int i = 0; i < players.size(); i++)
                    {
                        if (i == currentPlayer) continue;

                        std::cout << players[i].name << " gave: ";
                        int amount; std::cin >> amount;
                        while (amount < 0)
                        {
                            std::cout << "Invalid answer! Try again!\n";
                            std::cout << players[i].name << " gave: ";
                            std::cin >> amount;
                        }
                        amounts.push_back(amount);
                    }

                    doMonopol(players, currentPlayer, res, amounts);
                    break;
                }

                case 4:
                {
                    buyDevCard(players[currentPlayer]);
                    break;
                }

                case 3:
                {
                    std::cout << "Make city\n";
                    std::cout << players[currentPlayer].name << "'s city(n, spaces...): ";
                    int spacesNum; std::cin >> spacesNum;
                    while (spacesNum < 1 || spacesNum > 3)
                    {
                        std::cout << "Invalid answer! Try again!\n";
                        std::cout << players[currentPlayer].name << "'s city(n, spaces...): "; std::cin >> spacesNum;
                    }

                    std::vector<int> citySpaces;
                    for (int i = 0; i < spacesNum; i++)
                    {
                        int var; std::cin >> var;
                        citySpaces.push_back(--var);
                    }
                    makeCity(players[currentPlayer], citySpaces);

                    break;
                }

                case 2:
                {
                    std::cout << "Make settlement\n";
                    std::cout << players[currentPlayer].name << "'s settlement(n, spaces...): ";
                    int spacesNum; std::cin >> spacesNum;
                    while (spacesNum < 1 || spacesNum > 3)
                    {
                        std::cout << "Invalid answer! Try again!\n";
                        std::cout << players[currentPlayer].name << "'s settlement(n, spaces...): "; std::cin >> spacesNum;
                    }

                    std::vector<int> settlementSpaces;
                    for (int i = 0; i < spacesNum; i++)
                    {
                        int var; std::cin >> var;
                        settlementSpaces.push_back(--var);
                    }
                    makeSettlement(players[currentPlayer], settlementSpaces);
                    break;
                }

                case 1:
                {
                    makeRoad(players[currentPlayer]);
                    break;
                }

                default:
                    break;
            }
            if (wantsToUndo)
                break;
        }
        if (wantsToUndo)
            continue;

        // End Turn
        currentPlayer = (currentPlayer + 1) % players.size();
        updateUndoStack(players, board, currentPlayer);
    }

    return 0;
}

void updateUndoStack(std::vector<Player> players, Board board, int currentPlayer)
{
    undoStack.push(std::make_tuple(players, board, currentPlayer));
}

void undo(std::vector<Player>& players, Board& board, int& currentPlayer)
{
    if (undoStack.size() < 2)
        return;

    undoStack.pop();

    players = std::get<0>(undoStack.top());
    board = std::get<1>(undoStack.top());
    currentPlayer = std::get<2>(undoStack.top());
}*/