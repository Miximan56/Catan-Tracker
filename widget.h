#pragma once

#include "ui_widget.h"
#include <QtWidgets/QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>

#include "actions.h"
#include <functional>
#include <stack>

/*std::stack<std::tuple<std::vector<Player>, Board, int>> undoStack;

void clear(void);
void roundInfo(std::vector<Player>&, int, int);

void updateUndoStack(std::vector<Player>, Board, int);
void undo(std::vector<Player>&, Board&, int&);*/

class widget : public QWidget
{
    Q_OBJECT

public:
    widget(QWidget *parent = nullptr);
    ~widget();

    std::stack<std::tuple<std::vector<Player>, Board, int>> undoStack;
    std::vector<Player> players;
    Board board;
    
    bool tradeHidden, optionsDisabled, placingSettlement, placingCity, didDice, reverse, placingThief, discarded;
    int me, currentPlayer, playerNumber, initialCounter, res;
    std::vector<int> spacesSelected, monopolAmounts;
    
    std::vector<QHBoxLayout*> comboLayoutLayouts;
    std::vector<QLabel*> comboLayoutLabels;

    std::vector<QPushButton*> dices;
    
    std::vector<QPushButton*> boardSpace, p1TradeBtns, p2TradeBtns;
    std::vector<QLabel*> p1TradeLabels, p2TradeLabels;
    std::vector<QPushButton*> playerNamesBtns, buildBtns, devCardsBtns, tradeBtns, initialSettlementsButton;
    std::vector<QLabel*> labels;
    std::vector<QSpinBox*> spinBoxes;
    std::string playerTraded;
    
    void updateUndoStack(void);

    void onDice(void);
    void onBuild(void);
    void onBuyDevCard(void);
    void onSpendDevCard(void);
    void onTrade(void);
    void onEndTurn(void);
    void onUndo(void);

    void onDiceNum(void);
    
    void thiefSteal(void);
    void thiefStealMultiplePlayers(void);

    void selectMyCombo(void);
    void doSelectMyCombo(void);
    bool discardCheck(void);
    void discard(void);
    void doDiscard(void);

    void buildRoad(void);
    void buildSettlement(void);
    void doBuildSettlement(void);
    void buildCity(void);
    void doBuildCity(void);
    void removeBuildBtns(void);

    void onThiefBtn(void);
    void onMonopolBtn(void);
    void onYearOfPlentyBtn(void);
    void onElseDevCardBtn(void);
    void removeDevCardBtns(void);

    void doMonopol(void);
    void finishMonopol(void);
    
    void onPlayerTrade(void);
    void doPlayerTrade(void);
    void finishPlayerTrade(void);
    void onBankTrade(void);
    void finishBankTrade(void);
    void removeTradeOptions(void);
    
    void updateCombinations(void);
    void updateMap(void);
    void selectSpace(void);
    void removeSelected(void);
    QHBoxLayout* getCombination(Resources, int);

    void hideTrade(void);
    void unhideTrade(void);
    void enableOptions(void);
    void disableOptions(void);
    
    void setupBoard(QString);

private:
    Ui::widgetClass ui;

    void doneBtnClicked(void);
    void playerNum(void);
    void playerNames(void);
    void playerNamesBtnClicked(void);
    void firstPlayer(void);
    void mePlayer(void);
    void initialSettlements(void);
    
    void p1LumberBtnClicked(void);
    void p1BrickBtnClicked(void);
    void p1WoolBtnClicked(void);
    void p1GrainBtnClicked(void);
    void p1OreBtnClicked(void);

    void p2LumberBtnClicked(void);
    void p2BrickBtnClicked(void);
    void p2WoolBtnClicked(void);
    void p2GrainBtnClicked(void);
    void p2OreBtnClicked(void);
};

// Dice, Build(Road, Settlement, City), Buy Development Card, Spend Development Card(Thief, Monopol, Year of Plenty, Something else), End turn