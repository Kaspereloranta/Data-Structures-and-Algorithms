// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Replace this comment with your implementation
}

Datastructures::~Datastructures()
{
    // Replace this comment with your implementation
}

int Datastructures::place_count()
{
    return places_.size();
}

void Datastructures::clear_all()
{
    places_.clear();
    areas_.clear();
    clear_ways();
}

std::vector<PlaceID> Datastructures::all_places()
{
    std::vector<PlaceID> placeIDs;
    for(auto place : places_)
    {
        placeIDs.push_back(place.first); // push_back() is constant on complexity.
    }
    return placeIDs;
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    Place newPlace = {name, type, xy};
    auto result = places_.insert(std::make_pair(id,newPlace)); // insert() complexity: average theta(1), worst-case: O(n).
                                                               // make_pair() complexity: constant. (theta(1)).

    // if places_ already include an element with same key (PlaceID), insert() does not add anything to places_.

    return result.second; // result.second is now a bool value which tells whether the insertion above was successfull or not.
}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{
    std::unordered_map<PlaceID,Place>::const_iterator placeIt = places_.find(id); // Complexity of find(): Average: constant ( Theta(1))
                                                                                  // Worst case: Linear. O(n)
    if (placeIt == places_.end())
    {
        return {NO_NAME, PlaceType::NO_TYPE};
    }

    return std::make_pair(places_.at(id).placeName,places_.at(id).type); // .at() complexity: average constant, worst-case: O(n).
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    std::unordered_map<PlaceID,Place>::const_iterator placeIt = places_.find(id); // Complexity of find(): Average: constant ( Theta(1))
                                                                                  // Worst case: Linear. O(n).
    if (placeIt == places_.end())
    {
        return NO_COORD;
    }

    return places_.at(id).location; // .at() complexity: average constant (Theta(1)), worst-case: O(n).
}

bool Datastructures::add_area(AreaID id, const Name &name, std::vector<Coord> coords)
{
    Area new_area = {name,coords,false,NO_AREA,{}}; // NO_AREA and empty vector {} indicates that no parent area are added yet and
                                                    // that no subareas are added yet. bool-value tells is the are child of some area
                                                    // which is false at initialization.

    auto result = areas_.insert(std::make_pair(id,new_area)); // insert() complexity: average theta(1), worst-case: O(n).
                                                              // make_pair() complexity: constant. (theta(1)).

    // if areas_ already include an element with same key (AreaID), insert() does not add anything to areas_.
    return result.second; // result.second is now a bool value which tells whether the insertion above was successfull or not.
}

Name Datastructures::get_area_name(AreaID id)
{
    if(areas_.find(id) != areas_.end())
    {
        return areas_.at(id).areaName;
    }
    return NO_NAME;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    if(areas_.find(id) != areas_.end())
    {
        return areas_.at(id).shape;
    }
    return {NO_COORD};
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
}


std::vector<PlaceID> Datastructures::places_alphabetically()
{
    std::map<Name,PlaceID> placesInAlphabeticalOrder;

   // for-loop complexity: O(n)
    for (auto place : places_)
    {
        placesInAlphabeticalOrder.insert({place.second.placeName,place.first}); // .insert() for map complexity: O(log n)
    }

    // now places are in alphabetical order since map sorts them.
    std::vector<PlaceID> placeIDsInOrder;

    // for loop complexity: O(n)
    for (auto place : placesInAlphabeticalOrder)
    {
        placeIDsInOrder.push_back(place.second); // push_back() is constant in time. O(1).
    }

    return placeIDsInOrder;
}

