// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <QDebug>
#include <memory>

// Types for IDs
using PlaceID = long long int;
using AreaID = long long int;
using Name = std::string;
using WayID = std::string;

// Return values for cases where required thing was not found
PlaceID const NO_PLACE = -1;
AreaID const NO_AREA = -1;
WayID const NO_WAY = "!!No way!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Enumeration for different place types
// !!Note since this is a C++11 "scoped enumeration", you'll have to refer to
// individual values as PlaceType::SHELTER etc.
enum class PlaceType { OTHER=0, FIREPIT, SHELTER, PARKING, PEAK, BAY, AREA, NO_TYPE };

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;

enum Status {WHITE, GRAY, BLACK};

struct Place
{
    Name placeName;
    PlaceType type;
    Coord location;
};

struct Area
{
    Name areaName;
    std::vector<Coord> shape;
    bool isSubArea;
    AreaID parentAreaID;
    std::vector<AreaID> childrenAreas;
};

struct Node
{
    Coord location;
    // harkitse allaolevan rakenteen muuttamista siten, että avaimina on osoittimia Nodeihin
    // Voi olla hankalaa, saattaa tarvita oman hajautusfunktion? Jos tarvii, ÄLÄ EDES YRITÄ
    std::unordered_map<Coord,WayID,CoordHash> accesses;
    Status node_status;
    Distance route_distance_so_far;
    Coord previous_node;
    WayID previous_way;


//    std::unordered_map<struct Node,struct Way> connections; <--- this may come handy later?
    // add later the things             // a crossroad, otherwise it is an edge
};  // you need to use graph-algorithms // of the graph or a separate node


struct Way
{
    std::vector<Coord> coordinates;
    Distance distance;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: Constant. Theta(1).
    // Short rationale for estimate: unordered_map's member function size() is constant on complexity,
    // which is being used here.
    int place_count();

    // Estimate of performance: Linear. Theta(n)
    // Short rationale for estimate:  unordered_map's member function clear()'s complexity
    // is linear in the size of the container. Now clear() is being called for places_ and areas_
    // but we can say that this operation's asymptotic efficiency is theta(n).
    void clear_all();

    // Estimate of performance: Linear. O(n).
    // Short rationale for estimate: Processing data through for-loop cause it
    // to be a linear operation. In the "best case", if the datastructure is empty, it may
    // be a constant operation.
    std::vector<PlaceID> all_places();

    // Estimate of performance: Average: Constant, Theta(1), worst-case: O(n).
    // Short rationale for estimate: unordered_map.insert() is being used here,
    // which is constant on average, but in worst-case O(n). Other opeartions used here are constant.
    bool add_place(PlaceID id, Name const& name, PlaceType type, Coord xy);

    // Estimate of performance: Average: Constant, Theta(1). Worst-case O(n).
    // Short rationale for estimate: find() is being used here for unordered_map
    // which's complexity is constant on average but may be O(n) in the worst case.
    // at() is also used for unordered_map, which's complexity is similar than find()'s.
    std::pair<Name, PlaceType> get_place_name_type(PlaceID id);

    // Estimate of performance: Average: Constant, Theta(1). Worst-Case O(n)
    // Short rationale for estimate: find() is being used here for unordered_map
    // which's complexity is constant on average but may be O(n) in the worst case.
    // at() is also used for unordered_map, which's complexity is similar than find()'s.
    Coord get_place_coord(PlaceID id);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: In the first for-loop
    // we insert elements to map by using .insert(), which's complexity is O(log n). And
    // we are doing that n times due to for-loop. On the second
    // for-loop we add sorted placeIDs to vector using push_back(), which's
    // complexity is constant O(1). Since the first for-loop (in which
    // we insereted elements to map) is the most inefficient operation done here
    // the asymptotic efficiency for this function is O(n*log(n)).
    std::vector<PlaceID> places_alphabetically();

    // Estimate of performance: O(n * log n)
    // Short rationale for estimate: This function calls function
    // get_closest_places_in_order, which is defined in the private
    // interface of class datastructures, and its complexity is O(n*log n).
    // This function includes also another for-loop which
    // includes another for-loop. However, this double-for-loop structure's
    // complexity is not actually O(n^2) though you might think so.
    // The detailed explanaiton for this is written in function's definition
    // in datastructures.cc.
    std::vector<PlaceID> places_coord_order();

