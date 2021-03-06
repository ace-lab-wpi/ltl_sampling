#include <random>
#include <algorithm>
#include <bitset>
#include <climits>

#include "multi_sampling/multi_sampling_dubins.h"

double MultiSamplingDubins::get_dist_dubins(std::vector<double> states_1, std::vector<double> states_2, double min_radius) {
    double min_length = DubinsPath::GetDubinsPathLength(states_1, states_2, min_radius);
    return min_length;
}

double MultiSamplingDubins::get_dist_dubins(std::vector<std::vector<double>> states_1, std::vector<std::vector<double>> states_2, double min_radius){
    if (states_1.size() != states_2.size()){
        return -1;
    }
    double dist = 0;
    for (int i = 0; i < states_1.size(); i++) {
        dist = dist + DubinsPath::GetDubinsPathLength(states_1[i], states_2[i], min_radius);
    }
    return dist;
}

double MultiSamplingDubins::fRand(double fMin, double fMax)
{
    std::uniform_real_distribution<double> dist(fMin, fMax);
    std::mt19937 rng;
    rng.seed(std::random_device{}());
    return dist(rng);
}

void MultiSamplingDubins::set_num_agent(int num_agent){
    num_agent_ = num_agent;
}

std::vector<int> MultiSamplingDubins::sample_from_ba(BAStruct buchi, SampleSpace &sample_space)
{
    if (sample_space.total_sample_num() == 0) {
        return std::vector<int>();
    }
    int size_buchi = buchi.state_num;
    std::vector<uint32_t> ba_accept_state = buchi.acc_state_idx;
    int new_ba_sample = rand() % (size_buchi);
    std::vector<int> new_ba_candidate;
    std::vector<int> temp_candidate;
    std::vector<std::vector<int>> act_q;
    if (new_ba_sample == ba_accept_state[0])
    {
        for (int i = 0; i < size_buchi; i++)
        {
            if (i != new_ba_sample && !buchi.trans_con[i][new_ba_sample].empty())
            {
                temp_candidate.push_back(i);
                if (sample_space.get_sub_space(i).num_samples() > 0)
                {
                    new_ba_candidate.push_back(i);
                    act_q.push_back({i, new_ba_sample});
                }
            }
        }
    }
    else
    {

        temp_candidate.push_back(new_ba_sample);
        if (sample_space.get_sub_space(new_ba_sample).num_samples() > 0)
        {
            new_ba_candidate.push_back(new_ba_sample);
            act_q.push_back({new_ba_sample, new_ba_sample});
        }
    }
    while (new_ba_candidate.empty())
    {
        std::vector<int> new_temp_candidate;
        for (int i = 0; i < temp_candidate.size(); i++)
        {
            for (int j = 0; j < size_buchi; j++)
            {
                if (j != new_ba_sample && !buchi.trans_con[j][temp_candidate[i]].empty() && std::find(temp_candidate.begin(), temp_candidate.end(), j) == temp_candidate.end())
                {
                    new_temp_candidate.push_back(j);
                    if (sample_space.get_sub_space(j).num_samples() > 0)
                    {
                        new_ba_candidate.push_back(j);
                        act_q.push_back({j, temp_candidate[i]});
                    }
                }
            }
        }
        temp_candidate = new_temp_candidate;
    }
    int r = rand() % (new_ba_candidate.size());
    int new_ba_state = new_ba_candidate[r];
    return act_q[r];
}


