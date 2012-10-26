#pragma once


class AI
{
public:
	AI(void);
	~AI(void);

	DECISION_INFO decision(GAME_INFO gameInfo);

	int calculate(CBoard &board);

	void makeMessage(DECISION_INFO& decisionInfo,WCHAR *text);
};
