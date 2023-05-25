#include "widget.h"

widget::widget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    // Window title and size
    setWindowTitle("Catan Cards Tracker");
    setMinimumSize(1000, 500);

    tradeHidden = false;
    optionsDisabled = false;
    placingSettlement = false;
    placingCity = false;
    didDice = false;
    reverse = false;
    placingThief = false;
    initialCounter = 0;
    discarded = false;

    {
        boardSpace.push_back(ui.space0);
        boardSpace.push_back(ui.space1);
        boardSpace.push_back(ui.space2);
        boardSpace.push_back(ui.space3);
        boardSpace.push_back(ui.space4);
        boardSpace.push_back(ui.space5);
        boardSpace.push_back(ui.space6);
        boardSpace.push_back(ui.space7);
        boardSpace.push_back(ui.space8);
        boardSpace.push_back(ui.space9);
        boardSpace.push_back(ui.space10);
        boardSpace.push_back(ui.space11);
        boardSpace.push_back(ui.space12);
        boardSpace.push_back(ui.space13);
        boardSpace.push_back(ui.space14);
        boardSpace.push_back(ui.space15);
        boardSpace.push_back(ui.space16);
        boardSpace.push_back(ui.space17);
        boardSpace.push_back(ui.space18);

        p1TradeBtns.push_back(ui.p1LumberTradeBtn);
        p1TradeBtns.push_back(ui.p1BrickTradeBtn);
        p1TradeBtns.push_back(ui.p1WoolTradeBtn);
        p1TradeBtns.push_back(ui.p1GrainTradeBtn);
        p1TradeBtns.push_back(ui.p1OreTradeBtn);
        
        p1TradeLabels.push_back(ui.p1LumberLabel);
        p1TradeLabels.push_back(ui.p1BrickLabel);
        p1TradeLabels.push_back(ui.p1WoolLabel);
        p1TradeLabels.push_back(ui.p1GrainLabel);
        p1TradeLabels.push_back(ui.p1OreLabel);

        p2TradeBtns.push_back(ui.p2LumberTradeBtn);
        p2TradeBtns.push_back(ui.p2BrickTradeBtn);
        p2TradeBtns.push_back(ui.p2WoolTradeBtn);
        p2TradeBtns.push_back(ui.p2GrainTradeBtn);
        p2TradeBtns.push_back(ui.p2OreTradeBtn);

        p2TradeLabels.push_back(ui.p2LumberLabel);
        p2TradeLabels.push_back(ui.p2BrickLabel);
        p2TradeLabels.push_back(ui.p2WoolLabel);
        p2TradeLabels.push_back(ui.p2GrainLabel);
        p2TradeLabels.push_back(ui.p2OreLabel);
    }
    hideTrade();
    disableOptions();

    ui.boardCodeLineEdit->setPlaceholderText("Board code..");
    ui.groupBox->setStyleSheet("QGroupBox { border: none; }");

    QObject::connect(p1TradeBtns[0], &QPushButton::clicked, this, &widget::p1LumberBtnClicked);
    QObject::connect(p1TradeBtns[1], &QPushButton::clicked, this, &widget::p1BrickBtnClicked);
    QObject::connect(p1TradeBtns[2], &QPushButton::clicked, this, &widget::p1WoolBtnClicked);
    QObject::connect(p1TradeBtns[3], &QPushButton::clicked, this, &widget::p1GrainBtnClicked);
    QObject::connect(p1TradeBtns[4], &QPushButton::clicked, this, &widget::p1OreBtnClicked);

    QObject::connect(p2TradeBtns[0], &QPushButton::clicked, this, &widget::p2LumberBtnClicked);
    QObject::connect(p2TradeBtns[1], &QPushButton::clicked, this, &widget::p2BrickBtnClicked);
    QObject::connect(p2TradeBtns[2], &QPushButton::clicked, this, &widget::p2WoolBtnClicked);
    QObject::connect(p2TradeBtns[3], &QPushButton::clicked, this, &widget::p2GrainBtnClicked);
    QObject::connect(p2TradeBtns[4], &QPushButton::clicked, this, &widget::p2OreBtnClicked);

    QObject::connect(ui.doneBtn, &QPushButton::clicked, this, &widget::doneBtnClicked);

    QObject::connect(ui.diceBtn, &QPushButton::clicked, this, &widget::onDice);
    QObject::connect(ui.buildBtn, &QPushButton::clicked, this, &widget::onBuild);
    QObject::connect(ui.buyDevCardBtn, &QPushButton::clicked, this, &widget::onBuyDevCard);
    QObject::connect(ui.spendDevCardBtn, &QPushButton::clicked, this, &widget::onSpendDevCard);
    QObject::connect(ui.tradeBtn, &QPushButton::clicked, this, &widget::onTrade);
    QObject::connect(ui.endTurnBtn, &QPushButton::clicked, this, &widget::onEndTurn);
    QObject::connect(ui.undoBtn, &QPushButton::clicked, this, &widget::onUndo);
}

widget::~widget()
{
}

void widget::updateUndoStack()
{
    undoStack.push(std::make_tuple(players, board, currentPlayer));
}

