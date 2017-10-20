#include <cmath>
#include <utility>
#include <algorithm>
#include <queue>

#include "multi_sampling/sample_space.h"
#include "trajectory/dubins_steer.h"
SampleSpace::SampleSpace(int num_ba) {
    num_ba_ = num_ba;
    for (int i = 0; i < num_ba; i++) {
        SubSampleSpace new_sub_space;
        sub_sample_space_.push_back(new_sub_space);
        sample_space_ltl_map_[i] = new_sub_space;
    }
}

double SampleSpace::get_dist(std::vector<double> states_1, std::vector<double> states_2) {
    double dist = sqrt(pow(states_1[0] - states_2[0], 2) + pow(states_1[1] - states_2[1], 2));
    return dist;
}

double SampleSpace::get_dist_dubins(std::vector<double> states_1, std::vector<double> states_2, double radius_L, double radius_R) {
    double min_length = DubinsSteer::GetDubinsCurveLength(states_1, states_2, radius_L, radius_R);
    return min_length;
}


double SampleSpace::get_dist(MultiSampleNode multi_sample_1, MultiSampleNode multi_sample_2){
    std::vector<std::vector<double>> states_1 = multi_sample_1.get_all_states();
    std::vector<std::vector<double>> states_2 = multi_sample_2.get_all_states();
    if (states_1.size() != states_2.size()){
        return -1;
    }
    double dist = 0;
    for (int i = 0; i < states_1.size(); i++) {
        dist = dist + sqrt(pow(states_1[i][0] - states_2[i][0], 2) + pow(states_1[i][1] - states_2[i][1], 2));
    }
    return dist;
}

double SampleSpace::get_dist(std::vector<std::vector<double>> states_1, std::vector<std::vector<double>> states_2){
    if (states_1.size() != states_2.size()){
        return -1;
    }
    double dist = 0;
    for (int i = 0; i < states_1.size(); i++) {
        dist = dist + sqrt(pow(states_1[i][0] - states_2[i][0], 2) + pow(states_1[i][1] - states_2[i][1], 2));
    }
    return dist;
}


SubSampleSpace& SampleSpace::get_sub_space(int num_ba) {
    return sample_space_ltl_map_.find(num_ba)->second;
}

void SampleSpace::set_space(int num_ba) {
    num_ba_ = num_ba;
    for (int i = 0; i < num_ba; i++) {
        SubSampleSpace new_sub_space;
        sub_sample_space_.push_back(new_sub_space);
        sample_space_ltl_map_[i] = new_sub_space;
    }
}

void SampleSpace::insert_sample(MultiSampleNode new_sample, int sub_space_id) {
    sample_space_ltl_map_.find(sub_space_id)->second.insert_sample(new_sample);
}

uint64_t SampleSpace::total_sample_num() {
    uint64_t total_num = 0;
    for (int i = 0; i < num_ba_; i++) {
        total_num = total_num + sample_space_ltl_map_[i].num_samples();
    }
    return total_num;
}

void SampleSpace::rewire(uint64_t new_sample_id, int new_sample_ba, std::vector<Region> obstacles, double RADIUS) {
    MultiSampleNode &new_sample = sample_space_ltl_map_.find(new_sample_ba)->second.get_sample(new_sample_id);
    std::vector<MultiSampleNode>& all_sub_samples = sample_space_ltl_map_.find(new_sample_ba)->second.get_all_samples();
    for (int i = 0; i < all_sub_samples.size(); i++) {
        if (all_sub_samples[i].get_id() != new_sample.get_parent_id() &&
            get_dist(all_sub_samples[i].get_state(), new_sample.get_state()) < RADIUS &&
            get_dist(all_sub_samples[i].get_state(), new_sample.get_state()) + new_sample.get_cost() < 
            all_sub_samples[i].get_cost() ) {
            
                MultiSampleNode &rewire_sample = all_sub_samples[i];
                if (Region::collision_check_simple(rewire_sample.get_state(), new_sample.get_state(), obstacles)) {
                    continue;
                }

                uint64_t old_parent_id = rewire_sample.get_parent_id();
                int old_parent_ba = rewire_sample.get_parent_ba();

                std::vector<std::pair<int, uint64_t>>& children_from_old_parent = sample_space_ltl_map_.find(old_parent_ba)->second.get_sample(old_parent_id).get_children_id();
                std::pair <int, uint64_t> rewire_sample_id (rewire_sample.get_ba(), rewire_sample.get_id());
                children_from_old_parent.erase(std::remove(children_from_old_parent.begin(), children_from_old_parent.end(), rewire_sample_id), children_from_old_parent.end());
                rewire_sample.set_parent_ba(new_sample_ba);
                rewire_sample.set_parent_id(new_sample_id);
                double old_cost_of_rewire = rewire_sample.get_cost();
                rewire_sample.set_cost(new_sample.get_cost() + get_dist(rewire_sample.get_state(), new_sample.get_state()));
                double decreased_cost = old_cost_of_rewire - rewire_sample.get_cost();
                new_sample.add_children_id(rewire_sample_id);

                std::queue<std::pair <int, uint64_t>> Q_cost_update;
                std::vector<std::pair<int, uint64_t>> current_children = rewire_sample.get_children_id();
                for (int i = 0; i < current_children.size(); i++) {
                    Q_cost_update.push(current_children[i]);
                }
                while (!Q_cost_update.empty()) {
                    std::pair<int, uint64_t> child_need_fix_id = Q_cost_update.front();
                    Q_cost_update.pop();
                    MultiSampleNode &child_need_fix_sample = sample_space_ltl_map_.find(child_need_fix_id.first)->second.get_sample(child_need_fix_id.second);
                    child_need_fix_sample.set_cost(child_need_fix_sample.get_cost() - decreased_cost);
                    current_children = child_need_fix_sample.get_children_id();
                    for (int i = 0; i < current_children.size(); i++) {
                        Q_cost_update.push(current_children[i]);
                    }
                }


        }
    }
}