    // Estimate of performance: O(n)
    // Short rationale for estimate: For-loop causes it to
    // be a linear operation, push_back() doesn't affect to
    // efficiency, since it is a constant operation. On most
    // cases it is theta(n), but in the scenario where places_
    // is empty, it is constant.
    std::vector<PlaceID> find_places_name(Name const& name);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For-loop causes it to
    // be a linear operation, push_back() doesn't affect to
    // efficiency, since it is a constant operation. On most
    // cases it is theta(n), but in the scenario where places_
    // is empty, it is constant.
    std::vector<PlaceID> find_places_type(PlaceType type);

    // Estimate of performance: Average: Constant, Theta(1). Worst-case:  O(n)
    // Short rationale for estimate: This function uses uses .find() and
    // .at() for unordered_map, which are both constant on average, but on
    // worst-case they are linear.
    bool change_place_name(PlaceID id, Name const& newname);

    // Estimate of performance: Average: Constant, Theta(1). Worst-case:  O(n)
    // Short rationale for estimate: This function uses uses .find() and
    // .at() for unordered_map, which are both constant on average, but on
    // worst-case they are linear.
    bool change_place_coord(PlaceID id, Coord newcoord);

    // Estimate of performance: Average: constant, theta(1). Worst-case: O(n).
    // Short rationale for estimate: insert() is being used here for
    // unordered_map, which is constant on average but its worst_case is O(n).
    // All other operations used here are constant.
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: Average: constant,  theta(1). Worst-case: O(n).
    // Short rationale for estimate: .at() and .find() are both averagely
    // constant and linear in worst case and only those operations are used
    // here.
    Name get_area_name(AreaID id);

    // Estimate of performance: Average: constant,  theta(1). Worst-case: O(n).
    // Short rationale for estimate: .at() and .find() are both averagely
    // constant and linear in worst case and only those operations are used
    // here.
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance: Linear. O(n)
    // Short rationale for estimate: For-loop
    // causes it to be linear, since it loops always
    // as many times there are elements in areas_.
    // If areas_ is empty, it is constant.
    // push_back() for vector is constant which
    // is also being used here.
    std::vector<AreaID> all_areas();

    // Estimate of performance: Average: Constant, Theta(1). Worst-case: O(n)
    // Short rationale for estimate: .at() and .find() are
    // used here for unordered_map and .insert() is being used for unordered_set.
    // all of those operations are averagely constant on complexity, but their
    // worst-case is O(n).
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: This function
    // calls recursive function get_upper_areas_, which
    // is defined in the private interface of class datastructures.
    // get_upper_areas_ adds all direct or indirect "parents"
    // of an area given as parameter to a vector given as a parameter as well.
    // In the worst case Area-tree has only one branch, hence the operation is linear,
    // causing the asymptotic efficiency to be O(n).
    std::vector<AreaID> subarea_in_areas(AreaID id);

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance: Linear O(n).
    // Short rationale for estimate: All operations
    // used directly inside of this function are constants on average
    // but this function calls recursive function get_subareas_ which is
    // implemented in private interface of class datastructures. There,
    // if the given area is the "root" area, all areas will be added to
    // vector that this function will return. Therefore, the performance
    // is O(n) since the "worst-case" is linear operation.
    std::vector<AreaID> all_subareas_in_area(AreaID id);

    // Estimate of performance: O(n*log(n)).
    // Short rationale for estimate: This operation calls function
    // get_closest_places_in_order, which is defined in the private interface
    // of class datastructures. get_closest_places_in_order's complexity is
    // O(n*log(n)), and since the for-loop used inside this function is
    // actually constant in time (read from datastructures.cc why so),
    // the asymptotic efficiency for this function is O(n*log(n)) as well.
    std::vector<PlaceID> places_closest_to(Coord xy, PlaceType type);

    // Estimate of performance: Constant. Theta(1).
    // Short rationale for estimate: .erase() is being
    // used only here for unordered_map, which is linear in the
    // number of elements removed, and we are only removing one element
    // here, which makes this operation to be constant on time.
    bool remove_place(PlaceID id);

