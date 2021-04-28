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
        nodes_.at(node.first).route_distance_so_far = 0;
        nodes_.at(node.first).previous_node = nullptr;
        nodes_.at(node.first).previous_way = NO_WAY;
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
        std::unordered_map<Coord,WayID,CoordHash> accesses;
        accesses.insert(std::make_pair(coords.back(),id));
        Node new_node = {coords.front(),accesses,WHITE,0,nullptr,NO_WAY};
        nodes_.insert(std::make_pair(coords.front(),new_node));
    }
    else
    {
        nodes_.at(coords.front()).accesses.insert(std::make_pair(coords.back(),id));
    }
    if(nodes_.find(coords.back()) == nodes_.end())
    {
        std::unordered_map<Coord,WayID,CoordHash> accesses;
        accesses.insert(std::make_pair(coords.front(),id));
        Node new_node = {coords.back(),accesses,WHITE,0,nullptr,NO_WAY};
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
        // TÄTÄ ALLAOLEVAA IF-ELSEÄ KARSITTU, JOS TULEE ONGELMIA MYÖHEMMIN TSEKKAA GIT JA TESTAA VANHALLA VERSIOLLA

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
    std::vector<std::tuple<Coord, WayID, Distance>> route;

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
    DFS(fromxy,toxy);

    Node* current_node_1 = &nodes_.at(toxy); //.at() is now constant, becaue the node with Coord toxy exists in nodes_
    Node* current_node_2 = nodes_.at(toxy).previous_node;
    route.push_back(std::make_tuple(current_node_1->location,NO_WAY,current_node_1->route_distance_so_far)); // .push_back() is amortized constant

    if(current_node_1->previous_node != nullptr) // if we enter to this if-structure, the route was found
    {                                            // because if the route existed, DFS would have changed
        // while-loop is O(n).                   // target node's previous node to be something else than nullptr.
        while(true) // and because the route was found, this while-loop won't be an infinite loop
        {           // because at some point the starting point (fromxy) will be found
                    // and when it happens, we use break; to shut the loop down.
            route.push_back(std::make_tuple(current_node_2->location,current_node_1->previous_way,current_node_2->route_distance_so_far));
            if(current_node_2->location == fromxy)
            {
                break;
            }
            current_node_1 = current_node_2;
            current_node_2 = current_node_2->previous_node;
        }
        // route's data is now in reversed order because
        // we started looping backwards from the target node.
        std::reverse(route.begin(),route.end()); // std::reverse is O(n/2) on complexity
        return route;
    }
    //if we enter here, the route was not found
    route = {};
    return route;
}


void Datastructures::DFS(Coord & fromxy, Coord & toxy)
{
    // restore_nodes() complexity O(n).
    restore_nodes();
    // DFS's complexity is O(V+E) in which
    // V is the amount of nodes in a graph, and E
    // the amount of edges in a graph.
    std::stack<Node*> DFS_stack;
    Node* starting_point = &nodes_.at(fromxy); // .at() constant on average, linear in worst case.
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
                top_node->node_status = BLACK;
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
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

std::vector<std::tuple<Coord, WayID> > Datastructures::route_with_cycle(Coord fromxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY}};
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

Distance Datastructures::trim_ways()
{
    // Replace this comment with your implementation
    return NO_DISTANCE;
}