void MultiSamplingDubins::buchi_post (BAStruct &ba, std::vector<int> indep_set) {
    if (indep_set.size() == 1 || indep_set.size() == 0){
        return;
    }
    std::bitset<32> indep_bitset;
    for (auto it = indep_set.begin(); it != indep_set.end(); it++) {
        indep_bitset.set(*it, 1);
    }
    int alphabet_set_size = ba.alphabet_set.size();
    std::vector<int> indep_alphabet;
    for (int i = 0; i < alphabet_set_size; i++) {
        std::bitset<32> check_indep = std::bitset<32>(ba.alphabet_set[i]) & indep_bitset;
        if (check_indep.count() > 1) {
            indep_alphabet.push_back(i);
        }
    }

    for (int i = 0; i < ba.state_num; i++)
    {
        for (auto it = ba.trans_con[i].begin(); it != ba.trans_con[i].end(); it++)
        {
            std::vector<int> inter;
            std::set_intersection((*it).begin(), (*it).end(), indep_alphabet.begin(), indep_alphabet.end(), std::back_inserter(inter));
            (*it).erase(std::set_difference((*it).begin(), (*it).end(), inter.begin(), inter.end(), (*it).begin()), (*it).end());
        }
    }

}

bool MultiSamplingDubins::if_in_region (std::vector<double> state, Region region) {
    if (state[0] > region.get_x_position().first && state[0] < region.get_x_position().second && state[1] > region.get_y_position().first && state[1] < region.get_y_position().second) {
        return true;
    }
    else {
        return false;
    }
}

std::vector<double> MultiSamplingDubins::step_from_to (MultiSampleNode parent_sample, std::vector<double> sampled_state, DubinsPath::PathData& dubins_steer_data, double EPSILON_) {
    dubins_steer_data = DubinsPath::GetDubinsPathPointWise(parent_sample.get_state(), sampled_state, min_radius_, path_step_);
    // if (dubins_steer_data.traj_point_wise.size() < 30) {
    //     std::cout << "something wrong" << std::endl;
    //     return std::vector<double>();
    // }
    if (dubins_steer_data.traj_length < EPSILON_) {
        return sampled_state;
    }
    else {
        int id = 0;
        for (int i = 0; i < dubins_steer_data.traj_len_map.size(); i++) {
            if (dubins_steer_data.traj_len_map[i] > EPSILON_) {
                id = i;
                break;
            }
        }
        std::vector<double> new_state = dubins_steer_data.traj_point_wise[id];
        std::vector<std::vector<double>> new_traj(dubins_steer_data.traj_point_wise.begin(), dubins_steer_data.traj_point_wise.begin() + id);
        // traj_point_wise = new_traj;
        dubins_steer_data.traj_point_wise = new_traj;
        dubins_steer_data.traj_length = dubins_steer_data.traj_len_map[id];
        return new_state;
    }
}

std::vector<std::vector<double>> MultiSamplingDubins::step_from_to (MultiSampleNode parent_sample, std::vector<std::vector<double>> all_sampled_states, std::vector<DubinsPath::PathData>& all_dubins_steer_data, double EPSILON){
    all_dubins_steer_data.clear();
    for (int i = 0; i < num_agent_; i++){
        DubinsPath::PathData dubins_steer_data = DubinsPath::GetDubinsPathPointWise(parent_sample.get_all_states()[i], all_sampled_states[i], min_radius_, path_step_);
        all_dubins_steer_data.push_back(dubins_steer_data);
    }
    std::vector<std::vector<double>> all_new_states;
    for (int k = 0; k < num_agent_; k++){
        if (all_dubins_steer_data[k].traj_length < EPSILON_){
            all_new_states.push_back(all_sampled_states[k]);
        }
        else{
            int id = 0;
            for (int i = 0; i < all_dubins_steer_data[k].traj_len_map.size(); i++) {
                if (all_dubins_steer_data[k].traj_len_map[i] > EPSILON_) {
                    id = i;
                    break;
                }
            }
            std::vector<double> new_state = all_dubins_steer_data[k].traj_point_wise[id];
            std::vector<std::vector<double>> new_traj(all_dubins_steer_data[k].traj_point_wise.begin(), all_dubins_steer_data[k].traj_point_wise.begin() + id);
            // traj_point_wise = new_traj;
            all_dubins_steer_data[k].traj_point_wise = new_traj;
            all_dubins_steer_data[k].traj_length = all_dubins_steer_data[k].traj_len_map[id];
            all_new_states.push_back(new_state);
        }
    }
    return all_new_states;
}


