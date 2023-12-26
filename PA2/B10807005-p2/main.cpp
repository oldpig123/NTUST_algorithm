#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#define inf 1073741823;
using namespace std;
struct route_node {
    int loc_x;
    int loc_y;
};

struct net {
    int id;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    float dis;
    int loss;
    stack<route_node> route;
};
bool sort_by_dis(net x, net y) {
    return x.dis < y.dis;
}
bool sort_by_id(net x, net y) {
    return x.id < y.id;
}
struct grid {
    int from_x;
    int from_y;
    bool is_end_of_net = false;
    bool waveguide_on = false;
    int loss = inf;
    int status = 0;  // 0 for undiscover 1 for loss unconfirmed 2 for loss confirmed
};
bool sort_by_loss(grid x, grid y) {
    return x.loss < y.loss;
}
int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    if (argc != 3) {
        cout << "insufficient input" << endl;
        return 0;
    }

    fstream fin, fout;
    fin.open(argv[1], fstream::in);
    fout.open(argv[2], ios::out);
    if (!fin.is_open()) {
        cout << "input file error. file does not open" << endl;
        exit(1);
    }
    int propagation_loss, crossing_loss, bending_loss, size_x, size_y, net_num;
    string buffer1, buffer2;
    fin >> buffer1 >> size_x >> size_y;
    // cout<<buffer1<<" "<<size_x<<" "<<size_y<<endl;
    fin >> buffer1 >> buffer2 >> propagation_loss;
    // cout<<buffer1<<" "<<buffer2<<" "<<propagation_loss<<endl;
    fin >> buffer1 >> buffer2 >> crossing_loss;
    // cout<<buffer1<<" "<<buffer2<<" "<<crossing_loss<<endl;
    fin >> buffer1 >> buffer2 >> bending_loss;
    // cout<<buffer1<<" "<<buffer2<<" "<<bending_loss<<endl;
    fin >> buffer1 >> buffer2 >> net_num;
    // cout<<buffer1<<" "<<buffer2<<" "<<net_num<<endl;
    vector<net> nets;
    nets.resize(net_num);
    vector<vector<grid> > map;
    map.resize(size_x);
    for (int i = 0; i < size_x; i++) {
        map[i].resize(size_y);
    }

    for (int i = 0; i < net_num; i++) {
        net temp;
        fin >> temp.id >> temp.start_x >> temp.start_y >> temp.end_x >> temp.end_y;
        map[temp.start_x][temp.start_y].is_end_of_net = true;
        map[temp.end_x][temp.end_y].is_end_of_net = true;
        int xdis = abs(temp.end_x - temp.start_x), ydis = abs(temp.end_y - temp.start_y);
        temp.dis = sqrt(xdis * xdis + ydis * ydis);
        nets[i] = temp;
    }
    sort(nets.begin(), nets.end(), sort_by_dis);
    // for (auto i = nets.begin(); i < nets.end(); i++) {
    //     net temp = *i;
    //     cout << temp.id << " " << temp.start_x << " " << temp.start_y << " " << temp.end_x << " " << temp.end_y << " " << temp.dis << endl;
    // }
    for (auto i = nets.begin(); i < nets.end(); i++) {
        // cout << i->id << endl;
        vector<vector<grid> > temp_map = map;
        vector<route_node> unconfirmed;
        int end_x = i->end_x, end_y = i->end_y;
        int current_x = i->start_x, current_y = i->start_y;
        temp_map[current_x][current_y].loss = 0;
        temp_map[current_x][current_y].from_x = current_x;
        temp_map[current_x][current_y].from_y = current_y;

        // the dij part
        while (temp_map[i->end_x][i->end_y].status != 2) {
            temp_map[current_x][current_y].status = 2;
            // step right
            int try_x = current_x + 1;
            int try_y = current_y;
            if (try_x < size_x && temp_map[try_x][try_y].status != 2 && (!temp_map[try_x][try_y].is_end_of_net || (try_x == end_x && try_y == end_y))) {
                if (!temp_map[try_x][try_y].status) {
                    route_node temp;
                    temp.loc_x = try_x;
                    temp.loc_y = try_y;
                    unconfirmed.push_back(temp);
                    temp_map[try_x][try_y].status = 1;
                }
                int loss = temp_map[current_x][current_y].loss + propagation_loss + ((int)temp_map[try_x][try_y].waveguide_on) * crossing_loss + bending_loss * ((int)(!(current_y == temp_map[current_x][current_y].from_y)));
                if (temp_map[try_x][try_y].loss > loss) {
                    temp_map[try_x][try_y].loss = loss;
                    temp_map[try_x][try_y].from_x = current_x;
                    temp_map[try_x][try_y].from_y = current_y;
                }
            }

            // step down
            try_x = current_x;
            try_y = current_y + 1;
            if (try_y < size_y && temp_map[try_x][try_y].status != 2 && (!temp_map[try_x][try_y].is_end_of_net || (try_x == end_x && try_y == end_y))) {
                if (!temp_map[try_x][try_y].status) {
                    route_node temp;
                    temp.loc_x = try_x;
                    temp.loc_y = try_y;
                    unconfirmed.push_back(temp);
                    temp_map[try_x][try_y].status = 1;
                }
                int loss = temp_map[current_x][current_y].loss + propagation_loss + ((int)temp_map[try_x][try_y].waveguide_on) * crossing_loss + bending_loss * ((int)(!(current_x == temp_map[current_x][current_y].from_x)));
                if (temp_map[try_x][try_y].loss > loss) {
                    temp_map[try_x][try_y].loss = loss;
                    temp_map[try_x][try_y].from_x = current_x;
                    temp_map[try_x][try_y].from_y = current_y;
                }
            }

            // step left
            try_x = current_x - 1;
            try_y = current_y;
            if (try_x >= 0 && temp_map[try_x][try_y].status != 2 && (!temp_map[try_x][try_y].is_end_of_net || (try_x == end_x && try_y == end_y))) {
                if (!temp_map[try_x][try_y].status) {
                    route_node temp;
                    temp.loc_x = try_x;
                    temp.loc_y = try_y;
                    unconfirmed.push_back(temp);
                    temp_map[try_x][try_y].status = 1;
                }
                int loss = temp_map[current_x][current_y].loss + propagation_loss + ((int)temp_map[try_x][try_y].waveguide_on) * crossing_loss + bending_loss * ((int)(!(current_y == temp_map[current_x][current_y].from_y)));
                if (temp_map[try_x][try_y].loss > loss) {
                    temp_map[try_x][try_y].loss = loss;
                    temp_map[try_x][try_y].from_x = current_x;
                    temp_map[try_x][try_y].from_y = current_y;
                }
            }

            // step up
            try_x = current_x;
            try_y = current_y - 1;
            if (try_y >= 0 && temp_map[try_x][try_y].status != 2 && (!temp_map[try_x][try_y].is_end_of_net || (try_x == end_x && try_y == end_y))) {
                if (!temp_map[try_x][try_y].status) {
                    route_node temp;
                    temp.loc_x = try_x;
                    temp.loc_y = try_y;
                    unconfirmed.push_back(temp);
                    temp_map[try_x][try_y].status = 1;
                }
                int loss = temp_map[current_x][current_y].loss + propagation_loss + ((int)temp_map[try_x][try_y].waveguide_on) * crossing_loss + bending_loss * ((int)(!(current_x == temp_map[current_x][current_y].from_x)));
                if (temp_map[try_x][try_y].loss > loss) {
                    temp_map[try_x][try_y].loss = loss;
                    temp_map[try_x][try_y].from_x = current_x;
                    temp_map[try_x][try_y].from_y = current_y;
                }
            }

            sort(unconfirmed.begin(), unconfirmed.end(), [&](route_node a, route_node b) {
                return temp_map[a.loc_x][a.loc_y].loss > temp_map[b.loc_x][b.loc_y].loss;
            });

            current_x = unconfirmed.back().loc_x;
            current_y = unconfirmed.back().loc_y;
            unconfirmed.pop_back();
        }
        i->loss = temp_map[i->end_x][i->end_y].loss;
        // cout <<"net "<<i->id<<"loss is "<< i->loss << endl;
        // for (int i = size_y; i > 0; i--) {
        //     for (int j = 0; j < size_x; j++) {
        //         if (temp_map[j][i - 1].loss >= 1073741823) {
        //             cout << "x ";
        //         } else {
        //             cout << temp_map[j][i - 1].loss << " ";
        //         }
        //     }
        //     cout << endl;
        // }
        route_node node;
        // cout << i->start_x << " " << i->start_y << endl;
        node.loc_x = i->end_x;
        node.loc_y = i->end_y;
        while (node.loc_x != i->start_x || node.loc_y != i->start_y) {
            // cout << node.loc_x << " " << node.loc_y << endl;
            ;
            i->route.push(node);
            route_node next;
            // map[node.loc_x][node.loc_y].waveguide_on = true;
            next.loc_x = temp_map[node.loc_x][node.loc_y].from_x;
            next.loc_y = temp_map[node.loc_x][node.loc_y].from_y;
            node = next;
        }
        i->route.push(node);
    }
    // sort(nets.begin(), nets.end(), sort_by_id);
    for (auto i = nets.begin(); i < nets.end(); i++) {
        fout << i->id << " " << i->route.size() - 1 << endl;
        route_node buffer;
        buffer = i->route.top();
        i->route.pop();
        while (!i->route.empty()) {
            fout << buffer.loc_x << " " << buffer.loc_y << " " << i->route.top().loc_x << " " << i->route.top().loc_y << endl;
            ;
            buffer = i->route.top();
            i->route.pop();
        }
    }
}
/*
2147483647
1073741823
10000
*/