void widget::onDice()
{
    didDice = true;
    for (int i = 2; i <= 12; i++)
    {
        QPushButton* button = new QPushButton(this);
        button->setText(QString::number(i));
        button->setStyleSheet("font: 900 10pt \"Arial Black\";background-color: #d1f0ff;");

        ui.diceLayout->addWidget(button);

        dices.push_back(button);

        QObject::connect(button, &QPushButton::clicked, this, &widget::onDiceNum);
    }

    disableOptions();
}
void widget::onBuild()
{
    QPushButton* makeRoad = new QPushButton(this);
    QPushButton* makeSettlement = new QPushButton(this);
    QPushButton* makeCity = new QPushButton(this);

    makeRoad->setDisabled(!players[currentPlayer].couldMakeRoad());
    makeSettlement->setDisabled(!players[currentPlayer].couldMakeSettlement());
    makeCity->setDisabled(!players[currentPlayer].couldMakeCity());

    makeRoad->setIcon(QIcon(":/widget/Images/road.png"));
    makeSettlement->setIcon(QIcon(":/widget/Images/settlement.png"));
    makeCity->setIcon(QIcon(":/widget/Images/city.png"));

    makeRoad->setIconSize(QSize(100, 100));
    makeSettlement->setIconSize(QSize(100, 100));
    makeCity->setIconSize(QSize(100, 100));

    makeRoad->setStyleSheet("font: 900 10pt \"Arial Black\"; background-color: #d1f0ff; width: 75px; height: 75px;");
    makeSettlement->setStyleSheet("font: 900 10pt \"Arial Black\"; background-color: #d1f0ff; width: 75px; height: 75px;");
    makeCity->setStyleSheet("font: 900 10pt \"Arial Black\"; background-color: #d1f0ff; width: 75px; height: 75px;");

    ui.diceLayout->addWidget(makeRoad);
    ui.diceLayout->addWidget(makeSettlement);
    ui.diceLayout->addWidget(makeCity);

    buildBtns.push_back(makeRoad);
    buildBtns.push_back(makeSettlement);
    buildBtns.push_back(makeCity);

    QObject::connect(makeRoad, &QPushButton::clicked, this, &widget::buildRoad);
    QObject::connect(makeSettlement, &QPushButton::clicked, this, &widget::buildSettlement);
    QObject::connect(makeCity, &QPushButton::clicked, this, &widget::buildCity);

    disableOptions();
}
void widget::onBuyDevCard()
{
    buyDevCard(players[currentPlayer]);
    enableOptions();
    updateCombinations();
}
void widget::onSpendDevCard()
{
    players[currentPlayer].devCards--;

    QPushButton* thiefBtn = new QPushButton(this);
    QPushButton* monopolBtn = new QPushButton(this);
    QPushButton* yearOfPlentyBtn = new QPushButton(this);
    QPushButton* elseDevCard = new QPushButton(this);

    thiefBtn->setIcon(QIcon(":/widget/Images/robber.png"));

    thiefBtn->setIconSize(QSize(75, 75));
    monopolBtn->setText("Monopoly");
    yearOfPlentyBtn->setText("Year of\nPlenty");
    elseDevCard->setText("Else");

    thiefBtn->setStyleSheet("font: 900 10pt \"Arial Black\"; background-color: #d1f0ff; width: 75px; height: 75px;");
    monopolBtn->setStyleSheet("font: 900 10pt \"Arial Black\"; background-color: #d1f0ff; width: 75px; height: 75px;");
    yearOfPlentyBtn->setStyleSheet("font: 900 10pt \"Arial Black\"; background-color: #d1f0ff; width: 75px; height: 75px;");
    elseDevCard->setStyleSheet("font: 900 10pt \"Arial Black\"; background-color: #d1f0ff; width: 75px; height: 75px;");

    ui.diceLayout->addWidget(thiefBtn);
    ui.diceLayout->addWidget(monopolBtn);
    ui.diceLayout->addWidget(yearOfPlentyBtn);
    ui.diceLayout->addWidget(elseDevCard);

    devCardsBtns.push_back(thiefBtn);
    devCardsBtns.push_back(monopolBtn);
    devCardsBtns.push_back(yearOfPlentyBtn);
    devCardsBtns.push_back(elseDevCard);

    QObject::connect(thiefBtn, &QPushButton::clicked, this, &widget::onThiefBtn);
    QObject::connect(monopolBtn, &QPushButton::clicked, this, &widget::onMonopolBtn);
    QObject::connect(yearOfPlentyBtn, &QPushButton::clicked, this, &widget::onYearOfPlentyBtn);
    QObject::connect(elseDevCard, &QPushButton::clicked, this, &widget::onElseDevCardBtn);

    disableOptions();
}
void widget::onTrade()
{
    disableOptions();

    QPushButton* playerTradeBtn = new QPushButton(this);
    QPushButton* bankTradeBtn = new QPushButton(this);

    bankTradeBtn->setDisabled(!players[currentPlayer].couldTradeBank());

    playerTradeBtn->setIcon(QIcon(":/widget/Images/player.png"));
    bankTradeBtn->setIcon(QIcon(":/widget/Images/bank.png"));

    playerTradeBtn->setIconSize(QSize(75, 75));
    bankTradeBtn->setIconSize(QSize(75, 75));

    playerTradeBtn->setStyleSheet("font: 900 10pt \"Arial Black\"; background-color: #d1f0ff; width: 75px; height: 75px;");
    bankTradeBtn->setStyleSheet("font: 900 10pt \"Arial Black\"; background-color: #d1f0ff; width: 75px; height: 75px;");

    tradeBtns.push_back(playerTradeBtn);
    tradeBtns.push_back(bankTradeBtn);

    ui.diceLayout->addWidget(playerTradeBtn);
    ui.diceLayout->addWidget(bankTradeBtn);

    QObject::connect(playerTradeBtn, &QPushButton::clicked, this, &widget::onPlayerTrade);
    QObject::connect(bankTradeBtn, &QPushButton::clicked, this, &widget::onBankTrade);
}
void widget::onEndTurn()
{
    currentPlayer = (currentPlayer + 1) % players.size();
    ui.turnLabel->setText(QString::fromStdString("Turn: " + players[currentPlayer].name));
    didDice = false;
    updateUndoStack();
    enableOptions();
}
void widget::onUndo()
{
    players = std::get<0>(undoStack.top());
    board = std::get<1>(undoStack.top());
    currentPlayer = std::get<2>(undoStack.top());
    undoStack.pop();
    
    didDice = false;
    ui.turnLabel->setText(QString::fromStdString("Turn: " + players[currentPlayer].name));
    updateCombinations();
    updateMap();
    enableOptions();
}

