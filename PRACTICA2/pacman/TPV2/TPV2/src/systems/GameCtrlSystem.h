// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

class GameCtrlSystem: public ecs::System {
public:

	__SYSID_DECL__(ecs::sys::GAMECTRL)

	GameCtrlSystem();
	virtual ~GameCtrlSystem();

    inline auto getScore()
	{
        return score;
    }

    inline void setScore(unsigned int score_)
	{
        score = score_;
    }

    inline void incrScore(unsigned int n)
	{
        score += n;
    }

	void initSystem() override;
	void update() override;
	void recieve(const Message &m) override;

private:
    unsigned int score = 0;
};