#include <random>
#include <string>

#include "game_io/game_io.h"
#include "game_io/json_response.h"
#include "game_io/move_request.h"
#include "game_io/take_space_response.h"
#include "game_io/wait_response.h"
#include "game_io/greeting.h"
#include "game_io/game_result.h"

using namespace std;

class client : public game_player {
    public:
        client() : random_wait(0,1) {}
        virtual void error(error_msg*);
        virtual move_response* move(move_request*);
        virtual void server_greeting(greeting*);
        virtual void move_received(move_result*);
        virtual void hand_done(move_result*);
        virtual void game_over(game_result*);
    private:
        default_random_engine random_generator;
        uniform_int_distribution<int> random_wait;
};

struct GameError : exception {};