void widget::onDiceNum()
{
    QPushButton* dicePressed = qobject_cast<QPushButton*>(sender());
    int diceNum = dicePressed->text().toInt();
    for (auto& button : dices)
        delete button;
    dices.clear();

    if (diceNum == 7)
    {
        if (discardCheck())
        {
            discard();
            return;
        }
        else
        {
            ui.doneTradeBtn->disconnect();
            QObject::connect(ui.doneTradeBtn, &QPushButton::clicked, this, &widget::thiefSteal);
            ui.doneTradeBtn->show();
            ui.doneTradeBtn->setText("Select thief space");
            ui.doneTradeBtn->setDisabled(true);
            placingThief = true;
            return;
        }
    }
    else
    {
        std::vector<int> spaces;
        for (int i = 0, num = 2; i < 19 && num; i++)
        {
            if (std::get<0>(board.space[i]) == diceNum)
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

        updateCombinations();
        enableOptions();
    }
}


void widget::thiefSteal()
{
    ui.doneTradeBtn->hide();
    ui.doneTradeBtn->disconnect();

    std::vector<int> playersOnSpace;
    for (int i = 0; i < players.size(); i++)
        playersOnSpace.push_back((players[i].spacesOwned[spacesSelected.front()] > 0) ? 1 : 0);

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
            for (int i = 0; i < players.size(); i++)
            {
                if (i == currentPlayer) continue;

                if (playersOnSpace[i])
                {
                    QPushButton* playerNameBtn = new QPushButton(this);
                    playerNameBtn->setText(QString::fromStdString(players[i].name));
                    playerNameBtn->setStyleSheet("font: 900 10pt \"Arial Black\";background-color: #d1f0ff;");
                    ui.diceLayout->addWidget(playerNameBtn);

                    playerNamesBtns.push_back(playerNameBtn);

                    QObject::connect(playerNameBtn, &QPushButton::clicked, this, &widget::thiefStealMultiplePlayers);
                }
            }
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
                        std::get<2>(board.space[spacesSelected.front()]) = true;
                        updateMap();
                        placingThief = false;
                        continue;
                    }
                    thief(players[currentPlayer], players[i], board, spacesSelected.front());
                    updateMap();
                    updateCombinations();
                    placingThief = false;
                    break;
                }
            }
        }
        else
        {
            for (int i = 0; i < 19; i++)
                std::get<2>(board.space[i]) = false;
            std::get<2>(board.space[spacesSelected.front()]) = true;
            updateMap();
            placingThief = false;
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
                    std::get<2>(board.space[spacesSelected.front()]) = true;
                    updateMap();
                    placingThief = false;
                    break;
                }
                thief(players[currentPlayer], players[i], board, spacesSelected.front());
                updateMap();
                updateCombinations();
                placingThief = false;
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < 19; i++)
            std::get<2>(board.space[i]) = false;
        std::get<2>(board.space[spacesSelected.front()]) = true;
        updateMap();
        placingThief = false;
    }

    if (players[me].resourceCombinations.size() > 1)
        selectMyCombo();
    else
        enableOptions();

    updateMap();
}
void widget::thiefStealMultiplePlayers()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    disableOptions();

    for (int i = 0; i < players.size(); i++)
        if (players[i].name == button->text().toStdString())
        {
            thief(players[currentPlayer], players[i], board, spacesSelected.front());

            enableOptions();
            updateMap();
            updateCombinations();

            break;
        }

    for (auto& btn : playerNamesBtns)
        delete btn;
    playerNamesBtns.clear();

    if (players[me].resourceCombinations.size() > 1)
        selectMyCombo();
    else
        enableOptions();

    updateMap();
}

void widget::selectMyCombo()
{
    ui.boardCodeLineEdit->show();
    ui.doneBtn->show();
    ui.doneBtn->disconnect();

    ui.boardCodeLineEdit->clear();
    ui.boardCodeLineEdit->setPlaceholderText("Number of you combo..");
    ui.doneBtn->setText("Done");

    QWidget::connect(ui.doneBtn, &QPushButton::clicked, this, &widget::doSelectMyCombo);
}
void widget::doSelectMyCombo()
{
    ui.boardCodeLineEdit->hide();
    ui.doneBtn->hide();

    int num = ui.boardCodeLineEdit->text().toInt();

    for (int i = 0; i < num - 1; i++)
        players[me].resourceCombinations.pop_front();
    while (players[me].resourceCombinations.size() > 1)
        players[me].resourceCombinations.pop_back();

    updateCombinations();
    enableOptions();
}
bool widget::discardCheck()
{
    for (auto& var : players)
    {
        int num{};
        if (var.resourceCombinations.size())
            for (int i = 0; i < 5; i++)
                num += var.resourceCombinations.front()[i];
        if (num > 7)
            return true;
    }
    return false;
}
void widget::discard()
{
    for (auto& var : players)
    {
        int num{};
        for (int i = 0; i < 5; i++)
            num += var.resourceCombinations.front()[i];
        if (num > 7)
        {
            unhideTrade();
            for (int i = 0; i < 5; i++)
            {
                p1TradeLabels[i]->setText("0");
                p2TradeLabels[i]->setText("0");
            }
            ui.p1NameLabel->setText(QString::fromStdString(var.name + ":"));
            ui.p2NameLabel->setText("");
            ui.doneTradeBtn->setText(QString::fromStdString("Discard " + std::to_string(num / 2) + " cards"));
            ui.doneTradeBtn->disconnect();

            QObject::connect(ui.doneTradeBtn, &QPushButton::clicked, this, &widget::doDiscard);

            return;
        }
    }
}
void widget::doDiscard()
{
    for (auto& var : players)
    {
        int num{};
        for (int i = 0; i < 5; i++)
            num += var.resourceCombinations.front()[i];
        if (num > 7)
        {
            auto s = var.resourceCombinations.size();
            for (int i = 0; i < s; i++)
            {
                auto it = var.resourceCombinations.begin();
                std::advance(it, i);
                Resources& combination = *it;

                combination -= Resources(p1TradeLabels[0]->text().toInt(), p1TradeLabels[1]->text().toInt(), p1TradeLabels[2]->text().toInt(), p1TradeLabels[3]->text().toInt(), p1TradeLabels[4]->text().toInt());
                auto ok = false;

                for (int j = 0; j < 5; j++)
                    if (combination[j] < 0)
                    {
                        ok = true;
                        break;
                    }
                if (ok) continue;

                var.addCombination(combination);
            }

            for (int i = 0; i < s; i++) var.resourceCombinations.pop_front();

            updateCombinations();

            if (discardCheck())
            {
                discard();
                return;
            }
            else
            {
                ui.doneTradeBtn->disconnect();
                hideTrade();
                QObject::connect(ui.doneTradeBtn, &QPushButton::clicked, this, &widget::thiefSteal);
                ui.doneTradeBtn->show();
                ui.doneTradeBtn->setText("Select thief space");
                ui.doneTradeBtn->setDisabled(true);
                placingThief = true;
                return;
            }
        }
    }
}