int MultiSamplingDubins::step_from_to_buchi (int paraent_ba, std::vector<double> new_sample_state, BAStruct ba, std::map<int, Region> all_interest_regions) {
    std::bitset<32> bit_set;
    // std::cout << "*******in step from to buchi function: ******" << std::endl;
    int buchi_num = paraent_ba;
    for (int i = 0; i < all_interest_regions.size(); i++) {
        if (if_in_region(new_sample_state, all_interest_regions.find(i)->second)) {
            bit_set.set(all_interest_regions.find(i)->second.get_region_interest());
            break;
        }
    }
    int act = bit_set.to_ullong();
    for (int i = 0; i < ba.trans_con[paraent_ba].size(); i++) {
        if (std::find(ba.trans_con[paraent_ba][i].begin(), ba.trans_con[paraent_ba][i].end(), act) != ba.trans_con[paraent_ba][i].end()) {
            buchi_num = i;
            break;
        }
    }
    // std::cout << "new ba: " << buchi_num << std::endl;
    // std::cout << "(step_from_to_buchi function) out buchi state: " << buchi_num << std::endl;
    // std::cout << "(step_from_to_buchi function) test bit set: " << bit_set << std::endl;
    // std::cout << "(step_from_to_buchi function) test int: " << bit_set.to_ullong() << std::endl;
    // std::cout << "****************************************" << std::endl;
    return buchi_num;
}

int MultiSamplingDubins::step_from_to_buchi (int paraent_ba, std::vector<std::vector<double>> all_new_sample_state, BAStruct ba, std::map<int, Region> all_interest_regions){
    std::bitset<32> bit_set;
    int buchi_num = paraent_ba;
    for (int k = 0; k < all_new_sample_state.size(); k++){
        for (int i = 0; i < all_interest_regions.size(); i++) {
            if (if_in_region(all_new_sample_state[k], all_interest_regions.find(i)->second)) {
                bit_set.set(all_interest_regions.find(i)->second.get_region_interest());
                break;
            }
        }
    }
    
    int act = bit_set.to_ullong();
    for (int i = 0; i < ba.trans_con[paraent_ba].size(); i++) {
        if (std::find(ba.trans_con[paraent_ba][i].begin(), ba.trans_con[paraent_ba][i].end(), act) != ba.trans_con[paraent_ba][i].end()) {
            buchi_num = i;
            break;
        }
    }
    // std::cout << "new ba: " << buchi_num << std::endl;
    // std::cout << "(step_from_to_buchi function) out buchi state: " << buchi_num << std::endl;
    // std::cout << "(step_from_to_buchi function) test bit set: " << bit_set << std::endl;
    // std::cout << "(step_from_to_buchi function) test int: " << bit_set.to_ullong() << std::endl;
    // std::cout << "****************************************" << std::endl;
    return buchi_num;
}

void MultiSamplingDubins::read_formula(std::string ltl_formula, std::vector<std::string> buchi_regions, std::vector<int> indep_set) {
    SpotHoaInterpreter ltl2ba_lib;
    ba_ = ltl2ba_lib.GetBuchi(ltl_formula, buchi_regions);
    buchi_post(ba_, indep_set);
}
void MultiSamplingDubins::init_workspace(double work_space_size_x, double work_space_size_y) {
    work_space_size_x_ = work_space_size_x;
    work_space_size_y_ = work_space_size_y;
}

// void MultiSamplingDubins::init_parameter(double EPSILON, double RADIUS, double radius_L, double radius_R){
//     EPSILON_ = EPSILON;
//     RADIUS_ = RADIUS;
//     radius_L_ = radius_L;
//     radius_R_ = radius_R;
// }

void MultiSamplingDubins::init_parameter(double EPSILON, double RADIUS, double min_radius, double ground_speed, double time_step, double collision_check_rate){
    EPSILON_ = EPSILON;
    RADIUS_ = RADIUS;
    min_radius_ = min_radius;
    ground_speed_ = ground_speed;
    time_step_ = time_step;
    path_step_ = time_step_*ground_speed_;
    collision_check_rate_ = collision_check_rate_;
}

