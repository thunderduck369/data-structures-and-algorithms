using GameAI.GamePlaying.Core;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Diagnostics.Tracing;
using System.Linq;
using System.Net.NetworkInformation;
using System.Security.Policy;

namespace GameAI.GamePlaying
{
    public class StudentAI : Behavior
    {
        // TODO: Methods go here
        public StudentAI(){}

        private int get_next_player(int _color, Board game_state)
        {
            if (game_state.HasAnyValidMove(-_color))
            {
                return -_color;
            }
            else
            {
                return _color;
            }
        }
        public ComputerMove Run(int _color, Board _board, int _lookAheadDepth)
        {
            ComputerMove best_move = null;
            Board new_state = new Board();
            new_state.Copy(_board);

            List<ComputerMove> moves = new List<ComputerMove>();

            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (new_state.IsValidMove(_color, i, j))
                    {
                        ComputerMove newMove = new ComputerMove(i, j);
                        moves.Add(newMove);
                    }
                }
            }

 
            foreach (ComputerMove move in moves)
            {
                new_state.Copy(_board);
                new_state.MakeMove(_color, move.row, move.column);

                if (new_state.IsTerminalState() || _lookAheadDepth == 0)
                {
                    move.rank = Evaluate(new_state);
                }
                else
                {
                    move.rank = Run(get_next_player(_color, new_state), new_state, _lookAheadDepth - 1).rank;
                }

                if (best_move == null || move.rank * _color > best_move.rank * _color)
                {
                    best_move = move;
                }
            }

            return best_move;
        }
        private int Evaluate(Board board)
        {
            int evaluation = 0;
            int multiplier = 1;
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (i == 0 && j == 0)
                    {
                        multiplier = 100;
                    }
                    else if (i == 0 && j == 7)
                    {
                        multiplier = 100;
                    }
                    else if (i == 7 && j == 0)
                    {
                        multiplier = 100;
                    }
                    else if (i == 7 && j == 7)
                    {
                        multiplier = 100;
                    }
                    else if (i == 0 || i == 7 || j == 0 || j == 7)
                    {
                        multiplier = 10;
                    }
                    else
                    {
                        multiplier = 1;
                    }

                    if (board.GetTile(i, j) == 1)
                    {
                        evaluation += multiplier;
                    }
                    else if (board.GetTile(i, j) == -1)
                    {
                        evaluation -= multiplier;
                    }
                }
            }

            if (board.IsTerminalState())
            {
                if (evaluation > 0)
                {
                    evaluation += 10000;
                }
                else
                {
                    evaluation -= 10000;
                }
            }
            return evaluation;
        }
    }
}
