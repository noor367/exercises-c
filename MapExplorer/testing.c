int lose_boulder(struct location map[SIZE][SIZE], int play_row, int play_col) {
    
    int up;
    int down;
    int left;
    int right;
    int end;
    
    if (play_row < SIZE && play_row >= SIZE_LOWEST) {
        if (map[play_row++][play_col].occupier == BOULDER_TYPE) {
            down = LOSE;
        } else {
            down = WIN;
        }
    } else {
        down = LOSE;
    }
    
    if (play_row-- < SIZE && play_row-- >= SIZE_LOWEST) {
        if (map[play_row--][play_col].occupier == BOULDER_TYPE) {
            up = LOSE;
        } else {
            up = WIN;
        }
    } else {
        up = LOSE;
    }    
    
    if ((play_col++ < SIZE) && (play_col++ >= SIZE_LOWEST)) {
        if (map[play_row][play_col++].occupier == BOULDER_TYPE) {
            right = LOSE;
        } else {
            right = WIN;
        }
    } else {
        right = LOSE;
    }    
    
    if (play_col-- < SIZE && play_col-- >= SIZE_LOWEST) {
        if (map[play_row][play_col--].occupier == BOULDER_TYPE) {
            left = LOSE;
        } else {
            left = WIN;
        }
    } else {
        left = LOSE;
    }
    
    end = up + down + right + left;
    
    if (end == LOSE) {
        return LOSE;
    } else {
        return 1;
    }
    
}