std::vector<double> MultiSamplingDubins::sample_state(std::vector<int> ba_act) {
    double new_node_x = 0;
    double new_node_y = 0;
    if (ba_act[0] == ba_act[1]) {
        new_node_x = fRand(0, work_space_size_x_);
        new_node_y = fRand(0, work_space_size_y_);
    }
    else {
        int interest_id = 0;
        int act = ba_.trans_con[ba_act[0]][ba_act[1]].front();
        std::bitset<32> act_bit = std::bitset<32>(act);
        for (int i = 0; i < act_bit.size(); i++) {
            if (act_bit.test(i)){
                interest_id = i;
                break;
            }
        }
        double new_node_x_min = all_interest_regions_.find(interest_id)->second.get_x_position().first;
        double new_node_x_max = all_interest_regions_.find(interest_id)->second.get_x_position().second;
        double new_node_y_min = all_interest_regions_.find(interest_id)->second.get_y_position().first;
        double new_node_y_max = all_interest_regions_.find(interest_id)->second.get_y_position().second;
        new_node_x = fRand(new_node_x_min, new_node_x_max);
        new_node_y = fRand(new_node_y_min, new_node_y_max);
    }
    double yaw = fRand(0, M_PI*2);
    std::vector<double> sampled_position = {new_node_x, new_node_y, yaw}; 
    return sampled_position;
}

std::vector<std::vector<double>> MultiSamplingDubins::multi_sample_state (std::vector<int> ba_act){
    std::vector<std::vector<double>> multi_sampled_position;
    for (int i = 0; i < num_agent_; i++){
        double new_node_x = 0;
        double new_node_y = 0;
        if (ba_act[0] == ba_act[1]) {
            new_node_x = fRand(0, work_space_size_x_);
            new_node_y = fRand(0, work_space_size_y_);
        }
        else {
            int interest_id = 0;
            int act = ba_.trans_con[ba_act[0]][ba_act[1]].front();
            std::bitset<32> act_bit = std::bitset<32>(act);
            for (int i = 0; i < act_bit.size(); i++) {
                if (act_bit.test(i)){
                    interest_id = i;
                    break;
                }
            }
            double new_node_x_min = all_interest_regions_.find(interest_id)->second.get_x_position().first;
            double new_node_x_max = all_interest_regions_.find(interest_id)->second.get_x_position().second;
            double new_node_y_min = all_interest_regions_.find(interest_id)->second.get_y_position().first;
            double new_node_y_max = all_interest_regions_.find(interest_id)->second.get_y_position().second;
            new_node_x = fRand(new_node_x_min, new_node_x_max);
            new_node_y = fRand(new_node_y_min, new_node_y_max);
        }
        std::vector<double> sampled_position = {new_node_x, new_node_y}; 
        multi_sampled_position.push_back(sampled_position);
    }
    return multi_sampled_position;
}

std::vector<std::vector<double>> MultiSamplingDubins::multi_sample_state (BAStruct buchi, SampleSpace &sample_space){
   
    std::vector<std::vector<double>> multi_sampled_position;
    for (int i = 0; i < num_agent_; i++){
        std::vector<int> ba_act = sample_from_ba(buchi, sample_space);

        double new_node_x = 0;
        double new_node_y = 0;
        if (ba_act[0] == ba_act[1]) {
            new_node_x = fRand(0, work_space_size_x_);
            new_node_y = fRand(0, work_space_size_y_);
        }
        else {
            int interest_id = 0;
            int act = ba_.trans_con[ba_act[0]][ba_act[1]].front();
            std::bitset<32> act_bit = std::bitset<32>(act);
            for (int i = 0; i < act_bit.size(); i++) {
                if (act_bit.test(i)){
                    interest_id = i;
                    break;
                }
            }
            double new_node_x_min = all_interest_regions_.find(interest_id)->second.get_x_position().first;
            double new_node_x_max = all_interest_regions_.find(interest_id)->second.get_x_position().second;
            double new_node_y_min = all_interest_regions_.find(interest_id)->second.get_y_position().first;
            double new_node_y_max = all_interest_regions_.find(interest_id)->second.get_y_position().second;
            new_node_x = fRand(new_node_x_min, new_node_x_max);
            new_node_y = fRand(new_node_y_min, new_node_y_max);
        }
        std::vector<double> sampled_position = {new_node_x, new_node_y}; 
        multi_sampled_position.push_back(sampled_position);
    }
    return multi_sampled_position;
}