std::vector<PlaceID> Datastructures::places_coord_order()
{
    // Let's use multimap to sort places based on their location.
    // lets use strucutre map<double,map<int,PlaceID>>
    // because there might be places with same distance from origo
    // but with different y-coordinates. double is distance from origo,
    // int is y-coordinate.

    std::map<double,std::multimap<int,PlaceID>> places_in_order;
    get_places_in_order({0,0},PlaceType::NO_TYPE,places_in_order);

    std::vector<PlaceID> place_IDs_in_order;
    // For-loop above: O(n).
    for (auto place : places_in_order)
    {
        if (place.second.size()==1)
        {
            place_IDs_in_order.push_back(place.second.begin()->second);
        }
        else
        {
            for(auto place_with_same_distance : place.second)                   // Allthough there is a for-loop inside of for-loop
            {                                                                   // the complexity of this structure is not O(n^2).
                place_IDs_in_order.push_back(place_with_same_distance.second);  // Due to the datastructure map<double,multimap<int,PlaceID>> we are using,
            }                                                                   // if we enter to that internal for-loop, it means that there are less
        }                                                                       // places to loop through in the outermost for-loop. (Because if we enter to
     }                                                                          // to the second for-loop, there are at least two places with the exact same
    return place_IDs_in_order;                                                  // distance from origo, but we don't enter to that second for loop every time)                                                                                // so therefore the efficiency of this structure is theta(n), since .push_back() is constant                                                                                // in time for vector.
}                                                                               // This for-for-structure loops through every place there is,
                                                                                // causing it to be linear in complexity. (The exact efficiency would be something
                                                                                // like O(n + m) in which the m is amount of distances that multiple places have.
                                                                                // (Due to the if-else). But we can approximate and say that asymptotic efficiency is O(n).


std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{
    std::vector<PlaceID> places_with_name;

    // for-loop makes this operation linear.
    for(auto place : places_)
    {
        if(place.second.placeName == name)
        {
            places_with_name.push_back(place.first); //push_back() complexity: constant.
        }
    }
    return places_with_name;
}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{
    std::vector<PlaceID> places_with_right_type;

    // for-loop makes this operation linear.
    for(auto place : places_)
    {
        if(place.second.type == type)
        {
            places_with_right_type.push_back(place.first); //push_back() complexity: constant.
        }
    }
    return places_with_right_type;
}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    if (places_.find(id) != places_.end())
    {
        places_.at(id).placeName = newname;
        return true;
    }
    return false;
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    if (places_.find(id) != places_.end())
    {
        places_.at(id).location = newcoord;
        return true;
    }
    return false;
}

std::vector<AreaID> Datastructures::all_areas()
{
    std::vector<AreaID> areaIDs;
    for (auto area : areas_)
    {
        areaIDs.push_back(area.first);
    }
    return areaIDs;
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    if(areas_.find(id) != areas_.end() and
       areas_.find(parentid) != areas_.end())             // find() for unordered_map is constant averagely, worst-case O(n);
    {
        if(!areas_.at(id).isSubArea)
        {
            areas_.at(parentid).childrenAreas.push_back(id); // .push_back() to vector is constant.
            areas_.at(id).isSubArea = true;               // .at() for unordered_map is similar in complexity.
            areas_.at(id).parentAreaID = parentid;
            return true;
        }
    }
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    if(areas_.find(id)==areas_.end()) // .find() and .end() for unordered_map is constant on average, worst-case O(n).
    {
        return {NO_AREA};
    }
    std::vector<AreaID> upper_areas;
    get_upper_areas_(id,upper_areas); // O(n)
    return upper_areas;
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    std::map<double,std::multimap<int,PlaceID>> closest_places;
    get_places_in_order(xy,type,closest_places); // O(n*log(n))
    std::vector<PlaceID> three_closest_places;

    // Allthough there is for-loop inside a for-loop, the
    // complexity of this structure is not O(n^2).
    // (Read detailed explanation why it is not from
    // places_coord_order where this kind of structure
    // were used as well.)
    // That is because we stop looping by using break
    // after three places are added to vector.
    // On this function's case, the above for-structure
    // is actually consant, since the looping will
    // be stopped after 3 places are added to vector and
    // the places are already in right order in closest_places.
    for (auto place : closest_places)
    {
        if(place.second.size()==1)
        {
            three_closest_places.push_back(place.second.begin()->second);
            if(three_closest_places.size()==3)
            {
                break;
            }
        }
        else
        {
            for(auto place_with_nonunique_distance : place.second)
            {
                three_closest_places.push_back(place_with_nonunique_distance.second);
                if(three_closest_places.size()==3)
                {
                    break;
                }
            }
        }
    }

    return three_closest_places;;
}

bool Datastructures::remove_place(PlaceID id)
{
    auto result = places_.erase(id);
    return result; // if erase was successful, result == 1,
                   // otherwise (id not found), result == 0.
}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    if(areas_.find(id)==areas_.end()) // .find() and .end() for unordered_map is constant on average, worst-case O(n).
    {
        return {NO_AREA};
    }
    std::vector<AreaID> subareas;
    get_subareas_(id,subareas);
    return subareas;
}

