/*
 *      util.c
 *      Copyright © 2008 Martin Duquesnoy <xorg62@gmail.com>
 *      All rights reserved.
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are
 *      met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following disclaimer
 *        in the documentation and/or other materials provided with the
 *        distribution.
 *      * Neither the name of the  nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <termio.h>
#include <string.h>
#include "tetris.h"


/* getchar() but non-block */
int
getch(void)
{
     int ret;
     struct termios term, back;

     tcgetattr (0, &term);
     memcpy (&back, &term, sizeof(term));

     term.c_lflag &= ~(ICANON|ECHO);
     term.c_cc[VTIME] = 4;
     term.c_cc[VMIN]  = 0;

     tcsetattr(0, TCSANOW, &term);
     ret = getchar();
     tcsetattr(0, TCSANOW, &back);

     return ret;
}

void
printxy(int x, int y, char *str)
{
     printf("\033[%d;%dH%s", ++x, ++y, str);

     return;
}

void
set_color(int color)
{
     int bg = 0, fg = 0;

     switch(color)
     {
     default:
     case Black:   bg = 0;  break;
     case Blue:    bg = 44; break;
     case Red:     bg = 41; break;
     case Magenta: bg = 45; break;
     case White:   bg = 47; break;
     case Green:   bg = 42; break;
     case Cyan:    bg = 46; break;
     case Yellow:  bg = 43; break;
     case Border:  bg = 47; break;
     case Score:   fg = 37; bg = 49; break;
     }

     printf("\033[%d;%dm", fg, bg);

     return;
}