void MultiSamplingDubins::set_interest_region(std::pair <double, double> position_x, std::pair <double, double> position_y, int interest_id) {
    Region interest_region;
    interest_region.set_position(position_x, position_y);
    interest_region.set_region_interest(interest_id);
    all_interest_regions_[interest_id] = interest_region;
}


void MultiSamplingDubins::set_obstacle(std::pair <double, double> position_x, std::pair <double, double> position_y){
    Region obstacle;
    obstacle.set_position(position_x, position_y);
    all_obstacles_.push_back(obstacle);
}

void MultiSamplingDubins::set_init_state(std::vector<std::vector<double>> init_all_states) {
    all_space_.set_space(ba_.state_num);
    int init_ba = ba_.init_state_idx;
    int acc_ba = ba_.acc_state_idx.front();
    MultiSampleNode init_node;
    init_node.set_id(0);
    init_node.set_all_states(init_all_states);
    init_node.set_ba(ba_.init_state_idx);
    init_node.set_cost(0.0);
    all_space_.insert_sample(init_node, init_ba);
}

void MultiSamplingDubins::start_sampling(int iteration) {
    bool find_path = false;
    uint64_t first_acc_state_id;
    lcm::LCM lcm;
    for (int i = 0; i < iteration; i++) {
        if (i%100 == 0){
            std::cout << "iteration: " << i << std::endl;
        }
            
        // std::cout << "~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        // std::cout << "iteration: " << i << std::endl;
        std::vector<int> ba_act = sample_from_ba(ba_, all_space_);
        std::vector<std::vector<double>> sampled_position = multi_sample_state(ba_act);

        // std::vector<std::vector<double>> sampled_position = multi_sample_state(ba_, all_space_);

        std::vector<DubinsPath::PathData> multi_dubins_steer_data;
        MultiSampleNode parent_sample = all_space_.get_sub_space(ba_act[0]).get_parent_dubins(sampled_position, min_radius_);
        std::vector<std::vector<double>> new_sample_state = step_from_to(parent_sample, sampled_position, multi_dubins_steer_data, EPSILON_);

        if (Region::collision_check_multi_dubins(multi_dubins_steer_data, all_obstacles_, work_space_size_x_, work_space_size_y_, collision_check_rate_)) {
            continue;
        } 
        int new_ba = step_from_to_buchi(parent_sample.get_ba(), new_sample_state, ba_, all_interest_regions_);
        MultiSampleNode &chosen_parent_sample = all_space_.get_sub_space(parent_sample.get_ba()).rechoose_parent_dubins(parent_sample, new_sample_state, multi_dubins_steer_data, all_obstacles_, work_space_size_x_, work_space_size_y_, RADIUS_, min_radius_, path_step_, collision_check_rate_);
        // MultiSampleNode &chosen_parent_sample = parent_sample;
        MultiSampleNode new_node;
        uint64_t new_id = all_space_.get_sub_space(new_ba).num_samples();
        new_node.set_id(new_id);
        new_node.set_ba(new_ba);
        chosen_parent_sample.add_children_id(std::make_pair(new_ba, new_id));
        new_node.set_all_states(new_sample_state);
        double all_traj_length = 0;
        std::vector<std::vector<std::vector<double>>> multi_traj_point_wise;
        for (int i = 0; i < multi_dubins_steer_data.size(); i++){
            all_traj_length = all_traj_length + multi_dubins_steer_data[i].traj_length;
            multi_traj_point_wise.push_back(multi_dubins_steer_data[i].traj_point_wise);
        }
        new_node.set_traj_data(multi_dubins_steer_data);
        new_node.set_cost(chosen_parent_sample.get_cost() + all_traj_length);
        new_node.set_parent_ba(chosen_parent_sample.get_ba());
        new_node.set_parent_id(chosen_parent_sample.get_id());

        new_node.set_multi_traj(multi_traj_point_wise);

        all_space_.insert_sample(new_node, new_ba);

        all_space_.rewire_dubins(num_agent_, new_id, new_ba, all_obstacles_, work_space_size_x_, work_space_size_y_, RADIUS_, min_radius_, path_step_, collision_check_rate_);

        /// Vis for debug
        // sampling::sample_data node_data;
        // node_data.state[0] = new_sample_state[0];
        // node_data.state[1] = new_sample_state[1];
        // lcm.publish("SAMPLE", &node_data);

        // std::cout << "new ba state: " << new_ba << std::endl;
        // if (new_ba == ba_.acc_state_idx.front()) {
        //     // std::cout << "acc ba: " << ba.acc_state_idx.front() << std::endl;
        //     std::cout << "Find a solution!!!" << std::endl;
        //     first_acc_state_id = new_id;
        //     find_path = true;
        //     break;
        // }
    }
}

