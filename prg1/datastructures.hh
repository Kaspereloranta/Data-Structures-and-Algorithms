// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <map>

// Types for IDs
using PlaceID = long int;
using AreaID = long int;
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

struct Place
{
    Name placeName;
    PlaceType place;
    Coord location;
};

struct Area
{
    Name areaName;
    std::unordered_multiset<Coord, CoordHash> shape;
    bool isSubArea; // may be useless
    PlaceID upperAreaID; // the upper area's ID
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

    // Estimate of performance: Linear. 2*Theta(n)
    // Short rationale for estimate:  unordered_map's member function clear()'s complexity
    // is linear in the size of the container. Now clear() is being called for places_ and areas_
    // which brings the coefficient 2.
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

    // Estimate of performance: Average: Constant, Theta(1). Worst-case 2*O(n).
    // Short rationale for estimate: find() is being used here for unordered_map
    // which's complexity is constant on average but may be O(n) in the worst case.
    // at() is also used for unordered_map, which's complexity is similar than find()'s.
    std::pair<Name, PlaceType> get_place_name_type(PlaceID id);

    // Estimate of performance: Average: Constant, Theta(1). Worst-Case 2*O(n)
    // Short rationale for estimate: find() is being used here for unordered_map
    // which's complexity is constant on average but may be O(n) in the worst case.
    // at() is also used for unordered_map, which's complexity is similar than find()'s.
    Coord get_place_coord(PlaceID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n * log(n) + n)
    // Short rationale for estimate: In the first for-loop
    // we insert elements to map, which's complexity is O(log n). And
    // we are doing that n times due to for-loop. On the second
    // for-loop we add sorted placeIDs to vector using push_back(), which's
    // complexity is constant ( O(1)). But the second for-loop brings the
    // coefficient of 2 to the performance.
    std::vector<PlaceID> places_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> places_coord_order();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> find_places_name(Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> find_places_type(PlaceType type);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_place_name(PlaceID id, Name const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_place_coord(PlaceID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_area_name(AreaID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AreaID> all_areas();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AreaID> subarea_in_areas(AreaID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AreaID> all_subareas_in_area(AreaID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> places_closest_to(Coord xy, PlaceType type);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_place(PlaceID id);

    // Estimate of performance:
    // Short rationale for estimate:
    AreaID common_area_of_subareas(AreaID id1, AreaID id2);

private:
    // Add stuff needed for your class implementation here


    std::unordered_map<PlaceID,Place> places_;       //key of the map is int ID of the place
    std::unordered_map<AreaID,Area> areas_;         //key of the map is int ID of the area
};

#endif // DATASTRUCTURES_HH