void widget::buildRoad()
{
    Player& player = players[currentPlayer];

    for (int i = 0; i < player.resourceCombinations.size(); i++)
	{
		auto it = player.resourceCombinations.begin();
		std::advance(it, i);
		Resources& combination = *it;

		if (combination.brick < 1 || combination.lumber < 1)
		{
			player.removeCombination(i + 1);
			i--;
		}
		else
		{
			combination.brick--;
			combination.lumber--;
		}
	}

    updateCombinations();
    removeBuildBtns();
    enableOptions();
}
void widget::buildSettlement()
{
    removeBuildBtns();
    ui.doneBtn->show();
    ui.doneBtn->setText("Select spaces for settlement");
    ui.doneBtn->disconnect();
    ui.doneBtn->setDisabled(true);
    QObject::connect(ui.doneBtn, &QPushButton::clicked, this, &widget::doBuildSettlement);
    placingSettlement = true;
    spacesSelected.clear();
}
void widget::doBuildSettlement()
{
    ui.doneBtn->hide();
    removeSelected();
    placingSettlement = false;

    makeSettlement(players[currentPlayer], spacesSelected);

    enableOptions();
    updateCombinations();
}
void widget::buildCity()
{
    removeBuildBtns();
    ui.doneBtn->show();
    ui.doneBtn->setText("Select spaces for city");
    ui.doneBtn->disconnect();
    ui.doneBtn->setDisabled(true);
    QObject::connect(ui.doneBtn, &QPushButton::clicked, this, &widget::doBuildCity);
    placingCity = true;
    spacesSelected.clear();
}
void widget::doBuildCity()
{
    ui.doneBtn->hide();
    removeSelected();
    placingCity = false;

    makeCity(players[currentPlayer], spacesSelected);

    enableOptions();
    updateCombinations();
}
void widget::removeBuildBtns()
{
    for (auto& btn : buildBtns)
    {
        ui.diceLayout->removeWidget(btn);
        delete btn;
    }
    buildBtns.clear();

    enableOptions();
}

void widget::onThiefBtn()
{
    removeDevCardBtns();

    ui.doneTradeBtn->disconnect();
    QObject::connect(ui.doneTradeBtn, &QPushButton::clicked, this, &widget::thiefSteal);
    ui.doneTradeBtn->show();
    ui.doneTradeBtn->setText("Select thief space");
    ui.doneTradeBtn->setDisabled(true);
    placingThief = true;
}
void widget::onMonopolBtn()
{
    removeDevCardBtns();

    for (auto& var : p1TradeBtns)
    {
        var->show();
        QObject::connect(var, &QPushButton::clicked, this, &widget::doMonopol);
    }
}
void widget::onYearOfPlentyBtn()
{
    removeDevCardBtns();
    yearOfPlenty(players[currentPlayer]);
    updateCombinations();
    enableOptions();
}
void widget::onElseDevCardBtn()
{
    removeDevCardBtns();
    enableOptions();
}
void widget::removeDevCardBtns()
{
    for (auto& btn : devCardsBtns)
    {
        ui.diceLayout->removeWidget(btn);
        delete btn;
    }
    devCardsBtns.clear();
}

void widget::doMonopol()
{
    for (auto& label : p2TradeLabels)
        label->setText(QString::number(0));
    for (auto& var : p1TradeBtns)
    {
        var->hide();
        QObject::disconnect(var, &QPushButton::clicked, this, &widget::doMonopol);
    }

    for (int i = 0; i < 5; i++)
        if (p1TradeBtns[i] == QObject::sender())
        {
            res = i;
            break;
        }

    for (int i = 0; i < players.size(); i++)
    {
        if (i == currentPlayer) continue;

        QLabel* nameLabel = new QLabel(this);
        nameLabel->setText(QString::fromStdString(players[i].name));
        nameLabel->setStyleSheet("font: 900 10pt \"Arial Black\";");
        labels.push_back(nameLabel);
        ui.p2Layout->addWidget(nameLabel);
        
        QSpinBox* amountSpinBox = new QSpinBox(this);
        amountSpinBox->setRange(0, 100);
        amountSpinBox->setStyleSheet("font: 900 10pt \"Arial Black\"; background-color: #d1f0ff;");
        spinBoxes.push_back(amountSpinBox);
        ui.p1Layout->addWidget(amountSpinBox);
    }
    ui.doneTradeBtn->show();
    ui.doneTradeBtn->setText("Done");

    QObject::connect(ui.doneTradeBtn, &QPushButton::clicked, this, &widget::finishMonopol);
}
void widget::finishMonopol()
{
    QObject::disconnect(ui.doneTradeBtn, &QPushButton::clicked, this, &widget::finishMonopol);
    for (auto& var : labels)
    {
        ui.p2Layout->removeWidget(var);
        delete var;
    }
    for (auto& var : spinBoxes)
    {
        ui.p1Layout->removeWidget(var);
        monopolAmounts.push_back(var->value());
        delete var;
    }
    ui.doneTradeBtn->hide();

    players[currentPlayer].devCards--;
    std::vector<int> s;
    for (int i = 0; i < players.size(); i++)
        s.push_back(players[i].resourceCombinations.size());

    int sum{};
    for (int i = 0; i < monopolAmounts.size(); i++)
        sum += monopolAmounts[i];

    int k{};
    for (int i = 0; i < players.size(); i++)
    {
        if (i == currentPlayer) continue;

        for (int j = 0; j < s[i]; j++)
        {
            auto it = players[i].resourceCombinations.begin();
            std::advance(it, j);
            Resources& combination = *it;

            if (combination[res] - monopolAmounts[k] != 0)
            {
                players[i].removeCombination(j + 1);
                j--;
                s[i]--;
            }
        }
        for (auto& var : players[i].resourceCombinations)
            var[res] = 0;
        k++;
    }

    for (auto& var : players[currentPlayer].resourceCombinations)
        var[res] += sum;

    monopolAmounts.clear();
    enableOptions();
    updateCombinations();
}