void Datastructures::get_subareas_(AreaID id,std::vector<AreaID> & subareas_already_added)
{
    if(areas_.find(id) != areas_.end())
    {
        for(auto subarea : areas_.at(id).childrenAreas)
        {
            subareas_already_added.push_back(subarea);
            get_subareas_(subarea,subareas_already_added);
        }
    }
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    if(areas_.find(id1)==areas_.end() or
       areas_.find(id2)==areas_.end()) // .find() and .end() for unordered_map is constant on average, worst-case O(n).
    {
        return NO_AREA;
    }

    std::vector<AreaID> upper_areas_1;
    std::vector<AreaID> upper_areas_2;

    // Let's fullfill vector above with upper areas of
    // areas representing IDs id1 and id2. Fullfillment
    // is done by using recursive function get_upper_areas.
    // by using vector we ensure that areas are in right
    // order. ("father" -- "grandfather" -- etc.)

    get_upper_areas_(id1,upper_areas_1); // O(n)
    get_upper_areas_(id2,upper_areas_2); // O(n)

    AreaID nearest_common_area = NO_AREA;

    // The first common area we found in this double-for-loop above is the
    // nearest common area (Because the areas are in right order in vectors).
    // Once the first common ancestor is found, we can break the loops.

    //for-loop complexity: O(n^2).
    for(AreaID area1 : upper_areas_1)
    {
        for(AreaID area2 : upper_areas_2)
        {
            if(area1 == area2)
            {
                nearest_common_area = area1;
                break;
            }
        }
        if(nearest_common_area != NO_AREA)
        {
            break;
        }
    }
    return nearest_common_area;
}

void Datastructures::get_upper_areas_(AreaID id, std::vector<AreaID> & upper_areas)
{
    if(areas_.find(id) != areas_.end())
    {
        if(areas_.at(id).parentAreaID != NO_AREA)
        {
            upper_areas.push_back(areas_.at(id).parentAreaID);
            get_upper_areas_(areas_.at(id).parentAreaID,upper_areas);
        }
    }
}

void Datastructures::get_places_in_order(Coord xy, PlaceType type, std::map<double,std::multimap<int, PlaceID>> & places_in_order)
{
    if(type == PlaceType::NO_TYPE)
    {
        for (auto place : places_)
        {
            double distance = sqrt(pow(place.second.location.x-xy.x,2)+pow(place.second.location.y-xy.y,2));

            if (places_in_order.find(distance) != places_in_order.end())        // map.find() complexity O(log n)
            {
                places_in_order.at(distance).insert(std::make_pair(place.second.location.y,place.first)); // .insert() for map is O(log n) as well.
            }
            else
            {
                places_in_order[distance] = {std::make_pair(place.second.location.y,place.first)}; // O (log n)
            }
        }
        return;
    }
    for (auto place : places_)
    {
        double distance = sqrt(pow(place.second.location.x-xy.x,2)+pow(place.second.location.y-xy.y,2));

        if (places_in_order.find(distance) != places_in_order.end() and place.second.type == type)        // map.find() complexity O(log n)
        {
            places_in_order.at(distance).insert(std::make_pair(place.second.location.y,place.first)); // .insert() for map is O(log n) as well.
        }
        else if(place.second.type == type)
        {
            places_in_order[distance] = {std::make_pair(place.second.location.y,place.first)}; // O (log n)
        }
    }
    return;
}

void Datastructures::restore_nodes()
{
    // O(n). .at() for unordered_map is constant
    // inside the for-loop, because it is called
    // only for keys that exist in nodes_.
    // this is ensured by for-loop ( node : nodes_)
    for(auto node : nodes_)
    {
        nodes_.at(node.first).node_status = WHITE;
        nodes_.at(node.first).route_distance_so_far = 9999999;
        nodes_.at(node.first).steps_taken = -1;
        nodes_.at(node.first).route_distance_estimate = -1;
        nodes_.at(node.first).previous_node = nullptr;
        nodes_.at(node.first).secondary_previous_node = nullptr;
        nodes_.at(node.first).previous_way = NO_WAY;
        nodes_.at(node.first).secondary_previous_way = NO_WAY;
    }
}

