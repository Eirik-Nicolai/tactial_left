#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>

#include <vector>
#include <string>

#include "olcPixelGameEngine.h"
#include "globals.hpp"

class Debugger
{
    public:
        static Debugger& instance()
        {
            static Debugger instance;
            return instance;
        }
        void Print(olc::PixelGameEngine *pge, float posx, float posy)
        {
            auto [winx, winy] = pge->GetWindowSize();
            winx *= posx;
            winy *= posy;
            int offs = 1;
            pge->DrawString(winx, winy, "-----------------------",olc::WHITE);
            for (auto &s: debug_lines)
            {
                pge->DrawString(
                    winx,
                    winy + offs*PIXEL_OFFS,
                    s,
                    olc::WHITE
                );
                offs++;
            }
            pge->DrawString(winx, winy + (offs) * PIXEL_OFFS,"-----------------------",olc::WHITE);

        }
        void Clear()
        {
            debug_lines.clear();
        }

    public:
        Debugger operator+=(const std::string &rhs)
        {
            debug_lines.emplace_back(debug_line(debug_lines.size(), rhs));
            return *this;
        }

    private:
        Debugger() = default;
        //~Debugger() = default;
        std::string debug_line(int i, const std::string &s)
        {
            return std::string("[ "+ std::to_string(i) + " ] " + s);
        }
        std::vector<std::string> debug_lines;
};



#endif // DEBUG_H_
