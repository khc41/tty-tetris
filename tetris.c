/*
 *      tetris.c TTY-TETRIS Main file.
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

#include "tetris.h"
#include "config.h"

/* Functions */
void
init(void)
{

     printf("\033[2J");

     /* Make rand() really random :) */
     srand(getpid());

     /* Init variables */
     score = lines = 0;
     running = True;

     /* Score */
     printxy(FRAMEH_NB + 2, FRAMEW + 3, "Score:");
     printxy(FRAMEH_NB + 3, FRAMEW + 3, "Lines:");
     DRAW_SCORE();

     return;
}

Bool
kbhit(int t)
{
     struct timeval tv = {0, t};
     fd_set read_fd;

     FD_ZERO(&read_fd);
     FD_SET(0, &read_fd);

     if(select(1, &read_fd, NULL, NULL, &tv) == -1)
          return False;

     if(FD_ISSET(0, &read_fd))
          return True;

     return False;
}

void
get_key_event(void)
{

     switch(getch())
     {
     case KEY_MOVE_LEFT:             shape_move(-EXP_FACT);     break;
     case KEY_MOVE_RIGHT:            shape_move(EXP_FACT);      break;
     case KEY_CHANGE_POSITION_NEXT:  shape_set_position(N_POS); break;
     case KEY_CHANGE_POSITION_PREV:  shape_set_position(P_POS); break;
     case KEY_DROP_SHAPE:            shape_drop();              break;
     }

     return;
}

void
arrange_score(int l)
{

     /* Standard score count */
     switch(l)
     {
     case 1: score += 40;   break; /* One line */
     case 2: score += 100;  break; /* Two lines */
     case 3: score += 300;  break; /* Three lines */
     case 4: score += 1200; break; /* Four lines */
     }

     lines += l;

     DRAW_SCORE();

     return;
}

void
check_plain_line(void)
{
     int i, j, k, f, c = 0, nl = 0;

     for(i = 1; i < FRAMEH; ++i)
     {
          for(j = 1; j < FRAMEW; ++j)
               if(frame[i][j] == 0)
                    ++c;
          if(!c)
          {
               ++nl;
               for(k = i - 1; k > 1; --k)
                    for(f = 1; f < FRAMEW; ++f)
                         frame[k + 1][f] = frame[k][f];
          }
          c = 0;
     }
     arrange_score(nl);

     return;
}

int
check_possible_pos(int x, int y)
{
     int i, j, c = 0;

     for(i = 0; i < 4; ++i)
          for(j = 0; j < EXP_FACT; ++j)
               if(frame[x + shapes[current.num][current.pos][i][0]]
                  [y + shapes[current.num][current.pos][i][1] * EXP_FACT + j] != 0)
                    ++c;

     return c;
}

int
main(int argc, char **argv)
{

     init();
     frame_init();
     frame_nextbox_init();

     current.y = FRAMEW / 2 - 1;
     current.num = RAND(0, 6);
     current.next = RAND(0, 6);

     frame_nextbox_refresh();

     while(running)
     {
          get_key_event();
          shape_set();
          frame_refresh();
          shape_go_down();
     }

     return 0;
}