void widget::onPlayerTrade()
{
    removeTradeOptions();
    playerNamesBtns.clear();
    for (int i = 0; i < players.size(); i++)
    {
        if (i == currentPlayer) continue;

        QPushButton* playerNameBtn = new QPushButton(this);
        playerNameBtn->setStyleSheet("font: 900 10pt \"Arial Black\";background-color: #d1f0ff;");
        playerNameBtn->setText(QString::fromStdString(players[i].name));
        playerNameBtn->setDisabled(!(players[i].resourceCombinations.front()[0] + players[i].resourceCombinations.front()[1] + players[i].resourceCombinations.front()[2] + players[i].resourceCombinations.front()[3] + players[i].resourceCombinations.front()[4]));
        playerNamesBtns.push_back(playerNameBtn);
        ui.diceLayout->addWidget(playerNameBtn);

        QObject::connect(playerNameBtn, &QPushButton::clicked, this, &widget::doPlayerTrade);
    }
}
void widget::doPlayerTrade()
{
    playerTraded = qobject_cast<QPushButton*>(sender())->text().toStdString();
    for (auto& playerName : playerNamesBtns)
    {
        ui.diceLayout->removeWidget(playerName);
        delete playerName;
    }
    playerNamesBtns.clear();

    unhideTrade();
    ui.p2NameLabel->setText(QString::fromStdString(playerTraded + ":"));
    ui.p1NameLabel->setText(QString::fromStdString(players[currentPlayer].name + ":"));
    for (int i = 0; i < 5; i++)
    {
        p1TradeLabels[i]->setText("0");
        p2TradeLabels[i]->setText("0");
    }
    ui.doneTradeBtn->setText(QString::fromStdString("Trade " + playerTraded));
    ui.doneTradeBtn->disconnect();

    QObject::connect(ui.doneTradeBtn, &QPushButton::clicked, this, &widget::finishPlayerTrade);
}
void widget::finishPlayerTrade()
{
    hideTrade();

    for (int i = 0; i < players.size(); i++)
        if (players[i].name == playerTraded)
        {
            trade(players[currentPlayer], players[i],
                Resources(p1TradeLabels[0]->text().toInt(), p1TradeLabels[1]->text().toInt(), p1TradeLabels[2]->text().toInt(), p1TradeLabels[3]->text().toInt(), p1TradeLabels[4]->text().toInt()),
                Resources(p2TradeLabels[0]->text().toInt(), p2TradeLabels[1]->text().toInt(), p2TradeLabels[2]->text().toInt(), p2TradeLabels[3]->text().toInt(), p2TradeLabels[4]->text().toInt()));
            break;
        }
    updateCombinations();
    enableOptions();
}
void widget::onBankTrade()
{
    removeTradeOptions();
    unhideTrade();
    ui.p2NameLabel->setText("Bank:");
    ui.p1NameLabel->setText(QString::fromStdString(players[currentPlayer].name + ":"));
    for (int i = 0; i < 5; i++)
    {
        p1TradeLabels[i]->setText("0");
        p2TradeLabels[i]->setText("0");
    }
    ui.doneTradeBtn->setText("Trade bank");
    ui.doneTradeBtn->disconnect();
    QObject::connect(ui.doneTradeBtn, &QPushButton::clicked, this, &widget::finishBankTrade);
}
void widget::finishBankTrade()
{
    hideTrade();
    tradeBank(players[currentPlayer],
        Resources(p1TradeLabels[0]->text().toInt(), p1TradeLabels[1]->text().toInt(), p1TradeLabels[2]->text().toInt(), p1TradeLabels[3]->text().toInt(), p1TradeLabels[4]->text().toInt()),
        Resources(p2TradeLabels[0]->text().toInt(), p2TradeLabels[1]->text().toInt(), p2TradeLabels[2]->text().toInt(), p2TradeLabels[3]->text().toInt(), p2TradeLabels[4]->text().toInt()));
    updateCombinations();
    enableOptions();
}
void widget::removeTradeOptions()
{
    for (auto& btn : tradeBtns)
    {
        ui.diceLayout->removeWidget(btn);
        delete btn;
    }
    tradeBtns.clear();
}

