from __future__ import division
from collections import namedtuple
from enum import Enum
from random import randrange, random


ROWS = 27       # number of rows
COLS = 21       # number of columns

ALPHA   = 1.0   # learning rate
GAMMA   = 1.0   # discount factor
EPSILON = 1.0   # exploration factor (initial)

REWARD_OUT_OF_BOX   = -100      # punishment for crossing bounds
REWARD_COLLISION    = -200      # punishment for hitting obstacles
REWARD_DANGER       = -40       # punishment for moving to a dangerous position
REWARD_WARNING      = -10        # punishment for moving to a position near a dangerous one
REWARD_TARGET       = +500      # reward for reaching the target position
REWARD_STEP_DIAG    = -3
REWARD_STEP_DIRECT  = -2


Direction = namedtuple('Direction', 'row col')


class Move(Enum):
    """ All possible actions """
    __order__: 'UP_LEFT UP UP_RIGHT RIGHT DOWN_RIGHT DOWN DOWN_LEFT LEFT'
    UP_LEFT     = 0
    UP          = 1
    UP_RIGHT    = 2
    RIGHT       = 3
    DOWN_RIGHT  = 4
    DOWN        = 5
    DOWN_LEFT   = 6
    LEFT        = 7


"""
Direction (row, col) associated to each action
"""
directions = {
    Move.UP_LEFT:       Direction(-1, -1),
    Move.UP:            Direction(-1, 0),
    Move.UP_RIGHT:      Direction(-1, 1),
    Move.RIGHT:         Direction(0, 1),
    Move.DOWN_RIGHT:    Direction(1, 1),
    Move.DOWN:          Direction(1, 0),
    Move.DOWN_LEFT:     Direction(1, -1),
    Move.LEFT:          Direction(0, -1)
}

"""
Visual representation of directions
Two symbols for each direction: first is better-looking,
second is more compatible with editors: pick one!
← → ↑ ↓ ↔ ↕ ↖ ↗ ↘ ↙
"""
symbols = {
    Move.UP_LEFT:       ('🡔', '↖'),
    Move.UP:            ('🡑', '↑'),
    Move.UP_RIGHT:      ('🡕', '↗'),
    Move.RIGHT:         ('🡒', '→'),
    Move.DOWN_RIGHT:    ('🡖', '↘'),
    Move.DOWN:          ('🡓', '↓'),
    Move.DOWN_LEFT:     ('🡗', '↙'),
    Move.LEFT:          ('🡐', '←')
}