std::vector<WayID> Datastructures::all_ways()
{
    std::vector<WayID> ways;
    ways.reserve(ways_.size());     // .size() for unordered_map is consant on complexity
    for(auto way : ways_)           // .reserve() for vector is now O(n), it depends
    {                               // on size of ways_.
        ways.push_back(way.first); // push_back() is now constant, we reserved the exact right amount of memory
    }                              // by using reserve() earlier, so reallocation does not happen.
    return ways;
}

bool Datastructures::add_way(WayID id, std::vector<Coord> coords)
{
    if(ways_.find(id) != ways_.end()) // find() averagely constant for unordered_map, linear in worst case. end() is constant)
    {
        return false;
    }

    // calculate_distance's complexity is O(n).
    Distance way_distance = calculate_distance(coords);

    Way new_way = {coords,way_distance};
    ways_.insert(std::make_pair(id,new_way));

    // let's update nodes
    // .front() and .back() for vector are constants on complexity
    // .at(), .insert() and .find() for unordered_map are constants on average, linear on worst cases
    // .end() for unordered_map is constant on complexity
    // std::make_pair is constant
    // so the whole node-update-process above are constant on average case, but
    // in the worst case it's linear.
    if(nodes_.find(coords.front()) == nodes_.end())
    {
        std::unordered_multimap<Coord,WayID,CoordHash> accesses;
        accesses.insert(std::make_pair(coords.back(),id));
        Node new_node = {coords.front(),accesses,WHITE,-1,9999999,-1,nullptr,nullptr,NO_WAY,NO_WAY};
        nodes_.insert(std::make_pair(coords.front(),new_node));
    }
    else
    {
        nodes_.at(coords.front()).accesses.insert(std::make_pair(coords.back(),id));
    }
    if(nodes_.find(coords.back()) == nodes_.end())
    {
        std::unordered_multimap<Coord,WayID,CoordHash> accesses;
        accesses.insert(std::make_pair(coords.front(),id));
        Node new_node = {coords.back(),accesses,WHITE,-1,9999999,-1,nullptr,nullptr,NO_WAY,NO_WAY};
        nodes_.insert(std::make_pair(coords.back(),new_node));
    }
    else
    {
        nodes_.at(coords.back()).accesses.insert(std::make_pair(coords.front(),id));

    }
    return true;
}

Distance Datastructures::calculate_distance(const std::vector<Coord> coords)
{
    Distance dist = 0;
    if(coords.size() <= 1) //.size() is constant
    {
        return dist;
    }
    // for-loop complexity Theta(n)
    for(auto coord = coords.begin(); coord != coords.end()-1; //.begin() and .end() are consants
        ++coord)
    {
        auto next_coord = coord +1;
        Distance x_dist = next_coord->x-coord->x;
        Distance y_dist = next_coord->y-coord->y;
        Distance part_dist = floor(sqrt(pow(x_dist,2)+pow(y_dist,2)));
        dist += part_dist;
    }
    return dist;
}

Distance Datastructures::distance_between_nodes(Coord point1, Coord point2)
{
    Distance x_dist = point2.x-point1.x;
    Distance y_dist = point2.y-point2.y;
    Distance dist = sqrt(pow(x_dist,2)+pow(y_dist,2));
    return dist;
}

std::vector<std::pair<WayID, Coord>> Datastructures::ways_from(Coord xy)
{

    std::vector<std::pair<WayID, Coord>> ways_and_crossroads;
    if(nodes_.find(xy) == nodes_.end()) // for unordered_map, .find() constant on average, linear on worst case, .end() constant
    {
        return ways_and_crossroads; // there were no node at all at the given coordinate
    }

    if(nodes_.at(xy).accesses.size() < 1 ) // for unordered_map, .at() constant on average, worst case linear,
    {
         return ways_and_crossroads; // there were no crossroad at the given coordinate
    }

    // The for-loop above is O(n). Theoretically, it could be O(n^2) because
    // .at() for unordered_map is constant on average, but linear on worst case, but
    // I assume that .at() is constant here all the time, because all unordered_map
    // keys that are given here as a parameter for .at() exists in nodes_, because the execution
    // of this operation will be ended earlier if it did not.
    for(auto way : nodes_.at(xy).accesses)
    {
        // T??T?? ALLAOLEVAA IF-ELSE?? KARSITTU, JOS TULEE ONGELMIA MY??HEMMIN TSEKKAA GIT JA TESTAA VANHALLA VERSIOLLA

        // for unordered_map, .at() constant on average, worst case linear, .front() and .back() for vector are constants
        // .push_back() is amortized constant for vector, std::make_pair is constant
        if(ways_.at(way.second).coordinates.front() == xy)
        {
                ways_and_crossroads.push_back(std::make_pair(way.second,ways_.at(way.second).coordinates.back()));
        }
        else if(ways_.at(way.second).coordinates.back() == xy)
        {
                ways_and_crossroads.push_back(std::make_pair(way.second,ways_.at(way.second).coordinates.front()));
        }
    }
    return ways_and_crossroads;
}