void MultiSamplingDubins::start_sampling() {
    bool find_path = false;
    uint64_t first_acc_state_id;
    lcm::LCM lcm;
    int i = 0;
    while (!find_path){
        i = i + 1;
        if (i%100 == 0){
            std::cout << "iteration: " << i << std::endl;
        }
                // std::cout << "~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        // std::cout << "iteration: " << i << std::endl;
        std::vector<int> ba_act = sample_from_ba(ba_, all_space_);
        std::vector<std::vector<double>> sampled_position = multi_sample_state(ba_act);

        // std::vector<std::vector<double>> sampled_position = multi_sample_state(ba_, all_space_);

        std::vector<DubinsPath::PathData> multi_dubins_steer_data;
        MultiSampleNode parent_sample = all_space_.get_sub_space(ba_act[0]).get_parent_dubins(sampled_position, min_radius_);
        std::vector<std::vector<double>> new_sample_state = step_from_to(parent_sample, sampled_position, multi_dubins_steer_data, EPSILON_);

        if (Region::collision_check_multi_dubins(multi_dubins_steer_data, all_obstacles_, work_space_size_x_, work_space_size_y_, collision_check_rate_)) {
            continue;
        } 
        int new_ba = step_from_to_buchi(parent_sample.get_ba(), new_sample_state, ba_, all_interest_regions_);
        MultiSampleNode &chosen_parent_sample = all_space_.get_sub_space(parent_sample.get_ba()).rechoose_parent_dubins(parent_sample, new_sample_state, multi_dubins_steer_data, all_obstacles_, work_space_size_x_, work_space_size_y_, RADIUS_, min_radius_, path_step_, collision_check_rate_);
        // MultiSampleNode &chosen_parent_sample = parent_sample;
        MultiSampleNode new_node;
        uint64_t new_id = all_space_.get_sub_space(new_ba).num_samples();
        new_node.set_id(new_id);
        new_node.set_ba(new_ba);
        chosen_parent_sample.add_children_id(std::make_pair(new_ba, new_id));
        new_node.set_all_states(new_sample_state);
        double all_traj_length = 0;
        std::vector<std::vector<std::vector<double>>> multi_traj_point_wise;
        for (int i = 0; i < multi_dubins_steer_data.size(); i++){
            all_traj_length = all_traj_length + multi_dubins_steer_data[i].traj_length;
            multi_traj_point_wise.push_back(multi_dubins_steer_data[i].traj_point_wise);
        }
        new_node.set_traj_data(multi_dubins_steer_data);
        
        new_node.set_cost(chosen_parent_sample.get_cost() + all_traj_length);
        new_node.set_parent_ba(chosen_parent_sample.get_ba());
        new_node.set_parent_id(chosen_parent_sample.get_id());

        new_node.set_multi_traj(multi_traj_point_wise);

        all_space_.insert_sample(new_node, new_ba);

        all_space_.rewire_dubins(num_agent_, new_id, new_ba, all_obstacles_, work_space_size_x_, work_space_size_y_, RADIUS_, min_radius_, path_step_, collision_check_rate_);

        if (all_space_.get_sub_space(ba_.acc_state_idx.front()).num_samples() > 0) {
            find_path = true;
        }
    }
}

