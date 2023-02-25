// MyOthelloAI.hpp
#ifndef SCRAPAI_HPP
#define SCRAPAI_HPP

#include "OthelloAI.hpp"
#include <functional>

namespace alanc15
{
    class Scrappy : public OthelloAI
    {
    public:
        std::pair<int, int> chooseMove(const OthelloGameState& state) override;
    };
}

#endif