std::vector<Coord> Datastructures::get_way_coords(WayID id)
{
    // .at() and .find() for unordered_map are constant on average, linear on worst cases,
    // .end() is constant for unordered_map
    //
    if(ways_.find(id) != ways_.end())
    {
        return ways_.at(id).coordinates;
    }
    return {NO_COORD};
}

void Datastructures::clear_ways()
{
    ways_.clear();      // .clear()'s complexity
    nodes_.clear();     // for unordered map ilinear on size
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_any(Coord fromxy, Coord toxy)
{
    if(nodes_.find(fromxy) == nodes_.end() or
            nodes_.find(toxy) == nodes_.end()) // for unordered_map .find() is constant on average, linear on worst case, .end() constant
    {
        return {{NO_COORD, NO_WAY, NO_DISTANCE}}; // one or both of coordinates were not nodes.
    }

    if(nodes_.at(fromxy).accesses.size() < 1 or
       nodes_.at(toxy).accesses.size() < 1) // for unordered_map .find() .at() constant on average, worst case linear,
    {
          return {{NO_COORD, NO_WAY, NO_DISTANCE}}; // one or both of nodes were not crossroads.
    }

    // O(V+E) = O(N)
    DFS_route(fromxy,toxy);
    return track_route(toxy);
}

void Datastructures::DFS_route(Coord & fromxy, Coord & toxy)
{
    // restore_nodes() complexity O(n).
    restore_nodes();
    // DFS's complexity is O(V+E) in which
    // V is the amount of nodes in a graph, and E
    // the amount of edges in a graph.
    std::stack<Node*> DFS_stack;
    Node* starting_point = &nodes_.at(fromxy); // .at() constant on average, linear in worst case.
    starting_point->route_distance_so_far = 0;
    starting_point->steps_taken = 0;
    DFS_stack.push(starting_point);
    while (DFS_stack.size() > 0)
    {
        Node* top_node = DFS_stack.top();
        DFS_stack.pop();
        if(top_node->node_status == WHITE)
        {
            top_node->node_status = GRAY;
            DFS_stack.push(top_node);
            if(top_node->location == toxy)
            {
                top_node->node_status = BLACK; // is this necessary?
                DFS_stack = {};
                break;
            }
            for(auto neighbour : DFS_stack.top()->accesses)
            {
                if(nodes_.at(neighbour.first).node_status == WHITE)
                {
                    nodes_.at(neighbour.first).previous_node = top_node;
                    nodes_.at(neighbour.first).previous_way = neighbour.second;
                    nodes_.at(neighbour.first).route_distance_so_far = top_node->route_distance_so_far +
                                                                       ways_.at(neighbour.second).distance;
                    DFS_stack.push(&nodes_.at(neighbour.first));
                }
            }
        }
        else
        {
            top_node->node_status = BLACK;
        }
    }
}

Node* Datastructures::DFS_cycle(Coord &fromxy)
{
    // restore_nodes() complexity O(n).
    restore_nodes();
    // DFS's complexity is O(V+E) in which
    // V is the amount of nodes in a graph, and E
    // the amount of edges in a graph.
    std::stack<Node*> DFS_stack;
    Node* starting_point = &nodes_.at(fromxy); // .at() constant on average, linear in worst case.
    starting_point->steps_taken = 0;
    DFS_stack.push(starting_point);
    while (DFS_stack.size() > 0)
    {
        Node* top_node = DFS_stack.top();
        DFS_stack.pop();
        if(top_node->node_status == WHITE)
        {
            top_node->node_status = GRAY;
            DFS_stack.push(top_node);
            for(auto neighbour : DFS_stack.top()->accesses)
            {
                if(nodes_.at(neighbour.first).node_status == WHITE)
                {
                    nodes_.at(neighbour.first).previous_node = top_node;
                    nodes_.at(neighbour.first).previous_way = neighbour.second;
                    nodes_.at(neighbour.first).steps_taken = top_node->steps_taken + 1;
                    DFS_stack.push(&nodes_.at(neighbour.first));
                }
                else if(nodes_.at(neighbour.first).node_status == GRAY
                        and top_node->steps_taken - nodes_.at(neighbour.first).steps_taken > 1) // true cycle found
                {
                    nodes_.at(neighbour.first).secondary_previous_node = top_node;
                    nodes_.at(neighbour.first).secondary_previous_way = neighbour.second;
                    return &nodes_.at(neighbour.first);
                }
            }
        }
        else
        {
            top_node->node_status = BLACK;
        }
    }
    return nullptr;
}

void Datastructures::BFS(Coord &fromxy, Coord &toxy)
{
    restore_nodes(); // O(n)
    std::queue<Node*> BFS_queue;
    Node* starting_point = &nodes_.at(fromxy);
    starting_point->node_status = GRAY;
    starting_point->route_distance_so_far = 0;
    starting_point->steps_taken = 0;
    BFS_queue.push(starting_point);
    // BFS's complexity is O(V+E) in which
    // V is the amount of nodes in a graph, and E
    // the amount of edges in a graph.
    while(BFS_queue.size() > 0)
    {
        Node* current_node = BFS_queue.front();
        BFS_queue.pop();
        for(auto neighbour : current_node->accesses)
        {
            if(current_node->location == toxy)
            {
                current_node->node_status = BLACK; // is this necessary?
                BFS_queue = {};
                break;
            }
            if(nodes_.at(neighbour.first).node_status == WHITE)
            {
                nodes_.at(neighbour.first).node_status = GRAY;
                nodes_.at(neighbour.first).steps_taken = current_node->steps_taken + 1;
                nodes_.at(neighbour.first).previous_node = current_node;
                nodes_.at(neighbour.first).previous_way = neighbour.second;
                nodes_.at(neighbour.first).route_distance_so_far = current_node->route_distance_so_far +
                                                                   ways_.at(neighbour.second).distance;
                BFS_queue.push(&nodes_.at(neighbour.first));
            }
        }
        current_node->node_status = BLACK;
    }
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::track_route(Coord & route_end)
{
    std::vector<std::tuple<Coord, WayID, Distance>> route;

    if(nodes_.at(route_end).previous_node == nullptr) // if this is true, the earlier executed
    {                                                 //  graph algorithm did not found
        return route;                                 // a route between points of interest.
    }

    Node* current_node_1 = &nodes_.at(route_end); //.at() is now constant, because the node with Coord toxy exists in nodes_
    Node* current_node_2 = nodes_.at(route_end).previous_node;
    route.push_back(std::make_tuple(route_end,NO_WAY,current_node_1->route_distance_so_far)); // .push_back() is amortized constant, std::make_tuple is constant

    while(current_node_1->previous_node != nullptr) // there is no danger of an infinite loop because the route was found,
    {                                               // otherwise the execution of this method would have ended in the previous if-structure.
        route.push_back(std::make_tuple(current_node_2->location,current_node_1->previous_way, // And the starting point's node's is
                                        current_node_2->route_distance_so_far));               // restored and not edited afterwards by DFS, which
        current_node_1 = current_node_2;                                                       // means that when this while-loop reaches a node
        current_node_2 = current_node_2->previous_node;                                        // with previous_node == nullptr, the starting node
    }                                                                                          // is found.
    // route's data is now in reversed order because
    // we started looping backwards from the target node.
    std::reverse(route.begin(),route.end()); // O(n/2)
    return route;
}

void Datastructures::A_star(Coord &fromxy, Coord &toxy)
{
    restore_nodes(); // O(n)
    std::priority_queue<std::pair<Distance,Node*>> A_star_queue;
    Node* starting_point = &nodes_.at(fromxy);
    starting_point->route_distance_so_far = 0;
    Distance shortest_possible_distance = distance_between_nodes(fromxy, toxy);
    starting_point->node_status = GRAY;
    A_star_queue.push(std::make_pair(shortest_possible_distance*-1,starting_point));
    while(A_star_queue.size() != 0)
    {
        Node* current_node = A_star_queue.top().second;
        A_star_queue.pop();
        if(current_node->location == toxy)
        {
            A_star_queue = {};
            break;
        }
        if(current_node->node_status == BLACK)
        {
            continue; // to check duplicates
        }
        for(auto neighbour : current_node->accesses)
        {
            if(nodes_.at(neighbour.first).node_status == WHITE)
            {
                nodes_.at(neighbour.first).node_status = GRAY;

                nodes_.at(neighbour.first).route_distance_so_far = current_node->route_distance_so_far +
                                                                   ways_.at(neighbour.second).distance;

                nodes_.at(neighbour.first).route_distance_estimate = nodes_.at(neighbour.first).route_distance_so_far +
                        distance_between_nodes(neighbour.first,toxy);

                nodes_.at(neighbour.first).previous_way = neighbour.second;
                nodes_.at(neighbour.first).previous_node = current_node;

                A_star_queue.push(std::make_pair(nodes_.at(neighbour.first).route_distance_estimate*-1
                                                 ,&nodes_.at(neighbour.first)));
            }
            else if(nodes_.at(neighbour.first).route_distance_so_far > current_node->route_distance_so_far +
                                                                  ways_.at(neighbour.second).distance)
            {
                nodes_.at(neighbour.first).route_distance_so_far = current_node->route_distance_so_far +
                                                                   ways_.at(neighbour.second).distance;
                nodes_.at(neighbour.first).route_distance_estimate = nodes_.at(neighbour.first).route_distance_so_far +
                                                                   distance_between_nodes(neighbour.first,toxy);
                nodes_.at(neighbour.first).previous_way = neighbour.second;
                nodes_.at(neighbour.first).previous_node = current_node;

                A_star_queue.push(std::make_pair(nodes_.at(neighbour.first).route_distance_estimate*-1
                                                 ,&nodes_.at(neighbour.first)));
            }
        }
        current_node->node_status = BLACK;
    }
}

void Datastructures::Dijkstra(Coord fromxy, bool restoreNodes)
{
    if(restoreNodes)
    {
         restore_nodes();
    }
    std::priority_queue<std::pair<Distance,Node*>> Dijkstra_queue;
    nodes_.at(fromxy).node_status = GRAY;
    nodes_.at(fromxy).route_distance_so_far = 0;
    Dijkstra_queue.push(std::make_pair(0,&nodes_.at(fromxy)));
    while (Dijkstra_queue.size() != 0)
    {
        Node* current_node = Dijkstra_queue.top().second;
        Dijkstra_queue.pop();
        if(current_node->node_status == BLACK)
        {
            continue; // to check duplicates
        }
        for(auto neighbour : current_node->accesses)
        {
            if(nodes_.at(neighbour.first).node_status == WHITE)
            {
                nodes_.at(neighbour.first).node_status = GRAY;
                nodes_.at(neighbour.first).route_distance_so_far = current_node->route_distance_so_far
                                                                 + ways_.at(neighbour.second).distance;
                nodes_.at(neighbour.first).previous_node = current_node;
                nodes_.at(neighbour.first).previous_way = neighbour.second;
                Dijkstra_queue.push(std::make_pair(nodes_.at(neighbour.first).route_distance_so_far*-1,&nodes_.at(neighbour.first)));
            }
            else if(nodes_.at(neighbour.first).route_distance_so_far > current_node->route_distance_so_far
                                                                  + ways_.at(neighbour.second).distance)
            {
                nodes_.at(neighbour.first).route_distance_so_far = current_node->route_distance_so_far
                                                                 + ways_.at(neighbour.second).distance;
                nodes_.at(neighbour.first).previous_node = current_node;
                nodes_.at(neighbour.first).previous_way = neighbour.second;
                Dijkstra_queue.push(std::make_pair(nodes_.at(neighbour.first).route_distance_so_far*-1,&nodes_.at(neighbour.first)));
            }
        }
        current_node->node_status = BLACK;
    }
}


bool Datastructures::remove_way(WayID id)
{
    if(ways_.find(id) == ways_.end()) // find() constant on average, linear on worst case, end() is constant
    {
        return false;
    }
    nodes_.at(ways_.at(id).coordinates.front()).accesses.erase(ways_.at(id).coordinates.back());   // average: constant
    nodes_.at(ways_.at(id).coordinates.back()).accesses.erase((ways_.at(id).coordinates.front()));   // worst case: linear
    ways_.erase(id);
    return true;
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_least_crossroads(Coord fromxy, Coord toxy)
{
    if(nodes_.find(fromxy) == nodes_.end() or
            nodes_.find(toxy) == nodes_.end()) // for unordered_map .find() is constant on average, linear on worst case, .end() constant
    {
        return {{NO_COORD, NO_WAY, NO_DISTANCE}}; // one or both of coordinates were not nodes.
    }

    if(nodes_.at(fromxy).accesses.size() < 1 or
       nodes_.at(toxy).accesses.size() < 1) // for unordered_map .find() .at() constant on average, worst case linear,
    {
          return {{NO_COORD, NO_WAY, NO_DISTANCE}}; // one or both of nodes were not crossroads.
    }

    BFS(fromxy,toxy); // O(n) (O(V+E)).
    return track_route(toxy); // O(n)
}

std::vector<std::tuple<Coord, WayID> > Datastructures::route_with_cycle(Coord fromxy)
{
    if(nodes_.find(fromxy) == nodes_.end()) // for unordered_map .find() is constant on average, linear on worst case, .end() constant
    {
        return {{NO_COORD, NO_WAY}}; // given coordinate was not a node
    }

    if(nodes_.at(fromxy).accesses.size() < 1) // for unordered_map .find() .at() constant on average, worst case linear,
    {
          return {{NO_COORD, NO_WAY}}; // given node was not a crossroad
    }
    Node* cycle_node = DFS_cycle(fromxy);
    std::vector<std::tuple<Coord, WayID>> cycle_route;
    if(cycle_node == nullptr) //cycle was not found
    {
        return cycle_route;
    }

    cycle_route.push_back(std::make_tuple(cycle_node->location,NO_WAY));
    cycle_route.push_back(std::make_tuple(cycle_node->secondary_previous_node->location,
                                          cycle_node->secondary_previous_way));
    Node* current_node_1 = cycle_node->secondary_previous_node;
    Node* current_node_2 = cycle_node->secondary_previous_node->previous_node;

    while(current_node_1->previous_node != nullptr)
    {
        cycle_route.push_back(std::make_tuple(current_node_2->location,current_node_1->previous_way));
        current_node_1 = current_node_2;
        current_node_2 = current_node_2->previous_node;
    }

    std::reverse(cycle_route.begin(),cycle_route.end());
    return cycle_route;
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    if(nodes_.find(fromxy) == nodes_.end() or
            nodes_.find(toxy) == nodes_.end()) // for unordered_map .find() is constant on average, linear on worst case, .end() constant
    {
        return {{NO_COORD, NO_WAY, NO_DISTANCE}}; // one or both of coordinates were not nodes.
    }

    if(nodes_.at(fromxy).accesses.size() < 1 or
       nodes_.at(toxy).accesses.size() < 1) // for unordered_map .find() .at() constant on average, worst case linear,
    {
          return {{NO_COORD, NO_WAY, NO_DISTANCE}}; // one or both of nodes were not crossroads.
    }
    A_star(fromxy,toxy);
    return track_route(toxy);
}

Distance Datastructures::trim_ways()
{
    std::pair<Coord,Node> seed = *nodes_.begin();
    Coord seed_coord = seed.first;
    Dijkstra(seed_coord,true);
    // O(n*n*logn)).
    for(auto crossroad : nodes_)
    {
        if(crossroad.second.previous_way == NO_WAY)
        {
            Dijkstra(crossroad.first,false); // if entered here, there were a point of discontinuity in the graph.
        }   // and by doing this we ensure the whole graph gets handled.
    }
    // O(n) averagely.
    std::unordered_set<WayID> ways_to_be_saved;
    for(auto crossroad : nodes_)
    {
        ways_to_be_saved.insert(crossroad.second.previous_way);
    }
    // O(n) averagely.
    for(auto way : ways_)
    {
        if(ways_to_be_saved.find(way.first) == ways_to_be_saved.end())
        {
            remove_way(way.first);
        }
    }
    // O(n) averagely.
    for(auto node : nodes_)
    {
        for(auto access : node.second.accesses)
        {
            if(ways_to_be_saved.find(access.second) == ways_to_be_saved.end())
            {
                node.second.accesses.erase(access.first);
            }
        }
    }
    // O(n) averagely.
    Distance network_distance = 0;
    for(auto way : ways_)
    {
        network_distance += way.second.distance;
    }
    return network_distance;
}