void widget::updateCombinations()
{
    for (auto& label : comboLayoutLabels)
        if (label) delete label;
    for (auto& layout : comboLayoutLayouts)
        if (layout)
        {
            ui.comboLayout->removeItem(layout);
            delete layout;
        }
    comboLayoutLabels.clear();
    comboLayoutLayouts.clear();

    for (auto& player : players)
    {
        QLabel* playerName = new QLabel(QString::fromStdString(player.name + ":"), this);
        playerName->setFont(QFont("Arial Black", 10));
        playerName->setAlignment(Qt::AlignLeft);
        ui.comboLayout->addWidget(playerName);

        comboLayoutLabels.push_back(playerName);

        for (int j = 0; j < player.resourceCombinations.size(); j++)
        {
            auto it = player.resourceCombinations.begin();
            std::advance(it, j);
            ui.comboLayout->addLayout(getCombination(*it, j + 1));
        }
    }
}
void widget::updateMap()
{
    for (int i = 0; i < 19; i++)
    {
        QString currentStyleSheet = boardSpace[i]->styleSheet();
        currentStyleSheet.replace("border-width:3px", "border-width:1px");
        if (std::get<2>(board.space[i])) currentStyleSheet.replace("border-width:1px", "border-width:3px");
        boardSpace[i]->setStyleSheet(currentStyleSheet);
    }

    removeSelected();
}
void widget::selectSpace()
{
    if (!placingSettlement && !placingCity && !placingThief) return;

    if (!placingThief)
    {
        QPushButton* button = qobject_cast<QPushButton*>(sender());

        QString currentStyleSheet = button->styleSheet();
        currentStyleSheet.replace("border-width:1px", "border-width:5px");
        currentStyleSheet.replace("border-width:3px", "border-width:5px");
        currentStyleSheet.replace("border-color: black", "border-color: blue");
        button->setStyleSheet(currentStyleSheet);

        int index = -1;
        for (int i = 0; i < boardSpace.size(); i++)
            if (button == boardSpace[i])
            {
                index = i;
                break;
            }
        if (index != -1)
            spacesSelected.push_back(index);

        for (auto& btn : initialSettlementsButton)
            btn->setDisabled(false);
    }
    else
    {
        QPushButton* button = qobject_cast<QPushButton*>(sender());

        QString currentStyleSheet = button->styleSheet();
        currentStyleSheet.replace("border-width:1px", "border-width:5px");
        currentStyleSheet.replace("border-width:3px", "border-width:5px");
        currentStyleSheet.replace("border-color: black", "border-color: blue");
        button->setStyleSheet(currentStyleSheet);

        spacesSelected.clear();
        for (int i = 0; i < boardSpace.size(); i++)
            if (button == boardSpace[i])
            {
                spacesSelected.push_back(i);
                break;
            }
        placingThief = false;

        ui.doneTradeBtn->setDisabled(false);
    }

    ui.doneBtn->setDisabled(false);
}
void widget::removeSelected()
{
    for (int i = 0; i < 19; i++)
    {
        QString currentStyleSheet = boardSpace[i]->styleSheet();
        currentStyleSheet.replace("border-width:5px", "border-width:1px");
        if (std::get<2>(board.space[i])) currentStyleSheet.replace("border-width:5px", "border-width:3px");
        currentStyleSheet.replace("border-color: blue", "border-color: black");
        boardSpace[i]->setStyleSheet(currentStyleSheet);
    }
}
QHBoxLayout* widget::getCombination(Resources res, int seed)
{
    QHBoxLayout* layout = new QHBoxLayout(this);

    layout->setAlignment(Qt::AlignCenter);

    QLabel* nums[5], * imgs[5];
    for (int i = 0; i < 5; i++)
    {
        nums[i] = new QLabel(QString::number(res[i]), this);
        nums[i]->setFont(QFont("Arial Black", 10));
        if (!res[i]) nums[i]->setStyleSheet("color: gray");
        nums[i]->setWordWrap(true);

        imgs[i] = new QLabel(this);
        imgs[i]->setWordWrap(true);

        comboLayoutLabels.push_back(nums[i]);
        comboLayoutLabels.push_back(imgs[i]);
    }
    imgs[0]->setPixmap(QPixmap((res[0]) ? ":/widget/Images/lumber.png" : ":/widget/Images/lumberGray.png"));
    imgs[1]->setPixmap(QPixmap((res[1]) ? ":/widget/Images/brick.png" : ":/widget/Images/brickGray.png"));
    imgs[2]->setPixmap(QPixmap((res[2]) ? ":/widget/Images/wool.png" : ":/widget/Images/woolGray.png"));
    imgs[3]->setPixmap(QPixmap((res[3]) ? ":/widget/Images/grain.png" : ":/widget/Images/grainGray.png"));
    imgs[4]->setPixmap(QPixmap((res[4]) ? ":/widget/Images/ore.png" : ":/widget/Images/oreGray.png"));

    QLabel* seedNum = new QLabel(QString::fromStdString("#" + std::to_string(seed) + " |"));
    seedNum->setFont(QFont("Arial Black", 10));
    layout->addWidget(seedNum);

    comboLayoutLabels.push_back(seedNum);

    for (int i = 0; i < 5; i++)
    {
        //imgs[i]->setScaledContents(true);
        layout->addWidget(nums[i]);
        layout->addWidget(imgs[i]);
    }

    comboLayoutLayouts.push_back(layout);

    return layout;
}

void widget::hideTrade()
{
    ui.p1NameLabel->hide();
    ui.p2NameLabel->hide();
    ui.doneTradeBtn->hide();

    for (int i = 0; i < 5; i++)
    {
        p1TradeBtns[i]->hide();
        p1TradeLabels[i]->hide();
        p2TradeBtns[i]->hide();
        p2TradeLabels[i]->hide();
    }

    tradeHidden = true;
}
void widget::unhideTrade()
{
    ui.p1NameLabel->show();
    ui.p2NameLabel->show();
    ui.doneTradeBtn->show();

    for (int i = 0; i < 5; i++)
    {
        p1TradeBtns[i]->show();
        p1TradeLabels[i]->show();
        p2TradeBtns[i]->show();
        p2TradeLabels[i]->show();
    }

    tradeHidden = false;
}
void widget::enableOptions()
{
    ui.diceBtn->setDisabled((didDice) ? true : false);
    ui.buildBtn->setDisabled((didDice && (players[currentPlayer].couldMakeRoad() || players[currentPlayer].couldMakeSettlement() || players[currentPlayer].couldMakeCity())) ? false : true);
    ui.buyDevCardBtn->setDisabled((didDice && players[currentPlayer].couldBuyDevCard()) ? false : true);
    ui.spendDevCardBtn->setDisabled((players[currentPlayer].devCards) ? false : true);
    ui.tradeBtn->setDisabled((didDice && players[currentPlayer].couldTrade()) ? false : true);
    ui.endTurnBtn->setDisabled((didDice) ? false : true);
    ui.undoBtn->setDisabled((undoStack.size()) ? false : true);

    optionsDisabled = false;
}
void widget::disableOptions()
{
    ui.diceBtn->setDisabled(true);
    ui.buildBtn->setDisabled(true);
    ui.buyDevCardBtn->setDisabled(true);
    ui.spendDevCardBtn->setDisabled(true);
    ui.tradeBtn->setDisabled(true);
    ui.endTurnBtn->setDisabled(true);
    ui.undoBtn->setDisabled(true);

    optionsDisabled = true;
}

