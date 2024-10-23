// Assignment 0 cs_bowling.c
// For submission on March 7th 2022
// A program to calculate the scores of a 10 frame bowling game

#include <stdio.h>

#define NUM_FRAMES 10

#define HIGHEST_BOWL 10
#define LOWEST_BOWL 0

#define STRIKE 10
#define SPARE 10

#define TRUE 1
#define FALSE 0

int main(void) {
    
    printf("Welcome to CS Bowling!\n");
    
    int bowl_one;
    int bowl_two;
    int score;
    int bonus_bowl = 0;
    
    int frame = 1;
    int total = 0;
    
    int spare_bonus = FALSE;
    int strike_bonus = FALSE;
    
    while (frame <= NUM_FRAMES) {
        printf("Frame %d, Bowl 1: ", frame);
        scanf("%d", &bowl_one);
        
        if  (bowl_one == STRIKE) {
            score = STRIKE;
            printf("Score for Frame: %d\n", score);
            printf("Strike! Bonus for this frame is next two rolls.\n");
            bowl_two = 0;
            
        } else if (bowl_one > HIGHEST_BOWL || bowl_one < LOWEST_BOWL) {
            printf("Bowl 1 invalid!\n");
            bowl_one = LOWEST_BOWL;
            printf("Frame %d, Bowl 2: ", frame);
            scanf("%d", &bowl_two);
            if (bowl_two > HIGHEST_BOWL || bowl_two < LOWEST_BOWL) {
                printf("Bowl 2 invalid!\n"); 
            } else if ((bowl_one < HIGHEST_BOWL && bowl_one >= LOWEST_BOWL)
             &&((bowl_two > (HIGHEST_BOWL - bowl_one)) 
             || bowl_two < LOWEST_BOWL)) {
                printf("Bowl 2 invalid!\n");
            } 
        } else { 
            printf("Frame %d, Bowl 2: ", frame);
            scanf("%d", &bowl_two);
            if (bowl_two > HIGHEST_BOWL || bowl_two < LOWEST_BOWL) {
                printf("Bowl 2 invalid!\n"); 
            } else if ((bowl_one < HIGHEST_BOWL && bowl_one >= LOWEST_BOWL)
              &&((bowl_two > (HIGHEST_BOWL - bowl_one)) 
              || bowl_two < LOWEST_BOWL)) {
                printf("Bowl 2 invalid!\n");
            } 
        }
        
        score = bowl_one + bowl_two;
        
        if ((bowl_one > HIGHEST_BOWL || bowl_one < LOWEST_BOWL)
        &&(bowl_two > HIGHEST_BOWL || bowl_two < LOWEST_BOWL)) {
            score = LOWEST_BOWL;
            printf("Score for Frame: %d\n", score);
        } else if ((bowl_one > HIGHEST_BOWL || bowl_one < LOWEST_BOWL)
          &&(bowl_two < HIGHEST_BOWL && bowl_two >= LOWEST_BOWL)) {
            score = bowl_two;
            printf("Score for Frame: %d\n", score);
        } else if ((bowl_one < HIGHEST_BOWL && bowl_one >= LOWEST_BOWL )
          &&(bowl_two > HIGHEST_BOWL || bowl_two < LOWEST_BOWL)) {
            score = bowl_one;
            printf("Score for Frame: %d\n", score);
        } else if ((bowl_one < HIGHEST_BOWL && bowl_one >= LOWEST_BOWL )
          &&(bowl_two <= (HIGHEST_BOWL - bowl_one) 
          && bowl_two >= LOWEST_BOWL)) {
            printf("Score for Frame: %d\n", score);
        } else if ((bowl_one < HIGHEST_BOWL && bowl_one >= LOWEST_BOWL)
          &&(bowl_two > (HIGHEST_BOWL - bowl_one) || bowl_two < LOWEST_BOWL)) {
            score = bowl_one;
            printf("Score for Frame: %d\n", score);
        }
        
        if (spare_bonus == TRUE) {
            total = total + score + bowl_one;
            spare_bonus = FALSE;
        } else if (strike_bonus == TRUE) {
            total = total + score + score;
            strike_bonus = FALSE;
        } else {
            total = total + score;
        }
        
        if ((score == SPARE)&&(bowl_one != STRIKE)) {
            printf("Spare! Bonus for this frame is next roll.\n");
            spare_bonus = TRUE;
        } else if ((score == STRIKE)&&(spare_bonus == FALSE)) {
            strike_bonus = TRUE;
        }
        
        while (frame == NUM_FRAMES) {
            if (spare_bonus == TRUE || strike_bonus == TRUE) {
                printf("Bonus Bowl 1: ");
                scanf("%d", &bonus_bowl);
            }
    
            if (bonus_bowl > HIGHEST_BOWL || bonus_bowl < LOWEST_BOWL) {
                printf("Bonus Bowl Invalid!\n");
                bonus_bowl = 0;
            }
            frame = frame + 1;
        }             
        
        frame = frame + 1;
        
    }
    
    total = total + (2 * bonus_bowl); 
    
    printf("Total Score: %d\n", total);
        
    return 0;
}