    // Estimate of performance: O(n^2).
    // Short rationale for estimate: Double-for-loop inside this operation
    // causes it to be O(n^2). This is for the worst case only, in which the nearest common ancestor
    // is the root of the tree and the tree has only two branches. This function calls recursive function
    // get_upper_areas, which is defined in private interface of class datastructures.
    // get_upper_areas's complexity is O(n).
    AreaID common_area_of_subareas(AreaID id1, AreaID id2);

    // Phase 2 operations

    // Estimate of performance: Linear. Theta(n)
    // Short rationale for estimate: push.back() for vector
    // is true constant since we reserved the right amount of memory,
    // and it is being used here inside
    // a for loop that loops through all the ways.
    std::vector<WayID> all_ways();

    // Estimate of performance: Linear.  Theta(n). <-- CHECK THIS
    // Short rationale for estimate: The calculation of the way's distance
    // causes this methód to be linear. And it is exactly theta(n)
    // since the calculation for-loop has to go through all the
    // coordinates that creates the way which is given as a parameter
    bool add_way(WayID id, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Usage of for-loop cause
    // this operation to be O(n) on asymptotic efficiency. Methods used here
    // are all either constants on all cases, or constant on average and on worst
    // case linear. However, we can assume that on most cases they are rather consants
    // than linear. It is O(n) and not theta(n) because there might be cases in which
    // a crossroad has acces only to one road (and node).
    std::vector<std::pair<WayID, Coord>> ways_from(Coord xy);

    // Estimate of performance: Constant on average. Linear in worst case.
    // Short rationale for estimate: // .at() and .find() for unordered_map
    // are constant on average, linear on worst cases, .end() is constant for
    // unordered_map, and no other method is being used here.
    std::vector<Coord> get_way_coords(WayID id);

    // Estimate of performance: Linear. Theta(n)
    // Short rationale for estimate: ways_ is unordered_map,
    // and its method .clear() is linear on size, which is
    // being used here.
    void clear_ways();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_any(Coord fromxy, Coord toxy);

    // Non-compulsory operations

    // Estimate of performance: Constant on average, linear on worst case.
    // Short rationale for estimate: This method uses .find(), .end(), .at()
    // and .erase() for unordered_map and .front() and .back() for vector,
    // and all of these are either constants all the time or constants on average
    // but linear in worst case.
    bool remove_way(WayID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_least_crossroads(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID>> route_with_cycle(Coord fromxy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_shortest_distance(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_ways();

private:

    // Estimate of performance: Linear. O(n).
    // Short rationale for estimate: This is a recursive function that loops through every
    // subarea of an area (direct and indirect) in preorder and adds them to vector which is given as a
    // reference as a parameter. Usage of for-loop inside of this operation causes this to be linear on
    // complexity.
    void get_subareas_(AreaID id,std::vector<AreaID> & subareas_already_added);

    // Estimate of performance: Constant. Theta(1). Worst-case: O(n). (In its actual recursive use, it is O(n),
    // but one run of this function is constant on average).
    // Short rationale for estimate: This is a recursive function that loops through every
    // "parents" of an area (direct and indirect) in postorder and adds them to vector to which is given as a
    // reference as a parameter. However, one run of this function is actually constant in time since
    // this function is using only operations .find(), .end() and .at() for unordered_map and .push_back() for
    // vector, which are all constants on average, but some of them may be linear in the worst case.
    void get_upper_areas_(AreaID id1, std::vector<AreaID> & upper_areas);

    // Estimate of performance: O(n * log n)
    // Short rationale for estimate: This function sorts places based on their distance from location given
    // as a parameter xy by inserting them into map in for-loop. (And only certain types of places, if paramerer type is
    // something else than NO_PLACE.) Usage of insert() and find() for map
    // inside the for loop is the most inefficient operation done here, and the complexity for both of
    // those operations are logarithmic. The usage of those inside the for loop brings the coefficient n.
    void get_places_in_order(Coord xy, PlaceType type, std::map<double,std::multimap<int,PlaceID>> & places_in_order);

    void restore_nodes();

    void DFS_route_to_vector(std::vector<std::tuple<Coord, WayID, Distance>> & route, Node & node);

    std::unordered_map<PlaceID,Place> places_;
    std::unordered_map<AreaID,Area> areas_;
    std::unordered_map<WayID,Way> ways_;
    std::unordered_map<Coord,Node,CoordHash> nodes_;
};

#endif // DATASTRUCTURES_HH