std::vector<std::vector<std::vector<double>>> MultiSamplingDubins::get_path() {
    lcm::LCM lcm;
    bool find_path = false;
    int current_ba = ba_.acc_state_idx.front();
    int init_ba = ba_.init_state_idx;

    std::vector<std::vector<std::vector<double>>> path_nodes_sq;
    path_nodes_sq.resize(num_agent_);
    if (all_space_.get_sub_space(current_ba).num_samples() > 0) {
        find_path = true;
    }
    else {
        std::cout << "No feasible path found. " << std::endl;
        return path_;
    }
    double total_cost = INT_MAX;
    uint64_t current_id = 1;
    if (find_path) {
        MultiSampleNode min_cost_sample = all_space_.get_sub_space(ba_.acc_state_idx.front()).get_min_cost_sample();
        // sampling::sample_data node_data;
        // node_data.state[0] = min_cost_sample.get_state()[0];
        // node_data.state[1] = min_cost_sample.get_state()[1];
        // lcm.publish("SAMPLE", &node_data);

        std::cout << "Path cost is: " << min_cost_sample.get_cost() << std::endl;
        current_id = min_cost_sample.get_id();
        while (current_ba != init_ba || current_id != 0) {
            MultiSampleNode current_node = all_space_.get_sub_space(current_ba).get_sample(current_id);
            // sampling::sample_data node_data;
            // node_data.state[0] = current_node.get_state()[0];
            // node_data.state[1] = current_node.get_state()[1];
            // lcm.publish("SAMPLE", &node_data);
            std::vector<std::vector<std::vector<double>>> temp = current_node.get_multi_traj();
            // traj_point_wise.pop_back();
            // std::vector<double> traj_len_map = temp_data.traj_len_map;
            for (int k = 0; k < num_agent_; k++){
                std::reverse(temp[k].begin(), temp[k].end());
                path_nodes_sq[k].insert(path_nodes_sq[k].end(), temp[k].begin(), temp[k].end() );
            }
            // std::reverse(temp.begin(), temp.end());
            // path_nodes_sq.insert( path_nodes_sq.end(), temp.begin(), temp.end() );
            current_ba = current_node.get_parent_ba();
            current_id = current_node.get_parent_id();
            
        }
        for (int k = 0; k < num_agent_; k++){
            std::reverse(path_nodes_sq[k].begin(), path_nodes_sq[k].end());
        } 
        // std::reverse(path_nodes_sq.begin(), path_nodes_sq.end());
    
    }
    path_ = path_nodes_sq;
    return path_nodes_sq;
}



