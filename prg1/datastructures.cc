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
}

std::vector<PlaceID> Datastructures::all_places()
{
    //TÄTÄ KANNATTAA YRITTÄÄ HIENOSÄÄTÄÄ TEHOKKAAMMAKSI, JOS JÄÄ AIKAA
    // TOISAALTA TÄMÄ EI OLE MUKANA TEHOKKUUSTESTEISSÄ.

    // voi ehkä miettiä std::transform() tähän, mutta se taitaa tuottaa myös
    // lineaarisen operaation

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

    return std::make_pair(places_.at(id).placeName,places_.at(id).place); // .at() complexity: average constant, worst-case: O(n).
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
    Area new_area = {name,coords,false,NO_AREA,{}}; // NO_AREA and empty set {} indicates that no parent area are added yet and
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
    // TÄMÄN TEHOKKUUTTA EHKÄ SYYTÄ HIOA VIELÄ

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
    // PALAA TÄHÄN VIELÄ, VOI OLLA TEHOKKAAMPIAKIN TAPOJA TOTEUTTAA TÄMÄ

    // Let's use multimap to sort places based on their location.
    // lets use strucutre map<double,map<int,PlaceID>>
    // because there might be places with same distance from origo
    // but with different y-coordinates. double is distance from origo,
    // int is y-coordinate.

    std::map<double,std::multimap<int,PlaceID>> places_in_order;

    // for-loop complexity: O(n)
    for (auto place : places_)
    {
        double distance = sqrt(pow(place.second.location.x,2)+pow(place.second.location.y,2));

        if (places_in_order.find(distance) != places_in_order.end())        // map.find() complexity O(log n)
        {
            places_in_order.at(distance).insert(std::make_pair(place.second.location.y,place.first));
        }
        else                                                                      // map.insert() complexity: O(log n)
        {
            places_in_order[distance] = {std::make_pair(place.second.location.y,place.first)};
        }
    }

    std::vector<PlaceID> place_IDs_in_order;

    for (auto place : places_in_order)
    {
        if (place.second.size()==1)
        {
            place_IDs_in_order.push_back(place.second.begin()->second);
        }
        else
        {
            for(auto place_with_same_distance : place.second)
            {
                place_IDs_in_order.push_back(place_with_same_distance.second);  // Averagely should not need many for-loops here
            }                                                                   // so even this for-loop inside a for-loop
        }                                                                       // harms the asymptotic efficiency, it harms
                                                                                // the worst case only, averagely I'd say that
    }                                                                           // the efficiency of this whole operation is O(n log n)
                                                                                // while the worst-case is O(n^2)
    // PALAA TÄHÄN VIELÄ, VOI OLLA TEHOKKAAMPIAKIN TAPOJA TOTEUTTAA TÄMÄ
    return place_IDs_in_order;
}

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
        if(place.second.place == type)
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
            areas_.at(parentid).childrenAreas.insert(id); // insertion to unordered_set is constant averagely, worst-case O(n).
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
    std::stack<std::pair<AreaID,Area>> area_and_upper_areas;
    area_and_upper_areas.push(std::make_pair(id,areas_.at(id))); // .push() for stack is constant on time, .at() is constant on average,
    bool keep_looping = true;                                    // worst-case for .at() is O(n).

    // let's use stack here for assistance to get easy and efficient acces to the latest
    // area added to vector, which was ancestor of an original area whose id was given
    // as a parameter.

    // by doing so, we do not have to add the original area itself to vector at all
    // which will be returned from this operation. If we did that, we would have
    // to delete the first element from the vector in the end. Deleting the first element
    // from vector is a linear operation, so this operation is more efficient if we can
    // avoid doing so.

    // and using stack also simplifies the while loop below, since we can
    // use only one if-else structure by using stack

    // complexity of while-loop: O(n).
    // on most cases it is more efficient than linear, since the areas make up
    // a tree-struct. The while loop below would be linear only if every area
    // had only one child maximum. (Tree would not have any branches)
    while(keep_looping)
    {
        if(area_and_upper_areas.top().second.parentAreaID != NO_AREA)
        {
            upper_areas.push_back(area_and_upper_areas.top().second.parentAreaID); // parent found, let's store it to vector
            area_and_upper_areas.push(std::make_pair(area_and_upper_areas.top().second.parentAreaID,  // let's store the parent to stack as well,
                                                     areas_.at(area_and_upper_areas.top().second.parentAreaID))); // in order to get access to it easily
                                                                                                                  // on next while-loop round.
                                                     //.push_back() for vector is constant on time averagely, worst-case: O(n).
        }                                            // .push() and .top() for stack are constants on time,
        else                                         // make_pair() is constant as well.
        {
            keep_looping = false;
        }
    }
    return upper_areas;
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    // Replace this comment with your implementation
    return {};
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
    }    return {NO_AREA};
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    // Replace this comment with your implementation
    return NO_AREA;
}