"""
Matrix of the obstacles.
1 -> box contains an obstacle (it is a no go)
0 -> box is free
Tip: ctrl+F and search '1' to visualize the topology
"""
obstacles = (
    (1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0),
    (1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0),
    (1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0),
    (1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0),
    (1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0),
    (1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0),
    (0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0),
    (0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0),
    (0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0),
    (0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    (0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    (0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    (0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    (0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    (0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    (0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1),
    (0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1),
    (0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1),
    (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1),
    (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1),
    (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1),
    (1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1),
    (1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1),
    (1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1),
    (1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1),
    (1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1),
    (1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1),
)

target = (1, 18)


def next_pos(i, j, move):
    """ Compute the next position from (i, j) according to move.
    This next position is not guaranteed to be a legal one. """
    return i + directions[move].row, j + directions[move].col


def pos_out_of_box(i, j):
    """ Return True if the position is out of the bounds. """
    return not (0 <= i < ROWS and 0 <= j < COLS)


def pos_illegal(i, j):
    """ Return True if the position is illegal """
    return pos_out_of_box(i, j) or obstacles[i][j]


def pos_dangerous(i, j):
    """ Check if there is a barrier or obstacle nearby """
    if (i == 0) or (i == ROWS-1) or (j == 0) or (j == COLS-1):
        return True
    for p in range(-1, 2):
        for q in range(-1, 2):
            if obstacles[i+p][j+q]:
                return True
    return False


def pos_warning(i, j):
    """  Check if any of its neighbors is a dangerous spot """
    if (i == 0) or (i == ROWS-1) or (j == 0) or (j == COLS-1):
        return True

    for p in range(-1, 2):
        for q in range(-1, 2):
            if pos_dangerous(i+p, j+q):
                return True
    return False


def fix_pos(i, j, ii, jj):
    """ If the new position (ii, jj) is out of bounds, or it
    is colliding with an obstacle, return the previous one. """
    return (i, j) if pos_illegal(ii, jj) else (ii, jj)


def reward(i, j, move):
    """ Compute the reward associated to Move move
    starting from position (i,j) """
    ii, jj = next_pos(i, j, move)
    if pos_out_of_box(ii, jj):
        return REWARD_OUT_OF_BOX
    elif obstacles[ii][jj]:
        return REWARD_COLLISION
    elif (ii, jj) == target:
        return REWARD_TARGET
    elif pos_dangerous(ii, jj):
        return REWARD_DANGER
    elif pos_warning(ii, jj):
        return REWARD_WARNING
    elif ii == i or jj == j:
        return REWARD_STEP_DIRECT
    else:
        return REWARD_STEP_DIAG
    #else:
        #return REWARD_STEP


# Reward matrix
R = [[[reward(i, j, m) for m in Move] for j in range(COLS)] for i in range(ROWS)]

# Quality matrix
Q = [[[0 for m in Move] for j in range(COLS)] for i in range(ROWS)]


def get_Q_value(i, j, move):
    """ Get Q value associated to position (i, j) and action move """
    return Q[i][j][move.value]


def get_R_value(i, j, move):
    """ Get reward value associated to state (i, j) and action move """
    return  R[i][j][move.value]


def random_action():
    """ Generate a random action """
    return Move(randrange(len(Move)))


def greedy_action(i, j):
    """ Return the presumably best action from state (i, j) """
    return max(list(Move), key=lambda m: Q[i][j][m.value])


def eps_greedy_action(i, j, eps):
    """ Generate an action from state (i, j) according to the
    epsilon-greedy policy """
    if random() < eps:
        return random_action()
    else:
        return greedy_action(i, j)


def reduce_eps(eps):
    return 0.05 + (eps - 0.05)*0.995


def train_SARSA(n_episodes):
    """ Train the network for the specified number of episodes """
    epsilon = EPSILON
    for ep in range(n_episodes):
        if not ep % 100:
            print("Training... " + str(ep))
        # Random initial state and action (must be legal)
        while True:
            i, j = randrange(ROWS), randrange(COLS)
            if not pos_illegal(i, j):
                break
        a = random_action()

        # While not in goal state
        while (i, j) != target:
            # Get reward
            r = get_R_value(i, j, a)
            # Compute next state
            ii, jj = next_pos(i, j, a)
            # Adjust next state
            ii, jj = fix_pos(i, j, ii, jj)
            # Choose an action in the new state
            aa = eps_greedy_action(ii, jj, epsilon)
            # Update Q
            Q[i][j][a.value] = (1-ALPHA) * Q[i][j][a.value] + ALPHA * (r + GAMMA * Q[ii][jj][aa.value])
            # Prepare for next iteration
            i, j, a = ii, jj, aa
        epsilon = reduce_eps(epsilon)


def train_Q_learning(n_episodes):
    """ Train the network for the specified number of episodes """
    epsilon = EPSILON
    for ep in range(n_episodes):
        if not ep % 100:
            print("Training... " + str(ep))
        # Random initial state and action (must be legal)
        while True:
            i, j = randrange(ROWS), randrange(COLS)
            if not pos_illegal(i, j):
                break

        # While not in goal state
        while (i, j) != target:
            a = eps_greedy_action(i, j, epsilon)
            # Get reward
            r = get_R_value(i, j, a)
            # Compute next state
            ii, jj = next_pos(i, j, a)
            # Adjust next state
            ii, jj = fix_pos(i, j, ii, jj)
            #print("Going from (" + str(i) + ", " + str(j) + ") to (" + str(ii) + ", " + str(jj) + ")")
            # Update Q
            Q[i][j][a.value] = (1-ALPHA) * Q[i][j][a.value] + ALPHA * (r + GAMMA * max(Q[ii][jj]))
            # Prepare for next iteration
            i, j = ii, jj
        epsilon = reduce_eps(epsilon)


def find_optimal_path(i_start, j_start):
    """ Compute the optimal path from a starting point.
    Returns it as list of consecutive points. """
    i, j = i_start, j_start
    path = [(i, j)]
    while (i, j) != target:
        a = greedy_action(i, j)
        ii, jj = next_pos(i, j, a)
        ii, jj = fix_pos(i, j, ii, jj)
        path.append((ii, jj))
        i, j = ii, jj
    return path


def get_appropriate_symbol(i, j):
    if obstacles[i][j]:
        return '█'
    elif (i, j) == target:
        return '¤'
    else:
        return symbols[greedy_action(i, j)][1]


def plot_map():
    for i in range(ROWS):
        for j in range(COLS):
            print(get_appropriate_symbol(i, j), end='')
        print('')


def print_on_file(m):
    path = './codes.txt'
    out = open(path, 'w')
    table = str.maketrans("[]","{}")
    out.write("{\n" + ', \n'.join([str(row).translate(table) for row in m]) + "\n}")
    out.close()


if __name__ == "__main__":

    # Training phase
    train_Q_learning(100000)

    plot_map()

    decision_codes = [[greedy_action(i, j).value for j in range(COLS)] for i in range(ROWS)]
    print(decision_codes)
    print_on_file(decision_codes)