void SampleSpace::rewire_dubins(uint64_t new_sample_id, int new_sample_ba, std::vector<Region> obstacles, double work_space_size_x, double work_space_size_y, double RADIUS, double radius_L, double radius_R) {
    MultiSampleNode &new_sample = sample_space_ltl_map_.find(new_sample_ba)->second.get_sample(new_sample_id);
    std::vector<MultiSampleNode>& all_sub_samples = sample_space_ltl_map_.find(new_sample_ba)->second.get_all_samples();
    for (int i = 0; i < all_sub_samples.size(); i++) {
        if (all_sub_samples[i].get_id() != new_sample.get_parent_id() &&
            get_dist_dubins(all_sub_samples[i].get_state(), new_sample.get_state(), radius_L, radius_R) < RADIUS &&
            get_dist_dubins(all_sub_samples[i].get_state(), new_sample.get_state(), radius_L, radius_R) + new_sample.get_cost() < 
            all_sub_samples[i].get_cost() ) {
                
               
                MultiSampleNode &rewire_sample = all_sub_samples[i];
                DubinsSteer::SteerData dubins_steer_data_new = DubinsSteer::GetDubinsTrajectoryPointWise(rewire_sample.get_state(), new_sample.get_state(), radius_L, radius_R);
                if (Region::collision_check_dubins(dubins_steer_data_new.traj_point_wise, obstacles, work_space_size_x, work_space_size_y)) {
                    continue;
                }
                uint64_t old_parent_id = rewire_sample.get_parent_id();
                int old_parent_ba = rewire_sample.get_parent_ba();

                std::vector<std::pair<int, uint64_t>>& children_from_old_parent = sample_space_ltl_map_.find(old_parent_ba)->second.get_sample(old_parent_id).get_children_id();
                std::pair <int, uint64_t> rewire_sample_id (rewire_sample.get_ba(), rewire_sample.get_id());
                children_from_old_parent.erase(std::remove(children_from_old_parent.begin(), children_from_old_parent.end(), rewire_sample_id), children_from_old_parent.end());
                rewire_sample.set_parent_ba(new_sample_ba);
                rewire_sample.set_parent_id(new_sample_id);
                double old_cost_of_rewire = rewire_sample.get_cost();
                rewire_sample.set_cost(new_sample.get_cost() + get_dist_dubins(rewire_sample.get_state(), new_sample.get_state(), radius_L, radius_R));
                double decreased_cost = old_cost_of_rewire - rewire_sample.get_cost();
                new_sample.add_children_id(rewire_sample_id);
                new_sample.set_traj(dubins_steer_data_new.traj_point_wise);


                std::queue<std::pair <int, uint64_t>> Q_cost_update;
                std::vector<std::pair<int, uint64_t>> current_children = rewire_sample.get_children_id();
                for (int i = 0; i < current_children.size(); i++) {
                    Q_cost_update.push(current_children[i]);
                }
                while (!Q_cost_update.empty()) {
                    std::pair<int, uint64_t> child_need_fix_id = Q_cost_update.front();
                    Q_cost_update.pop();
                    MultiSampleNode &child_need_fix_sample = sample_space_ltl_map_.find(child_need_fix_id.first)->second.get_sample(child_need_fix_id.second);
                    child_need_fix_sample.set_cost(child_need_fix_sample.get_cost() - decreased_cost);
                    current_children = child_need_fix_sample.get_children_id();
                    for (int i = 0; i < current_children.size(); i++) {
                        Q_cost_update.push(current_children[i]);
                    }
                }


        }
    }
}