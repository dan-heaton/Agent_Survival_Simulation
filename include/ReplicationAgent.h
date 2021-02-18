#ifndef REPLICATIONAGENT_H
#define REPLICATIONAGENT_H

#include "../include/AdvancedAgent.h"


class ReplicationAgent: public AdvancedAgent {

    private:
        int speed;
        int energy_to_replicate;
        int num_lookaheads;
        int min_energy_to_replicate;
        int max_energy_to_replicate; 
        int max_num_lookaheads;
        void find_outbounds_directions(vector <vector <int>> search_positions);
        void lookahead_reset(int original_x_pos, int original_y_pos, vector <vector <char>> original_environment_tiles, 
                             map <char, int> original_blocked_directions, vector <vector <int>> original_energies_consumed);
        vector <int> lookahead(vector <vector <int>> search_positions, int num_lookaheads);

    public:
        ReplicationAgent(string name, int min_energy_to_replicate, int max_energy_to_replicate, 
                         int max_num_lookaheads, int x_bound=100, int y_bound=100);
        void seek_energy();
        bool check_replicate();
        map <string, int> generate_mutations();
        void mutate(map<string, int> mutations_dict);
};


#endif