void widget::setupBoard(QString code)
{
    if (code.toStdString().size() != 78)
    {
        ui.boardCodeLineEdit->setPlaceholderText("Incorrect code! Try again!");
        return;
    }

    std::string codeStd = code.toStdString();

    int count{};
    for (int i = 0; i < 19; i++)
    {
        if (codeStd[count + 1] == ' ')
        {
            if (codeStd[count] == '0')
            {
                board.space[i] = std::make_tuple(0, "Thief", true);
                boardSpace[i]->setStyleSheet("border-style: solid;border-width:3px;border-radius:15px;border-color: black;max-width:30px;max-height:30px;min-width:30px;min-height:30px;font: 900 10pt \"Arial Black\";background-color: #7aff9e;");
                count += 2;
                continue;
            }
            board.space[i] = std::make_tuple(int(codeStd[count] - '0'), "", false);
            boardSpace[i]->setText(QString::number(int(codeStd[count] - '0')));
            count += 2;
        }
        else
        {
            board.space[i] = std::make_tuple(int(codeStd[count] - '0') * 10 + int(codeStd[count + 1] - '0'), "", false);
            boardSpace[i]->setText(QString::number(int(codeStd[count] - '0') * 10 + int(codeStd[count + 1] - '0')));
            count += 3;
        }
        switch (int(codeStd[count]))
        {
        case 76:
        case 108:
            std::get<1>(board.space[i]) = "Lumber";
            boardSpace[i]->setStyleSheet("border-style: solid;border-width:1px;border-radius:15px;border-color: black;max-width:30px;max-height:30px;min-width:30px;min-height:30px;font: 900 10pt \"Arial Black\";background-color: green;");
            break;

        case 66:
        case 98:
            std::get<1>(board.space[i]) = "Brick";
            boardSpace[i]->setStyleSheet("border-style: solid;border-width:1px;border-radius:15px;border-color: black;max-width:30px;max-height:30px;min-width:30px;min-height:30px;font: 900 10pt \"Arial Black\";background-color: red;");
            break;

        case 87:
        case 119:
            std::get<1>(board.space[i]) = "Wool";
            boardSpace[i]->setStyleSheet("border-style: solid;border-width:1px;border-radius:15px;border-color: black;max-width:30px;max-height:30px;min-width:30px;min-height:30px;font: 900 10pt \"Arial Black\";background-color: white;");
            break;

        case 71:
        case 103:
            std::get<1>(board.space[i]) = "Grain";
            boardSpace[i]->setStyleSheet("border-style: solid;border-width:1px;border-radius:15px;border-color: black;max-width:30px;max-height:30px;min-width:30px;min-height:30px;font: 900 10pt \"Arial Black\";background-color: yellow;");
            break;

        case 79:
        case 111:
            std::get<1>(board.space[i]) = "Ore";
            boardSpace[i]->setStyleSheet("border-style: solid;border-width:1px;border-radius:15px;border-color: black;max-width:30px;max-height:30px;min-width:30px;min-height:30px;font: 900 10pt \"Arial Black\";background-color: gray;");
            break;
        }
        count += 2;
    }

    ui.boardCodeLineEdit->clear();
    ui.boardCodeLineEdit->setPlaceholderText("How many players are in this game?");
    ui.doneBtn->disconnect();
    QObject::connect(ui.doneBtn, &QPushButton::clicked, this, &widget::playerNum);
}