std::vector<std::vector<WayPoint>> MultiSamplingDubins::get_waypoints() {
    lcm::LCM lcm;
    bool find_path = false;
    int current_ba = ba_.acc_state_idx.front();
    int init_ba = ba_.init_state_idx;

    // std::vector<std::vector<double>> path_nodes_sq;
    std::vector<std::vector<WayPoint>> path_way_points;
    std::vector<std::vector<DubinsPath::PathData>> way_points_data;
    if (all_space_.get_sub_space(current_ba).num_samples() > 0) {
        find_path = true;
    }
    else {
        std::cout << "No feasible path found. " << std::endl;
        return path_way_points_;
    }
    
    double total_cost = INT_MAX;
    uint64_t current_id = 1;
    if (find_path) {
        MultiSampleNode min_cost_sample = all_space_.get_sub_space(ba_.acc_state_idx.front()).get_min_cost_sample();
        // sampling::sample_data node_data;
        // node_data.state[0] = min_cost_sample.get_state()[0];
        // node_data.state[1] = min_cost_sample.get_state()[1];
        // lcm.publish("SAMPLE", &node_data);

        std::cout << "Path cost is: " << min_cost_sample.get_cost() << std::endl;
        current_id = min_cost_sample.get_id();
        while (current_ba != init_ba || current_id != 0) {
            MultiSampleNode current_node = all_space_.get_sub_space(current_ba).get_sample(current_id);
            // sampling::sample_data node_data;
            // node_data.state[0] = current_node.get_state()[0];
            // node_data.state[1] = current_node.get_state()[1];
            // lcm.publish("SAMPLE", &node_data);
            std::vector<DubinsPath::PathData> temp_data = current_node.get_traj_data();
            way_points_data.push_back(temp_data);

            // std::vector<std::vector<double>> temp = current_node.get_traj();
            // std::reverse(temp.begin(), temp.end());
            // path_nodes_sq.insert( path_nodes_sq.end(), temp.begin(), temp.end() );
            current_ba = current_node.get_parent_ba();
            current_id = current_node.get_parent_id();
            
        }
        // std::reverse(path_nodes_sq.begin(), path_nodes_sq.end());
        std::reverse(way_points_data.begin(), way_points_data.end());
        
        for (int k = 0; k < num_agent_; k++){
            double start = 0;
            std::vector<WayPoint> path_way_points_single;
            for (int i = 0; i < way_points_data.size(); i++){
                DubinsPath::PathData temp_data = way_points_data[i][k];
                std::vector<std::vector<double>> traj_point_wise = temp_data.traj_point_wise;
                traj_point_wise.pop_back();
                std::vector<double> traj_len_map = temp_data.traj_len_map;
                traj_len_map.pop_back();
                for (int j = 0; j < traj_point_wise.size(); j++){
                    double x = traj_point_wise[j][0];
                    double y = traj_point_wise[j][1];
                    double time_traj = traj_len_map[j] + start;
                    WayPoint way_point;
                    way_point.x = x;
                    way_point.y = y;
                    way_point.t = time_traj/ground_speed_;
                    // std::cout << "x: " << x << ", y: " << y << ", time: "<< time_traj/ground_speed_ << std::endl;
                    path_way_points_single.push_back(way_point);
                }
                start = start + traj_len_map.back();
            }
            path_way_points.push_back(path_way_points_single);
        }

        // for (int i = 0; i < way_points_data.size(); i++){
        //     // std::cout << "~~~~~~" << std::endl;
        //     DubinsPath::PathData temp_data = way_points_data[i];
        //     std::vector<std::vector<double>> traj_point_wise = temp_data.traj_point_wise;
        //     traj_point_wise.pop_back();
        //     std::vector<double> traj_len_map = temp_data.traj_len_map;
        //     traj_len_map.pop_back();
        //     for (int j = 0; j < traj_point_wise.size(); j++){
        //         double x = traj_point_wise[j][0];
        //         double y = traj_point_wise[j][1];
        //         double time_traj = traj_len_map[j] + start;
        //         WayPoint way_point;
        //         way_point.x = x;
        //         way_point.y = y;
        //         way_point.t = time_traj/ground_speed_;
        //         // std::cout << "x: " << x << ", y: " << y << ", time: "<< time_traj/ground_speed_ << std::endl;
        //         path_way_points.push_back(way_point);
        //     }
        //     start = start + traj_len_map.back();
        // }

    }
    // path_ = path_nodes_sq;
    return path_way_points;
}



double MultiSamplingDubins::get_path_length(){
    if (all_space_.get_sub_space(ba_.acc_state_idx.front()).num_samples() > 0) {
        MultiSampleNode min_cost_sample = all_space_.get_sub_space(ba_.acc_state_idx.front()).get_min_cost_sample();
        return min_cost_sample.get_cost();
    }
    else {
        return INT_MAX;
    }
}