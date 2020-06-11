#include "human.h"

Human::Human() : animation_move(HUMAN_WIDTH, HUMAN_HEIGHT, TOTAL_CLIPS) {}

Human::~Human() {}

SDL_Rect Human::move(move_t move_type) {
    return animation_move.getNextClip(move_type);
}

SDL_Rect Human::getFaceProfile(move_t move_type) {
    SDL_Rect face_profile = {move_type * HEAD_WIDTH, 0, HEAD_WIDTH,
                             HEAD_HEIGHT};
    return face_profile;
}