void widget::doneBtnClicked()
{
    setupBoard(ui.boardCodeLineEdit->text());

    ui.boardCodeLineEdit->clear();
    ui.boardCodeLineEdit->setPlaceholderText("How many players are there?");

    ui.doneBtn->disconnect();
    QObject::connect(ui.doneBtn, &QPushButton::clicked, this, &widget::playerNum);
}
void widget::playerNum()
{
    playerNumber = ui.boardCodeLineEdit->text().toInt();

    ui.boardCodeLineEdit->clear();
    ui.boardCodeLineEdit->setPlaceholderText("Enter the names, one by one");
    ui.doneBtn->disconnect();
    QObject::connect(ui.doneBtn, &QPushButton::clicked, this, &widget::playerNames);
}
void widget::playerNames()
{
    players.push_back(Player(ui.boardCodeLineEdit->text().toStdString()));
    ui.boardCodeLineEdit->clear();

    playerNumber--;

    if (!playerNumber)
    {
        ui.boardCodeLineEdit->hide();
        ui.doneBtn->disconnect();
        ui.doneBtn->setText("Select first player and then you");
        ui.doneBtn->disconnect();
        QObject::connect(ui.doneBtn, &QPushButton::clicked, this, &widget::playerNamesBtnClicked);
    }
}
void widget::playerNamesBtnClicked()
{
    ui.doneBtn->hide();
    for (auto& player : players)
    {
        QPushButton* button = new QPushButton(this);
        button->setText(QString::fromStdString(player.name));
        button->setStyleSheet("font: 900 10pt \"Arial Black\";background-color: #d1f0ff;");
        playerNamesBtns.push_back(button);
        ui.upperLayout->addWidget(button);

        QObject::connect(button, &QPushButton::clicked, this, &widget::firstPlayer);
    }
}
void widget::firstPlayer()
{
    QPushButton* firstPlayerBtn = qobject_cast<QPushButton*>(sender());
    firstPlayerBtn->setStyleSheet("font: 900 10pt \"Arial Black\";background-color: red;");

    for (int i = 0; i < players.size(); i++)
        if (players[i].name == firstPlayerBtn->text().toStdString())
        {
            currentPlayer = i;
            break;
        }

    for (auto& button : playerNamesBtns)
    {
        button->disconnect();
        QObject::connect(button, &QPushButton::clicked, this, &widget::mePlayer);
    }
}
void widget::mePlayer()
{
    QPushButton* mePlayerBtn = qobject_cast<QPushButton*>(sender());
    mePlayerBtn->setStyleSheet("font: 900 10pt \"Arial Black\";background-color: yellow;");

    for (int i = 0; i < players.size(); i++)
        if (players[i].name == mePlayerBtn->text().toStdString())
        {
            me = i;
            break;
        }

    for (auto& button : playerNamesBtns)
        delete button;
    playerNamesBtns.clear();

    updateCombinations();
    ui.turnLabel->setText(QString::fromStdString("Turn: " + players[currentPlayer].name));

    placingSettlement = true;

    QPushButton* button = new QPushButton(this);
    button->setText(QString::fromStdString("Select the spaces affected by settlement"));
    button->setStyleSheet("font: 900 10pt \"Arial Black\";background-color: #d1f0ff;");
    button->setDisabled(true);
    initialSettlementsButton.push_back(button);
    ui.upperLayout->addWidget(button);

    QObject::connect(button, &QPushButton::clicked, this, &widget::initialSettlements);

    for (auto& var : boardSpace)
        QObject::connect(var, &QPushButton::clicked, this, &widget::selectSpace);
}
void widget::initialSettlements()
{
    for (auto space : spacesSelected)
    {
        players[currentPlayer].spacesOwned[space]++;
        if (reverse && std::get<0>(board.space[space])) players[currentPlayer].getResource(space, 1, board);
    }

    if (!reverse && initialCounter != players.size() - 1)
    {
        currentPlayer = (currentPlayer + 1) % players.size();
        initialCounter++;
    }
    else if (!reverse && initialCounter == players.size() - 1)
    {
        reverse = true;
        initialCounter = 0;
    }
    else if (reverse && initialCounter != players.size() - 1)
    {
        currentPlayer--;
        if (currentPlayer < 0) currentPlayer = players.size() - 1;
        initialCounter++;
    }
    else
    {
        placingSettlement = false;
        enableOptions();

        for (auto& btn : initialSettlementsButton)
        {
            ui.upperLayout->removeWidget(btn);
            delete btn;
        }
        initialSettlementsButton.clear();
    }

    ui.turnLabel->setText(QString::fromStdString("Turn: " + players[currentPlayer].name));
    removeSelected();
    updateCombinations();

    for (int i = spacesSelected.size(); i > 0; i--)
        spacesSelected.pop_back();
    for (auto& btn : initialSettlementsButton)
        btn->setDisabled(true);
}

void widget::p1LumberBtnClicked()
{
    if (p2TradeLabels[0]->text().toInt())
        p2TradeLabels[0]->setText(QString::number(p2TradeLabels[0]->text().toInt() - 1));
    else
        p1TradeLabels[0]->setText(QString::number(p1TradeLabels[0]->text().toInt() + 1));
}
void widget::p1BrickBtnClicked()
{
    if (p2TradeLabels[1]->text().toInt())
        p2TradeLabels[1]->setText(QString::number(p2TradeLabels[1]->text().toInt() - 1));
    else
        p1TradeLabels[1]->setText(QString::number(p1TradeLabels[1]->text().toInt() + 1));
}
void widget::p1WoolBtnClicked()
{
    if (p2TradeLabels[2]->text().toInt())
        p2TradeLabels[2]->setText(QString::number(p2TradeLabels[2]->text().toInt() - 1));
    else
        p1TradeLabels[2]->setText(QString::number(p1TradeLabels[2]->text().toInt() + 1));
}
void widget::p1GrainBtnClicked()
{
    if (p2TradeLabels[3]->text().toInt())
        p2TradeLabels[3]->setText(QString::number(p2TradeLabels[3]->text().toInt() - 1));
    else
        p1TradeLabels[3]->setText(QString::number(p1TradeLabels[3]->text().toInt() + 1));
}
void widget::p1OreBtnClicked()
{
    if (p2TradeLabels[4]->text().toInt())
        p2TradeLabels[4]->setText(QString::number(p2TradeLabels[4]->text().toInt() - 1));
    else
        p1TradeLabels[4]->setText(QString::number(p1TradeLabels[4]->text().toInt() + 1));
}

void widget::p2LumberBtnClicked()
{
    if (p1TradeLabels[0]->text().toInt())
        p1TradeLabels[0]->setText(QString::number(p1TradeLabels[0]->text().toInt() - 1));
    else
        p2TradeLabels[0]->setText(QString::number(p2TradeLabels[0]->text().toInt() + 1));
}
void widget::p2BrickBtnClicked()
{
    if (p1TradeLabels[1]->text().toInt())
        p1TradeLabels[1]->setText(QString::number(p1TradeLabels[1]->text().toInt() - 1));
    else
        p2TradeLabels[1]->setText(QString::number(p2TradeLabels[1]->text().toInt() + 1));
}
void widget::p2WoolBtnClicked()
{
    if (p1TradeLabels[2]->text().toInt())
        p1TradeLabels[2]->setText(QString::number(p1TradeLabels[2]->text().toInt() - 1));
    else
        p2TradeLabels[2]->setText(QString::number(p2TradeLabels[2]->text().toInt() + 1));
}
void widget::p2GrainBtnClicked()
{
    if (p1TradeLabels[3]->text().toInt())
        p1TradeLabels[3]->setText(QString::number(p1TradeLabels[3]->text().toInt() - 1));
    else
        p2TradeLabels[3]->setText(QString::number(p2TradeLabels[3]->text().toInt() + 1));
}
void widget::p2OreBtnClicked()
{
    if (p1TradeLabels[4]->text().toInt())
        p1TradeLabels[4]->setText(QString::number(p1TradeLabels[4]->text().toInt() - 1));
    else
        p2TradeLabels[4]->setText(QString::number(p2TradeLabels[4]->text().toInt() + 1));
}